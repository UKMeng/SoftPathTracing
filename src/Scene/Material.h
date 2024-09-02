//
// Created by UKMeng on 2024/9/1.
//

#pragma once

#include "MyMath.h"

class Material
{
public:
    // TODO: use this as a base class for other materials
    Vec3f albedo = { 1, 1, 1};
    bool isSpecular = false;
    Vec3f emissive = {0, 0, 0};
};
