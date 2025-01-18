#pragma once
#include "../../../util/math/geometry.h"

#include <vector>
#include <string>

struct Esp
{
	inline static bool Enabled = true;

	struct Data {
		std::vector<Vector3> boxVerticies;
		std::string name;
		std::string distText;
		float dist;
		float opacityFadeFactor;
		float health;
		float maxHealth;
	};

	inline static std::vector<Data> renderData;

	inline static bool Box = true;
	inline static float BoxColor[4]{ 0, 0, 0, 1 }; // Box-Farbe (RGBA)

	inline static bool FilledBox = true;
	inline static float FilledBoxColor[3]{ 0, 0, 0 }; // Gefüllte Box-Farbe (RGB)
	inline static float SecondFilledBoxColor[3]{ 0, 0, 0 }; // Zweite gefüllte Box-Farbe (RGB)
	inline static float FilledBoxOpacity = 0.15f; // Deckkraft gefüllte Box

	inline static bool Outline = true;
	inline static float OutlineColor[4]{ 0, 0, 0, 0.25 }; // Umrandungsfarbe (RGBA)

	inline static bool Text = true;
	inline static float TextSize = 18;
	inline static float TextColor[4]{ 1.0f, 1.0f, 1.0f, 1.0f }; // Textfarbe (RGBA)
	inline static bool TextOutline = true;
	inline static float TextOutlineColor[4]{ 0, 0, 0, 1.0f }; // Textumrandungsfarbe (RGBA)
	inline static float TextUnrenderDistance = 14.0f; // Text wird über diese Distanz nicht gerendert

	inline static float FadeDistance = 3.0f; // Distanz, bei der das Verblassen beginnt

	inline static bool HealthBar = true;
	inline static float HealthBarColor[4]{ 0.0f, 1.0f, 0.0f, 1.0f }; // Farbe der Healthbar (Standard: Grün)

	// Neue Features
	inline static bool GayMode = false; // Regenbogen-Modus
	inline static float RainbowSpeed = 0.5f; // Geschwindigkeit der Farbänderung im Regenbogen-Modus

	// Tracer-Einstellungen
	inline static bool DrawTracers = false; // Aktiviert/Deaktiviert Tracer
	inline static float TracerColor[4]{ 1.0f, 1.0f, 1.0f, 1.0f }; // Tracer-Farbe (RGBA)
	inline static int TracerStartPosition = 1; // 0: Top, 1: Bottom (Default), 2: Middle

	static void Update();

	static void RenderUpdate();
	static void RenderMenu();
};
