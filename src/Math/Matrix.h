//
// Created by UKMeng on 2024/8/18.
//

#pragma once

#include <array>
#include <cassert>
#include <cmath>

#include "Vector.h"

class Mat4f
{
public:
    Mat4f() { data.fill(0.0f); }
    Mat4f(float diagonal)
    {
        data.fill(0.0f);
        data[0] = data[5] = data[10] = data[15] = diagonal;
    }
    Mat4f(const std::array<float, 16>& initData) : data(initData) {}

    // Access element at row, col
    float& operator()(int row, int col)
    {
        return data[row * 4 + col];
    }

    const float& operator()(int row, int col) const
    {
        return data[row * 4 + col];
    }

    // Matrix multiplication
    Mat4f operator*(const Mat4f& other) const
    {
        Mat4f result;
        for (int i = 0; i < 4; ++i) {
            for (int j = 0; j < 4; ++j) {
                result(i, j) = 0.0f;
                for (int k = 0; k < 4; ++k) {
                    result(i, j) += this->operator()(i, k) * other(k, j);
                }
            }
        }
        return result;
    }

    // Matrix determinant
    float determinant() const
    {
        return data[0] * (data[5] * (data[10] * data[15] - data[11] * data[14]) - data[6] * (data[9] * data[15] - data[11] * data[13]) + data[7] * (data[9] * data[14] - data[10] * data[13]))
               - data[1] * (data[4] * (data[10] * data[15] - data[11] * data[14]) - data[6] * (data[8] * data[15] - data[11] * data[12]) + data[7] * (data[8] * data[14] - data[10] * data[12]))
               + data[2] * (data[4] * (data[9] * data[15] - data[11] * data[13]) - data[5] * (data[8] * data[15] - data[11] * data[12]) + data[7] * (data[8] * data[13] - data[9] * data[12]))
               - data[3] * (data[4] * (data[9] * data[14] - data[10] * data[13]) - data[5] * (data[8] * data[14] - data[10] * data[12]) + data[6] * (data[8] * data[13] - data[9] * data[12]));
    }

    // Matrix inversion (simple version)
    Mat4f Inverse() const
    {
        Mat4f result;
        float det = determinant();
        assert(det != 0.0f); // Matrix must be invertible
        float detInv = 1.0 / det;

        result(0, 0) = (data[5] * data[10] * data[15] - data[5] * data[11] * data[14] - data[9] * data[6] * data[15] + data[9] * data[7] * data[14] + data[13] * data[6] * data[11] - data[13] * data[7] * data[10]) * detInv;
        result(0, 1) = (-data[1] * data[10] * data[15] + data[1] * data[11] * data[14] + data[9] * data[2] * data[15] - data[9] * data[3] * data[14] - data[13] * data[2] * data[11] + data[13] * data[3] * data[10]) * detInv;
        result(0, 2) = (data[1] * data[6] * data[15] - data[1] * data[7] * data[14] - data[5] * data[2] * data[15] + data[5] * data[3] * data[14] + data[13] * data[2] * data[7] - data[13] * data[3] * data[6]) * detInv;
        result(0, 3) = (-data[1] * data[6] * data[11] + data[1] * data[7] * data[10] + data[5] * data[2] * data[11] - data[5] * data[3] * data[10] - data[9] * data[2] * data[7] + data[9] * data[3] * data[6]) * detInv;
        result(1, 0) = (-data[4] * data[10] * data[15] + data[4] * data[11] * data[14] + data[8] * data[6] * data[15] - data[8] * data[7] * data[14] - data[12] * data[6] * data[11] + data[12] * data[7] * data[10]) * detInv;
        result(1, 1) = (data[0] * data[10] * data[15] - data[0] * data[11] * data[14] - data[8] * data[2] * data[15] + data[8] * data[3] * data[14] + data[12] * data[2] * data[11] - data[12] * data[3] * data[10]) * detInv;
        result(1, 2) = (-data[0] * data[6] * data[15] + data[0] * data[7] * data[14] + data[4] * data[2] * data[15] - data[4] * data[3] * data[14] - data[12] * data[2] * data[7] + data[12] * data[3] * data[6]) * detInv;
        result(1, 3) = (data[0] * data[6] * data[11] - data[0] * data[7] * data[10] - data[4] * data[2] * data[11] + data[4] * data[3] * data[10] + data[8] * data[2] * data[7] - data[8] * data[3] * data[6]) * detInv;
        result(2, 0) = (data[4] * data[9] * data[15] - data[4] * data[11] * data[13] - data[8] * data[5] * data[15] + data[8] * data[7] * data[13] + data[12] * data[5] * data[11] - data[12] * data[7] * data[9]) * detInv;
        result(2, 1) = (-data[0] * data[9] * data[15] + data[0] * data[11] * data[13] + data[8] * data[1] * data[15] - data[8] * data[3] * data[13] - data[12] * data[1] * data[11] + data[12] * data[3] * data[9]) * detInv;
        result(2, 2) = (data[0] * data[5] * data[15] - data[0] * data[7] * data[13] - data[4] * data[1] * data[15] + data[4] * data[3] * data[13] + data[12] * data[1] * data[7] - data[12] * data[3] * data[5]) * detInv;
        result(2, 3) = (-data[0] * data[5] * data[11] + data[0] * data[7] * data[9] + data[4] * data[1] * data[11] - data[4] * data[3] * data[9] - data[8] * data[1] * data[7] + data[8] * data[3] * data[5]) * detInv;
        result(3, 0) = (-data[4] * data[9] * data[14] + data[4] * data[10] * data[13] + data[8] * data[5] * data[14] - data[8] * data[6] * data[13] - data[12] * data[5] * data[10] + data[12] * data[6] * data[9]) * detInv;
        result(3, 1) = (data[0] * data[9] * data[14] - data[0] * data[10] * data[13] - data[8] * data[1] * data[14] + data[8] * data[2] * data[13] + data[12] * data[1] * data[10] - data[12] * data[2] * data[9]) * detInv;
        result(3, 2) = (-data[0] * data[5] * data[14] + data[0] * data[6] * data[13] + data[4] * data[1] * data[14] - data[4] * data[2] * data[13] - data[12] * data[1] * data[6] + data[12] * data[2] * data[5]) * detInv;
        result(3, 3) = (data[0] * data[5] * data[10] - data[0] * data[6] * data[9] - data[4] * data[1] * data[10] + data[4] * data[2] * data[9] + data[8] * data[1] * data[6] - data[8] * data[2] * data[5]) * detInv;
        return result;
    }

    static Mat4f Identity();

    /// Get a perspective projection matrix (Use left hand coordinates, and view direction is +z)
    /// \param fovy is a radian, if use degree, should convert to radians by use Radians(degree)
    /// \param aspect
    /// \param zNear
    /// \param zFar
    /// \return
    static Mat4f Perspective(float fovy, float aspect, float zNear, float zFar);

    static Mat4f LookAt(Vec3f eye, Vec3f center, Vec3f up);
private:
    std::array<float, 16> data;
};
