//
// Created by UKMeng on 2024/9/3.
//

#pragma once

#include <random>

class RNG
{
public:
    RNG()
    {
        std::random_device dev;
        SetSeed(dev());
    }
    RNG(size_t seed) { SetSeed(seed); }

    void SetSeed(size_t seed) { gen.seed(seed); }
    float Uniform() { return dist(gen); }

    static Vec4f CosineSampleHemisphere(Vec2f E)
    {
        float Phi = 2 * M_PI * E.x;
        float CosTheta = Sqrt(E.y);
        float SinTheta = Sqrt(1 - CosTheta * CosTheta);

        float x = SinTheta * Cos(Phi);
        float y = CosTheta;
        float z = SinTheta * Sin(Phi);

        float PDF = CosTheta * INV_PI;

        return Vec4f{ x, y, z, PDF };
    }

    /// Importance Sampling GGX
    /// \param E two random numbers between 0 and 1
    /// \param a2 roughness * roughness
    /// \return Sampling direction and PDF
    static Vec4f ImportanceSampleGGX(Vec2f E, float a2)
    {
        float Phi = 2.0f * M_PI * E.x;
        float CosTheta = Sqrt((1.0f - E.y) / (1.0f + (a2 - 1.0f) * E.y));
        float SinTheta = Sqrt(1.0f - CosTheta * CosTheta);

        float x = SinTheta * Cos(Phi);
        float y = CosTheta;
        float z = SinTheta * Sin(Phi);

        float d = (a2 * CosTheta - CosTheta) * CosTheta + 1;
        float D = a2 / (M_PI * d * d);
        float PDF = D * CosTheta;

        return Vec4f {x, y, z, PDF};
    }

    // reference: https://learnopengl-cn.github.io/07%20PBR/03%20IBL/02%20Specular%20IBL/
    // Hammersley Sequence
    float RadicalInverse_VdC(uint32_t bits)
    {
        bits = (bits << 16u) | (bits >> 16u);
        bits = ((bits & 0x55555555u) << 1u) | ((bits & 0xAAAAAAAAu) >> 1u);
        bits = ((bits & 0x33333333u) << 2u) | ((bits & 0xCCCCCCCCu) >> 2u);
        bits = ((bits & 0x0F0F0F0Fu) << 4u) | ((bits & 0xF0F0F0F0u) >> 4u);
        bits = ((bits & 0x00FF00FFu) << 8u) | ((bits & 0xFF00FF00u) >> 8u);
        return float(bits) * 2.3283064365386963e-10; // / 0x100000000
    }

    Vec2f Hammersley(uint32_t i, uint32_t N)
    {
        return Vec2f(float(i)/float(N), RadicalInverse_VdC(i));
    }

