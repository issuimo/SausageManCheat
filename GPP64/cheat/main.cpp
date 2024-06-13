#include "main.h"

#include <TlHelp32.h>
#include <psapi.h>
#include <shlwapi.h>

#include "features.h"

#include "Base/HookManager.h"
#include "Base/ResourceLoader.h"
#include "Base/SEH.h"
#include "Base/cheat/misc/Settings.h"
#include "Base/render/gui-util.h"

#include <winternl.h>

#include "Base/VEH.h"
#include "Base/VEHHook.h"

#define ACTIVE_COLOR ImColor(0.13f, 0.8f, 0.08f)

#pragma comment(lib,"ntdll.lib")

#define NT_SUCCESS(x) ((x) >= 0)
#define STATUS_INFO_LENGTH_MISMATCH 0xc0000004

#define SystemHandleInformation 16
#define ObjectBasicInformation 0
#define ObjectNameInformation 1
#define ObjectTypeInformation 2

#define InitializeObjectAttributes( p, n, a, r, s ) { \
    (p)->Length = sizeof( OBJECT_ATTRIBUTES );          \
    (p)->RootDirectory = r;                             \
    (p)->Attributes = a;                                \
    (p)->ObjectName = n;                                \
    (p)->SecurityDescriptor = s;                        \
    (p)->SecurityQualityOfService = NULL;               \
    }

static int num = 0;
ULONG myModuleSize;

static auto GetLibraryProcAddress(LPCSTR LibraryName, LPCSTR ProcName) -> PVOID {
	auto hModule = GetModuleHandleA(LibraryName);
	if (hModule == nullptr) return nullptr;
	return GetProcAddress(hModule, ProcName);
}

VOID SuspendProcess(DWORD dwProcessID, BOOL fSuspend) {
	HANDLE hSnapshot = CreateToolhelp32Snapshot(TH32CS_SNAPTHREAD, dwProcessID);

	if (hSnapshot != INVALID_HANDLE_VALUE) {

		THREADENTRY32 te = { sizeof(te) };
		BOOL fOk = Thread32First(hSnapshot, &te);
		for (; fOk; fOk = Thread32Next(hSnapshot, &te)) {
			if (te.th32OwnerProcessID == dwProcessID) {
				HANDLE hThread = OpenThread(THREAD_SUSPEND_RESUME, FALSE, te.th32ThreadID);

				if (hThread != nullptr) {
					if (fSuspend) {
						SuspendThread(hThread);
					} else {
						ResumeThread(hThread);
					}
				}
				CloseHandle(hThread);
			}
		}
		CloseHandle(hSnapshot);
	}
}

std::vector<DWORD> GetProceeIDfromParentID(const DWORD& dwParentProcessId) {
	std::vector<DWORD> parent_id_vec;
	DWORD dwProcessID = 0;

	//进行一个进程快照
	HANDLE hProcessSnap = CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS, 0);
	if (hProcessSnap == INVALID_HANDLE_VALUE) {
		LOG_ERROR("CreateToolhelp32Snapshot error");
		return parent_id_vec;
	}

	PROCESSENTRY32 pe;
	TCHAR procID[100] = { 0 };
	pe.dwSize = sizeof(pe);
	BOOL bProcess = Process32First(hProcessSnap, &pe);
	//遍历所有进程
	while (bProcess) {
		if (pe.th32ParentProcessID == dwParentProcessId) {

			dwProcessID = pe.th32ProcessID;
			parent_id_vec.push_back(dwProcessID);
		}
		bProcess = Process32Next(hProcessSnap, &pe);
	}
	CloseHandle(hProcessSnap);
	return parent_id_vec;
}

