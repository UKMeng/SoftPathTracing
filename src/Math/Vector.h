//
// Created by UKMeng on 2024/8/17.
//

#pragma once
#include <iostream>

class Vec3
{
public:
    Vec3(float x): x(x), y(x), z(x) {};
    Vec3(float x, float y, float z) : x(x), y(y), z(z) {};

    void Print() const;
private:
    float x, y, z;
};
