#pragma once

#include <chrono>

namespace qk_gui
{
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
}