auto CloseHandleByName(const wchar_t* name, HANDLE processHandle = GetCurrentProcess()) -> bool {
	auto     NtDuplicateObject = static_cast<_NtDuplicateObject>(GetLibraryProcAddress("ntdll.dll", "NtDuplicateObject"));
	auto     NtQueryObject     = static_cast<_NtQueryObject>(GetLibraryProcAddress("ntdll.dll", "NtQueryObject"));
	NTSTATUS status;

	ULONG handleInfoSize = 0x10000;
	auto  handleInfo     = static_cast<PSYSTEM_HANDLE_INFORMATION>(malloc(handleInfoSize));

	/* NtQuerySystemInformation won't give us the correct buffer size,
	   so we guess by doubling the buffer size. */
	while ((status = NtQuerySystemInformation(static_cast<SYSTEM_INFORMATION_CLASS>(SystemHandleInformation), handleInfo, handleInfoSize, nullptr)) == STATUS_INFO_LENGTH_MISMATCH) handleInfo = static_cast<PSYSTEM_HANDLE_INFORMATION>(realloc(handleInfo, handleInfoSize *= 2));

	/* NtQuerySystemInformation stopped giving us STATUS_INFO_LENGTH_MISMATCH. */
	if (!NT_SUCCESS(status)) {
		LOG_ERROR("NtQuerySystemInformation failed!");
		return false;
	}

	bool closed = false;
	for (ULONG i = 0; i < handleInfo->HandleCount; i++) {
		if (closed) break;

		SYSTEM_HANDLE handle    = handleInfo->Handles[i];
		HANDLE        dupHandle = nullptr;
		ULONG         returnLength;

		/* Duplicate the handle so we can query it. */
		if (!NT_SUCCESS(NtDuplicateObject(processHandle, reinterpret_cast<HANDLE>(handle.Handle), GetCurrentProcess(), &dupHandle, 0, 0, 0))) continue;

		/* Query the object type. */
		auto objectTypeInfo = static_cast<POBJECT_TYPE_INFORMATION>(malloc(0x1000));
		if (!NT_SUCCESS(NtQueryObject(dupHandle, ObjectTypeInformation, objectTypeInfo, 0x1000, NULL))) {
			CloseHandle(dupHandle);
			continue;
		}

		/* Query the object name (unless it has an access of
		   0x0012019f, on which NtQueryObject could hang. */
		if (handle.GrantedAccess == 0x0012019f) {
			free(objectTypeInfo);
			CloseHandle(dupHandle);
			continue;
		}

		PVOID objectNameInfo = malloc(0x1000);
		if (!NT_SUCCESS(NtQueryObject(dupHandle, ObjectNameInformation, objectNameInfo, 0x1000, &returnLength))) {
			/* Reallocate the buffer and try again. */
			objectNameInfo = realloc(objectNameInfo, returnLength);
			if (!NT_SUCCESS(NtQueryObject(dupHandle, ObjectNameInformation, objectNameInfo, returnLength, NULL))) {
				free(objectTypeInfo);
				free(objectNameInfo);
				CloseHandle(dupHandle);
				continue;
			}
		}

		/* Cast our buffer into an UNICODE_STRING. */
		UNICODE_STRING objectName = *static_cast<PUNICODE_STRING>(objectNameInfo);

		/* Print the information! */
		if (objectName.Length && lstrcmpiW(objectName.Buffer, name) == 0) {
			CloseHandle(reinterpret_cast<HANDLE>(handle.Handle));
			closed = true;
		}

		free(objectTypeInfo);
		free(objectNameInfo);
		CloseHandle(dupHandle);
	}

	free(handleInfo);
	CloseHandle(processHandle);
	return closed;
}

