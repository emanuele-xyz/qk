#pragma once

#include <chrono>

namespace qk_gui
{
    class Stopwatch
    {
    private:
        using Time = std::chrono::time_point<std::chrono::high_resolution_clock>;
    public:
        Stopwatch() : m_begin{}, m_end{} {}
    public:
        float ElapsedSec() const noexcept { return std::chrono::duration_cast<std::chrono::duration<float>>(m_end - m_begin).count(); }
    public:
        void Begin() noexcept { m_begin = std::chrono::high_resolution_clock::now(); }
        void End() noexcept { m_end = std::chrono::high_resolution_clock::now(); }
    private:
        Time m_begin;
        Time m_end;
    };
}
