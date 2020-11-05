#include "depth_buffer.h"

#define _USE_MATH_DEFINES
#include <math.h>

#include <iostream>

cg::ZCulling::ZCulling(uint32_t width, uint32_t height, std::string obj_file)
    : TriangleRasterization(width, height, obj_file) {
  depth_buffer.resize(width * height, 0.f);
}

void cg::ZCulling::Clear() {
  frame_buffer.resize(width * height);
  std::fill(depth_buffer.begin(), depth_buffer.end(), 0.f);
}

void cg::ZCulling::drawTriangle(cg::face t) {
  float x_max =
      std::max(t.vertexes[0].x, std::max(t.vertexes[1].x, t.vertexes[2].x));
  float x_min =
      std::min(t.vertexes[0].x, std::min(t.vertexes[1].x, t.vertexes[2].x));
  float y_max =
      std::max(t.vertexes[0].y, std::max(t.vertexes[1].y, t.vertexes[2].y));
  float y_min =
      std::min(t.vertexes[0].y, std::min(t.vertexes[1].y, t.vertexes[2].y));

  bool ccw = false;
  float direction = ccw ? -1.f : 1.f;
  float area =
      direction * calcEdgeFunction(t.vertexes[0].xy(), t.vertexes[1].xy(),
                                   t.vertexes[2].xy());
  for (float x = x_min; x < x_max; x += 1.f) {
    for (float y = y_min; y < y_max; y += 1.f) {
      float2 point{x, y};
      auto e0 = direction *
                calcEdgeFunction(t.vertexes[0].xy(), t.vertexes[1].xy(), point);
      auto e1 = direction *
                calcEdgeFunction(t.vertexes[1].xy(), t.vertexes[2].xy(), point);
      auto e2 = direction *
                calcEdgeFunction(t.vertexes[2].xy(), t.vertexes[0].xy(), point);
      if (e0 >= 0 && e1 >= 0 && e2 >= 0) {
        float3 bari{e1 / area, e2 / area, e0 / area};
        float z = bari.x * t.vertexes[0].z + bari.y * t.vertexes[1].z +
                  bari.z * t.vertexes[2].z;
        SetPixel(round(x), round(y), applyFragmentShader(point, t.id, bari), z);
      }
    }
  }
  // wireframe
  // Projections::drawTriangle(t);
}

void cg::ZCulling::SetPixel(uint32_t x, uint32_t y, color color, float z) {
  if (depth_buffer[y * width + x] > z) return;
  frame_buffer[y * width + x] = typename cg::color{
      uint8_t(color.r * z), uint8_t(color.g * z), uint8_t(color.b * z)};
  depth_buffer[y * width + x] = z;
}
