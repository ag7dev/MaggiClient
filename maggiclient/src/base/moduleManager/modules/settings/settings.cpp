#include "settings.h"
#define _CRT_SECURE_NO_WARNINGS
#include "../../../../../ext/imgui/imgui.h"
#include "../../../menu/menu.h" 
#include <fstream>
#include <iostream> 
#include <sstream> 
#include <filesystem> // Für Dateisystemoperationen
#include "../../../moduleManager/modules/visual/esp.h" 
#include "../../../moduleManager/modules/combat/reach.h" 
#include "../../../moduleManager/modules/combat/aimassist.h" 
#include "../../../moduleManager/modules/clicker/leftAutoClicker.h" 
#include "../../../moduleManager/modules/clicker/rightAutoClicker.h" 

namespace fs = std::filesystem; // Kürzel für die Nutzung von std::filesystem

void Settings::RenderMenu() {
    ImGui::BeginGroup();
    ImGui::SetCursorPosY(ImGui::GetCursorPosY() + 20);
    ImGui::PushStyleColor(ImGuiCol_ChildBg, ImVec4(0.12f, 0.12f, 0.12f, 0.5));
    ImGui::PushStyleVar(ImGuiStyleVar_ChildRounding, 10);

    if (ImGui::BeginChild("settings", ImVec2(425, 300))) {  // Erhöhte Höhe für mehr Platz
        ImGui::SetCursorPosY(ImGui::GetCursorPosY() + 3);

        // Toggle für das Aktivieren der Einstellungen
        Menu::DoToggleButtonStuff(12345, "Enable Settings", &Settings::Enabled);

        // Button zum Laden der Konfiguration
        if (ImGui::Button("Load Config")) {
            fs::path configPath = fs::path(getenv("USERPROFILE")) / "Desktop" / "maggiclient" / "maggiclientconfig.txt";
            std::ifstream inputFile(configPath);
            if (inputFile) {
                std::string line;
                while (std::getline(inputFile, line)) {
                    if (line.empty() || line[0] == '#') continue; // Kommentare überspringen
                    size_t delimiterPos = line.find('=');
                    if (delimiterPos != std::string::npos) {
                        std::string key = line.substr(0, delimiterPos);
                        std::string value = line.substr(delimiterPos + 1);


                        if (key == "esp_enabled") {
                            Esp::Enabled = (value == "true");
                        }
                        else if (key == "box_color") {
                            std::istringstream iss(value);
                            iss >> Esp::BoxColor[0] >> Esp::BoxColor[1] >> Esp::BoxColor[2] >> Esp::BoxColor[3];
                        }
                        else if (key == "reach_enabled") {
                            Reach::Enabled = (value == "true");
                        }
                        else if (key == "reach_distance") {
                            Reach::ReachDistance = std::stof(value);
                        }
                        else if (key == "aimassist_enabled") {
                            AimAssist::Enabled = (value == "true");
                        }
                        else if (key == "aim_fov") {
                            AimAssist::fov = std::stof(value);
                        }
                        else if (key == "right_clicker_enabled") {
                            RightAutoClicker::Enabled = (value == "true");
                        }
                        else if (key == "right_max_cps") {
                            RightAutoClicker::rightMaxCps = std::stof(value);
                        }
                        else if (key == "right_min_cps") {
                            RightAutoClicker::rightMinCps = std::stof(value);
                        }
                        else if (key == "right_blocks_only") {
                            RightAutoClicker::blocksOnly = (value == "true");
                        }
                        else if (key == "left_clicker_enabled") {
                            LeftAutoClicker::Enabled = (value == "true");
                        }
                        else if (key == "left_max_cps") {
                            LeftAutoClicker::leftMaxCps = std::stof(value);
                        }
                        else if (key == "left_min_cps") {
                            LeftAutoClicker::leftMinCps = std::stof(value);
                        }
                        else if (key == "left_ignore_blocks") {
                            LeftAutoClicker::ignoreBlocks = (value == "true");
                        }
                        // Additional ESP settings
                        else if (key == "esp_box") {
                            Esp::Box = (value == "true");
                        }
                        else if (key == "filled_box") {
                            Esp::FilledBox = (value == "true");
                        }
                        else if (key == "filled_box_color") {
                            std::istringstream iss(value);
                            iss >> Esp::FilledBoxColor[0] >> Esp::FilledBoxColor[1] >> Esp::FilledBoxColor[2];
                        }
                        else if (key == "second_filled_box_color") {
                            std::istringstream iss(value);
                            iss >> Esp::SecondFilledBoxColor[0] >> Esp::SecondFilledBoxColor[1] >> Esp::SecondFilledBoxColor[2];
                        }
                        else if (key == "filled_box_opacity") {
                            Esp::FilledBoxOpacity = std::stof(value);
                        }
                        else if (key == "outline") {
                            Esp::Outline = (value == "true");
                        }
                        else if (key == "outline_color") {
                            std::istringstream iss(value);
                            iss >> Esp::OutlineColor[0] >> Esp::OutlineColor[1] >> Esp::OutlineColor[2] >> Esp::OutlineColor[3];
                        }
                        else if (key == "esp_text") {
                            Esp::Text = (value == "true");
                        }
                        else if (key == "text_size") {
                            Esp::TextSize = std::stof(value);
                        }
                        else if (key == "text_color") {
                            std::istringstream iss(value);
                            iss >> Esp::TextColor[0] >> Esp::TextColor[1] >> Esp::TextColor[2] >> Esp::TextColor[3];
                        }
                        else if (key == "text_outline") {
                            Esp::TextOutline = (value == "true");
                        }
                        else if (key == "text_outline_color") {
                            std::istringstream iss(value);
                            iss >> Esp::TextOutlineColor[0] >> Esp::TextOutlineColor[1] >> Esp::TextOutlineColor[2] >> Esp::TextOutlineColor[3];
                        }
                        else if (key == "text_unrender_distance") {
                            Esp::TextUnrenderDistance = std::stof(value);
                        }
                        else if (key == "fade_distance") {
                            Esp::FadeDistance = std::stof(value);
                        }
                        else if (key == "health_bar") {
                            Esp::HealthBar = (value == "true");
                        }
                        else if (key == "healthbar_color") {
                            std::istringstream iss(value);
                            iss >> Esp::HealthBarColor[0] >> Esp::HealthBarColor[1] >> Esp::HealthBarColor[2] >> Esp::HealthBarColor[3];
                        }
                        else if (key == "name_tag_box") {
                            Esp::NameTagBox = (value == "true");
                        }
                        else if (key == "name_tag_box_color") {
                            std::istringstream iss(value);
                            iss >> Esp::NameTagBoxColor[0] >> Esp::NameTagBoxColor[1] >> Esp::NameTagBoxColor[2] >> Esp::NameTagBoxColor[3];
                        }
                        else if (key == "name_tag_outline_color") {
                            std::istringstream iss(value);
                            iss >> Esp::NameTagBoxOutlineColor[0] >> Esp::NameTagBoxOutlineColor[1] >> Esp::NameTagBoxOutlineColor[2] >> Esp::NameTagBoxOutlineColor[3];
                        }
                        else if (key == "gay_mode") {
                            Esp::GayMode = (value == "true");
                        }
                        else if (key == "rainbow_speed") {
                            Esp::RainbowSpeed = std::stof(value);
                        }
                        else if (key == "draw_tracers") {
                            Esp::DrawTracers = (value == "true");
                        }
                        else if (key == "tracer_color") {
                            std::istringstream iss(value);
                            iss >> Esp::TracerColor[0] >> Esp::TracerColor[1] >> Esp::TracerColor[2] >> Esp::TracerColor[3];
                        }
                        else if (key == "tracer_start_position") {
                            Esp::TracerStartPosition = std::stoi(value);
                        }
                        // AimAssist settings
                        else if (key == "aim_visibility_check") {
                            AimAssist::visibilityCheck = (value == "true");
                        }
                        else if (key == "aim_feedback") {
                            AimAssist::aimAssistFeedback = (value == "true");
                        }
                        else if (key == "aim_fov_circle") {
                            AimAssist::fovCircle = (value == "true");
                        }
                        else if (key == "aim_key") {
                            AimAssist::aimKey = (value == "true");
                        }
                        else if (key == "aim_adaptive") {
                            AimAssist::adaptive = (value == "true");
                        }
                        else if (key == "aim_adaptive_offset") {
                            AimAssist::adaptiveOffset = std::stof(value);
                        }
                        else if (key == "aim_smooth") {
                            AimAssist::smooth = std::stof(value);
                        }
                        else if (key == "aim_distance") {
                            AimAssist::aimDistance = std::stof(value);
                        }
                        else if (key == "aim_random_yaw") {
                            AimAssist::randomYaw = std::stof(value);
                        }
                        else if (key == "aim_random_pitch") {
                            AimAssist::randomPitch = std::stof(value);
                        }
                        else if (key == "aim_target_priority") {
                            AimAssist::targetPriority = std::stoi(value);
                        }
                    }
                }
                inputFile.close();
                ImGui::Text("Config loaded successfully!");
            }
            else {
                ImGui::Text("Config file not found!");
            }
        }

        // Button zum Speichern der Konfiguration
        if (ImGui::Button("Save Config")) {
            fs::path configPath = fs::path(getenv("USERPROFILE")) / "Desktop" / "maggiclient" / "maggiclientconfig.txt";
            fs::create_directories(configPath.parent_path());

            std::ofstream outputFile(configPath);
            if (outputFile) {
                outputFile << "esp_enabled=" << (Esp::Enabled ? "true" : "false") << std::endl;
                outputFile << "box_color=" << Esp::BoxColor[0] << " " << Esp::BoxColor[1] << " " << Esp::BoxColor[2] << " " << Esp::BoxColor[3] << std::endl;
                outputFile << "reach_enabled=" << (Reach::Enabled ? "true" : "false") << std::endl;
                outputFile << "reach_distance=" << Reach::ReachDistance << std::endl;
                outputFile << "aimassist_enabled=" << (AimAssist::Enabled ? "true" : "false") << std::endl;
                outputFile << "aim_fov=" << AimAssist::fov << std::endl;
                outputFile << "right_clicker_enabled=" << (RightAutoClicker::Enabled ? "true" : "false") << std::endl;
                outputFile << "right_max_cps=" << RightAutoClicker::rightMaxCps << std::endl;
                outputFile << "right_min_cps=" << RightAutoClicker::rightMinCps << std::endl;
                outputFile << "right_blocks_only=" << (RightAutoClicker::blocksOnly ? "true" : "false") << std::endl;
                outputFile << "left_clicker_enabled=" << (LeftAutoClicker::Enabled ? "true" : "false") << std::endl;
                outputFile << "left_max_cps=" << LeftAutoClicker::leftMaxCps << std::endl;
                outputFile << "left_min_cps=" << LeftAutoClicker::leftMinCps << std::endl;
                outputFile << "left_ignore_blocks=" << (LeftAutoClicker::ignoreBlocks ? "true" : "false") << std::endl;

                // ESP settings
                outputFile << "esp_box=" << (Esp::Box ? "true" : "false") << std::endl;
                outputFile << "filled_box=" << (Esp::FilledBox ? "true" : "false") << std::endl;
                outputFile << "filled_box_color=" << Esp::FilledBoxColor[0] << " " << Esp::FilledBoxColor[1] << " " << Esp::FilledBoxColor[2] << std::endl;
                outputFile << "second_filled_box_color=" << Esp::SecondFilledBoxColor[0] << " " << Esp::SecondFilledBoxColor[1] << " " << Esp::SecondFilledBoxColor[2] << std::endl;
                outputFile << "filled_box_opacity=" << Esp::FilledBoxOpacity << std::endl;
                outputFile << "outline=" << (Esp::Outline ? "true" : "false") << std::endl;
                outputFile << "outline_color=" << Esp::OutlineColor[0] << " " << Esp::OutlineColor[1] << " " << Esp::OutlineColor[2] << " " << Esp::OutlineColor[3] << std::endl;
                outputFile << "esp_text=" << (Esp::Text ? "true" : "false") << std::endl;
                outputFile << "text_size=" << Esp::TextSize << std::endl;
                outputFile << "text_color=" << Esp::TextColor[0] << " " << Esp::TextColor[1] << " " << Esp::TextColor[2] << " " << Esp::TextColor[3] << std::endl;
                outputFile << "text_outline=" << (Esp::TextOutline ? "true" : "false") << std::endl;
                outputFile << "text_outline_color=" << Esp::TextOutlineColor[0] << " " << Esp::TextOutlineColor[1] << " " << Esp::TextOutlineColor[2] << " " << Esp::TextOutlineColor[3] << std::endl;
                outputFile << "text_unrender_distance=" << Esp::TextUnrenderDistance << std::endl;
                outputFile << "fade_distance=" << Esp::FadeDistance << std::endl;
                outputFile << "health_bar=" << (Esp::HealthBar ? "true" : "false") << std::endl;
                outputFile << "healthbar_color=" << Esp::HealthBarColor[0] << " " << Esp::HealthBarColor[1] << " " << Esp::HealthBarColor[2] << " " << Esp::HealthBarColor[3] << std::endl;
                outputFile << "name_tag_box=" << (Esp::NameTagBox ? "true" : "false") << std::endl;
                outputFile << "name_tag_box_color=" << Esp::NameTagBoxColor[0] << " " << Esp::NameTagBoxColor[1] << " " << Esp::NameTagBoxColor[2] << " " << Esp::NameTagBoxColor[3] << std::endl;
                outputFile << "name_tag_outline_color=" << Esp::NameTagBoxOutlineColor[0] << " " << Esp::NameTagBoxOutlineColor[1] << " " << Esp::NameTagBoxOutlineColor[2] << " " << Esp::NameTagBoxOutlineColor[3] << std::endl;
                outputFile << "gay_mode=" << (Esp::GayMode ? "true" : "false") << std::endl;
                outputFile << "rainbow_speed=" << Esp::RainbowSpeed << std::endl;
                outputFile << "draw_tracers=" << (Esp::DrawTracers ? "true" : "false") << std::endl;
                outputFile << "tracer_color=" << Esp::TracerColor[0] << " " << Esp::TracerColor[1] << " " << Esp::TracerColor[2] << " " << Esp::TracerColor[3] << std::endl;
                outputFile << "tracer_start_position=" << Esp::TracerStartPosition << std::endl;

                // AimAssist settings
                outputFile << "aim_visibility_check=" << (AimAssist::visibilityCheck ? "true" : "false") << std::endl;
                outputFile << "aim_feedback=" << (AimAssist::aimAssistFeedback ? "true" : "false") << std::endl;
                outputFile << "aim_fov_circle=" << (AimAssist::fovCircle ? "true" : "false") << std::endl;
                outputFile << "aim_key=" << (AimAssist::aimKey ? "true" : "false") << std::endl;
                outputFile << "aim_adaptive=" << (AimAssist::adaptive ? "true" : "false") << std::endl;
                outputFile << "aim_adaptive_offset=" << AimAssist::adaptiveOffset << std::endl;
                outputFile << "aim_smooth=" << AimAssist::smooth << std::endl;
                outputFile << "aim_distance=" << AimAssist::aimDistance << std::endl;
                outputFile << "aim_random_yaw=" << AimAssist::randomYaw << std::endl;
                outputFile << "aim_random_pitch=" << AimAssist::randomPitch << std::endl;
                outputFile << "aim_target_priority=" << AimAssist::targetPriority << std::endl;

                outputFile.close();
                ImGui::Text("Config saved successfully!");
            }
            else {
                std::cerr << "Error opening config file for writing!" << std::endl;
            }
        }

        // Neue Einstellungen im Menü anzeigen
        ImGui::Checkbox("Enable GayMode", &Esp::GayMode);

        ImGui::EndChild();
    }

    ImGui::PopStyleVar();
    ImGui::PopStyleColor();
    ImGui::EndGroup();
}

