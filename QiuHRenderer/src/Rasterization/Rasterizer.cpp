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
	//渲染架构：
	//传入模型、读取光照信息
	//--for(每个面)
	//		定义v2f结构体数组
	//		--for(3个顶点)
	//			GetVertexData获取当前顶点appdate_base顶点信息
	//			VertexShader给v2f赋值
	//		----
	//		使用v2f信息:
	//		GetAABB获取BoudingBox
	//		--for(BoundingBox)
	//			GetBarycentric获取重心坐标
	//			-if(在三角形内部)
	//				FragmentShader输出片元颜色color4
	//				DrawPixel
	//			-else
	//				continue
	//		----

	std::cout << std::endl;
	std::cout << "|||||||||||||||||||ExeRenderPipeline 单帧开始|||||||||||||||||||" << std::endl;

	std::cout << "|||||||||||||||||||摄像机基本信息： " << "position: " << camera.position << " LookAt: " << camera.g << " LookUp: " << camera.t << std::endl;

	for (int i = 0; i < model->nfaces(); i++)
	{
		v2f vertexout[3];
		//逐顶点执行VertexShader
		std::vector<int> face = model->face(i);//指定当前遍历到的面
		for (int j = 0; j < 3; j++)
		{
			//遍历边上的每一个顶点，进行处理
			appdata_base VertexData = GetVertexData(model, face[j]);
			vertexout[j] = VertexShader(VertexData);
		}
#ifdef DEBUG_DATASTREAMING
		std::cout << "------------------" << std::endl;
		std::cout << "v2f data streaming: " << std::endl;
		std::cout << "--屏幕空间坐标:  ";
		for (int k = 0;k < 3;k++)
		{
			std::cout << vertexout[k].screen_coord << "   ";
		}
		std::cout << std::endl;
		/*std::cout << "--世界空间坐标:  ";
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
		//逐片元执行FragmentShader
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

	std::cout << "|||||||||||||||||||ExeRenderPipeline 单帧结束|||||||||||||||||||" << std::endl;
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
	//将坐标转换为列向量
	mat<4, 1, float> colVector;
	colVector[0][0] = v.vertex.x;
	colVector[1][0] = v.vertex.y;
	colVector[2][0] = v.vertex.z;
	colVector[3][0] = 1;
	//获得世界空间下的坐标
	ret.worldPos = Vec3f((Matrix_M * colVector)[0][0], (Matrix_M * colVector)[1][0], (Matrix_M * colVector)[2][0]);

#ifdef DEBUG_VERT
	std::cout << "-------------" << std::endl;
	std::cout << "顶点世界空间坐标为：" << ret.worldPos << std::endl;
	/*std::cout << "Model Matrix = " << std::endl;
	std::cout << Matrix_M << std::endl;*/
#endif // DEBUG

	//获得屏幕空间下的坐标
	ret.screen_coord[0] = (Matrix_MVP * colVector)[0][0];
	ret.screen_coord[1] = (Matrix_MVP * colVector)[1][0];
	ret.screen_coord[2] = (Matrix_MVP * colVector)[2][0];
	ret.screen_coord[3] = (Matrix_MVP * colVector)[3][0];

	ret.PerspectiveCoefficient = ret.screen_coord[3];//获取透视除法系数，用于后续还原线性空间

	ret.screen_coord = ret.screen_coord / ret.screen_coord[3];//透视除法，获取透视正确的屏幕空间坐标

#ifdef DEBUG_VERT
	std::cout << "顶点屏幕空间坐标为：" << ret.screen_coord << std::endl;
	std::cout << "透视系数为：" << ret.PerspectiveCoefficient << std::endl;
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

