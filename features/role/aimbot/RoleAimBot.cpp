#include "RoleAimBot.h"

#include "BaseMath.h"

#include "../../../CLASS/Role.h"

cheat::feature::RoleAimBot* cheat::feature::RoleAimBot::GetInstance() {
	static RoleAimBot role;
	return &role;
}
const cheat::FeatureGUIInfo& cheat::feature::RoleAimBot::GetGUIInfo() const {
	static const FeatureGUIInfo info{ "自瞄", "玩家", true };
	return info;
}
void cheat::feature::RoleAimBot::DrawMain() {
	ConfigWidget(f_AimBot, "启用自瞄");
	ImGui::SameLine();
	ConfigWidget(f_AimBotHotKey, "自瞄热键");
	if (f_AimBot) {
		ConfigWidget(f_AimBotLock, "锁定自瞄玩家在自瞄结束前不切换目标");
		ImGui::SameLine();
		ConfigWidget(f_AimBotWeak, "自瞄已经倒地的玩家");
		ConfigWidget(f_AimBotX, .1f, 0.f, 9000.f, "屏幕范围X");
		ConfigWidget(f_AimBotY, .1f, 0.f, 9000.f, "屏幕范围Y");
		ConfigWidget(f_AimBotSpeedY, .1f, 1.f, 9000.f, "越低越快");
		ConfigWidget(f_AimBotSpeedX, .1f, 1.f, 9000.f, "越低越快");
		ConfigWidget(f_AimBotPrejudgment, .001f, 0.000f, 1.f, "越低越快 0为关闭");
	}
}
bool cheat::feature::RoleAimBot::NeedStatusDraw() const { return Feature::NeedStatusDraw(); }
void cheat::feature::RoleAimBot::DrawStatus() { Feature::DrawStatus(); }
bool cheat::feature::RoleAimBot::NeedInfoDraw() const { return Feature::NeedInfoDraw(); }
void cheat::feature::RoleAimBot::DrawInfo() { Feature::DrawInfo(); }

bool hasObject_;
ImVec2 screenPoint_;
Role* target_;
ImVec2 previousScreenPoint_;
Role* previousTarget_;
II::Vector2 newPoint;
void cheat::feature::RoleAimBot::DrawExternal() {
    const auto bg = ImGui::GetBackgroundDrawList();
    if (hasObject_ && f_AimBot) {
        bg->AddCircleFilled(screenPoint_, 5, ImColor{ 255, 0, 0 }, 15);
        bg->AddCircleFilled({ newPoint.x, newPoint.y }, 5, ImColor{ 0, 255, 255 }, 15);
    }
    if (f_AimBot) {
        bg->AddRect({ (renderer::windowWidth / 2 - f_AimBotX), (renderer::windowHeight / 2 - f_AimBotY) }, { (renderer::windowWidth / 2 + f_AimBotX.value()), (renderer::windowHeight / 2 + f_AimBotY.value()) }, ImColor{ 0, 255, 0 }, 0, 0, 2);
    }
}

