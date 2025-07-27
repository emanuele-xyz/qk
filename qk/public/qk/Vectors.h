#pragma once

namespace qk
{
    struct v4
    {
        float elems[4];

        v4() : elems{} {}
        v4(float a, float b, float c, float d) : elems{ a, b, c, d } {}

        const float& x() const noexcept { return elems[0]; }
        const float& y() const noexcept { return elems[1]; }
        const float& z() const noexcept { return elems[2]; }
        const float& w() const noexcept { return elems[3]; }
        const float& r() const noexcept { return elems[0]; }
        const float& g() const noexcept { return elems[1]; }
        const float& b() const noexcept { return elems[2]; }
        const float& a() const noexcept { return elems[3]; }

        float& x() noexcept { return elems[0]; }
        float& y() noexcept { return elems[1]; }
        float& z() noexcept { return elems[2]; }
        float& w() noexcept { return elems[3]; }
        float& r() noexcept { return elems[0]; }
        float& g() noexcept { return elems[1]; }
        float& b() noexcept { return elems[2]; }
        float& a() noexcept { return elems[3]; }
    };

    struct v3
    {
        float elems[3];

        v3() : elems{} {}
        v3(float a, float b, float c) : elems{ a, b, c } {}

        const float& x() const noexcept { return elems[0]; }
        const float& y() const noexcept { return elems[1]; }
        const float& z() const noexcept { return elems[2]; }
        const float& r() const noexcept { return elems[0]; }
        const float& g() const noexcept { return elems[1]; }
        const float& b() const noexcept { return elems[2]; }

        float& x() noexcept { return elems[0]; }
        float& y() noexcept { return elems[1]; }
        float& z() noexcept { return elems[2]; }
        float& r() noexcept { return elems[0]; }
        float& g() noexcept { return elems[1]; }
        float& b() noexcept { return elems[2]; }
    };

    struct v2
    {
        float elems[2];

        v2() : elems{} {}
        v2(float a, float b) : elems{ a, b } {}

        const float& x() const noexcept { return elems[0]; }
        const float& y() const noexcept { return elems[1]; }
        const float& r() const noexcept { return elems[0]; }
        const float& g() const noexcept { return elems[1]; }

        float& x() noexcept { return elems[0]; }
        float& y() noexcept { return elems[1]; }
        float& r() noexcept { return elems[0]; }
        float& g() noexcept { return elems[1]; }
    };
}
