//
// Created by UKMeng on 2024/8/18.
//

#include "Matrix.h"

Mat4f Mat4f::Identity()
{
    return Mat4f(1.0f);
}

Mat4f Mat4f::Perspective(float fovy, float aspect, float zNear, float zFar)
{
    Mat4f projection = Mat4f::Identity();

    float top = zNear * tan(fovy / 2.0);
    float bottom = -top;
    float right = top * aspect;
    float left = -right;
    float near = zNear;
    float far = zFar;

    Mat4f ortho({2 / (right - left), 0, 0, 0,
                    0, 2 / (top - bottom), 0, 0,
                    0, 0, 2/ (far - near), 0,
                    0, 0, 0, 1});

    Mat4f orthoTranslate({1, 0, 0, -(right + left) / 2,
                          0, 1, 0, -(top + bottom) / 2,
                          0, 0, 1, -(near + far) / 2,
                          0, 0, 0, 1});

    ortho = ortho * orthoTranslate;

    Mat4f persp2ortho({near, 0, 0, 0,
                       0, near, 0, 0,
                       0, 0, near + far, -near * far,
                       0, 0, 1, 0});

    projection = ortho * persp2ortho * projection;

    return projection;
}

Mat4f Mat4f::LookAt(Vec3f eye, Vec3f center, Vec3f up)
{
    Mat4f view = Mat4f::Identity();
    Mat4f translate({1, 0, 0, -eye.x,
                     0, 1, 0, -eye.y,
                     0, 0, 1, -eye.z,
                     0, 0, 0, 1});

    Vec3f zAxis = (center - eye).Normalize();
    Vec3f xAxis = (up.Cross(zAxis)).Normalize();
    Vec3f yAxis = zAxis.Cross(xAxis);

    Mat4f rotate({xAxis.x, xAxis.y, xAxis.z, 0,
                  yAxis.x, yAxis.y, yAxis.z, 0,
                  zAxis.x, zAxis.y, zAxis.z, 0,
                  0, 0, 0, 1});
    view = rotate * translate;
    return view;
}


