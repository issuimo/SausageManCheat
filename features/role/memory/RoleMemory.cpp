#include "RoleMemory.h"

#include "BaseMath.h"

#include "../../../CLASS/Role.h"
#include "../../../CLASS/RoleControl.h"
class Role;

cheat::feature::RoleMemory* cheat::feature::RoleMemory::GetInstance() {
	static RoleMemory role;
	return &role;
}
const cheat::FeatureGUIInfo& cheat::feature::RoleMemory::GetGUIInfo() const {
	static const FeatureGUIInfo info{ "内存", "玩家", true };
	return info;
}
void cheat::feature::RoleMemory::DrawMain() {
	ConfigWidget(f_Transmit, "启用玩家传送");
	ImGui::SameLine();
	ConfigWidget(f_TransmitHotKey, "传送热键");
	if (f_Transmit) {
		ConfigWidget(f_TransmitDistance, .5f, 0.f, 100.f, "安全距离");
		ConfigWidget(f_TransmitX, .5f, 0.f, 9000.f, "屏幕范围X");
		ConfigWidget(f_TransmitY, .5f, 0.f, 9000.f, "屏幕范围Y");
	}
}
bool cheat::feature::RoleMemory::NeedStatusDraw() const { return f_Transmit; }
void cheat::feature::RoleMemory::DrawStatus() {
	if (f_Transmit) {
		ImGui::Text("玩家传送");
	}
}
bool cheat::feature::RoleMemory::NeedInfoDraw() const { return Feature::NeedInfoDraw(); }
void cheat::feature::RoleMemory::DrawInfo() { Feature::DrawInfo(); }

bool hasObject;
ImVec2 screenPoint;
Role* target;
void cheat::feature::RoleMemory::DrawExternal() {
	const auto bg = ImGui::GetBackgroundDrawList();
	if (hasObject && f_Transmit) {
		DrawTextWithOutline(bg, screenPoint, "瞬移目标", ImColor{ 255, 0, 0, 200 }, 1, OutlineSide::All, ImColor{ 0, 0, 0 });
	}
	if (f_Transmit) bg->AddRect({ (renderer::windowWidth / 2 - f_TransmitX), (renderer::windowHeight / 2 - f_TransmitY) }, { (renderer::windowWidth / 2 + f_TransmitX.value()), (renderer::windowHeight / 2 + f_TransmitY.value()) }, ImColor{ 255, 255, 0 }, 0, 0, 2);
}

bool cheat::feature::RoleMemory::catchError = false;
void cheat::feature::RoleMemory::CallBack(const SEH_exception& s, PEXCEPTION_POINTERS& p) {
	LOG_ERROR("error");
}
void cheat::feature::RoleMemory::OnKeyUp(short key, bool& cancelled) {
	try {
		if (hasObject && target && f_Transmit && f_TransmitHotKey.value().IsPressed(key)) {
			catchError = false;
			VEH* pVEH = new VEH();
			pVEH->SetCallback(CallBack);
			pVEH->CaptureContext();
			if (catchError) {
				delete pVEH;
				return;
			}
			try {
				const float fovX = -RoleControl::y_camRot.Get(Role::MyRoleControl.Get(target));
				const II::Vector3 point = RoleLogic::NowPoint.Get(Role::roleLogic.Get(target));
				II::Vector3 topPoint1 = { point.x, point.y, point.z - f_TransmitDistance };
				ImVec2 newPoint;
				BaseMath::RotatePoint({ topPoint1.x, topPoint1.z }, { point.x, point.z }, fovX, newPoint);
				Role::localRole->GetTransform()->SetPosition({ newPoint.x, topPoint1.y, newPoint.y });
			} catch (...) { LOG_ERROR("error"); }
			delete pVEH;
		}
	} catch (...) { LOG_ERROR("error"); }
}

void cheat::feature::RoleMemory::Updata() {
	if (f_Transmit) {
		hasObject = false;
		target = nullptr;
		float len = std::numeric_limits<float>::max();

		II::Camera* cam = nullptr;

		try {
			auto cams = II::Camera::GetAllCamera();
			if (!cams.empty()) cam = cams[0];
		}
		catch (...) {
			LOG_ERROR("error");
			return;
		}

		auto vector = Role::vector.GetValue();
		// 预先分配数组以存储批量计算所需的值
		std::vector<II::Vector3> screenPoints(vector.size());
		std::vector<float> distances(vector.size());

		// 使用AVX2和MKL进行批量计算
		for (size_t i = 0; i < vector.size(); ++i) {
			auto player = vector[i];
			if (player == Role::localRole || IsBadReadPtr(Role::localRole, sizeof(void*)) || IsBadReadPtr(player, sizeof(void*))) continue;

			II::Vector3 xy = cam->WorldToScreenPoint(RoleLogic::NowPoint.Get(Role::roleLogic.Get(player)));
			xy.y = renderer::windowHeight - xy.y;

			if (xy.z < 0) continue;

			screenPoints[i] = xy;
			// 使用MKL函数计算距离
			distances[i] = cblas_snrm2(2, &xy.x, 1);
		}

		// 找到最近的目标
		for (size_t i = 0; i < vector.size(); ++i) {
			auto& xy = screenPoints[i];
			if ((xy.x > (renderer::windowWidth / 2 - f_TransmitX)) && (xy.x < (renderer::windowWidth / 2 + f_TransmitX.value())) && (xy.y > (renderer::windowHeight / 2 - f_TransmitY)) && (xy.y < (renderer::windowHeight / 2 + f_TransmitY.value()))) {
				if (distances[i] < len) {
					len = distances[i];
					screenPoint = { xy.x, xy.y };
					hasObject = true;
					target = vector[i];
				}
			}
		}
	}
	else {
		hasObject = false;
		target = nullptr;
	}
}

cheat::feature::RoleMemory::RoleMemory() :
	NF(f_Transmit, "玩家传送", "Role::Memory", false),
	NF(f_TransmitHotKey, "传送热键", "Role::Memory", Hotkey('T')),
	NF(f_TransmitDistance, "安全距离(M)", "Role::Memory", 5.f),
	NF(f_TransmitX, "屏幕范围X", "Role::Memory", 500.f),
	NF(f_TransmitY, "屏幕范围Y", "Role::Memory", 400.f)
{
	events::KeyUpEvent += MY_METHOD_HANDLER(cheat::feature::RoleMemory::OnKeyUp);
	Main::GameUpdateEvent += MY_METHOD_HANDLER(cheat::feature::RoleMemory::Updata);
}
