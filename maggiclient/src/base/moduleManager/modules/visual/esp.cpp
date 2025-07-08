#include "esp.h"
#include "../../commonData.h"
#include "../../../sdk/sdk.h"
#include "../../../util/logger.h"
#include "../../../../../ext/imgui/imgui.h"
#include "../../../util/math/math.h"
#include "../../../util/math/worldToScreen.h"
#include "../../../util/render/renderqolf.h"
#include "../../../menu/menu.h"
#include <gl/GL.h>


void Esp::Update()
{
	if (!Enabled) return;
	if (!CommonData::SanityCheck()) return;

	CEntityPlayerSP* player = SDK::Minecraft->thePlayer;
	CWorld* world = SDK::Minecraft->theWorld;
	std::vector<CommonData::PlayerData> playerList = CommonData::nativePlayerList;
	if (playerList.empty()) return;

	Vector3 renderPos = CommonData::renderPos;
	Vector3 pos = player->GetPos();

	// This is to fix the third person issue, there is still one issue with it.
	// When the camera collides in the wall or the ground, or something like that, the calculation fails entirely because the position is obviously not the same.
	if (CommonData::thirdPersonView != 0) {
		Vector2 angles = player->GetAngles();
		float cameraDist = 8;
		if (CommonData::thirdPersonView == 2) {
			cameraDist = -cameraDist;
		}

		// This whole calculation came from gaspers source from their reach module, which you can find in this cheat as well
		float cos = std::cos(Math::degToRadiants(angles.x + 90.0f));
		float sin = std::sin(Math::degToRadiants(angles.x + 90.0f));
		float cosPitch = std::cos(Math::degToRadiants(angles.y));
		float sinPitch = std::sin(Math::degToRadiants(angles.y));

		float x = renderPos.x - (cos * cameraDist * cosPitch);
		float y = renderPos.y + (cameraDist * sinPitch);
		float z = renderPos.z - (sin * cameraDist * cosPitch);

		// The raycast that is commented out below does not work that well for some reason, acts weirdly when colliding with chests, and other things.
		// Also might be poor in performance.
		//Vector3 ray = world->rayTraceBlocks(renderPos, Vector3{ x, y, z }, false, false, false);
		//renderPos = ray.x == 0 ? Vector3{ x,y,z } : ray;

		renderPos = Vector3{ x,y,z };
	}

        // container to hold all rendering data for this frame
        std::vector<EntityVisual> newData;

	float renderPartialTicks = CommonData::renderPartialTicks;

        for (const CommonData::PlayerData& entity : playerList)
	{
		Vector3 entityPos = entity.pos;
		Vector3 entityLastPos = entity.lastPos;

		float entityWidth = 0.7f;
		float entityHeight = (float)(entity.height / 2) + 0.2f;

		Vector3 diff = pos - entityPos;
		float distanceVal = sqrt(pow(diff.x, 2) + pow(diff.y, 2) + pow(diff.z, 2)); // Sqrt((x2 - x1)^2 + (y2 - y1)^2 + (z2 - z1)^2)
		// Regular distance check.
		if (distanceVal > 300) {
			continue;
		}

		// You can't put one vector3 for fixing the jittering and use that to subtract all of them with it
		// it will mess screw up for some weird reason. Try it for yourself if you wish!
		//Vector3 fixedEntityPos{ entityLastPos + (entityLastPos - entityPos) * renderPartialTicks };

		// This stops the jittering, the calculations must be inverted for some reason from the original
		Vector3 origin{ renderPos - entityLastPos + (entityLastPos - entityPos) * renderPartialTicks }; // At the feet

		// Same with the offset of the point, it must be offset from the render position, not the entity position for some weird reason.
		Vector3 top{ (renderPos - Vector3{0, entityHeight * 2, 0}) - entityLastPos + (entityLastPos - entityPos) * renderPartialTicks }; // Over the head
		Vector3 left{ (renderPos - Vector3{entityWidth, entityHeight, 0}) - entityLastPos + (entityLastPos - entityPos) * renderPartialTicks }; // In the middle to the left
		Vector3 right{ (renderPos - Vector3{-entityWidth, entityHeight, 0}) - entityLastPos + (entityLastPos - entityPos) * renderPartialTicks }; // In the middle to the right
		Vector3 back{ (renderPos - Vector3{0, entityHeight, entityWidth}) - entityLastPos + (entityLastPos - entityPos) * renderPartialTicks }; // In the middle to the back
		Vector3 front{ (renderPos - Vector3{0, entityHeight, -entityWidth}) - entityLastPos + (entityLastPos - entityPos) * renderPartialTicks }; // And in the middle to the front

		entityWidth /= 1.388888;
		Vector3 left2{ (renderPos - Vector3{entityWidth, entityHeight, entityWidth}) - entityLastPos + (entityLastPos - entityPos) * renderPartialTicks }; // In the middle to the left
		Vector3 right2{ (renderPos - Vector3{-entityWidth, entityHeight, -entityWidth}) - entityLastPos + (entityLastPos - entityPos) * renderPartialTicks }; // In the middle to the right
		Vector3 back2{ (renderPos - Vector3{-entityWidth, entityHeight, entityWidth}) - entityLastPos + (entityLastPos - entityPos) * renderPartialTicks }; // In the middle to the back
		Vector3 front2{ (renderPos - Vector3{entityWidth, entityHeight, -entityWidth}) - entityLastPos + (entityLastPos - entityPos) * renderPartialTicks }; // And in the middle to the front

		// ^ This shapes a diamond like formation which results in 6 world to screens instead of 8.
		// However if a 3d ESP is desired, 8 world to screens would be required.

		// Another note for this data, is we cannot use the bounding box values because it can be changed by the reach module, so its best we make our own values with the cost
		// of consuming a little bit of resources for a bit of math.
		std::vector<Vector3> boxVertices{
			origin, top, left, right, back, front, left2, right2,back2, front2
		};

		// For when the player gets close to an entity, a fade factor; a value between 0 and 1, with basic math, can get a cool looking fade effect if the player is too close
		// or inside the FadeDistance radius.
		float fadeFactor = 1.0f;
		if ((distanceVal - 1) <= FadeDistance)
			fadeFactor = ((distanceVal - 1) / FadeDistance);

		// To render the distance value under the ESP box.
		char distC[32];
		std::snprintf(distC, sizeof(distC), "%.1f", distanceVal);
		std::string distS(distC);

		// Then finally taking all the data we acquired for this loop and pushing it to the data list.
		newData.push_back(EntityVisual{
			boxVertices, // Box data
			entity.name, // Entity name
			distS + "m", // Distance
			distanceVal, // Real distance value (for fade)
			fadeFactor, // Fade factor
			entity.health, // Entity health
			entity.maxHealth, // And max health (for health bar)
			});
	}
	renderQueue = newData;
}


