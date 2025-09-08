#pragma once

#include <qk/Math.h>

#include <imgui.h>

namespace qk::ImGuiEx
{
    bool ColorEdit3(const char* label, Vector3& color, ImGuiColorEditFlags flags = 0);
    bool DragFloat3(const char* label, Vector3& val, float v_speed = 1.0f, float v_min = 0.0f, float v_max = 0.0f, const char* format = "%.3f", ImGuiSliderFlags flags = 0);
}
