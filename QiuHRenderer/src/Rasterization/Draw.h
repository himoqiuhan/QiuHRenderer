#include <Eigen/Dense>
#include <GLFW/glfw3.h>
#include <tuple>
#include "TinyRMath/Math.h"

using namespace Eigen;

void DrawTriangle(Vec2i* vertex, Vec3f triangleColor);
void DrawPixel(Vec2f pos, Vec3f color);
std::tuple<Vec2i,Vec2i> GetBoundingBox(Vec2i* vertex);
Vec3f GetBarycentricCoordinates(Vec2i* triangleVertex, Vec2i target);