#include <iostream>
#include "MyMath.h"
#include "ThreadPool.h"
#include "Film.h"
#include "Camera.h"
#include "Sphere.h"
#include "Model.h"
#include "Plane.h"
#include "Scene.h"
#include "Frame.h"
#include "ColorRGB.h"
#include "RNG.h"
#include "NormalRenderer.h"
#include "DebugRenderer.h"
#include "PathTracingRenderer.h"

int main()
{
    const int width = 192 * 4;
    const int height = 108 * 4;

    Film film { width, height };

    Camera camera { film, { -7, 5, -7 }, { 0, 0, 0 }, 45};

    Sphere sphere { {0, 0, 0}, 1.0f};

    Plane plane { {0, 0, 0}, {0, 1, 0}};

    Model model("models/dragon_871k.obj");

    Scene scene {};

    RNG rng { 1234 };
    for (int i = 0; i < 100; i ++) {
        Vec3f random_pos {
                rng.Uniform() * 10 - 5,
                rng.Uniform() * 2,
                rng.Uniform() * 10 - 5,
        };
        float u = rng.Uniform();
        if (u < 0.9) {
            scene.AddObject(
                    model,
                    {{0.5, 0.5, 0.5}, rng.Uniform() > 0.5 },
                    random_pos,
                    { rng.Uniform() * 360, rng.Uniform() * 360, rng.Uniform() * 360 },
                    { 1, 1, 1 }

            );
        } else if (u < 0.95) {
            scene.AddObject(
                    sphere,
                    { { rng.Uniform(), rng.Uniform(), rng.Uniform() }, true },
                    random_pos,
                    { 0, 0, 0},
                    { 0.4, 0.4, 0.4 }
            );
        } else {
            random_pos.y += 4;
            scene.AddObject(
                    sphere,
                    { { 1, 1, 1 }, false, { rng.Uniform() * 4, rng.Uniform() * 4, rng.Uniform() * 4 } },
                    random_pos
            );
        }
    }
    scene.AddObject(plane, { ColorRGB(120, 204, 157) }, { 0, -0.5, 0 });

    scene.BuildBVH();

//    Camera camera { film, { -3.6, 0, 0 }, { 0, 0, 0 }, 45};
//    scene.AddObject(model, {ColorRGB(202,159,117)}, {0, 0, 0}, {0, 0, 0}, {1, 3, 2});
//    scene.AddObject(
//            sphere,
//            {{1, 1, 1}, false, ColorRGB(255, 128, 128)},
//            {0, 0.0, 2.5}
//            );
//
//    scene.AddObject(
//            sphere,
//            {{1, 1, 1}, false, ColorRGB(128, 128, 255)},
//            {0, 0.0, -2.5}
//            );
//
//    scene.AddObject(
//            sphere,
//            {{1, 1, 1}, true },
//            {3, 0.5, -2}
//            );
//
//    scene.AddObject(plane, {ColorRGB(120, 204, 157)}, { 0, -0.5, 0});
//
//    scene.BuildBVH();

    NormalRenderer normalRenderer { camera, scene };
    normalRenderer.Render(1, "Normal.ppm");

    DebugBTCRenderer debugBTCRenderer {camera, scene };
    debugBTCRenderer.Render(1, "DebugBTC.ppm");
    DebugTTCRenderer debugTTCRenderer {camera, scene };
    debugTTCRenderer.Render(1, "DebugTTC.ppm");


    PathTracingRenderer pathTracingRenderer { camera, scene };
    pathTracingRenderer.Render(128, "PathTracing.ppm");

    return 0;
}
