//
// Created by UKMeng on 2024/8/18.
//

#include "Matrix.h"
#include "GMethod.h"

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

Mat4f Mat4f::Translate(Vec3f t)
{
    return Mat4f({1, 0, 0, t.x,
                  0, 1, 0, t.y,
                  0, 0, 1, t.z,
                  0, 0, 0, 1});
}

Mat4f Mat4f::Scale(Vec3f s)
{
    return Mat4f({s.x, 0, 0, 0,
                  0, s.y, 0, 0,
                  0, 0, s.z, 0,
                  0, 0, 0, 1});
}

Mat4f Mat4f::Rotate(Vec3f eularAngle)
{
    // TODO: Quaternion still has problem
//    Vec4f quaternion = eularAngle.EularAngleToQuaternion();
//
//    float a = quaternion.x, b = quaternion.y, c = quaternion.z, d = quaternion.w;
//
//    return Mat4f({
//                  1 - 2 * c * c - 2 * d * d, 2 * b * c - 2 * a * d, 2 * a * c + 2 * b * d, 0,
//                  2 * b * c + 2 * a * d, 1 - 2 * b * b - 2 * d * d, 2 * c * d - 2 * a * b, 0,
//                  2 * b * d - 2 * a * c, 2 * a * b + 2 * c * d, 1 - 2 * b * b - 2 * c * c, 0,
//                  0, 0, 0, 1});

    // reference: https://pbr-book.org/4ed/Geometry_and_Transformations/Transformations
    float x = Radians(eularAngle.x), y = Radians(eularAngle.y), z = Radians(eularAngle.z);

    Mat4f rotateX({1, 0, 0, 0,
                   0, cos(x), -sin(x), 0,
                   0, sin(x), cos(x), 0,
                   0, 0, 0, 1});

    Mat4f rotateY({cos(y), 0, sin(y), 0,
                   0, 1, 0, 0,
                   -sin(y), 0, cos(y), 0,
                   0, 0, 0, 1});

    Mat4f rotateZ({cos(z), -sin(z), 0, 0,
                   sin(z), cos(z), 0, 0,
                   0, 0, 1, 0,
                   0, 0, 0, 1});

    return rotateZ * rotateY * rotateX;
}


