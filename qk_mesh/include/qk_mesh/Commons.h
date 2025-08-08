#pragma once

#include <stdexcept>

#define qk_mesh_Error(msg) throw qk_mesh::Error{ msg }

namespace qk_mesh
{
    class Error : public std::runtime_error
    {
    public:
        explicit Error(const char* msg);
    };
}
