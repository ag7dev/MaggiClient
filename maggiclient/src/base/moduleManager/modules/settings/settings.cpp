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

                        // Hier kannst du die Werte zuweisen
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
                        // Füge hier weitere Einstellungen hinzu
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
                // Neue Optionen hinzufügen
                outputFile << "gay_mode=" << (Esp::GayMode ? "true" : "false") << std::endl;
                outputFile << "rainbow_speed=" << Esp::RainbowSpeed << std::endl;
                outputFile << "draw_tracers=" << (Esp::DrawTracers ? "true" : "false") << std::endl;
                outputFile << "tracer_color=" << Esp::TracerColor[0] << " " << Esp::TracerColor[1] << " " << Esp::TracerColor[2] << " " << Esp::TracerColor[3] << std::endl;
                outputFile << "tracer_start_position=" << Esp::TracerStartPosition << std::endl;

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

