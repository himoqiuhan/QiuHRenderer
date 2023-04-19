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

Vec3f GetBarycentricCoordinate(Vec3f* triangle, Vec2i targetFragment)
{
	//[u,v,1]和[AB,AC,PA]对应的x和y向量都垂直, (ABx,ACx,PAx)叉乘(ABy,ACy,PAy)即可得到重心坐标
	Vec2f AB = Vec2f(triangle[1].x - triangle[0].x, triangle[1].y - triangle[0].y);
	Vec2f AC = Vec2f(triangle[2].x - triangle[0].x, triangle[2].y - triangle[0].y);
	Vec2f PA = Vec2f(triangle[0].x - targetFragment.x, triangle[0].y - targetFragment.y);

	Vec3f X(AB.x, AC.x, PA.x);
	Vec3f Y(AB.y, AC.y, PA.y);

	Vec3f u = cross(X, Y);

	if (std::abs(u.z) < 1)
		return Vec3f(-1, 1, 1);
	return Vec3f(1.f - (u.x + u.y) / u.z, u.y / u.z, u.x / u.z);
}
