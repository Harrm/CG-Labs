#include "clear_rendertarget.h"

#define STBI_MSC_SECURE_CRT
#define STB_IMAGE_WRITE_IMPLEMENTATION
#include "stb_image_write.h"

#include <stdexcept>
#include <filesystem>
#include <iostream>


cg::ClearRenderTarget::ClearRenderTarget(uint32_t width, uint32_t height):
    width(width),
    height(height),
    frame_buffer(width * height, color{})
{
    
}

cg::ClearRenderTarget::~ClearRenderTarget()
{
    
}

void cg::ClearRenderTarget::Clear()
{
    std::fill_n(frame_buffer.begin(), frame_buffer.size(), color{0, 0, 0});
}

void cg::ClearRenderTarget::Save(std::string_view filename) const
{
    using std::string_literals::operator""s;
    namespace fs = std::filesystem;
    auto path = fs::path(filename).remove_filename();
    if (!fs::exists(path)) {
        fs::create_directory(path);
    }
    auto res = stbi_write_png(filename.data(), 
        width, height, 3, frame_buffer.data(), width * sizeof(color));
    if (res != 1) {
        throw std::runtime_error("Error saving PNG file '"s + std::string(filename) + "'"s);
    }
}

void cg::ClearRenderTarget::SetPixel(uint32_t x, uint32_t y, color color)
{
    frame_buffer[y * width + x] = std::move(color);
}
