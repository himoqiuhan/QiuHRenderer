#pragma once

#pragma once

#include "Math/Math.h"
#include "Support/Information/BasicInformation.h"

Matrix GetTransition(Vec3f transition);
Matrix GetRotation(Vec3f rotateRadian);
Matrix GetScale(Vec3f scale = Vec3f(1, 1, 1));

Matrix GetModelMatrix(const Transform& transform);
/// @brief 获取View矩阵
/// @param camera 相机
/// @return View
Matrix GetViewMatrix(const Camera& camera);
/// @brief 获取透视矩阵
/// @param camera 相机
/// @return Perspective
Matrix GetPerspectiveMatrix(const Camera& camera);
