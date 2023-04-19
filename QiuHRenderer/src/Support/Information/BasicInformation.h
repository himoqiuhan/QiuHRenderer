#pragma once

#include "Support/Math/Math.h"

/// @brief 屏幕/窗口信息
struct Screen
{
	int width = 640;
	int height = 480;
	Screen() {};
	Screen(int w, int h) :width(w), height(h) {};
};

/// @brief 物体在世界空间下的变换（角度制）
struct Transform
{
	Vec3f transition = Vec3f(0, 0, 0);
	Vec3f rotate = Vec3f(0, 0, 0);
	Vec3f scale = Vec3f(1, 1, 1);

	void SetTransform(Vec3f _transition, Vec3f _rotation, Vec3f _scale)
	{
		transition = _transition;
		rotate = _rotation;
		scale = _scale;
	}
};

/// @brief 相机信息
struct Camera
{
	Vec3f position = Vec3f(0, 0, 0); //世界空间位置
	Vec3f g = Vec3f(0, 0, -1); //摄像机看向方向
	Vec3f t = Vec3f(0, 1, 0); //摄像机向上方向

	float FOV = 60; //Field of view
	float aspect = 16 / (float)9; //宽高比
	float near = 0.1f; //近裁剪面距离
	float far = 100.f;	//远裁剪面距离
};
