#include "Support/Math/Matrix.h"

//#define DEBUG_MATRIX

Matrix GetTransition(Vec3f transition)
{
	Matrix ret = mat<4, 4, float>::identity();
	ret[0][3] = transition.x;
	ret[1][3] = transition.y;
	ret[2][3] = transition.z;

#ifdef DEBUG_MATRIX
	std::cout << std::endl;
	std::cout << "Transition Matrix: " << std::endl;
	for (int i = 0; i < 4; i++)
	{
		for (int j = 0; j < 4; j++)
		{
			std::cout << ret[i][j] << " ";
		}
		std::cout << std::endl;
	}
#endif

	return ret;
}


Matrix GetRotation(Vec3f rotateRadian)
{
	Matrix rotateX, rotateY, rotateZ;
	rotateX[0][0] = 1;
	rotateX[1][1] = std::cos(rotateRadian.x);
	rotateX[1][2] = -std::sin(rotateRadian.x);
	rotateX[2][1] = std::sin(rotateRadian.x);
	rotateX[2][2] = std::cos(rotateRadian.x);
	rotateX[3][3] = 1;

	rotateY[0][0] = std::cos(rotateRadian.y);
	rotateY[0][2] = std::sin(rotateRadian.y);
	rotateY[1][1] = 1;
	rotateY[2][0] = -std::sin(rotateRadian.y);
	rotateY[2][2] = std::cos(rotateRadian.y);
	rotateY[3][3] = 1;

	rotateZ[0][0] = std::cos(rotateRadian.z);
	rotateZ[0][1] = -std::sin(rotateRadian.z);
	rotateZ[1][0] = std::sin(rotateRadian.z);
	rotateZ[1][1] = std::cos(rotateRadian.z);
	rotateZ[2][2] = 1;
	rotateZ[3][3] = 1;

#ifdef DEBUG_MATRIX
	std::cout << std::endl;
	Matrix ret;
	ret = rotateX * rotateY * rotateZ;
	std::cout << "Rotation Matrix: " << std::endl;
	for (int i = 0; i < 4; i++)
	{
		for (int j = 0; j < 4; j++)
		{
			std::cout << ret[i][j] << " ";
		}
		std::cout << std::endl;
	}
#endif

	return rotateX * rotateY * rotateZ;
}

Matrix GetScale(Vec3f scale)
{
	Matrix ret = mat<4, 4, float>::identity();
	ret[0][0] = scale.x;
	ret[1][1] = scale.y;
	ret[2][2] = scale.z;
	ret[3][3] = 1;

#ifdef DEBUG_MATRIX
	std::cout << std::endl;
	std::cout << "Scale Matrix: " << std::endl;
	for (int i = 0; i < 4; i++)
	{
		for (int j = 0; j < 4; j++)
		{
			std::cout << ret[i][j] << " ";
		}
		std::cout << std::endl;
	}
#endif

	return ret;
}

Matrix GetModelMatrix(const Transform& transform)
{
	Vec3f rotateRadian(transform.rotate.x * 3.14f / 180, transform.rotate.y * 3.14f / 180, transform.rotate.z * 3.14f / 180);
#ifdef DEBUG_MATRIX
	std::cout << std::endl;
	Matrix ret;
	ret = GetTransition(transform.transition) * GetScale(transform.scale) * GetRotation(rotateRadian);
	std::cout << "Model Matrix: " << std::endl;
	for (int i = 0; i < 4; i++)
	{
		for (int j = 0; j < 4; j++)
		{
			std::cout << ret[i][j] << " ";
		}
		std::cout << std::endl;
	}
#endif
	return GetTransition(transform.transition) * GetScale(transform.scale) * GetRotation(rotateRadian);
}

Matrix GetViewMatrix(const Camera& camera)
{
	Matrix t = mat<4, 4, float>::identity();
	t[0][3] = -camera.position.x;
	t[1][3] = -camera.position.y;
	t[2][3] = -camera.position.z;

#ifdef DEBUG_MATRIX
	std::cout << std::endl;
	std::cout << "CameraTransition Matrix: " << std::endl;
	for (int i = 0; i < 4; i++)
	{
		for (int j = 0; j < 4; j++)
		{
			std::cout << t[i][j] << " ";
		}
		std::cout << std::endl;
	}
#endif

	Vec3f gxt = cross(camera.g, camera.t);

	Matrix r;
	r[0][0] = gxt.x;       r[0][1] = gxt.y;       r[0][2] = gxt.z;
	r[1][0] = camera.t.x;  r[1][1] = camera.t.y;  r[1][2] = camera.t.z;
	r[2][0] = -camera.g.x; r[2][1] = -camera.g.y; r[2][2] = -camera.g.z;
	r[3][3] = 1;

#ifdef DEBUG_MATRIX
	std::cout << std::endl;
	std::cout << "CameraRotation Matrix: " << std::endl;
	for (int i = 0; i < 4; i++)
	{
		for (int j = 0; j < 4; j++)
		{
			std::cout << r[i][j] << " ";
		}
		std::cout << std::endl;
	}
#endif

	return r * t;//先平移再旋转
}

Matrix GetPerspectiveMatrix(const Camera& camera)
{
	float cot = 1.0f / std::tan((camera.FOV / 2.0f) * 3.14f / 180);
	std::cout << "COT:" << cot << std::endl; std::cout << "ASPECT:" << camera.aspect << std::endl;
	Matrix ret;
	ret[0][0] = cot / camera.aspect;
	ret[1][1] = cot;
	ret[2][2] = -(camera.far + camera.near) / (camera.far - camera.near);
	ret[2][3] = -2 * camera.far * camera.near / (camera.far - camera.near);
	ret[3][2] = -1;

#ifdef DEBUG_MATRIX
	std::cout << std::endl;
	std::cout << "Perspective Matrix: " << std::endl;
	for (int i = 0; i < 4; i++)
	{
		for (int j = 0; j < 4; j++)
		{
			std::cout << ret[i][j] << " ";
		}
		std::cout << std::endl;
	}
#endif

	return ret;
}