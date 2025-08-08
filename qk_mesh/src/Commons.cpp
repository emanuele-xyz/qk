#include <qk_mesh/PCH.h>
#include <qk_mesh/Commons.h>

namespace qk_mesh
{
    Error::Error(const char* msg)
        : std::runtime_error{ std::format("{}\n{}", msg, std::stacktrace::current(1)) }
    {
    }
}
