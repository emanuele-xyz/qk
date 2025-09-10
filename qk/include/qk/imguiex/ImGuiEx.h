#pragma once

#include <qk/Math.h>

#include <imgui.h>

namespace qk::ImGuiEx
{
    template <typename T>
    void Combo(const char* label, const char* (*value_to_str_fn)(T value), T(*get_next_value)(T value), T& current_value, T max_value)
    {
        if (ImGui::BeginCombo(label, value_to_str_fn(current_value)))
        {
            for (T value{}; value < max_value; value = get_next_value(value))
            {
                bool is_selected{ current_value == value };
                if (ImGui::Selectable(value_to_str_fn(value), is_selected))
                {
                    current_value = value;
                }
                if (is_selected)
                {
                    ImGui::SetItemDefaultFocus();
                }
            }
            ImGui::EndCombo();
        }
    }

    bool ColorEdit3(const char* label, Vector3& color, ImGuiColorEditFlags flags = 0);
    bool DragFloat3(const char* label, Vector3& val, float v_speed = 1.0f, float v_min = 0.0f, float v_max = 0.0f, const char* format = "%.3f", ImGuiSliderFlags flags = 0);
}
