#pragma once
typedef unsigned char BYTE;

namespace Colors
{
    const Color UnloadedTextureColor(100, 0, 100);
    const Color UnHandledTextureColor(255, 0, 0);
}

class Color
{
public:
    Color() :color(0) {}
    Color(unsigned int col) :color(col) {}
    Color(BYTE r, BYTE g, BYTE b) :Color(r, g, b, 255) {}
    Color(BYTE r, BYTE g, BYTE b, BYTE a)
    {
        rgba[0] = r;
        rgba[1] = g;
        rgba[2] = b;
        rgba[3] = a;
    }
    Color(const Color& cop) :color(cop.color) {}

    Color& operator=(const Color& cop) { color = cop.color; return *this; }
    bool operator==(const Color& cop) const { return color == cop.color; }
    bool operator!=(const Color& cop) const { return color != cop.color; }

    constexpr BYTE getR() const { return rgba[0]; }
    void setR(BYTE r) { rgba[0] = r; }

    constexpr BYTE getG() const { return rgba[1]; }
    void setG(BYTE g) { rgba[1] = g; }

    constexpr BYTE getB() const { return rgba[2]; }
    void setB(BYTE b) { rgba[2] = b; }

    constexpr BYTE getA() const { return rgba[3]; }
    void setA(BYTE a) { rgba[3] = a; }

private:
    union
    {
        BYTE rgba[4];
        unsigned int color;
    };
};