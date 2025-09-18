#include <chrono>
#include <filesystem>
#include <format>
#include <iostream>
#include <string>

#include <DirectXTex.h>

static std::string FormatChronoDuration(std::chrono::nanoseconds ns)
{
    if (ns < std::chrono::microseconds(1))
    {
        return std::to_string(ns.count()) + " ns";
    }
    else if (ns < std::chrono::milliseconds(1))
    {
        return std::to_string(std::chrono::duration_cast<std::chrono::microseconds>(ns).count()) + " µs";
    }
    else if (ns < std::chrono::seconds(1))
    {
        return std::to_string(std::chrono::duration_cast<std::chrono::milliseconds>(ns).count()) + " ms";
    }
    else
    {
        return std::to_string(std::chrono::duration_cast<std::chrono::duration<double>>(ns).count()) + " s";
    }
}

int main(int argc, char* argv[])
{
    auto start_time{ std::chrono::high_resolution_clock::now() };

    HRESULT hresult{ S_OK };

    hresult = CoInitializeEx(NULL, COINIT_MULTITHREADED);
    if (FAILED(hresult))
    {
        std::cout << "failed to initialize com\n";
        return 0;
    }

    if (argc <= 1)
    {
        std::cout << "usage: qk_import albedo\n";
        return 0;
    }

    std::string what{ argv[1] };

    if (what == "albedo") // we are importing an albedo texture
    {
        if (argc <= 3)
        {
            std::cout << "usage: qk_import albedo src_image dst_image\n";
            return 0;
        }

        if (argc > 3)
        {
            // fetch path of source texture /w sanity check
            std::filesystem::path src_image_path{ argv[2] };
            if (!std::filesystem::is_regular_file(src_image_path))
            {
                std::cout << src_image_path << " is not a file\n";
                return 0;
            }

            // fetch path of destination texture
            std::filesystem::path dst_image_path{ argv[3] };

            // if the destination path is a directory, append to it the file name /w .dds as new extension
            if (std::filesystem::is_directory(dst_image_path))
            {
                dst_image_path = dst_image_path / std::format("{}.dds", src_image_path.stem().string());
            }

            // check for destination texture to not exist
            if (std::filesystem::exists(dst_image_path))
            {
                std::cout << dst_image_path << " already exists\n";
                return 0;
            }

            std::cout << "importing albedo texture: " << src_image_path << " into " << dst_image_path << " ... ";

            HRESULT hr{ S_OK };

            // load source texture
            DirectX::ScratchImage src_img{};
            hr = DirectX::LoadFromWICFile(src_image_path.wstring().c_str(), DirectX::WIC_FLAGS_NONE, nullptr, src_img);
            if (FAILED(hr))
            {
                std::cout << "fail: failed to load source\n";
                return 0;
            }

            // generate mip maps for texture
            DirectX::ScratchImage src_img_mip_chain{};
            hr = DirectX::GenerateMipMaps(*src_img.GetImage(0, 0, 0), DirectX::TEX_FILTER_DEFAULT, 0, src_img_mip_chain, false);
            if (FAILED(hr))
            {
                std::cout << "fail: failed to generate mip chain for source\n";
                return 0;
            }

            // compress source mip chain to BC7
            DirectX::ScratchImage dst_img{};
            hresult = DirectX::Compress(
                src_img_mip_chain.GetImages(), src_img_mip_chain.GetImageCount(), src_img_mip_chain.GetMetadata(),
                DXGI_FORMAT_BC7_UNORM_SRGB, DirectX::TEX_COMPRESS_DEFAULT | DirectX::TEX_COMPRESS_PARALLEL,
                DirectX::TEX_THRESHOLD_DEFAULT, dst_img
            );
            if (FAILED(hr))
            {
                std::cout << "fail: failed to compress mip chain for source\n";
                return 0;
            }

            // save compressed texture to disk
            hr = DirectX::SaveToDDSFile(
                dst_img.GetImages(), dst_img.GetImageCount(), dst_img.GetMetadata(),
                DirectX::DDS_FLAGS_NONE,
                dst_image_path.wstring().c_str()
            );
            if (FAILED(hr))
            {
                std::cout << "fail: failed to save destination\n";
                return 0;
            }

            auto end_time{ std::chrono::high_resolution_clock::now() };
            auto duration{ end_time - start_time };

            std::cout << "done (" << FormatChronoDuration(duration) << ")\n";
        }
    }

    return 0;
}
