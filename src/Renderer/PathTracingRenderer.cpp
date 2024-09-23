//
// Created by UKMeng on 2024/9/10.
//

#include "PathTracingRenderer.h"
#include "Frame.h"

Vec3f PathTracingRenderer::RenderPixel(const Vec2i &pixelCoords, const Vec2f& xi, const size_t& currentSpp)
{
    auto ray = camera.GenerateRay(pixelCoords, xi);
    // auto ray = camera.GenerateRay(pixelCoords, {rng.Uniform(), rng.Uniform()});
    m_Spp = currentSpp;
    m_PixelCoords = pixelCoords;
    return MISCastRay(ray, 0);
}

Vec3f PathTracingRenderer::RRCastRay(const Ray &ray, int depth)
{
    if (depth >= maxDepth) return {0, 0, 0};

    Vec3f L_Indirect = {0, 0, 0};
    Vec3f L_Direct = {0, 0, 0};
    auto result = scene.Intersect(ray);

    // If not hit any object, return black color
    if (!result.has_value())
    {
        return {0, 0, 0};
    }

    // Hit Something
    // If the object is emissive, and if depth == 0, return emissive color
    // If depth != 0, return black color
    if (result->material->isEmissive)
    {
        if (depth == 0)
        {
            return result->material->emissive;
        }
        else
        {
            return {0, 0, 0};
        }
    }

    // Hit info
    auto hitPos = result->hitPos;
    auto normal = result->normal;
    auto viewDir = -ray.direction;
    Frame hitPosFrame(normal);
    Vec3f viewDirInLocal = hitPosFrame.GetLocalFromWorld(viewDir);

    // Sample light and calculate direct radiance
    float pdfLight = 1.0f / scene.GetArea();
    auto sampleLightResult = scene.Sample(rng);
    if (sampleLightResult.has_value())
    {
        Vec3f lightPos = sampleLightResult->hitPos;
        Vec3f lightNormal = sampleLightResult->normal;
        Vec3f emit = sampleLightResult->material->emissive;
        Vec3f lightDir = (lightPos - hitPos).Normalize();
        Vec3f lightDirInLocal = hitPosFrame.GetLocalFromWorld(lightDir);
        float distance = (lightPos - hitPos).Norm();

        auto hitTest = scene.Intersect(Ray(hitPos, lightDir));
        if (hitTest.has_value())
        {
            float testDistance = (hitTest->hitPos - hitPos).Norm();
            if (testDistance - distance >= -0.001) // if the ray is not blocked in the middle way
            {
                Vec3f brdf = result->material->BRDF(lightDirInLocal, viewDirInLocal);
                L_Direct = emit * brdf * lightDirInLocal.y * Dot(-lightDir, lightNormal) / (distance * distance) / pdfLight;
            }
        }
    }

    // Sample indirect light and calculate indirect radiance
    // Russian roulette test
    float pdfBRDF = 0.0f;
    if (rng.Uniform() < P_RR)
    {
        Vec4f sample = result->material->Sample(viewDirInLocal, {rng.Uniform(), rng.Uniform()});
        Vec3f rayDirInLocal = sample.xyz();
        pdfBRDF = sample.w;
        Vec3f Li = RRCastRay(Ray(hitPos, hitPosFrame.GetWorldFromLocal(rayDirInLocal)), depth + 1);

        // If the ray hit non-emitting object
        if (Li.Norm() != 0.0)
        {
            Vec3f brdf = result->material->BRDF(rayDirInLocal, viewDirInLocal);
            L_Indirect = Li * brdf * rayDirInLocal.y / pdfBRDF / P_RR;
        }
    }

    return L_Indirect + L_Direct;
}


Vec3f PathTracingRenderer::CastRay(const Ray &ray, int depth)
{
    if (depth >= maxDepth) return {0, 0, 0};

    Vec3f L_Indirect = {0, 0, 0};
    Vec3f L_Direct = {0, 0, 0};
    auto result = scene.Intersect(ray);

    // If not hit any object, return black color
    if (!result.has_value())
    {
        return {0, 0, 0};
    }

    // Hit Something
    // If the object is emissive, and if depth == 0, return emissive color
    // If depth != 0, return black color
    if (result->material->isEmissive)
    {
        if (depth == 0)
        {
            return result->material->emissive;
        }
        else
        {
            return {0, 0, 0};
        }
    }

    // Hit info
    auto hitPos = result->hitPos;
    auto normal = result->normal;
    auto viewDir = -ray.direction;
    Frame hitPosFrame(normal);
    Vec3f viewDirInLocal = hitPosFrame.GetLocalFromWorld(viewDir);

    // Sample light and calculate direct radiance
    float pdfLight = 1.0f / scene.GetArea();
    auto sampleLightResult = scene.Sample(rng);
    if (sampleLightResult.has_value())
    {
        Vec3f lightPos = sampleLightResult->hitPos;
        Vec3f lightNormal = sampleLightResult->normal;
        Vec3f emit = sampleLightResult->material->emissive;
        Vec3f lightDir = (lightPos - hitPos).Normalize();
        Vec3f lightDirInLocal = hitPosFrame.GetLocalFromWorld(lightDir);
        float distance = (lightPos - hitPos).Norm();

        auto hitTest = scene.Intersect(Ray(hitPos, lightDir));
        if (hitTest.has_value())
        {
            float testDistance = (hitTest->hitPos - hitPos).Norm();
            if (testDistance - distance >= -0.001) // if the ray is not blocked in the middle way
            {
                Vec3f brdf = result->material->BRDF(lightDirInLocal, viewDirInLocal);
                L_Direct = emit * brdf * lightDirInLocal.y * Dot(-lightDir, lightNormal) / (distance * distance) / pdfLight;
            }
        }
    }

    // Sample indirect light and calculate indirect radiance
    float pdfBRDF = 0.0f;
    Vec2f xi = RNG::SobolVec2(m_Spp, depth);
    xi = RNG::CranleyPattersonRotation(m_PixelCoords, xi);
    Vec4f sample = result->material->Sample(viewDirInLocal, xi);
    Vec3f rayDirInLocal = sample.xyz();
    pdfBRDF = sample.w;
    Vec3f Li = CastRay(Ray(hitPos, hitPosFrame.GetWorldFromLocal(rayDirInLocal)), depth + 1);

    // If the ray hit non-emitting object
    if (Li.Norm() != 0.0)
    {
        Vec3f brdf = result->material->BRDF(rayDirInLocal, viewDirInLocal);
        L_Indirect = Li * brdf * rayDirInLocal.y / pdfBRDF;
    }

    return L_Indirect + L_Direct;
}

