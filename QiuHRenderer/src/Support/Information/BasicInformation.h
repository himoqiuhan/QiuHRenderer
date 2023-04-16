#pragma once

#include "Math/Math.h"

/// @brief ��Ļ/������Ϣ
struct Screen
{
	int width = 640;
	int height = 480;
	Screen() {};
	Screen(int w, int h) :width(w), height(h) {};
};

/// @brief ����������ռ��µı任���Ƕ��ƣ�
struct Transform
{
	Vec3f transition = Vec3f(0, 0, 0);
	Vec3f rotate = Vec3f(0, 0, 0);
	Vec3f scale = Vec3f(1, 1, 1);
};

/// @brief �����Ϣ
struct Camera
{
	Vec3f position = Vec3f(0, 0, 0); //����ռ�λ��
	Vec3f g = Vec3f(0, 0, -1); //�����������
	Vec3f t = Vec3f(0, 1, 0); //��������Ϸ���

	float FOV = 60; //Field of view
	float aspect = 16 / (float)9; //��߱�
	float near = 0.1f; //���ü������
	float far = 100.f;	//Զ�ü������

	Camera(){}
	Camera(const Screen& screen) { aspect = screen.width / (float)screen.height; }
};