std::function<void(PCONTEXT, _EXCEPTION_RECORD*)> RestoreContext;
_NtQuerySection NtQuerySection;
_ZwCreateSection ZwCreateSection;
_ZwMapViewOfSection ZwMapViewOfSection;
_NtUnmapViewOfSection NtUnmapViewOfSection;
auto Main::Run(const HMODULE hModule) -> void {
	_set_se_translator(SEH_exception::TranslateSEHtoCE);

	myModule = hModule;

	LOG_DEBUG("模块大小: %d", myModuleSize);

	LOG_DEBUG("配置路径在 %s", (util::GetCurrentPath() / "cfg.json").string().c_str());

	while (gameModule == nullptr) {
		LOG_DEBUG("GameAssembly.dll未初始化，等待1秒.");
		GetModuleHandleExW(0, L"GameAssembly.dll", &gameModule);
		Sleep(1000);
	}

	LOG_DEBUG("初始化Unity解析器.");
	UnityResolve::Init(gameModule, UnityResolve::Mode::Il2Cpp);
	LOG_DEBUG("Unity解析器初始化完成.");

	LOG_DEBUG("初始化配置更新器.");
	config::SetupUpdate(&GameUpdateEvent);
	LOG_DEBUG("配置更新器初始化完成.");

	LOG_DEBUG("初始化功能.");
	InitFeatures();
	LOG_DEBUG("功能初始化完成.");

	LOG_DEBUG("初始化字体和DX11Hook.");
	const auto manager = Instance();

	LPBYTE pFontData = nullptr;
	DWORD dFontSize = 0;
	if (!ResourceLoader::LoadEx("ImGui_Font", MAKEINTRESOURCEA(10), pFontData, dFontSize)) {
		LOG_WARNING("从资源中获取字体失败.");
	}

	manager->Init(pFontData, dFontSize);
	LOG_DEBUG("字体和DX11Hook初始化完成.");

	LOG_DEBUG("初始化帐号管理器.");
	AccountChangedEvent(114514);
	LOG_DEBUG("帐号管理器初始化完成.");

	LOG_DEBUG("初始事件.");
	InitEvent();
	LOG_DEBUG("事件初始化完成.");

	httplib::Server server;
	server.Get("/", [](const httplib::Request& request, httplib::Response& response) {
		response.set_content("<!DOCTYPE html><html><head><meta content=\"text/html; charset=utf-8\" http-equiv=\"content-type\" /></head><body><a href=\"https://遂沫.com/\">访问主页</a><a href=\"https://qm.qq.com/q/gmacG1SnqU>加入群聊</a></body></html>", "text/html");
	});
	port = server.bind_to_any_port("localhost");
	LOG_INFO("HTTP服务器网址: http://localhost:%d/", port);
	server.listen_after_bind();
}

Main* Main::Instance() {
	static Main instance;
	return &instance;
}

Main::Main() :
	NFEX(f_AccConfig, "帐户配置", "数据", "General::Multi-Account", internal::AccountConfig(), true),
	NFS(f_ShowType, "名称显示类型", "General::Multi-Account", ShowType::Pseudo) {
	AccountChangedEvent += MY_METHOD_HANDLER(Main::OnAccountChanged);
}


auto Main::IsAccountAttached(const uint32_t userID, const std::string& profileName) const -> bool {
	auto& profiles = f_AccConfig.value().id2Profiles;
	return profiles.contains(userID) && profiles[m_CurrentAccount.userID] == profileName;
}

auto Main::GetAccountDisplayName(const uint32_t uid) -> std::string {
	switch (f_ShowType.value()) {
		case ShowType::Nickname: return uid == m_CurrentAccount.userID ? m_CurrentAccount.nickName : f_AccConfig.value().accounts[uid].nickName;
		case ShowType::UserID: return std::to_string(uid == m_CurrentAccount.userID ? m_CurrentAccount.userID : uid);
		case ShowType::Pseudo: default: {
			auto& pseudos = f_AccConfig.value().pseudos;
			return !pseudos.contains(uid) ? m_CurrentAccount.nickName : pseudos[uid];
		}
	}
}

auto Main::DetachAccount(const uint32_t uid, const std::string& profileName) const -> void {
	auto& accConfig = f_AccConfig.value();
	accConfig.id2Profiles.erase(uid);
	accConfig.accounts.erase(uid);
	accConfig.profiles2id[profileName].erase(uid);

	f_AccConfig.FireChanged();
}

