#include "base.h"
#include "sdk/sdk.h"

#include "moduleManager/modules/visual/esp.h"
#include "moduleManager/modules/combat/aimAssist.h"

#include "../../ext/imgui/imgui.h"
#include "menu/menu.h"

void Base::RenderLoop() // Runs every frame
{
	if (!Base::Running) return;
        const char* watermark = "Made by Maggi";

        float margin = 10.f;
        float paddingX = 10.f;
        float paddingY = 5.f;
        float font_size = 24;

        ImVec2 screenSize = ImGui::GetWindowSize();
        ImVec2 textSize = Menu::Font->CalcTextSizeA(font_size, FLT_MAX, 0.0f, watermark);

        ImVec2 boxMin(screenSize.x - textSize.x - paddingX * 2 - margin, margin);
        ImVec2 boxMax(screenSize.x - margin, boxMin.y + textSize.y + paddingY * 2);

        ImColor boxFill(0, 0, 0, 180);
        ImColor boxOutline(255, 255, 255, 220);

        ImDrawList* drawList = ImGui::GetWindowDrawList();
        drawList->AddRectFilled(boxMin, boxMax, boxFill, 5.f);
        drawList->AddRect(boxMin, boxMax, boxOutline, 5.f);
        drawList->AddText(Menu::Font, font_size,
                ImVec2(boxMin.x + paddingX, boxMin.y + paddingY),
                ImColor(255, 255, 255), watermark);

	Esp::RenderUpdate();
	AimAssist::RenderUpdate();
}