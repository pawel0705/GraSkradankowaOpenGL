#pragma once
#include "../Window/windowMode.h"

namespace Config
{
	constexpr WindowMode g_defaultWindowMode = WindowMode::eWindowed;

	constexpr uint32_t g_defaultWidth = 800;
	constexpr uint32_t g_defaultHeight = 600;

	constexpr const char* g_defaultWindowTitle = "PwAG Project!";
}