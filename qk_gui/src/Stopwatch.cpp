#include <qk_gui/PCH.h>
#include <qk_gui/Stopwatch.h>

namespace qk_gui
{
    Stopwatch::Stopwatch()
        : m_last_time{ std::chrono::high_resolution_clock::now() }
        , m_elapsed_sec{}
    {
    }
    void Stopwatch::Mark() noexcept
    {
        Time now{ std::chrono::high_resolution_clock::now() };
        auto dt{ std::chrono::duration_cast<std::chrono::duration<float>>(now - m_last_time) };
        m_last_time = now;
        m_elapsed_sec = dt.count();
    }
}
