#pragma once

#pragma once

#include "Math/Math.h"
#include "Support/Information/BasicInformation.h"

Matrix GetTransition(Vec3f transition);
Matrix GetRotation(Vec3f rotateRadian);
Matrix GetScale(Vec3f scale = Vec3f(1, 1, 1));

Matrix GetModelMatrix(const Transform& transform);
/// @brief ��ȡView����
/// @param camera ���
/// @return View
Matrix GetViewMatrix(const Camera& camera);
/// @brief ��ȡ͸�Ӿ���
/// @param camera ���
/// @return Perspective
Matrix GetPerspectiveMatrix(const Camera& camera);