// Funktion zum Berechnen von Regenbogenfarben
static ImColor GetRainbowColor(float offset = 0.0f)
{
    float time = ImGui::GetTime() + offset;
    float r = (std::sin(time * 2.0f) * 0.5f) + 0.5f;
    float g = (std::sin(time * 2.0f + 2.0944f) * 0.5f) + 0.5f; // 2.0944 rad = 120
    float b = (std::sin(time * 2.0f + 4.1888f) * 0.5f) + 0.5f; // 4.1888 rad = 240
    return ImColor(r, g, b, 1.0f); // Volle Deckkraft
}

void Esp::RenderUpdate()
{
    if (!Enabled || !CommonData::dataUpdated) return;

    ImVec2 screenSize = ImGui::GetWindowSize();

    // Neue Tracer-Startposition basierend auf Einstellung
    ImVec2 tracerStartPosition;
    switch (TracerStartPosition) {
    case 0: // Top of Screen
        tracerStartPosition = ImVec2(screenSize.x / 2.0f, 0);
        break;
    case 1: // Bottom of Screen
        tracerStartPosition = ImVec2(screenSize.x / 2.0f, screenSize.y);
        break;
    case 2: // Center of Screen
        tracerStartPosition = ImVec2(screenSize.x / 2.0f, screenSize.y / 2.0f);
        break;
    default:
        tracerStartPosition = ImVec2(screenSize.x / 2.0f, screenSize.y);
        break;
    }

    for (const EntityVisual& data : renderQueue)
    {
        std::vector<Vector3> bv = data.boxVertices;

        float left = FLT_MAX;
        float top = FLT_MAX;
        float right = FLT_MIN;
        float bottom = FLT_MIN;

        bool skip = false;
        for (const Vector3& position : bv)
        {
            Vector2 p;
            if (!CWorldToScreen::WorldToScreen(position, CommonData::modelView, CommonData::projection, (int)screenSize.x, (int)screenSize.y, p))
            {
                skip = true;
                break;
            }

            left = fmin(p.x, left);
            top = fmin(p.y, top);
            right = fmax(p.x, right);
            bottom = fmax(p.y, bottom);
        }

        if (skip) continue;

        // Box und Farben
        ImColor boxColor = GayMode ? GetRainbowColor() : ImColor(BoxColor[0], BoxColor[1], BoxColor[2], BoxColor[3] * data.opacityFactor);
        ImColor healthBarColor = GayMode ? GetRainbowColor(0.5f) : ImColor(HealthBarColor[0], HealthBarColor[1], HealthBarColor[2], HealthBarColor[3] * data.opacityFactor);
        ImColor outlineColor = GayMode ? GetRainbowColor(1.0f) : ImColor(OutlineColor[0], OutlineColor[1], OutlineColor[2], OutlineColor[3] * data.opacityFactor);
        ImColor textColor = GayMode ? GetRainbowColor(1.5f) : ImColor(TextColor[0], TextColor[1], TextColor[2], TextColor[3] * data.opacityFactor);
        ImColor tracerColor = GayMode ? GetRainbowColor(2.0f) : ImColor(TracerColor[0], TracerColor[1], TracerColor[2], TracerColor[3]);

        // Tracer-Logik
        if (DrawTracers)
        {
            ImVec2 boxCenter((left + right) / 2, (top + bottom) / 2); // Mitte der Box
            ImGui::GetWindowDrawList()->AddLine(tracerStartPosition, boxCenter, tracerColor, 2.0f);
        }

        // FilledBox nur ohne GayMode oder nicht aktiviert
        if (FilledBox && !GayMode)
        {
            ImColor bottomColor = GayMode ? GetRainbowColor(0.2f) : ImColor(SecondFilledBoxColor[0], SecondFilledBoxColor[1], SecondFilledBoxColor[2], FilledBoxOpacity * data.opacityFactor);
            ImColor topColor = GayMode ? GetRainbowColor(0.3f) : ImColor(FilledBoxColor[0], FilledBoxColor[1], FilledBoxColor[2], FilledBoxOpacity * data.opacityFactor);
            ImGui::GetWindowDrawList()->AddRectFilledMultiColor(ImVec2(left, top), ImVec2(right, bottom), topColor, topColor, bottomColor, bottomColor);
        }

        // Box nur mit Umrandung (Kein Fill bei GayMode)
        if (Box)
        {
            ImGui::GetWindowDrawList()->AddRect(ImVec2(left, top), ImVec2(right, bottom), boxColor);
        }

        // Outline (immer in Regenbogenfarbe, wenn GayMode aktiv)
        if (Outline)
        {
            ImGui::GetWindowDrawList()->AddRect(ImVec2(left - 1, top - 1), ImVec2(right + 1, bottom + 1), outlineColor);
            ImGui::GetWindowDrawList()->AddRect(ImVec2(left + 1, top + 1), ImVec2(right - 1, bottom - 1), outlineColor);
        }

        // Healthbar
        if (HealthBar)
        {
            float scaleFactor = data.health / data.maxHealth;
            float diff = bottom - top;
            ImGui::GetWindowDrawList()->AddRectFilled(ImVec2(left - 3, bottom - (diff * scaleFactor)), ImVec2(left - 1, bottom), healthBarColor);
        }

        // Text (Name) mit Regenbogenfarbe
        if (Text && Menu::Font->IsLoaded())
        {
            const char* name = data.name.c_str();
            ImVec2 textSize = Menu::Font->CalcTextSizeA(TextSize, FLT_MAX, 0.0f, name);
            float posX = left + ((right - left) / 2) - (textSize.x / 2);
            float posY = top - textSize.y - 1;

            if (data.distance > TextUnrenderDistance)
            {
                if (TextOutline)
                {
                    RenderQOLF::DrawOutlinedText(Menu::Font, TextSize, ImVec2(posX, posY), textColor, outlineColor, name);
                }
                else
                {
                    RenderQOLF::DrawShadowedText(Menu::Font, TextSize, ImVec2(posX, posY), textColor, name);
                }
            }

            // Zeichne den Distanztext
            const char* distanceStr = data.distanceText.c_str();
            float distanceTextSize = TextSize / 1.5;
            textSize = Menu::Font->CalcTextSizeA(distanceTextSize, FLT_MAX, 0.0f, distanceStr);
            posX = left + ((right - left) / 2) - (textSize.x / 2);
            posY = bottom;

            if (TextOutline)
            {
                RenderQOLF::DrawOutlinedText(Menu::Font, distanceTextSize, ImVec2(posX, posY), textColor, outlineColor, distanceStr);
            }
            else
            {
                RenderQOLF::DrawShadowedText(Menu::Font, distanceTextSize, ImVec2(posX, posY), textColor, distanceStr);
            }
        }
    }
}








