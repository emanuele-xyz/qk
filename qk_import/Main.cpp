#include <iostream>

int main(int argc, char* argv[])
{
    if (argc <= 1)
    {
        std::cout << "usage: qk_import albedo\n";
    }
    else // argc > 1
    {
        std::string what{ argv[1] };

        if (what == "albedo") // we are importing albedo texture
        {
            if (argc > 3)
            {
                // TODO: to be implemented
            }
            else // argc <= 3
            {
                std::cout << "usage: qk_import albedo src_image dst_image\n";
            }
        }
    }

    return 0;
}
