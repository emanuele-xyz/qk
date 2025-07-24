#pragma once

#include <stdexcept>
#include <string>

#define qk_gui_Crash(msg) throw qk_gui::Crash{ msg }
#define qk_gui_Check(p) do { if (!(p)) { qk_gui_Crash("check '" #p "' failed"); } } while (false)

namespace qk_gui
{
    class Crash : public std::runtime_error
    {
    public:
        explicit Crash(const char* msg);
    };

    std::string StrFromWStr(const std::wstring& wstr);
    std::wstring WStrFromStr(const std::string& wstr);
    std::string GetBytesStr(size_t bytes);
}
