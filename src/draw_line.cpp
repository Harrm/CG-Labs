#include "draw_line.h"


#define _USE_MATH_DEFINES
#include <math.h>

#include <algorithm>
#include <stdexcept>
#include <iostream>

cg::LineDrawing::LineDrawing(uint32_t width, uint32_t height): 
	cg::ClearRenderTarget(width, height)
{
    
}

cg::LineDrawing::~LineDrawing()
{
}

void cg::LineDrawing::DrawLine(uint32_t x_begin, uint32_t y_begin, 
							   uint32_t x_end, uint32_t y_end, 
							   color color)
{
	bool is_steep = abs((int64_t)y_end - (int64_t)y_begin) > abs((int64_t)x_end - (int64_t)x_begin);
	if (is_steep) {
		std::swap(x_begin, y_begin);
		std::swap(x_end, y_end);
	}
	if (x_begin > x_end) {
		std::swap(x_begin, x_end);
		std::swap(y_begin, y_end);
	}
	const float dx = (float)x_end - (float)x_begin;
	const float dy = abs((float)y_end - (float)y_begin);
	float error = dx / 2.f;
	const int y_step = (y_begin < y_end) ? 1 : -1;
	uint32_t y = y_begin;
	for (uint32_t x = x_begin; x <= x_end; x++) {
		if (is_steep) {
			SetPixel(y, x, color);
		} else {
			SetPixel(x, y, color);
		}
		error -= dy;
		if (error < 0) {
			y += y_step;
			error += dx;
		}
	}
}

void cg::LineDrawing::DrawScene()
{
    uint32_t x_center = width / 2;
    uint32_t y_center = height / 2;
	uint32_t radius = std::min(x_center, y_center) - 1;
	for (double angle = 0.0; angle < 360.0; angle += 5.0)
	{
		auto x_end = static_cast<uint32_t>(x_center + radius * cos(angle * M_PI / 180.0));
		auto y_end = static_cast<uint32_t>(y_center + radius * sin(angle * M_PI / 180.0));
		auto colour = color{static_cast<uint8_t>(255 * sin(angle * M_PI / 180.0)),
							static_cast<uint8_t>(255 * cos(angle * M_PI / 180.0)), 
							255};
		DrawLine(x_center, y_center, x_end, y_end, colour);
	}

}

