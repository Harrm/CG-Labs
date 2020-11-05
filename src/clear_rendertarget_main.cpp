#include "clear_rendertarget.h"

#include <iostream>
#include <chrono>
#include <memory>

int main(int argc, char* argv[])
{
    std::ios::sync_with_stdio(false);
    try
    {
        using clock_t = std::chrono::steady_clock;
        auto start = clock_t::now();
        auto render = std::make_unique<cg::ClearRenderTarget>(1920, 1080);

        render->Clear();

        auto finish = clock_t::now();
        std::cout << std::chrono::duration_cast<std::chrono::milliseconds>(finish - start).count();
        render->Save("results/clear_rendertarget.png");
    }
    catch (std::exception & e)
    {
        std::cerr << e.what() << std::endl;
    }
}