#pragma once


#include "clear_rendertarget.h"

namespace cg
{

	class LineDrawing : public ClearRenderTarget
	{
	public:
		LineDrawing(uint32_t width, uint32_t height);
		virtual ~LineDrawing();

		void DrawLine(uint32_t x_begin, uint32_t y_begin, uint32_t x_end, uint32_t y_end, color color);

		void DrawScene();

	};

}
