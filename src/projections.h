#pragma once

#include <linalg.h>

#include "draw_line.h"
using namespace linalg::aliases;

#include <filesystem>
#include <iostream>
#include <span>

namespace cg {

struct face {
  float4 vertexes[3];
  uint16_t id;
};

class ObjParser {
 public:
  ObjParser(std::string filename);
  virtual ~ObjParser();

  void Parse();

  const std::vector<face> &GetFaces();

 protected:
  std::filesystem::path filename;
  std::vector<face> faces;
};

struct ConstantBuffer {
  float4x4 world;
  float4x4 view;
  float4x4 projection;
};

class Projections : public LineDrawing {
 public:
  Projections(uint32_t width, uint32_t height,
              std::string obj_file);
  virtual ~Projections() = default;

  virtual void DrawScene();

 protected:
  std::unique_ptr<ObjParser> parser;
  ConstantBuffer buffer;

  void rasterize(face face);
  virtual void drawTriangle(face traingle);
  float4 applyVertexShader(float4 v);
};

}  // namespace cg
