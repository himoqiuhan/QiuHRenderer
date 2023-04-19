#include "Rasterization/Draw.h"

#include <iostream>

void DrawTriangle(Vec3f* vertex, Vec3f triangleColor)
{
	//1.��ȡAABB��
	auto bbox = GetBoundingBox(vertex);
	Vec3f BarycentricCoordinate;
	for (int y = std::get<0>(bbox).y;y<=std::get<1>(bbox).y;y++)
	{
		for (int x = std::get<0>(bbox).x; x <= std::get<1>(bbox).x; x++)
		{
			//2.����AABB�У��ж϶����Ƿ�����������
			BarycentricCoordinate = GetBarycentricCoordinates(vertex,Vec2f(x, y));
			if (BarycentricCoordinate.x < 0 || BarycentricCoordinate.y < 0 || BarycentricCoordinate.z < 0)
			{
				continue;
			}
			//3.������������ڣ��������ɫ����������
			DrawPixel(Vec2f(x, y), triangleColor);
		}
	}
}

void DrawPixel(Vec2f pos, Vec3f color)
{
	float posx = (pos.x / 640) * 2 - 1.f;
	float posy = (pos.y / 640) * 2 - 1.f;
	glBegin(GL_POINTS);
	glPointSize(1);
	glColor3f(color.x, color.y, color.z);
	glVertex2f(posx, posy);
	glEnd();
}

std::tuple<Vec2i, Vec2i> GetBoundingBox(Vec3f* vertex)
{
	Vec2i bbox_min = Vec2i(vertex[0].x,vertex[0].y);
	Vec2i bbox_max = Vec2i(vertex[0].x,vertex[0].y);
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

Vec3f GetBarycentricCoordinates(Vec3f* triangleVertex, Vec2i target)
{
	//[u,v,1]��[AB,AC,PA]��Ӧ��x��y��������ֱ, (ABx,ACx,PAx)���(ABy,ACy,PAy)���ɵõ���������
	Vec2f AB = Vec2f(triangleVertex[1].x - triangleVertex[0].x, triangleVertex[1].y - triangleVertex[0].y);
	Vec2f AC = Vec2f(triangleVertex[2].x - triangleVertex[0].x, triangleVertex[2].y - triangleVertex[0].y);
	Vec2f PA = Vec2f(triangleVertex[0].x - target.x, triangleVertex[0].y - target.y);

	Vec3f X(AB.x, AC.x, PA.x);
	Vec3f Y(AB.y, AC.y, PA.y);

	Vec3f u = cross(X, Y);

	if (std::abs(u.z) < 1) 
		return Vec3f(-1, 1, 1);
	return Vec3f(1.f - (u.x + u.y) / u.z, u.y / u.z, u.x / u.z);
}

