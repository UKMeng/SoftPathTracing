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
#include "SimpleRTRenderer.h"
#include "DebugRenderer.h"

int main()
{
    const int width = 192 * 4;
    const int height = 108 * 4;

    Film film { width, height };

    Camera camera { film, { 3.6, 0.0, 0 }, { 0, 0, 0 }, 45};

    Sphere sphere { {0, 0, 0}, 1.0f};

    Plane plane { {0, 0, 0}, {0, 1, 0}};

    Model model("models/dragon_87k.obj");

    Scene scene {};

    scene.AddObject(model, {ColorRGB(202,159,117)}, {0, 0, 0}, {0, 0, 0}, {3, 3, 3});
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

    NormalRenderer normalRenderer { camera, scene };
    normalRenderer.Render(1, "Normal.ppm");

    DebugBTCRenderer debugBTCRenderer {camera, scene };
    debugBTCRenderer.Render(1, "DebugBTC.ppm");
    DebugTTCRenderer debugTTCRenderer {camera, scene };
    debugTTCRenderer.Render(1, "DebugTTC.ppm");

    SimpleRTRenderer simpleRenderer { camera, scene };
    simpleRenderer.Render(8, "SimpleRT.ppm");



    return 0;
}
