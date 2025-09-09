#pragma once

#include <directxtk/SimpleMath.h>

namespace dx = DirectX;
namespace sm = DirectX::SimpleMath;

using Quaternion = DirectX::SimpleMath::Quaternion;
using Vector4 = DirectX::SimpleMath::Vector4;
using Vector3 = DirectX::SimpleMath::Vector3;
using Vector2 = DirectX::SimpleMath::Vector2;
using Matrix = DirectX::SimpleMath::Matrix;

namespace qk
{
    inline Vector3 GammaCorrectToLinear(Vector3 color)
    {
        constexpr float GAMMA{ 2.2f };
        return Vector3{ std::pow(color.x, GAMMA), std::pow(color.y, GAMMA), std::pow(color.z, GAMMA) };
    }
}
