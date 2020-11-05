#pragma once

#include "projections.h"

namespace cg {

class TriangleRasterization : public Projections {
 public:
  TriangleRasterization(uint32_t width, uint32_t height, std::string obj_file);
  virtual ~TriangleRasterization() = default;

 protected:
  void drawTriangle(face traingle) override;
  float calcEdgeFunction(float2 a, float2 b, float2 target);
  color applyFragmentShader(float2 pos, uint16_t id,
                            float3 baricentric_pos) const;
};

}  // namespace cg
