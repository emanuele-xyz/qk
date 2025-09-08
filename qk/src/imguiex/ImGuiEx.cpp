#include <qk/PCH.h>
#include <qk/imguiex/imguiex.h>

namespace qk::ImGuiEx
{
    bool ColorEdit3(const char* label, Vector3& color, ImGuiColorEditFlags flags)
    {
        float v[3]{ color.x, color.y, color.z };
        bool res{ ImGui::ColorEdit3(label, v, flags) };
        color = Vector3{ v };
        return res;
    }
    bool DragFloat3(const char* label, Vector3& val, float v_speed, float v_min, float v_max, const char* format, ImGuiSliderFlags flags)
    {
        float v[3]{ val.x, val.y, val.z };
        bool res{ ImGui::DragFloat3(label, v, v_speed, v_min, v_max, format, flags) };
        val = Vector3{ v };
        return res;
    }
}
