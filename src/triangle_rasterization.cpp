#include "triangle_rasterization.h"

#define _USE_MATH_DEFINES
#include <math.h>

#include <iostream>

cg::TriangleRasterization::TriangleRasterization(uint32_t width,
                                                 uint32_t height,
                                                 std::string obj_file)
    : cg::Projections(width, height, obj_file) {}

void cg::TriangleRasterization::drawTriangle(face t) {
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
      auto e0 = direction * calcEdgeFunction(t.vertexes[0].xy(), t.vertexes[1].xy(), point);
      auto e1 = direction * calcEdgeFunction(t.vertexes[1].xy(), t.vertexes[2].xy(), point);
      auto e2 = direction * calcEdgeFunction(t.vertexes[2].xy(), t.vertexes[0].xy(), point);
      if ( e0 >= 0 && e1 >= 0 && e2 >= 0) {
        float3 bari { e0 / area, e1 / area, e2 / area };
        SetPixel(round(x), round(y), applyFragmentShader(point, t.id, bari));
      }
    }
  }
  // wireframe
  Projections::drawTriangle(t);
}

float cg::TriangleRasterization::calcEdgeFunction(float2 a, float2 b,
                                                  float2 target) {
  auto dy = b.y - a.y;
  auto dx = b.x - a.x;
  return (target.x - a.x) * dy - (target.y - a.y) * dx;
}

cg::color cg::TriangleRasterization::applyFragmentShader(
    float2 pos, uint16_t id, float3 baricentric_pos) const {
  float3 color = baricentric_pos * 255.f;
  if (id < 10) return cg::color{255, 255, 255};
  if (id < 19) return cg::color{255, 0, 0};
  if (id < 34) return cg::color{0, 0, 255};
  return cg::color{255, 255, 0};
}
