#pragma once

#include <string>
#include <vector>
#include "Support/Math/Math.h"

class Model {
private:
	std::vector<Vec3f> verts_;//�涥��λ����Ϣ
	std::vector<Vec3f> norms_;//�涥�㷨����Ϣ
	std::vector<Vec2f> uv_;//�涥��UV��Ϣ
	std::vector<std::vector<Vec3i> > faces_;//��ÿ�����µĶ���λ�á�UV�ͷ��ߵ�����
public:
	Model(const char* filename);
	~Model();
	int nverts(); //��ö�������
	int nfaces(); //����������
	Vec3f vertPos(int idx); //����������ö���������Ϣ
	Vec2f vertUV(int idx); //����������ö���UV��Ϣ
	Vec3f vertNormal(int idx); //����������ö��㷨����Ϣ
	std::vector<Vec3i> face(int idx); //��ȡ��ǰidx��Ķ���λ�á�UV�ͷ��ߵ�����
};
