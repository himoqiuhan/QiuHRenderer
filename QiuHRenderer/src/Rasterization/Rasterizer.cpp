#include "Rasterization/Rasterizer.h"

Rasterizer::Rasterizer(const Screen& screen)
{
	camera.aspect = screen.width / (float)screen.height;
}

Rasterizer::~Rasterizer() {}


void Rasterizer::SetTransform(Vec3f transition, Vec3f rotation, Vec3f scale)
{
	transform.transition = transition;
	transform.rotate = rotation;
	transform.scale = scale;
	Matrix_M = GetModelMatrix(transform);
}

void Rasterizer::SetPerspective()
{
	Matrix_V = GetViewMatrix(camera);
	Matrix_P = GetPerspectiveMatrix(camera);
	Matrix_MVP = Matrix_P * Matrix_V * Matrix_M;
}

void Rasterizer::SetOrthogonal()
{

}

void Rasterizer::SetCamera(Vec3f _position, Vec3f _lookAt, Vec3f _lookUp, float _FOV, float _aspect, float _near, float _far)
{
	camera.position = _position;
	camera.g = _lookAt;
	camera.t = _lookUp;
	camera.FOV = _FOV;
	camera.aspect = _aspect;
	camera.near = _near;
	camera.far = _far;
}

void Rasterizer::ExeRenderPipeline(Model* model, Vec3f light_dir)
{
	//��Ⱦ�ܹ���
	//����ģ�͡���ȡ������Ϣ
	//--for(ÿ����)
	//		����v2f�ṹ������
	//		--for(3������)
	//			GetVertexData��ȡ��ǰ����appdate_base������Ϣ
	//			VertexShader��v2f��ֵ
	//		----
	//		ʹ��v2f��Ϣ:
	//		GetAABB��ȡBoudingBox
	//		--for(BoundingBox)
	//			GetBarycentric��ȡ��������
	//			-if(���������ڲ�)
	//				FragmentShader���ƬԪ��ɫcolor4
	//				DrawPixel
	//			-else
	//				continue
	//		----

	for (int i = 0; i < model->nfaces(); i++)
	{
		v2f vertexout[3];
		//�𶥵�ִ��VertexShader
		std::vector<int> face = model->face(i);
		for (int j = 0; j < 3; j++)
		{
			appdata_base VertexData = GetVertexData(model, face[j]);
			vertexout[j] = VertexShader(VertexData);
		}
		Vec3f ScreenTriangle[3] = {
			Vec3f(vertexout[0].screen_coord[0],vertexout[0].screen_coord[1],vertexout[0].screen_coord[2]),
			Vec3f(vertexout[1].screen_coord[0],vertexout[1].screen_coord[1],vertexout[1].screen_coord[2]),
			Vec3f(vertexout[2].screen_coord[0],vertexout[2].screen_coord[1],vertexout[2].screen_coord[2]) 
		};
		std::tuple<Vec2i,Vec2i> bbox = GetBoudingBox(ScreenTriangle);
		//��ƬԪִ��FragmentShader
		for (int y = std::get<0>(bbox).y; y <= std::get<1>(bbox).y; y++)
		{
			for (int x = std::get<0>(bbox).x; x <= std::get<1>(bbox).x; x++)
			{
				Vec3f BarycentricCoordinate = GetBarycentricCoordinate(ScreenTriangle, Vec2f(x, y));

				if (BarycentricCoordinate.x < 0 || BarycentricCoordinate.y < 0 || BarycentricCoordinate.z < 0)
				{
					continue;
				}
				
				color4 FragColor = FragmentShader(vertexout, BarycentricCoordinate);
				DrawFrag(Vec2f(x, y), FragColor);
			}
		}
	}
}

appdata_base Rasterizer::GetVertexData(Model* model, int vertexIndex)
{
	appdata_base ret;
	ret.vertex = model->vert(vertexIndex);

	return ret;
}

v2f Rasterizer::VertexShader(appdata_base v)
{
	v2f ret;
	mat<4, 1, float> colVector;
	colVector[0][0] = v.vertex.x;
	colVector[1][0] = v.vertex.y;
	colVector[2][0] = v.vertex.z;
	colVector[3][0] = 1;
	//�������ռ��µ�����
	ret.worldPos = Vec3f((Matrix_M * colVector)[0][0], (Matrix_M * colVector)[1][0], (Matrix_M * colVector)[2][0]);
	//�����Ļ�ռ��µ�����
	ret.screen_coord[0] = (Matrix_MVP * colVector)[0][0];
	ret.screen_coord[1] = (Matrix_MVP * colVector)[1][0];
	ret.screen_coord[2] = (Matrix_MVP * colVector)[2][0];
	ret.screen_coord[3] = (Matrix_MVP * colVector)[3][0];

	ret.PerspectiveCoefficient = ret.screen_coord[3];//��ȡ͸�ӳ���ϵ�������ں�����ԭ���Կռ�
	
	ret.screen_coord = ret.screen_coord / ret.screen_coord[3];//͸�ӳ�������ȡ��ȷ����Ļ�ռ�����

	return ret;
}

color4 Rasterizer::FragmentShader(v2f* i, Vec3f barycoord)
{
	return { 1,1,1,1 };
}

void Rasterizer::DrawFrag(Vec2f pos, color4 color)
{
	float posx = (pos.x / 640) * 2 - 1.f;
	float posy = (pos.y / 640) * 2 - 1.f;
	glBegin(GL_POINTS);
	glPointSize(1);
	glColor3f(color.r, color.g, color.b);
	glVertex2f(posx, posy);
	glEnd();
}

