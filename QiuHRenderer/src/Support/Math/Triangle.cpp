#include "Support/Math/Triangle.h"

std::tuple<Vec2i, Vec2i> GetBoudingBox(Vec3f* vertex)
{
	Vec2i bbox_min = Vec2i(vertex[0].x, vertex[0].y);
	Vec2i bbox_max = Vec2i(vertex[0].x, vertex[0].y);
	for (int i = 0; i < 3; i++)
	{
		if (vertex[i].x < bbox_min.x)
		{
			bbox_min.x = vertex[i].x;
		}
		else if (vertex[i].x > bbox_max.x)
		{
			bbox_max.x = vertex[i].x;
		}

		if (vertex[i].y < bbox_min.y)
		{
			bbox_min.y = vertex[i].y;
		}
		else if (vertex[i].y > bbox_max.y)
		{
			bbox_max.y = vertex[i].y;
		}
	}
	return { bbox_min, bbox_max };
}

Vec3f GetBarycentricCoordinate(Vec3f* triangle, Vec2f targetFragment)
{
	float alpha = (-1.f * (targetFragment.x - triangle[1].x) * (triangle[2].y - triangle[1].y) + (targetFragment.y - triangle[1].y) * (triangle[2].x - triangle[1].x)) / (float)(-1.f * (triangle[0].x - triangle[1].x) * (triangle[2].y - triangle[1].y) + (triangle[0].y - triangle[1].y) * (triangle[2].x - triangle[1].x));
	float beta = (-1.f * (targetFragment.x - triangle[2].x) * (triangle[0].y - triangle[2].y) + (targetFragment.y - triangle[2].y) * (triangle[0].x - triangle[2].x)) / (float)(-1.f * (triangle[1].x - triangle[2].x) * (triangle[0].y - triangle[2].y) + (triangle[1].y - triangle[2].y) * (triangle[0].x - triangle[2].x));
	
	return Vec3f(alpha, beta, 1.f - alpha - beta);
}