    // --Sobol--
    // Reference https://github.com/AKGWSB/EzRT/blob/main/part%205%20--%20Importance%20Sampling%20%26%20Low%20Discrepancy%20Sequence/tutorial%20(.md%20%26%20.pdf%20file)/tutorial.md
    // 1 ~ 8 维的 sobol 生成矩阵
    constexpr static const uint32_t V[8*32] = {
            2147483648, 1073741824, 536870912, 268435456, 134217728, 67108864, 33554432, 16777216, 8388608, 4194304, 2097152, 1048576, 524288, 262144, 131072, 65536, 32768, 16384, 8192, 4096, 2048, 1024, 512, 256, 128, 64, 32, 16, 8, 4, 2, 1,
            2147483648, 3221225472, 2684354560, 4026531840, 2281701376, 3422552064, 2852126720, 4278190080, 2155872256, 3233808384, 2694840320, 4042260480, 2290614272, 3435921408, 2863267840, 4294901760, 2147516416, 3221274624, 2684395520, 4026593280, 2281736192, 3422604288, 2852170240, 4278255360, 2155905152, 3233857728, 2694881440, 4042322160, 2290649224, 3435973836, 2863311530, 4294967295,
            2147483648, 3221225472, 1610612736, 2415919104, 3892314112, 1543503872, 2382364672, 3305111552, 1753219072, 2629828608, 3999268864, 1435500544, 2154299392, 3231449088, 1626210304, 2421489664, 3900735488, 1556135936, 2388680704, 3314585600, 1751705600, 2627492864, 4008611328, 1431684352, 2147543168, 3221249216, 1610649184, 2415969680, 3892340840, 1543543964, 2382425838, 3305133397,
            2147483648, 3221225472, 536870912, 1342177280, 4160749568, 1946157056, 2717908992, 2466250752, 3632267264, 624951296, 1507852288, 3872391168, 2013790208, 3020685312, 2181169152, 3271884800, 546275328, 1363623936, 4226424832, 1977167872, 2693105664, 2437829632, 3689389568, 635137280, 1484783744, 3846176960, 2044723232, 3067084880, 2148008184, 3222012020, 537002146, 1342505107,
            2147483648, 1073741824, 536870912, 2952790016, 4160749568, 3690987520, 2046820352, 2634022912, 1518338048, 801112064, 2707423232, 4038066176, 3666345984, 1875116032, 2170683392, 1085997056, 579305472, 3016343552, 4217741312, 3719483392, 2013407232, 2617981952, 1510979072, 755882752, 2726789248, 4090085440, 3680870432, 1840435376, 2147625208, 1074478300, 537900666, 2953698205,
            2147483648, 1073741824, 1610612736, 805306368, 2818572288, 335544320, 2113929216, 3472883712, 2290089984, 3829399552, 3059744768, 1127219200, 3089629184, 4199809024, 3567124480, 1891565568, 394297344, 3988799488, 920674304, 4193267712, 2950604800, 3977188352, 3250028032, 129093376, 2231568512, 2963678272, 4281226848, 432124720, 803643432, 1633613396, 2672665246, 3170194367,
            2147483648, 3221225472, 2684354560, 3489660928, 1476395008, 2483027968, 1040187392, 3808428032, 3196059648, 599785472, 505413632, 4077912064, 1182269440, 1736704000, 2017853440, 2221342720, 3329785856, 2810494976, 3628507136, 1416089600, 2658719744, 864310272, 3863387648, 3076993792, 553150080, 272922560, 4167467040, 1148698640, 1719673080, 2009075780, 2149644390, 3222291575,
            2147483648, 1073741824, 2684354560, 1342177280, 2281701376, 1946157056, 436207616, 2566914048, 2625634304, 3208642560, 2720006144, 2098200576, 111673344, 2354315264, 3464626176, 4027383808, 2886631424, 3770826752, 1691164672, 3357462528, 1993345024, 3752330240, 873073152, 2870150400, 1700563072, 87021376, 1097028000, 1222351248, 1560027592, 2977959924, 23268898, 437609937
    };

    // 格林码
    static uint32_t grayCode(uint32_t i) {
        return i ^ (i>>1);
    }

    // 生成第 d 维度的第 i 个 sobol 数
    static float sobol(uint32_t d, uint32_t i) {
        uint32_t result = 0;
        uint32_t offset = d * 32;
        for(uint32_t j = 0; i; i >>= 1, j++)
            if(i & 1)
                result ^= V[j+offset];

        return float(result) * (1.0f/float(0xFFFFFFFFU));
    }

    static uint32_t wang_hash(uint32_t seed) {
        seed = uint32_t(seed ^ uint32_t(61)) ^ uint32_t(seed >> uint32_t(16));
        seed *= uint32_t(9);
        seed = seed ^ (seed >> 4);
        seed *= uint32_t(0x27d4eb2d);
        seed = seed ^ (seed >> 15);
        return seed;
    }

    static Vec2f CranleyPattersonRotation(const Vec2i& pixelCoords, Vec2f& p) {
        uint32_t pseed = uint32_t(
                uint32_t((pixelCoords.x)) * uint32_t(1973) +
                uint32_t((pixelCoords.y)) * uint32_t(9277) +
                uint32_t(114514/1919) * uint32_t(26699)) | uint32_t(1);

        float u = float(wang_hash(pseed)) / 4294967296.0;
        float v = float(wang_hash(pseed)) / 4294967296.0;

        p.x += u;
        if(p.x>1) p.x -= 1;
        if(p.x<0) p.x += 1;

        p.y += v;
        if(p.y>1) p.y -= 1;
        if(p.y<0) p.y += 1;

        return p;
    }

    // 生成第 i 帧的第 b 次反弹需要的二维随机向量
    static Vec2f SobolVec2(uint32_t i, uint32_t b) {
        float u = sobol(b*2, grayCode(i));
        float v = sobol(b*2+1, grayCode(i));
        return Vec2f(u, v);
    }
    // --Sobol--

private:
    std::mt19937 gen;
    std::uniform_real_distribution<float> dist {0.f, 1.f};
};
