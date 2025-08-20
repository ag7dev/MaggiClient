#pragma once
#include "../../../util/math/geometry.h"

#include <array>
#include <string>
#include <vector>

struct Esp
{
    inline static bool Enabled = true;

    struct EntityVisual {
        std::vector<Vector3> boxVertices;
        std::string name;
        std::string distanceText;
        float distance;
        float opacityFactor;
        float health;
        float maxHealth;
    };

    inline static std::vector<EntityVisual> renderQueue;

    inline static bool Box = true;
    inline static std::array<float, 4> BoxColor{0, 0, 0, 1};
    inline static float BoxThickness = 1.0f;

    inline static bool FilledBox = true;
    inline static std::array<float, 3> FilledBoxColor{0, 0, 0};
    inline static std::array<float, 3> SecondFilledBoxColor{0, 0, 0};
    inline static float FilledBoxOpacity = 0.15f;

    inline static bool Outline = true;
    inline static std::array<float, 4> OutlineColor{0, 0, 0, 0.25f};
    inline static float OutlineThickness = 1.0f;

    inline static bool BoxShadow = false;
    inline static std::array<float, 4> BoxShadowColor{0.0f, 0.0f, 0.0f, 0.5f};
    inline static float BoxRounding = 0.0f;

    inline static bool Text = true;
    inline static float TextSize = 18;
    inline static std::array<float, 4> TextColor{1.0f, 1.0f, 1.0f, 1.0f};
    inline static bool TextOutline = true;
    inline static std::array<float, 4> TextOutlineColor{0, 0, 0, 1.0f};

    // Background box behind the name tag for better contrast
    inline static bool NameTagBox = true;
    inline static std::array<float, 4> NameTagBoxColor{0.0f, 0.0f, 0.0f, 0.7f};
    inline static std::array<float, 4> NameTagBoxOutlineColor{0.5f, 0.5f, 0.5f, 1.0f};

    inline static float TextUnrenderDistance = 14.0f;

    inline static float FadeDistance = 3.0f;

    inline static bool HealthBar = true;
    inline static std::array<float, 4> HealthBarColor{0.0f, 1.0f, 0.0f, 1.0f};
    inline static bool DynamicHealthBarColor = true;

    inline static bool GayMode = false;
    inline static float RainbowSpeed = 0.5f;

    inline static bool DrawTracers = false;
    inline static std::array<float, 4> TracerColor{1.0f, 1.0f, 1.0f, 1.0f};
    inline static int TracerStartPosition = 1; // 0: Top, 1: Bottom, 2: Middle
    inline static float TracerThickness = 2.0f;

    static void Update();
    static void RenderUpdate();
    static void RenderMenu();
};
