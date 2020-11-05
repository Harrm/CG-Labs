#include "projections.h"

#define _USE_MATH_DEFINES
#include <math.h>

#include <iostream>

#define TINYOBJLOADER_IMPLEMENTATION  // define this in only *one* .cc
#include "tiny_obj_loader.h"

cg::ObjParser::ObjParser(std::string filename) : filename(filename) {}

cg::ObjParser::~ObjParser() {}

void cg::ObjParser::Parse() {
  tinyobj::attrib_t attrib;
  std::vector<tinyobj::shape_t> shapes;
  std::vector<tinyobj::material_t> materials;

  std::string warn;
  std::string err;

  bool ret = tinyobj::LoadObj(&attrib, &shapes, &materials, &warn, &err,
                              filename.string().c_str(),
                              filename.parent_path().string().c_str(), true);

  if (!warn.empty()) {
    std::cout << warn << std::endl;
  }

  if (!err.empty()) {
    std::cerr << err << std::endl;
  }

  if (!ret) {
    throw std::runtime_error("Failed to parse OBJ file " + filename.string() +
                             " reason: " + err);
  }

  // Loop over shapes
  for (size_t s = 0; s < shapes.size(); s++) {
    // Loop over faces(polygon)
    size_t index_offset = 0;
    for (size_t f = 0; f < shapes[s].mesh.num_face_vertices.size(); f++) {
      int fv = shapes[s].mesh.num_face_vertices[f];

      face current;
      // Loop over vertices in the face.
      for (size_t v = 0; v < fv; v++) {
        // access to vertex
        tinyobj::index_t idx = shapes[s].mesh.indices[index_offset + v];
        tinyobj::real_t vx = attrib.vertices[3 * idx.vertex_index + 0];
        tinyobj::real_t vy = attrib.vertices[3 * idx.vertex_index + 1];
        tinyobj::real_t vz = attrib.vertices[3 * idx.vertex_index + 2];
        current.vertexes[v] = float4(vx, vy, vz, 1.f);
      }
      index_offset += fv;
      faces.push_back(std::move(current));
    }
  }
}

const std::vector<cg::face>& cg::ObjParser::GetFaces() { return faces; }

cg::Projections::Projections(uint32_t width, uint32_t height,
                             std::string obj_file)
    : cg::LineDrawing(width, height),
      parser{std::make_unique<ObjParser>(obj_file)},
      // column-wise
      buffer{} {
  buffer.world = {
    {1, 0, 0, 0},
    {0, 1, 0, 0},
    {0, 0, 1, 0},
    {0, -1, -3, 1}};
  float3 eye{0, 0, 1};
  float3 at{0, 0, 0};
  float3 up{0, -1, 0};

  float3 zaxis = normalize(at - eye);
  float3 xaxis = normalize(cross(up, zaxis));
  float3 yaxis = cross(zaxis, xaxis);

  buffer.view = float4x4{{xaxis.x, xaxis.y, xaxis.z, -dot(xaxis, eye)},
                         {yaxis.x, yaxis.y, yaxis.z, -dot(yaxis, eye)},
                         {zaxis.x, zaxis.y, zaxis.z, -dot(zaxis, eye)},
                         {0, 0, 0, 1}};

  float z_near = 1.0f;
  float z_far = 10.f;
  float near_height = 1.f;
  float near_width = width / (float)height;

  buffer.projection = float4x4{
      {2 * z_near / near_width, 0, 0, 0},
      {0, z_near / near_height, 0, 0},
      {0, 0, z_far / (z_far - z_near), z_near * z_far / (z_far - z_near)},
      {0, 0, 1, 0}};

  parser->Parse();
}

float4 cg::Projections::applyVertexShader(float4 v) {
  return linalg::mul(buffer.projection,
                     linalg::mul(buffer.view, 
                     linalg::mul(buffer.world, v)));
}

void cg::Projections::rasterize(face face) {
  // to screen space
  uint32_t x_center{width / 2}, y_center{height / 2};
  uint32_t scale = std::min(x_center, y_center) - 1;

  for (uint8_t i = 0; i < 3; i++) {
    face.vertexes[i] /= face.vertexes[i].w;
    face.vertexes[i].x =
        std::clamp(x_center + scale * face.vertexes[i].x, 0.f, width - 1.f);
    face.vertexes[i].y =
        std::clamp(y_center + scale * face.vertexes[i].y, 0.f, height - 1.f);
  }
  drawTriangle(face);
}

void cg::Projections::drawTriangle(face face) {
  DrawLine(face.vertexes[0].x, face.vertexes[0].y, face.vertexes[1].x,
           face.vertexes[1].y, color{255, 0, 0});
  DrawLine(face.vertexes[1].x, face.vertexes[1].y, face.vertexes[2].x,
           face.vertexes[2].y, color{0, 255, 0});
  DrawLine(face.vertexes[2].x, face.vertexes[2].y, face.vertexes[0].x,
           face.vertexes[0].y, color{0, 0, 255});
}

void cg::Projections::DrawScene() {
  using namespace linalg::ostream_overloads;
  uint16_t id = 0;
  for (auto face : parser->GetFaces()) {
    face.id = id++;
    for (auto& v : face.vertexes) v = applyVertexShader(v);
    rasterize(face);
  }
}