Vec3f PathTracingRenderer::MISCastRay(const Ray &ray, int depth)
{
    if (depth >= maxDepth) return {0, 0, 0};

    Vec3f L_Indirect = {0, 0, 0};
    Vec3f L_Direct = {0, 0, 0};
    auto result = scene.Intersect(ray);

    // If not hit any object, return black color
    if (!result.has_value())
    {
        return {0, 0, 0};
    }

    // Hit Something
    // If the object is emissive, and if depth == 0, return emissive color
    // If depth != 0, return black color
    if (result->material->isEmissive)
    {
        return result->material->emissive;
    }

    // Hit info
    auto hitPos = result->hitPos;
    auto normal = result->normal;
    auto viewDir = -ray.direction;
    Frame hitPosFrame(normal);
    Vec3f viewDirInLocal = hitPosFrame.GetLocalFromWorld(viewDir);

    // Sample light and calculate direct radiance
    auto sampleLightResult = scene.Sample(rng);
    if (sampleLightResult.has_value())
    {
        Vec3f lightPos = sampleLightResult->hitPos;
        Vec3f lightNormal = sampleLightResult->normal;
        Vec3f emit = sampleLightResult->material->emissive;
        Vec3f lightDir = (lightPos - hitPos).Normalize();
        Vec3f lightDirInLocal = hitPosFrame.GetLocalFromWorld(lightDir);
        float distance = (lightPos - hitPos).Norm();
        float pdfLight = 1.0f / ( scene.GetArea() * Dot(-lightDir, lightNormal) / (distance * distance) );

        auto hitTest = scene.Intersect(Ray(hitPos, lightDir));
        if (hitTest.has_value())
        {
            float testDistance = (hitTest->hitPos - hitPos).Norm();
            if (testDistance - distance >= -0.001) // if the ray is not blocked in the middle way
            {
                Vec3f brdf = result->material->BRDF(lightDirInLocal, viewDirInLocal);
                float pdfBRDF = result->material->PDF(lightDirInLocal, viewDirInLocal);
                float misWeight = MISMixWeight(pdfLight, pdfBRDF);
                L_Direct = misWeight * emit * brdf * lightDirInLocal.y / pdfLight;
            }
        }
    }

    // Sample indirect light and calculate indirect radiance
    float pdfBRDF = 0.0f;
    Vec2f xi = RNG::SobolVec2(m_Spp, depth);
    xi = RNG::CranleyPattersonRotation(m_PixelCoords, xi);
    Vec4f sample = result->material->Sample(viewDirInLocal, xi);
    Vec3f rayDirInLocal = sample.xyz();
    pdfBRDF = sample.w;
    auto test = scene.Intersect(Ray(hitPos, hitPosFrame.GetWorldFromLocal(rayDirInLocal)));

    if (test.has_value())
    {
        Vec3f brdf = result->material->BRDF(rayDirInLocal, viewDirInLocal);
        if (test->material->isEmissive)
        {
            Vec3f lightPos = test->hitPos;
            Vec3f lightNormal = test->normal;
            Vec3f lightDir = (lightPos - hitPos).Normalize();
            float distance = (lightPos - hitPos).Norm();
            float pdfL = 1.0f / ( scene.GetArea() * Dot(-lightDir, lightNormal) / (distance * distance) );
            float misWeight = MISMixWeight(pdfBRDF, pdfL);
            Vec3f Le = test->object->material->emissive;
            L_Direct += misWeight * Le * brdf * rayDirInLocal.y / pdfBRDF;
        }
        else
        {
            Vec3f Li = MISCastRay(Ray(hitPos, hitPosFrame.GetWorldFromLocal(rayDirInLocal)), depth + 1);
            L_Indirect = Li * brdf * rayDirInLocal.y / pdfBRDF;
        }
    }

    return L_Indirect + L_Direct;
}