void cheat::feature::RoleAimBot::Updata() {
    if (f_AimBot) {
        hasObject_ = false;
        float len = std::numeric_limits<float>::max();

        II::Camera* cam = nullptr;

        try {
            if (const auto cams = II::Camera::GetAllCamera(); !cams.empty()) {
                cam = cams[0];
            }
        }
        catch (...) {
            LOG_ERROR("Failed to get cameras");
            return;
        }

        auto vector = Role::vector.GetValue();

        if (f_AimBotLock && target_ && f_AimBotHotKey.value().IsPressed() && std::ranges::find(vector, target_) == vector.end()) {
            target_ = nullptr;
            hasObject_ = false;
        }

        for (auto player : vector) {
            try {
                if (f_AimBotLock && hasObject_ && f_AimBotHotKey.value().IsPressed() && player != target_) continue;
                if (player == Role::localRole ||
                    IsBadReadPtr(Role::localRole, sizeof(void*)) ||
                    IsBadReadPtr(player, sizeof(void*)) ||
                    RoleLogic::TeamNum.Get(Role::roleLogic.Get(player)) == RoleLogic::TeamNum.Get(Role::roleLogic.Get(Role::localRole))) continue;

                bool weak = (RoleLogic::hp.Get(Role::roleLogic.Get(player)) < 0 && RoleLogic::WeakValue.Get(Role::roleLogic.Get(player)) < 100);
                if (weak && !f_AimBotWeak) continue;

                II::Transform* pHead = AnimatorControl::animator.Get(RoleControl::animatorControl.Get(Role::MyRoleControl.Get(player)))->GetBoneTransform(II::Animator::HumanBodyBones::Head);
                II::Vector3 headPoint = pHead->GetPosition();
                II::Vector3 xy = weak ? cam->WorldToScreenPoint(headPoint - II::Vector3{ 0, 1, 0 }) : cam->WorldToScreenPoint(headPoint + II::Vector3{ 0, 0.15, 0 });
                xy.y = renderer::windowHeight - xy.y;

                if (xy.z < 0) continue;

                const float oLen = std::hypot(xy.x - renderer::windowWidth / 2, xy.y - renderer::windowHeight / 2);
                if (xy.x > renderer::windowWidth / 2 - f_AimBotX && xy.x < renderer::windowWidth / 2 + f_AimBotX.value() && xy.y > renderer::windowHeight / 2 - f_AimBotY && xy.y < renderer::windowHeight / 2 + f_AimBotY.value()) {
                    if (oLen < len) {
                        len = oLen;
                        screenPoint_ = { xy.x, xy.y };
                        hasObject_ = true;
                        target_ = player;
                    }
                }
            } catch (...) { }
        }

        if (!((screenPoint_.x > renderer::windowWidth / 2 - f_AimBotX.value() && screenPoint_.x < renderer::windowWidth / 2 + f_AimBotX.value()) &&
            (screenPoint_.y > renderer::windowHeight / 2 - f_AimBotY.value() && screenPoint_.y < renderer::windowHeight / 2 + f_AimBotY.value()))) {
            hasObject_ = false;
            target_ = nullptr;
            previousTarget_ = nullptr;
            return;
        }

        if (target_ != previousTarget_) {
            previousTarget_ = target_;
            previousScreenPoint_ = screenPoint_;
        }

        newPoint = BaseMath::PredictNextPosition({ screenPoint_.x, 0 }, { previousScreenPoint_.x, 0 }, f_AimBotPrejudgment);
        newPoint.y = screenPoint_.y;
        previousScreenPoint_ = screenPoint_;

        if (hasObject_ && target_) {
	        if (f_AimBotHotKey.value().IsPressed()) {
                const long MouseX = static_cast<long>((newPoint.x - renderer::windowWidth / 2) / f_AimBotSpeedX);
                const long MouseY = static_cast<long>((newPoint.y - renderer::windowHeight / 2) / f_AimBotSpeedY);
                mouse_event(MOUSEEVENTF_MOVE, MouseX, MouseY, 0, 0);
	        }
        }
    }
    else {
        hasObject_ = false;
        target_ = nullptr;
        previousTarget_ = nullptr;
    }
}

cheat::feature::RoleAimBot::RoleAimBot() :
	NF(f_AimBot, "启用自瞄", "Role::AimBot", false),
	NF(f_AimBotLock, "自瞄锁定", "Role::AimBot", true),
	NF(f_AimBotWeak, "自瞄倒地", "Role::AimBot", false),
	NF(f_AimBotHotKey, "自瞄热键", "Role::AimBot", Hotkey(VK_LSHIFT)),
	NF(f_AimBotX, "屏幕范围X", "Role::AimBot", 400.f),
	NF(f_AimBotY, "屏幕范围Y", "Role::AimBot", 300.f),
	NF(f_AimBotSpeedY, "垂直速度", "Role::AimBot", 14.f),
	NF(f_AimBotSpeedX, "水平速度", "Role::AimBot", 14.f),
	NF(f_AimBotPrejudgment, "预判倍数", "Role::AimBot", 1.f)
{
	Main::GameUpdateEvent += MY_METHOD_HANDLER(cheat::feature::RoleAimBot::Updata);
}
