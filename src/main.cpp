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
#include "SpecularMaterial.h"
#include "LambertianMaterial.h"
#include "MicroFacetMaterial.h"

int main()
{
    const int width = 1920;
    const int height = 1080;

    Film film { width, height };



    Plane plane { {0, 0, 0}, {0, 1, 0}, {100, 100}};

    Plane areaLight { {0, 0, 0}, {0, 0, 1}, {1, 1}};

    Sphere smallLightBall { {0, 0, 0}, 0.2f};
    Sphere midLightBall { {0, 0, 0}, 0.5f};

    Model model("models/dragon_871k.obj");
//    Model bunny("models/bunny/bunny.obj");

    Scene scene {};

    Camera camera { film, { -3, 0, 0 }, { 0, 0, 0 }, 45};

    Material* lightMaterial = new LambertianMaterial({1, 1, 1});
    lightMaterial->SetEmissive(8.0f * Vec3f(0.747f+0.058f, 0.747f+0.258f, 0.747f) + 15.6f * Vec3f(0.740f+0.287f,0.740f+0.160f,0.740f) + 18.4f *Vec3f(0.737f+0.642f,0.737f+0.159f,0.737f));

    //scene.AddObject(smallLightBall, lightMaterial, {0, 0.5, -2});
    scene.AddObject(areaLight, lightMaterial, {0, 0, -2});
    scene.AddObject(midLightBall, lightMaterial, {0, 0.4, 2});
    scene.AddObject(plane,
//                    new LambertianMaterial{{ColorRGB(153,136,204)}},
                    new MicroFacetMaterial{{ColorRGB(153,136,204)}, 0.5, 0.5},
                    {0, -0.5, 0});

    scene.AddObject(
        model,
//        new LambertianMaterial{{ColorRGB(204,136,153)}},
        new MicroFacetMaterial{{ColorRGB(204,136,153)}, 0.2, 0.5},
        {0, 0.0, 0},
        {0, 0, 0},
        {2, 2, 2}
        );


//    scene.AddObject(plane, lightMaterial, { 0, 2, 0});

    scene.BuildBVH();

//    NormalRenderer normalRenderer { camera, scene };
//    normalRenderer.Render(1, "Normal.ppm");
//
//    DebugBTCRenderer debugBTCRenderer {camera, scene };
//    debugBTCRenderer.Render(1, "DebugBTC.ppm");
//    DebugTTCRenderer debugTTCRenderer {camera, scene };
//    debugTTCRenderer.Render(1, "DebugTTC.ppm");


    PathTracingRenderer pathTracingRenderer { camera, scene };
    pathTracingRenderer.Render(512, "PT_test.ppm");

    return 0;
}

// Cornell box scene
//Model floor("models/cornellbox/floor.obj");
//Model left("models/cornellbox/left.obj");
//Model right("models/cornellbox/right.obj");
//Model light("models/cornellbox/light.obj");
//Model shortbox("models/cornellbox/shortbox.obj");
//Model tallbox("models/cornellbox/tallbox.obj");
//
//Camera camera { film, { 270, 270, -750 }, { 270, 270, 270 }, 45};
//
//Material* lightMaterial = new LambertianMaterial({0, 0, 0});
//lightMaterial->SetEmissive(8.0f * Vec3f(0.747f+0.058f, 0.747f+0.258f, 0.747f) + 15.6f * Vec3f(0.740f+0.287f,0.740f+0.160f,0.740f) + 18.4f *Vec3f(0.737f+0.642f,0.737f+0.159f,0.737f));
//
//Material* white = new LambertianMaterial({0.75, 0.71, 0.68});
//
//scene.AddObject(floor, white);
//scene.AddObject(left, new LambertianMaterial{{0.14f, 0.45f, 0.091f}});
//scene.AddObject(right, new LambertianMaterial{{0.63f, 0.065f, 0.05f}});
//scene.AddObject(shortbox, white);
//scene.AddObject(tallbox, white);
//scene.AddObject(light, lightMaterial);


// 100 objs scene
//Camera camera { film, { -7, 5, -7 }, { 0, 0, 0 }, 45};
//RNG rng { 1234 };
//for (int i = 0; i < 100; i ++) {
//Vec3f random_pos {
//        rng.Uniform() * 10 - 5,
//        rng.Uniform() * 2,
//        rng.Uniform() * 10 - 5,
//};
//float u = rng.Uniform();
//Material* material;
//if (u < 0.9) {
//if (rng.Uniform() > 0.5)
//{
//material = new SpecularMaterial({ 0.5, 0.5, 0.5 });
//}
//else
//{
//material = new LambertianMaterial({0.5, 0.5, 0.5});
//}
//scene.AddObject(
//        model,
//        material,
//        random_pos,
//{ rng.Uniform() * 360, rng.Uniform() * 360, rng.Uniform() * 360 },
//{ 1, 1, 1 }
//
//);
//} else if (u < 0.95) {
//material = new SpecularMaterial({ rng.Uniform(), rng.Uniform(), rng.Uniform() });
//scene.AddObject(
//        sphere,
//        material,
//        random_pos,
//{ 0, 0, 0},
//{ 0.4, 0.4, 0.4 }
//);
//} else {
//material = new LambertianMaterial( { 0, 0, 0 });
//material->SetEmissive({ rng.Uniform() * 4, rng.Uniform() * 4, rng.Uniform() * 4 });
//random_pos.y += 4;
//scene.AddObject(
//        sphere,
//        material,
//        random_pos
//);
//}
//}
//scene.AddObject(plane, new LambertianMaterial(ColorRGB(120, 204, 157)), { 0, -0.5, 0 });
//
//scene.BuildBVH();