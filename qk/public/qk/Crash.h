#pragma once

#include <stdexcept>

namespace qk
{
    class Crash : public std::runtime_error
    {
    public:
        explicit Crash(const char* msg);
    };
}
