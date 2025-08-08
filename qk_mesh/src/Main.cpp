#include <qk_mesh/PCH.h>

#include <qk/Qk.h>

#define TINYOBJLOADER_IMPLEMENTATION
#include <tiny_obj_loader.h>

namespace qk_mesh
{
    constexpr int PROGRAM_NAME_IDX{ 0 };
    constexpr int MESH_PATH_IDX{ PROGRAM_NAME_IDX + 1 };

    static void Entry(int argc, char* argv[])
    {
        if (argc <= MESH_PATH_IDX)
        {
            std::cout << "Usage: qk_mesh meshname.obj\n";
        }
        else
        {
            const char* mesh_path{ argv[MESH_PATH_IDX] };
            std::cout << "Loading '" << mesh_path << "' ... ";

            tinyobj::ObjReaderConfig reader_config{};
            tinyobj::ObjReader reader{};

            if (reader.ParseFromFile(mesh_path, reader_config))
            {
                std::cout << "OK\n";

                if (!reader.Warning().empty())
                {
                    std::cout << "[WARN]: " << reader.Warning();
                }
            }
            else // parse failed
            {
                std::cout << "FAILED";

                if (!reader.Error().empty())
                {
                    std::cout << ": " << reader.Error();
                }
            }
        }
    }
}

int main(int argc, char* argv[])
{
    qk_mesh::Entry(argc, argv);
    return 0;
}