auto Main::AttachAccount(const uint32_t uid, const std::string& profileName) const -> void {
	auto& accConfig = f_AccConfig.value();

	if (accConfig.id2Profiles.contains(uid)) {
		const auto prevProfile = accConfig.id2Profiles[uid];
		accConfig.profiles2id[prevProfile].erase(uid);
	}

	accConfig.id2Profiles[uid] = profileName;
	accConfig.accounts[uid] = m_CurrentAccount;
	accConfig.profiles2id[profileName].insert(uid);

	if (!accConfig.pseudos.contains(uid)) { accConfig.pseudos[uid] = m_CurrentAccount.nickName; }

	f_AccConfig.FireChanged();
}

auto Main::DrawProfileEntryActivities(const std::string& profileName) -> void {
	CheatManagerBase::DrawProfileEntryActivities(profileName);

	ImGui::SameLine();

	if (m_CurrentAccount.userID == 0) { ImGui::BeginDisabled(); }

	auto& profiles = f_AccConfig.value().id2Profiles;
	const bool isAccountAttached = IsAccountAttached(m_CurrentAccount.userID, profileName);
	if (ImGui::SmallButton(isAccountAttached ? "-" : "+")) { if (isAccountAttached) { DetachAccount(m_CurrentAccount.userID, profileName); } else { AttachAccount(m_CurrentAccount.userID, profileName); } }

	if (ImGui::IsItemHovered()) { ImGui::SetTooltip(isAccountAttached ? reinterpret_cast<const char*>(u8"脱离") : reinterpret_cast<const char*>(u8"附加")); }

	if (m_CurrentAccount.userID == 0) { ImGui::EndDisabled(); }

	ImGui::SameLine();

	if (ImGui::SmallButton(reinterpret_cast<const char*>(u8"列表"))) { ImGui::OpenPopup("Account list"); }
	if (ImGui::IsItemHovered()) { ImGui::SetTooltip(reinterpret_cast<const char*>(u8"账户列表")); }

	if (ImGui::BeginPopup("Account list", ImGuiWindowFlags_AlwaysAutoResize)) {
		DrawAccountsList(profileName);
		ImGui::EndPopup();
	}
}

auto Main::DrawProfileEntry(const std::string& profileName) -> void {
	CheatManagerBase::DrawProfileEntry(profileName);
	ImGui::TableNextColumn();

	auto& profileIds = f_AccConfig.value().profiles2id;
	const auto accountCount = profileIds.contains(profileName) ? profileIds[profileName].size() : 0;
	if (accountCount == 0) {
		ImGui::Text(reinterpret_cast<const char*>(u8"没有账户."));
		return;
	}

	const ImColor textColor = IsAccountAttached(m_CurrentAccount.userID, profileName) ? ACTIVE_COLOR : ImColor(ImGui::GetColorU32(ImGuiCol_Text));
	ImGui::TextColored(textColor, reinterpret_cast<const char*>(u8"%d 账户%s"), accountCount, accountCount > 1 ? "s" : "");
}

auto Main::DrawProfileTableHeader() -> void {
	CheatManagerBase::DrawProfileTableHeader();
	ImGui::TableSetupColumn(reinterpret_cast<const char*>(u8"账户"));
}

auto Main::GetProfileTableColumnCount() -> int { return CheatManagerBase::GetProfileTableColumnCount() + 1; }

auto Main::DrawAccountsList(const std::string& profileName) -> void {
	static ImGuiTableFlags flags = ImGuiTableFlags_Resizable | ImGuiTableFlags_Reorderable | ImGuiTableFlags_Hideable | ImGuiTableFlags_RowBg | ImGuiTableFlags_BordersOuter | ImGuiTableFlags_BordersV | ImGuiTableFlags_NoBordersInBody | ImGuiTableFlags_ScrollY;
	if (ImGui::BeginTable("Accounts", 2, flags, ImVec2(0.0f, ImGui::GetTextLineHeightWithSpacing() * 10), 0.0f)) {
		ImGui::TableSetupColumn(reinterpret_cast<const char*>(u8"名称"));
		ImGui::TableSetupColumn(reinterpret_cast<const char*>(u8"操作"));
		ImGui::TableSetupScrollFreeze(0, 1);
		ImGui::TableHeadersRow();

		// Perform copying due to we can change values
		const auto userIDs = f_AccConfig.value().profiles2id[profileName];
		for (auto& userID : userIDs) {
			ImGui::TableNextRow();
			ImGui::TableNextColumn();

			ImGui::PushID(userID);

			std::string name = GetAccountDisplayName(userID);
			ImColor nameColor = m_CurrentAccount.userID == userID ? ACTIVE_COLOR : ImColor(ImGui::GetColorU32(ImGuiCol_Text));
			ImGui::TextColored(nameColor, name.c_str());

			ImGui::TableNextColumn();

			if (ImGui::Button(reinterpret_cast<const char*>(u8"删除"))) { DetachAccount(userID, profileName); }

			ImGui::SameLine();

			DrawPseudoRename(userID);

			ImGui::PopID();
		}

		ImGui::EndTable();
	}
}

