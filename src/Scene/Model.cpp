//
// Created by UKMeng on 2024/8/18.
//

#include "Model.h"

Model::Model(const std::filesystem::path &filename)
{
    auto result = rapidobj::ParseFile(filename, rapidobj::MaterialLibrary::Ignore());

    std::vector<Object*> objs;

    area = 0.f;

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
                    Object* tri = new Triangle(v0, v1, v2, normal);
                    area += tri->GetArea();
                    objs.emplace_back(tri);
                    //triangles.emplace_back(v0, v1, v2, normal);
                }
                else
                {
                    Object* tri = new Triangle(v0, v1, v2);
                    area += tri->GetArea();
                    objs.emplace_back(tri);
                    // triangles.emplace_back(v0, v1, v2);
                }
            }
            indexOffset += numFaceVertices;
        }
    }

    bvh = std::make_unique<BVH>(std::move(objs), 1, BVH::SplitMethod::SAH);
}

std::optional<HitInfo> Model::Sample(RNG &rng) const
{
    return bvh->Sample(rng);
}

std::optional<HitInfo> Model::Intersect(const Ray &ray, float tMin, float tMax) const
{
    std::optional<HitInfo> closetHitInfo = bvh->Intersect(ray, tMin, tMax);
    return closetHitInfo;
}

AABB Model::GetAABB() const
{
    return bvh->GetAABB();
}