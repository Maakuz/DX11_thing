#pragma once
#include "Color.h"

struct Material
{
    Color ambient;
    Color diffuse;
    Color specular;
    Color emissive;
    float opacity = 1;
    float shininess = 0;
    float reflectivity = 0;
    //Shading model?
};