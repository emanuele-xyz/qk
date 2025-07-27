#include <qk/PCH.h>
#include <qk/Crash.h>

namespace qk
{
    Crash::Crash(const char* msg)
        : std::runtime_error{ std::format("{}\n{}", msg, std::stacktrace::current(1)) }
    {
    }
}
