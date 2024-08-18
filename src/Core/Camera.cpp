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

Ray Camera::GenerateRay(const Vec2i &pixelCoord, const Vec2f &offset) const
{
    Vec2f ndc = (Vec2f(pixelCoord) + offset) / (Vec2f(m_Film.GetWidth(), m_Film.GetHeight()));
    // Screen Space Coord Origin on the left top corner
    ndc.y = 1.f - ndc.y;
    ndc = 2.f * ndc - 1.f;

    // clip(x, y, 0, near) -> ndc(x/near, y/near, 0)
    Vec4f clip = {ndc, 0.f, 1.f};
    Vec4f world = m_WorldFromCamera * m_CameraFromClip * clip;
    Vec3f worldPos = world.xyz() / world.w;
    return Ray(m_Pos, Normalize(worldPos - m_Pos));
}
