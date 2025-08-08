#include <qk_mesh/PCH.h>

#include <qk/Qk.h>

#define TINYOBJLOADER_IMPLEMENTATION
#include <tiny_obj_loader.h>

int main(int argc, char* argv[])
{
    std::cout << "args:\n";
    for (int i{}; i < argc; i++)
    {
        std::cout << argv[i] << "\n";
    }

    return 0;
}
