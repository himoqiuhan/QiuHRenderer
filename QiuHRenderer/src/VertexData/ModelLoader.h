#pragma once

#include <string>
#include <vector>
#include "Support/Math/Math.h"

class Model {
private:
	std::vector<Vec3f> verts_;//存顶点位置信息
	std::vector<Vec3f> norms_;//存顶点法线信息
	std::vector<Vec2f> uv_;//存顶点UV信息
	std::vector<std::vector<Vec3i> > faces_;//存每个面下的顶点位置、UV和法线的索引
public:
	Model(const char* filename);
	~Model();
	int nverts(); //获得顶点数量
	int nfaces(); //获得面的数量
	Vec3f vertPos(int idx); //根据索引获得顶点坐标信息
	Vec2f vertUV(int idx); //根据索引获得顶点UV信息
	Vec3f vertNormal(int idx); //根据索引获得顶点法线信息
	std::vector<Vec3i> face(int idx); //获取当前idx面的顶点位置、UV和法线的索引
};