void Esp::RenderMenu()
{
    ImGui::BeginGroup();
    ImGui::SetCursorPosY(ImGui::GetCursorPosY() + 20);
    ImGui::PushStyleColor(ImGuiCol_ChildBg, ImVec4(0.12f, 0.12f, 0.12f, 0.5));
    ImGui::PushStyleVar(ImGuiStyleVar_ChildRounding, 10);
    if (ImGui::BeginChild("esp", ImVec2(425, 300))) { // Adjusted height to fit the new option
        ImGui::SetCursorPosY(ImGui::GetCursorPosY() + 3);
        Menu::DoToggleButtonStuff(28374, "Toggle ESP", &Esp::Enabled);
        ImGui::SetCursorPosY(ImGui::GetCursorPosY() + 5);
        ImGui::Separator();
        Menu::DoToggleButtonStuff(23445, "Show Healthbar", &Esp::HealthBar);
        Menu::DoToggleButtonStuff(34576, "Show Text", &Esp::Text);
        Menu::DoSliderStuff(34875, "Fade Distance", &Esp::FadeDistance, 0, 10);
        Menu::DoSliderStuff(128763, "Text Size", &Esp::TextSize, 12, 24);

        ImGui::ColorEdit4("Box Color", BoxColor.data());
        ImGui::ColorEdit4("Healthbar Color", HealthBarColor.data());


        // Added "Draw Tracers" toggle
        Menu::DoToggleButtonStuff(192837, "Draw Tracers", &Esp::DrawTracers);
        if (Esp::DrawTracers) {
            ImGui::ColorEdit4("Tracer Color", TracerColor.data()); // Color picker for tracers

            // Dropdown for tracer starting position
            static const char* tracerOptions[] = { "Top of Screen", "Bottom of Screen", "Middle of Screen" };
            static int selectedTracerOption = 1; // Default: Bottom of Screen
            if (ImGui::BeginCombo("Tracer Start Position", tracerOptions[selectedTracerOption])) {
                for (int i = 0; i < IM_ARRAYSIZE(tracerOptions); i++) {
                    bool isSelected = (selectedTracerOption == i);
                    if (ImGui::Selectable(tracerOptions[i], isSelected)) {
                        selectedTracerOption = i;
                        Esp::TracerStartPosition = i; // Update tracer start position
                    }
                    if (isSelected) {
                        ImGui::SetItemDefaultFocus();
                    }
                }
                ImGui::EndCombo();
            }
        }

        ImGui::EndChild();
    }
    ImGui::PopStyleVar();
    ImGui::PopStyleColor();
    ImGui::EndGroup();
}

