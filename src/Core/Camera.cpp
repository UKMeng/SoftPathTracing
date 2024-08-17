//
// Created by UKMeng on 2024/8/18.
//

#include "Camera.h"

Camera::Camera(Film &film, const Vec3f &pos, const Vec3f &viewPoint, float fovy) : m_Film(film), m_Pos(pos)
{
    m_CameraFromClip = Mat4f::Perspective(Radians(fovy),
                                          static_cast<float>(film.GetWidth()) / static_cast<float>(film.GetHeight()),
                                          1.f, 2.f).Inverse();
    m_WorldFromCamera = Mat4f::LookAt(pos, viewPoint, Vec3f(0, 1, 0)).Inverse();
}
