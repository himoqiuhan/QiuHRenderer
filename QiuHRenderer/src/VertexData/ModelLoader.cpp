#include <iostream>
#include <string>
#include <fstream>
#include <sstream>
#include <vector>
#include "ModelLoader.h"

Model::Model(const char* filename) : verts_(), faces_() {
	std::ifstream in;
	in.open(filename, std::ifstream::in);
	if (in.fail()) return;
	std::string line;
	while (!in.eof()) {
		std::getline(in, line);
		std::istringstream iss(line.c_str());
		char trash;
		if (!line.compare(0, 2, "v ")) {
			iss >> trash;
			Vec3f v;
			for (int i = 0; i < 3; i++) iss >> v[i];
			verts_.push_back(v);
		} else if (!line.compare(0, 3, "vn ")) {
			iss >> trash >> trash;
			Vec3f n;
			for (int i = 0; i < 3; i++) iss >> n[i];
			norms_.push_back(n);
		} else if (!line.compare(0, 3, "vt ")) {
			iss >> trash >> trash;
			Vec2f uv;
			for (int i = 0; i < 2; i++) iss >> uv[i];
			uv_.push_back(uv);
		} else if (!line.compare(0, 2, "f ")) {
			std::vector<Vec3i> f;
			Vec3i tmp;
			iss >> trash;
			while (iss >> tmp[0] >> trash >> tmp[1] >> trash >> tmp[2]) {
				for (int i = 0; i < 3; i++) tmp[i]--; // obj文件的所有索引值起始于1，而非0
				f.push_back(tmp);
			}
			faces_.push_back(f);
		}
	}
	std::cerr << "模型加载完成" << std::endl;
	std::cerr << "Model Information:  Vertexes Count# " << verts_.size() << " Faces Count# " << faces_.size() << " VertexTexcoord Count# " << uv_.size() << " VertexNormal Count# " << norms_.size() << std::endl;
}

Model::~Model() {
}

int Model::nverts() {
	return (int)verts_.size();
}

int Model::nfaces() {
	return (int)faces_.size();
}

std::vector<Vec3i> Model::face(int idx) {
	return faces_[idx];
}

Vec3f Model::vertPos(int idx) {
	return verts_[idx];
}

Vec2f Model::vertUV(int idx)
{
	return uv_[idx];
}

Vec3f Model::vertNormal(int idx)
{
	return norms_[idx];
}

