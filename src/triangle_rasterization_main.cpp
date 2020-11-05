#include "triangle_rasterization.h"

#include <iostream>

int main(int argc, char* argv[])
{
    try
    {
        cg::TriangleRasterization* render = new cg::TriangleRasterization(1920, 1080, "models/CornellBox-Original.obj");
        using clock_t = std::chrono::steady_clock;
        auto start = clock_t::now();

        render->Clear();

        render->DrawScene();
        auto end = clock_t::now();
        std::cout << std::chrono::duration_cast<std::chrono::milliseconds>(end - start).count() << "\n";
        render->Save("results/triangle_rasterization.png");

        // Just show the resulted image
        //system("start results/triangle_rasterization.png");
    }
    catch (std::exception& e)
    {
        std::cerr << e.what() << std::endl;
    }
}