auto Main::DrawProfileLine() -> void {
	CheatManagerBase::DrawProfileLine();

	if (m_CurrentAccount.userID == 0) { return; }

	auto& currentProfile = config::CurrentProfileName();
	const std::string name = GetAccountDisplayName(m_CurrentAccount.userID);

	constexpr float buttonWidth = 75.0f;
	const auto innerSpacing = ImGui::GetStyle().ItemInnerSpacing.x;
	const auto textWidth = CalcWidth(name);
	const auto width = textWidth + innerSpacing + buttonWidth;

	const auto position = max(ImGui::GetCursorPosX() + innerSpacing + 130.0f, ImGui::GetContentRegionAvail().x - width + 20.0f);

	ImGui::SameLine(position);

	const bool accountAttached = IsAccountAttached(m_CurrentAccount.userID, currentProfile);

	const ImColor textColor = accountAttached ? ACTIVE_COLOR : ImColor(ImGui::GetColorU32(ImGuiCol_Text));
	ImGui::TextColored(textColor, name.c_str());
	ImGui::SameLine();

	if (ImGui::Button(accountAttached ? reinterpret_cast<const char*>(u8"脱离") : reinterpret_cast<const char*>(u8"附加"), ImVec2(buttonWidth, 0.0f))) {
		if (accountAttached) {
			DetachAccount(m_CurrentAccount.userID, currentProfile);
		} else {
			AttachAccount(m_CurrentAccount.userID, currentProfile);
		}
	}
}

auto Main::DrawProfileConfiguration() -> void {
	ConfigWidget(f_ShowType, "设置帐户的名称显示类型.");
	auto& pseudos = f_AccConfig.value().pseudos;
	if (pseudos.contains(m_CurrentAccount.userID)) {
		ImGui::Text(reinterpret_cast<const char*>(u8"假名: %s"), pseudos[m_CurrentAccount.userID].c_str());
		ImGui::SameLine();
		DrawPseudoRename(m_CurrentAccount.userID);
	}

	CheatManagerBase::DrawProfileConfiguration();
}


auto Main::DrawPseudoRename(const uint32_t userID) const -> void {
	auto& pseudo = f_AccConfig.value().pseudos[userID];
	if (ImGui::Button(reinterpret_cast<const char*>(u8"重命名 (假名)"))) { ImGui::OpenRenamePopup(pseudo); }

	if (ImGui::DrawRenamePopup(pseudo)) { f_AccConfig.FireChanged(); }
}

auto Main::OnAccountChanged(uint32_t userID) -> void {
	m_CurrentAccount.nickName = "User";
	m_CurrentAccount.userID = 114514;

	auto& profiles = f_AccConfig.value().id2Profiles;
	if (!profiles.contains(m_CurrentAccount.userID)) { return; }

	config::ChangeProfile(profiles[m_CurrentAccount.userID]);

	const auto& settings = cheat::feature::Settings::GetInstance();

	ImGuiToast toast(ImGuiToastType_Info, settings->f_NotificationsDelay.value(), reinterpret_cast<const char*>(u8"账户更新.\n修改配置文件."));
	toast.set_title(reinterpret_cast<const char*>(u8"配置多个账户"));
	ImGui::InsertNotification(toast);
}
