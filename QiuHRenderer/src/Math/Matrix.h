#pragma once
#include "Math.h"

namespace MVP
{
	/// @brief 物体在世界空间下的变换（角度制）
	struct Transform 
	{
		Vec3f transition = Vec3f(0,0,0);
		Vec3f rotate = Vec3f(0,0,0);
		Vec3f scale = Vec3f(1,1,1);
	};

	struct Camera
	{
		Vec3f position = Vec3f(0,0,0); //世界空间位置
		Vec3f g = Vec3f(0,0,-1); //摄像机看向方向
		Vec3f t = Vec3f(0,1,0); //摄像机向上方向

		float FOV = 60; //Field of view
		float aspect = 16 / (float)9; //宽高比
		float near = 0.1f; //近裁剪面距离
		float far = 100.f;	//远裁剪面距离
	};

	//////////////////////////////////////////////////////////////////////////------------Model 矩阵-----------------
	Matrix GetTransition(Vec3f transition);
	Matrix GetRotation(Vec3f rotateRadian);
	Matrix GetScale(Vec3f scale = Vec3f(1,1,1));
	/// @brief 获取模型矩阵
	/// @param transition 平移
	/// @param rotate 旋转的角度
	/// @param scale 缩放
	/// @return Model Matrix
	Matrix GetModelMatrix(const Transform transform);

	//////////////////////////////////////////////////////////////////////////------------View 矩阵-----------------
	Matrix GetViewMatrix(const Camera& camera);

	//////////////////////////////////////////////////////////////////////////------------Perspective 矩阵-----------------
	Matrix GetPerspectiveMatrix(const Camera& camera);
}