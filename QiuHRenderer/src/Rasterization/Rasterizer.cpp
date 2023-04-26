#include "Rasterization/Rasterizer.h"

//#define DEBUG_VERT
//#define DEBUG_DATASTREAMING

Rasterizer::Rasterizer(const Screen& screen)
{
	//camera.aspect = screen.width / (float)screen.height;
	camera.position = Vec3f(0, 0, 2);
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

void Rasterizer::SetCamera(Vec3f _position, Vec3f _lookAt, Vec3f _lookUp)
{
	camera.position = _position;
	camera.g = _lookAt;
	camera.t = _lookUp;
}


void Rasterizer::ExeRenderPipeline(Model* model, Vec3f light_dir, Screen screen)
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

	std::cout << std::endl;
	std::cout << "|||||||||||||||||||ExeRenderPipeline ��֡��ʼ|||||||||||||||||||" << std::endl;

	std::cout << "|||||||||||||||||||�����������Ϣ�� " << "position: " << camera.position << " LookAt: " << camera.g << " LookUp: " << camera.t << std::endl;

	for (int i = 0; i < model->nfaces(); i++)
	{
		v2f vertexout[3];
		//�𶥵�ִ��VertexShader
		std::vector<int> face = model->face(i);//ָ����ǰ����������
		for (int j = 0; j < 3; j++)
		{
			//�������ϵ�ÿһ�����㣬���д���
			appdata_base VertexData = GetVertexData(model, face[j]);
			vertexout[j] = VertexShader(VertexData);
		}
#ifdef DEBUG_DATASTREAMING
		std::cout << "------------------" << std::endl;
		std::cout << "v2f data streaming: " << std::endl;
		std::cout << "--��Ļ�ռ�����:  ";
		for (int k = 0;k < 3;k++)
		{
			std::cout << vertexout[k].screen_coord << "   ";
		}
		std::cout << std::endl;
		/*std::cout << "--����ռ�����:  ";
		for (int k = 0; k < 3; k++)
		{
			std::cout << vertexout[k].worldPos << "   ";
		}
		std::cout << std::endl;*/
		std::cout << "--PerspectiveCoefficient:  ";
		for (int k = 0; k < 3; k++)
		{
			std::cout << vertexout[k].PerspectiveCoefficient << "   ";
		}
		std::cout << std::endl;

#endif // DEBUG

		Vec3f ScreenTriangle[3] = {
			ScreenMapping(Vec3f(vertexout[0].screen_coord[0],vertexout[0].screen_coord[1],vertexout[0].screen_coord[2]),screen),
			ScreenMapping(Vec3f(vertexout[1].screen_coord[0],vertexout[1].screen_coord[1],vertexout[1].screen_coord[2]),screen),
			ScreenMapping(Vec3f(vertexout[2].screen_coord[0],vertexout[2].screen_coord[1],vertexout[2].screen_coord[2]),screen)
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

	std::cout << "|||||||||||||||||||ExeRenderPipeline ��֡����|||||||||||||||||||" << std::endl;
	std::cout << std::endl;
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
	//������ת��Ϊ������
	mat<4, 1, float> colVector;
	colVector[0][0] = v.vertex.x;
	colVector[1][0] = v.vertex.y;
	colVector[2][0] = v.vertex.z;
	colVector[3][0] = 1;
	//�������ռ��µ�����
	ret.worldPos = Vec3f((Matrix_M * colVector)[0][0], (Matrix_M * colVector)[1][0], (Matrix_M * colVector)[2][0]);

#ifdef DEBUG_VERT
	std::cout << "-------------" << std::endl;
	std::cout << "��������ռ�����Ϊ��" << ret.worldPos << std::endl;
	/*std::cout << "Model Matrix = " << std::endl;
	std::cout << Matrix_M << std::endl;*/
#endif // DEBUG

	//�����Ļ�ռ��µ�����
	ret.screen_coord[0] = (Matrix_MVP * colVector)[0][0];
	ret.screen_coord[1] = (Matrix_MVP * colVector)[1][0];
	ret.screen_coord[2] = (Matrix_MVP * colVector)[2][0];
	ret.screen_coord[3] = (Matrix_MVP * colVector)[3][0];

	ret.PerspectiveCoefficient = ret.screen_coord[3];//��ȡ͸�ӳ���ϵ�������ں�����ԭ���Կռ�

	ret.screen_coord = ret.screen_coord / ret.screen_coord[3];//͸�ӳ�������ȡ͸����ȷ����Ļ�ռ�����

#ifdef DEBUG_VERT
	std::cout << "������Ļ�ռ�����Ϊ��" << ret.screen_coord << std::endl;
	std::cout << "͸��ϵ��Ϊ��" << ret.PerspectiveCoefficient << std::endl;
	/*std::cout << "MVP Matrix = " << std::endl;
	std::cout << Matrix_MVP << std::endl;*/
	std::cout << "-------------" << std::endl;
	
#endif // DEBUG
	
	return ret;
}

Vec3f Rasterizer::ScreenMapping(Vec3f screen_coord, Screen screen)
{
	return Vec3f((screen_coord.x + 1.) * screen.width/2, (screen_coord.y + 1.) * screen.height/2, screen_coord.z);
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

