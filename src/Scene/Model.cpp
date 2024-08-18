//
// Created by UKMeng on 2024/8/18.
//

#include "Model.h"

Model::Model(const std::filesystem::path &filename)
{
    auto result = rapidobj::ParseFile(filename, rapidobj::MaterialLibrary::Ignore());

    Vec3f minVert = Vec3f{std::numeric_limits<float>::infinity(),
                         std::numeric_limits<float>::infinity(),
                         std::numeric_limits<float>::infinity()};
    Vec3f maxVert = Vec3f{-std::numeric_limits<float>::infinity(),
                         -std::numeric_limits<float>::infinity(),
                         -std::numeric_limits<float>::infinity()};

    for (const auto &shape: result.shapes)
    {
        size_t indexOffset = 0;
        for (size_t numFaceVertices : shape.mesh.num_face_vertices)
        {
            if (numFaceVertices == 3)
            {
                auto index = shape.mesh.indices[indexOffset];
                Vec3f v0 {
                    result.attributes.positions[index.position_index * 3 + 0],
                    result.attributes.positions[index.position_index * 3 + 1],
                    result.attributes.positions[index.position_index * 3 + 2]
                };
                index = shape.mesh.indices[indexOffset + 1];
                Vec3f v1 {
                    result.attributes.positions[index.position_index * 3 + 0],
                    result.attributes.positions[index.position_index * 3 + 1],
                    result.attributes.positions[index.position_index * 3 + 2]
                };
                index = shape.mesh.indices[indexOffset + 2];
                Vec3f v2 {
                        result.attributes.positions[index.position_index * 3 + 0],
                        result.attributes.positions[index.position_index * 3 + 1],
                        result.attributes.positions[index.position_index * 3 + 2]
                };
                if (index.normal_index >= 0)
                {
                    Vec3f normal {
                        result.attributes.normals[index.normal_index * 3 + 0],
                        result.attributes.normals[index.normal_index * 3 + 1],
                        result.attributes.normals[index.normal_index * 3 + 2]
                    };
                    triangles.emplace_back(v0, v1, v2, normal);
                }
                else
                {
                    triangles.emplace_back(v0, v1, v2);
                }
            }
            indexOffset += numFaceVertices;
        }
    }


    boundingBox = AABB(minVert, maxVert);
}

std::optional<HitInfo> Model::Intersect(const Ray &ray, float tMin, float tMax) const
{
    // TODO: use AABB and BVH to test intersection
    std::optional<HitInfo> closetHitInfo {};
    for (const auto& triangle : triangles)
    {
        auto hitInfo = triangle.Intersect(ray, tMin, tMax);
        if (hitInfo.has_value())
        {
            tMax = hitInfo->t;
            closetHitInfo = hitInfo;
        }
    }
    return closetHitInfo;
}

AABB Model::GetAABB()
{
    return boundingBox;
}
