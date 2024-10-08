//
// Created by UKMeng on 2024/9/3.
//

#include "Renderer.h"
#include "ThreadPool.h"
#include "ProgressBar.h"
#include "Profile.h"

void Renderer::Render(size_t spp, const std::filesystem::path &fileName)
{
    Profile profile("Render");

    size_t currentSpp = 0, increase = 1;
    auto& film = camera.GetFilm();
    film.Clear();
    ProgressBar progress { film.GetWidth() * film.GetHeight() * spp };
    while (currentSpp < spp)
    {
        g_ThreadPool.ParallelFor(film.GetWidth(), film.GetHeight(), [&](size_t x, size_t y)
        {
            for (int i = 0; i < increase; ++i)
            {
                Vec2f xi = rng.Hammersley(currentSpp + i, spp);
                film.AddSample(x, y, RenderPixel({x, y}, xi, currentSpp + i));
            }
            progress.Update(increase);
        });
        g_ThreadPool.Wait();

        currentSpp += increase;
        increase = std::min<size_t>(currentSpp, 32);

        film.Save(fileName);
        std::cout << currentSpp << " SPP Saved in " << fileName << std::endl;
    }
}