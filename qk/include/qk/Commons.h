#pragma once

#include <chrono>
#include <stdexcept>
#include <string>

#define qk_Crash(msg) throw qk::Crash{ msg }
#define qk_Check(p) do { if (!(p)) { qk_Crash("check '" #p "' failed"); } } while (false)
#define qk_Unreachable() qk_Crash("urneachable code path")

namespace qk
{
    class Crash : public std::runtime_error
    {
    public:
        explicit Crash(const char* msg);
    };

    class Stopwatch
    {
    private:
        using Time = std::chrono::time_point<std::chrono::high_resolution_clock>;
    public:
        Stopwatch();
    public:
        void Mark() noexcept;
    public:
        float ElapsedSec() const noexcept { return m_elapsed_sec; }
    public:
    private:
        Time m_last_time;
        float m_elapsed_sec;
    };

    std::string StrFromWStr(const std::wstring& wstr);
    std::wstring WStrFromStr(const std::string& wstr);
    std::string GetBytesStr(size_t bytes);
}
