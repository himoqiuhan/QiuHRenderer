#pragma once
#include <GLFW/glfw3.h>
#include "VertexData/ModelLoader.h"
#include "Support/Information/BasicInformation.h"
#include "Support/Math/Triangle.h"
#include "Support/Math/Matrix.h"
#include "VertexData/TGALoader.h"

struct appdata_base
{
	Vec3f vertex;//顶点数据
	Vec3f normal;//顶点法线
	Vec2f texcoord;//顶点UV坐标
};

struct v2f
{
	Vec4f clipPos;//裁剪空间坐标
	Vec3f worldPos;//世界空间坐标
	Vec3f worldNromal;//世界空间顶点法线
	Vec2f uv;//顶点UV坐标
};

struct color4 
{
	float r;
	float g;
	float b;
	float a;
	color4() : r(1), g(1), b(1), a(1) {};
	color4(float R, float G, float B, float A) : r(R), g(G), b(B), a(A) {}
	color4 operator*(float num) { return color4(r * num, g * num, b * num, a * num); }
	color4 operator*(color4 other) { return color4(r * other.r, g * other.g, b * other.b, a * other.a); }
	color4 operator+(color4 other) { return color4(r + other.r, g + other.g, b + other.b, a + other.a); }
	color4 AddContrast(float power) { return color4(std::pow(r, power), std::pow(g, power), std::pow(b, power), 1.f); }
};

class Rasterizer
{
private:
	Transform transform;
	Camera camera;
	Screen screen;

	Matrix Matrix_M;
	Matrix Matrix_V;
	Matrix Matrix_P;
	Matrix Matrix_MVP;

	Vec3f worldLightDir;

	TGAImage* diffuseTex;

	std::vector<float> ZBuffer;
	std::vector<color4> FrameBuffer;
	std::vector<float> LuminanceBuffer;

	//DEBUG
	int fragCounter = 0;

public:
	Rasterizer(const Screen& screen);//构造对象，指定摄像机
	~Rasterizer();

	void SetTransform(Vec3f transition, Vec3f rotation, Vec3f scale);
	void SetPerspective();
	void SetOrthogonal();
	void SetCamera(Vec3f _position, Vec3f _lookAt, Vec3f _lookUp, float _FOV, float _aspect, float _near, float _far);
	void SetCamera(Vec3f _position, Vec3f _lookAt, Vec3f _lookUp);
	void SetCamera(Vec3f _position);

	void ExeRenderPipeline(Model* model, TGAImage* diffuse, Vec3f light_dir);//执行渲染管线，其中包含模型的读取、顶点着色器、图片的读取和片元着色器，并且最终渲染到RenderTarget上

	//Used In RenderPipeline
	appdata_base GetVertexData(Model* model, Vec3i faceIndex);
	v2f VertexShader(appdata_base v);
	bool canClip(Vec3f* homogeneousPos);
	Vec3f ScreenMapping(Vec3f screen_coord);
	bool IsInTriangle(Vec2i frag, Vec3f* triangleVertex);
	float GetFragHomogeneousDepth(float* triangleVertexDepths, Vec3f barycoord);
	float GetFragW(float* triangleVertexW, Vec3f barycoord);
	color4 FragmentShader(v2f* i, float fragW, Vec3f barycoord);
	//Used In Vertex Shader
	Vec3f ObjectToWorldNormal(Vec3f normal);
	

	Vec3f GetFragNormalByVertNormal(Vec3f* triangleVertexNormals, Vec3f barycoord);

	void DrawFrag(Vec2f pos, color4 color);
};