#pragma once
#include "Math.h"

namespace MVP
{
	/// @brief ����������ռ��µı任���Ƕ��ƣ�
	struct Transform 
	{
		Vec3f transition = Vec3f(0,0,0);
		Vec3f rotate = Vec3f(0,0,0);
		Vec3f scale = Vec3f(1,1,1);
	};

	struct Camera
	{
		Vec3f position = Vec3f(0,0,0); //����ռ�λ��
		Vec3f g = Vec3f(0,0,-1); //�����������
		Vec3f t = Vec3f(0,1,0); //��������Ϸ���

		float FOV = 60; //Field of view
		float aspect = 16 / (float)9; //��߱�
		float near = 0.1f; //���ü������
		float far = 100.f;	//Զ�ü������
	};

	//////////////////////////////////////////////////////////////////////////------------Model ����-----------------
	Matrix GetTransition(Vec3f transition);
	Matrix GetRotation(Vec3f rotateRadian);
	Matrix GetScale(Vec3f scale = Vec3f(1,1,1));
	/// @brief ��ȡģ�;���
	/// @param transition ƽ��
	/// @param rotate ��ת�ĽǶ�
	/// @param scale ����
	/// @return Model Matrix
	Matrix GetModelMatrix(const Transform transform);

	//////////////////////////////////////////////////////////////////////////------------View ����-----------------
	Matrix GetViewMatrix(const Camera& camera);

	//////////////////////////////////////////////////////////////////////////------------Perspective ����-----------------
	Matrix GetPerspectiveMatrix(const Camera& camera);
}