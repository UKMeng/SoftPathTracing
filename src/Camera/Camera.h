//
// Created by UKMeng on 2024/8/18.
//

#pragma once

#include "Film.h"
#include "Ray.h"
#include "MyMath.h"

class Camera
{
public:
    Camera(Film &film, const Vec3f &pos, const Vec3f &viewPoint, float fovy);

    /// Get a Ray in World Space
    /// \param pixelCoord pixel coordinates in Screen Space
    /// \param offset pixel center offset
    /// \return a Ray in World Space
    Ray GenerateRay(const Vec2i &pixelCoord, const Vec2f &offset = {0.5, 0.5 }) const;

    Film& GetFilm() const { return m_Film; }

private:
    Film& m_Film;
    Vec3f m_Pos;

    Mat4f m_CameraFromClip;
    Mat4f m_WorldFromCamera;
};
