#pragma once
#include <GLFW/glfw3.h>
#include "VertexData/ModelLoader.h"
#include "Support/Information/BasicInformation.h"
#include "Support/Math/Triangle.h"
#include "Support/Math/Matrix.h"

struct appdata_base
{
	Vec3f vertex;//��������
	Vec3f normal;//���㷨��
	Vec3f texcoord;//����UV����
};

struct v2f
{
	Vec4f screen_coord;//��Ļ�ռ�����
	Vec3f worldPos;//����ռ�����
	float PerspectiveCoefficient;//͸�ӳ���ϵ��
};

struct color4 
{
	float r;
	float g;
	float b;
	float a;
};

class Rasterizer
{
private:
	Transform transform;
	Camera camera;

	Matrix Matrix_M;
	Matrix Matrix_V;
	Matrix Matrix_P;
	Matrix Matrix_MVP;

	Vec3f world_coords[3];
	Vec3i screen_coords[3];

public:
	Rasterizer(const Screen& screen);//�������ָ�������
	~Rasterizer();

	void SetTransform(Vec3f transition, Vec3f rotation, Vec3f scale);
	void SetPerspective();
	void SetOrthogonal();
	void SetCamera(Vec3f _position, Vec3f _lookAt, Vec3f _lookUp, float _FOV, float _aspect, float _near, float _far);
	void SetCamera(Vec3f _position, Vec3f _lookAt, Vec3f _lookUp);

	void ExeRenderPipeline(Model* model, Vec3f light_dir, Screen screen);//ִ����Ⱦ���ߣ����а���ģ�͵Ķ�ȡ��������ɫ����ͼƬ�Ķ�ȡ��ƬԪ��ɫ��������������Ⱦ��RenderTarget��

	appdata_base GetVertexData(Model* model, int vertexIndex);
	v2f VertexShader(appdata_base v);
	Vec3f ScreenMapping(Vec3f screen_coord, Screen screen);
	color4 FragmentShader(v2f* i, Vec3f barycoord);

	void DrawFrag(Vec2f pos, color4 color);

	
};