#include <qk_mesh/PCH.h>
#include <qk_mesh/Commons.h>

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

                {
                    std::vector<qk::MeshVertex> vertices{};
                    std::vector<qk::MeshIndex> indices{};
                    std::unordered_map<qk::MeshVertex, qk::MeshIndex> vertex_to_index_table{};

                    const tinyobj::attrib_t& attrib{ reader.GetAttrib() };
                    const std::vector<tinyobj::shape_t>& shapes{ reader.GetShapes() };

                    // loop over shapes
                    for (std::size_t s{}; s < shapes.size(); s++)
                    {
                        // loop over faces (polygon)
                        std::size_t index_offset{};
                        for (std::size_t f{}; f < shapes[s].mesh.num_face_vertices.size(); f++)
                        {
                            std::size_t fv{ size_t(shapes[s].mesh.num_face_vertices[f]) };

                            // make sure that all faces are triangles
                            if (fv != 3)
                            {
                                qk_mesh_Error("all mesh faces must be triangles");
                            }

                            // loop over vertices in the face
                            for (std::size_t v{}; v < fv; v++)
                            {
                                // access to vertex
                                tinyobj::index_t idx{ shapes[s].mesh.indices[index_offset + v] };
                                tinyobj::real_t vx{ attrib.vertices[3 * size_t(idx.vertex_index) + 0] };
                                tinyobj::real_t vy{ attrib.vertices[3 * size_t(idx.vertex_index) + 1] };
                                tinyobj::real_t vz{ attrib.vertices[3 * size_t(idx.vertex_index) + 2] };

                                // make sure that the mesh has normals
                                if (idx.normal_index < 0)
                                {
                                    qk_mesh_Error("the mesh should have normals");
                                }

                                // access to normals
                                tinyobj::real_t nx{ attrib.normals[3 * size_t(idx.normal_index) + 0] };
                                tinyobj::real_t ny{ attrib.normals[3 * size_t(idx.normal_index) + 1] };
                                tinyobj::real_t nz{ attrib.normals[3 * size_t(idx.normal_index) + 2] };

                                // make sure that the mesh has uvs
                                if (idx.texcoord_index < 0)
                                {
                                    qk_mesh_Error("the mesh should have UVs");
                                }

                                // access to uvs
                                tinyobj::real_t tx{ attrib.texcoords[2 * size_t(idx.texcoord_index) + 0] };
                                tinyobj::real_t ty{ attrib.texcoords[2 * size_t(idx.texcoord_index) + 1] };

                                qk::MeshVertex vertex
                                {
                                    .position = qk::v3{ vx, vy, vz },
                                    .normal = qk::v3{ nx, ny, nz },
                                    .uvs = qk::v2{ tx, ty },
                                };

                                // search for the vertex inside the table
                                if (auto it{ vertex_to_index_table.find(vertex) }; it != vertex_to_index_table.end())
                                {
                                    // a vertex equal to this one was already inserted, thus re-use it
                                    indices.emplace_back(it->second);
                                }
                                else
                                {
                                    // a vertex equal to this one has never been inserted, thus append it
                                    qk::MeshIndex index{ static_cast<qk::MeshIndex>(vertices.size()) };
                                    indices.emplace_back(index);
                                    vertices.emplace_back(vertex);

                                    // don't forget to update the table
                                    vertex_to_index_table.emplace(vertex, index);
                                }
                            }
                            index_offset += fv;
                        }
                    }
                }
            }
            else // parse failed
            {
                std::cout << "FAILED";

                if (!reader.Error().empty())
                {
                    std::cout << "[ERROR]: " << reader.Error();
                }
            }
        }
    }
}

int main(int argc, char* argv[])
{
    try
    {
        qk_mesh::Entry(argc, argv);
    }
    catch (const qk_mesh::Error& e)
    {
        std::cout << e.what() << "\n";
    }
    return 0;
}
