#pragma once

#include "../../../../ext/imgui/imgui.h"

// Render quality of life functions

class RenderQOLF {
public:
        static void DrawOutlinedText(ImFont* font, float textSize, ImVec2 pos, ImColor color, ImColor outlineColor, const char* text) {
                ImGui::GetWindowDrawList()->AddText(font, textSize, ImVec2(pos.x + 1, pos.y), outlineColor, text);
                ImGui::GetWindowDrawList()->AddText(font, textSize, ImVec2(pos.x - 1, pos.y), outlineColor, text);
                ImGui::GetWindowDrawList()->AddText(font, textSize, ImVec2(pos.x, pos.y + 1), outlineColor, text);
                ImGui::GetWindowDrawList()->AddText(font, textSize, ImVec2(pos.x, pos.y - 1), outlineColor, text);

                ImGui::GetWindowDrawList()->AddText(font, textSize, pos, color, text);
        }

        // Draw text with a subtle shadow for a cleaner look
        static void DrawShadowedText(ImFont* font, float textSize, ImVec2 pos, ImColor color, const char* text) {
                ImColor shadow(0, 0, 0, 160);
                ImGui::GetWindowDrawList()->AddText(font, textSize, ImVec2(pos.x + 1, pos.y + 1), shadow, text);
                ImGui::GetWindowDrawList()->AddText(font, textSize, pos, color, text);
        }};