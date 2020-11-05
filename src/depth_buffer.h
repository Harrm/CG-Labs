#pragma once


#include "triangle_rasterization.h"

using namespace linalg::ostream_overloads;

namespace cg
{

	class ZCulling : public TriangleRasterization
	{
	public:
		ZCulling(uint32_t width, uint32_t height, std::string obj_file);
		virtual ~ZCulling() = default;

		void Clear();

	protected:
		void drawTriangle(cg::face t) override;
		void SetPixel(uint32_t x, uint32_t y, color color, float z);
		std::vector<float> depth_buffer;

		std::vector<color> colors;
		int draw_call_counter;
	};

}
