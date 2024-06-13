#include "About.h"

#include "../../../main.h"

#include "Base/render/gui-util.h"

namespace cheat::feature {
    const FeatureGUIInfo& About::GetGUIInfo() const {
        static const FeatureGUIInfo info{ "", "关于", false };
        return info;
    }

    void About::DrawMain() {
        ImGui::Text(reinterpret_cast<const char*>(u8"作者:"));
        ImGui::SameLine();
        ImGui::TextColored(ImColor(0, 102, 255, 255), reinterpret_cast<const char*>(u8"遂沫"));
        ImGui::Text("本程序完全免费禁止倒卖");
        ImGui::Text("本程序只在官方QQ群发布");
        ImGui::Text("其他渠道均为第三方搬运");
        ImGui::Text("搬运渠道不保证程序无毒");
        ImGui::Text("官方QQ群: 472659840");
        ImGui::Text("加入官方群更快获取更新");
        ImGui::Text("联系我请访问 遂沫.com");
    }

    About* About::GetInstance() {
        static About instance;
        return &instance;
    }
}
