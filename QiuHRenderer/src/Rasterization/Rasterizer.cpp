#include "Rasterization/Rasterizer.h"

//#define DEBUG_VERT
//#define DEBUG_DATASTREAMING
//#define DEBUG_FRAG
//#define DEBUG_TEXTUREREADING

//#define FLATSHADER

Rasterizer::Rasterizer(const Screen& screen)
{
	camera.aspect = screen.width / (float)screen.height;
	this->screen = screen;
	ZBuffer.resize(screen.width * screen.height);
	std::fill(ZBuffer.begin(), ZBuffer.end(), 99);
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

void Rasterizer::SetCamera(Vec3f _position)
{
	camera.position = _position;
}

void Rasterizer::ExeRenderPipeline(Model* model, TGAImage* diffuse, Vec3f light_dir)
{
	//��Ⱦ�ܹ���
	//����ģ�͡���ȡ������Ϣ����ͼ��Ϣ
	//--for(ÿ����)
	//		����v2f�ṹ������
	//		
	//		--for(3������)
	//			GetVertexData��ȡ��ǰ����appdate_base������Ϣ
	//									{����λ�á�����UV�����㷨��}
	//			VertexShader��v2f��ֵ
	//						  {�ü��ռ����ꡢ����ռ����ꡢ����ռ䷨�ߡ�����UV}
	//		----
	//		
	//		͸�ӳ���->��βü��ռ䣺�򵥴ֱ��ķ��������Ƭ continue!(��Ϊд��Ⱦ����Ŀ�������������Ⱦ��ˮ�ߣ����Բ�������)
	// 
	//		��Ļӳ��->��Ļ�ռ䣺����������
	//			����������GetAABB��ȡBoudingBox
	// 
	//		�����α���
	//		--for(BoundingBox)
	//			IsInTriangle����ж�ƬԪ�Ƿ�����������
	//			-if(���������ڲ�)
	//				GetBarycentric��ȡ��Ļ�ϵ���������
	//				��ֵ�������ƬԪ��candidate cube �е�zֵ->��ȡcandidate cube �е�����
	//				��βü��ռ������ת��������ռ���
	//				����͸�ӳ���ϵ�������Ƴ��ü��ռ��е����ֵfragDepth
	// 
	//				�ж�fragDepth��zbuffer�ڵ����ֵ
	//				if(fragDepth.abs < zbuffer[])
	//					д�뵱ǰ���ֵ
	//					FragmentShader���ƬԪ��ɫcolor4
	//					DrawPixel
	//				else
	//				  �������κδ���
	//			-else
	//				continue
	//		----

	worldLightDir = light_dir * -1.f;
	this->diffuseTex = diffuse;

	std::cout << std::endl;
	std::cout << "|||||||||||||||||||ExeRenderPipeline ��֡��ʼ|||||||||||||||||||" << std::endl;

	std::cout << "||||||||||||||||�����������Ϣ�� " << "position: " << camera.position << " LookAt: " << camera.g << " LookUp: " << camera.t << std::endl;
	std::cout << "||||||||||||||||���շ���" << worldLightDir << std::endl;

	for (int i = 0; i < model->nfaces(); i++)
	{

		v2f vertexout[3];
		std::vector<Vec3i> face = model->face(i);//ָ����ǰ����������

		for (int j = 0; j < 3; j++)
		{
			//�������ϵ�ÿһ�����㣬���д���
			appdata_base VertexData = GetVertexData(model, face[j]);
			vertexout[j] = VertexShader(VertexData);
		}

#ifdef DEBUG_DATASTREAMING

		std::cout << "------------------" << std::endl;
		std::cout << "v2f data streaming: " << std::endl;
		
		std::cout << "--�ü��ռ�����:  ";
		for (int k = 0; k < 3; k++)
		{
			std::cout << vertexout[k].clipPos << "   ";
		}
		std::cout << std::endl;
		std::cout << "--����ռ�����:  ";
		for (int k = 0; k < 3; k++)
		{
			std::cout << vertexout[k].worldPos << "   ";
		}
		std::cout << std::endl;
		std::cout << "--����ռ䷨�ߣ�  ";
		for (int k = 0; k < 3; k++)
		{
			std::cout << vertexout[k].worldNromal << "   ";
		}
		std::cout << std::endl;
		std::cout << "--UV����:  ";
		for (int k = 0; k < 3; k++)
		{
			std::cout << vertexout[k].uv << "   ";
		}
		std::cout << std::endl;		

#endif // DEBUG
		
		//͸�ӳ���
		float w[3];//�洢͸�ӳ���ϵ��
		Vec3f homogeneousClipPos[3];//��βü��ռ�
		for (int j = 0; j < 3; j++)
		{
			w[j] = vertexout[j].clipPos.w;
			homogeneousClipPos[j] = (vertexout[j].clipPos / w[j]).xyz();
		}

		//������βü�
		if (canClip(homogeneousClipPos)) continue;

		//��Ļӳ��
		Vec3f screen_coords[3] = {
			ScreenMapping(homogeneousClipPos[0]),
			ScreenMapping(homogeneousClipPos[1]),
			ScreenMapping(homogeneousClipPos[2])
		};

		//����������-->���������λ�ȡAABB
		std::tuple<Vec2i,Vec2i> bbox = GetBoudingBox(screen_coords);


		//////////////////////////////////////////////////////
		//------------------�𶥵�Ĳ�������------------------//
		//////////////////////////////////////////////////////
		//////////////////////////////////////////////////////
		//////////////////////////////////////////////////////
		//////////////////////////////////////////////////////
		//--------------���ˣ���ʼ��ƬԪ���в���--------------//
		//////////////////////////////////////////////////////

		//��ƬԪִ��FragmentShader
		for (int y = std::get<0>(bbox).y; y <= std::get<1>(bbox).y; y++)
		{
			for (int x = std::get<0>(bbox).x; x <= std::get<1>(bbox).x; x++)
			{
				//�ж��Ƿ����������ڲ�����������������ڲ�������������һ��frag
				if (!IsInTriangle(Vec2i(x, y), screen_coords)) continue;

				//��ȡƬԪ���ĵ���������
				Vec3f BarycentricCoordinate = GetBarycentricCoordinate(screen_coords, Vec2f(x+.5, y+.5));//��������ֱ��Ӧvertexout[0],vertexout[1],vertexout[2]

				//��ֵ���㵱ǰfrag��candidate cube �е�zֵ
				float vertexDepth[3] = { screen_coords[0].z,screen_coords[1].z,screen_coords[2].z };
				float homogeneousDepth = GetFragHomogeneousDepth(vertexDepth, BarycentricCoordinate);

				//���������Ĳ�ֵ����ȡ��ǰfrag��wֵ
				float fragW = GetFragW(w, BarycentricCoordinate);
				//��ȡ��ǰfrag�����Կռ��£��˴��õ��ǲü��ռ䣩���ֵ
				float depth = homogeneousDepth * fragW;

				//����ж�
				if (std::abs(depth) < ZBuffer[y * screen.width + x])
				{
					//���ֵ��С�������ZBuffer������FragmentShader����Ⱦ��ǰfrag
					ZBuffer[y * screen.width + x] = depth;
					color4 FragColor = FragmentShader(vertexout, fragW, BarycentricCoordinate);

					fragCounter++;

					if (FragColor.r == -1 || FragColor.g == -1 || FragColor.b == -1) continue;
						
					DrawFrag(Vec2f(x + .5, y + .5), FragColor);
				}
			}
		}
	}

	std::fill(ZBuffer.begin(), ZBuffer.end(), 99);

	std::cout << "ִ��Fragment Shader ������" << fragCounter << std::endl;
	fragCounter = 0;
	std::cout << "|||||||||||||||||||ExeRenderPipeline ��֡����|||||||||||||||||||" << std::endl;
	std::cout << std::endl;
}

appdata_base Rasterizer::GetVertexData(Model* model, Vec3i faceIndex)
{
	appdata_base ret;
	ret.vertex = model->vertPos(faceIndex[0]);
	ret.texcoord = model->vertUV(faceIndex[1]);
	ret.normal = model->vertNormal(faceIndex[2]);

	return ret;
}

v2f Rasterizer::VertexShader(appdata_base v)
{
	v2f ret;//��������ֵ

	//������ת��Ϊ������
	mat<4, 1, float> colVector;
	colVector[0][0] = v.vertex.x;
	colVector[1][0] = v.vertex.y;
	colVector[2][0] = v.vertex.z;
	colVector[3][0] = 1;

	//��òü��ռ��µ�����
	ret.clipPos = Vec4f((Matrix_MVP * colVector)[0][0], (Matrix_MVP * colVector)[1][0], (Matrix_MVP * colVector)[2][0], (Matrix_MVP * colVector)[3][0]);

	//�������ռ��µ�����
	ret.worldPos = Vec3f((Matrix_M * colVector)[0][0], (Matrix_M * colVector)[1][0], (Matrix_M * colVector)[2][0]);

	//�������ռ��µķ���
	ret.worldNromal = ObjectToWorldNormal(v.normal);

	//��ö���UV����
	ret.uv = v.texcoord;

#ifdef DEBUG_VERT
	std::cout << "-------------" << std::endl;
	std::cout << "����ü��ռ�����Ϊ��" << ret.clipPos << std::endl;
	/*std::cout << "MVP Matrix = " << std::endl;
	std::cout << Matrix_MVP << std::endl;*/
	std::cout << "��������ռ�����Ϊ��" << ret.worldPos << std::endl;
	/*std::cout << "Model Matrix = " << std::endl;
	std::cout << Matrix_M << std::endl;*/
	std::cout << "��������ռ䷨��Ϊ��" << ret.worldNromal << std::endl;
	/*std::cout << "Normal Matrix = " << std::endl;
	std::cout << Matrix_M.invert_transpose() << std::endl;*/
	std::cout << "����UV����Ϊ��" << ret.uv << std::endl;
	std::cout << "-------------" << std::endl;
	
#endif // DEBUG_VERT
	
	return ret;
}

bool Rasterizer::canClip(Vec3f* homogeneousPos)
{
	return (homogeneousPos[0].x < -1.f || homogeneousPos[0].x > 1.f ||
			homogeneousPos[0].y < -1.f || homogeneousPos[0].y > 1.f ||
			homogeneousPos[0].z < -1.f || homogeneousPos[0].z > 1.f ||
		homogeneousPos[1].x < -1.f || homogeneousPos[1].x > 1.f ||
		homogeneousPos[1].y < -1.f || homogeneousPos[1].y > 1.f ||
		homogeneousPos[1].z < -1.f || homogeneousPos[1].z > 1.f ||
			homogeneousPos[2].x < -1.f || homogeneousPos[2].x > 1.f ||
			homogeneousPos[2].y < -1.f || homogeneousPos[2].y > 1.f ||
			homogeneousPos[2].z < -1.f || homogeneousPos[2].z > 1.f);
}

Vec3f Rasterizer::ScreenMapping(Vec3f screen_coord)
{
	return Vec3f((screen_coord.x + 1.) * screen.width/2, (screen_coord.y + 1.) * screen.height/2, screen_coord.z);
}

color4 Rasterizer::FragmentShader(v2f* i, float FragW, Vec3f barycoord)
{

	//��ȡ��ͼ
	Vec2f fragUV = i[0].uv * barycoord[0] + i[1].uv * barycoord[1] + i[2].uv * barycoord[2];
	TGAColor tex = diffuseTex->get(fragUV.x * diffuseTex->width(), fragUV.y * diffuseTex->height());
	Vec3f diffuse = Vec3f(tex.bgra[2], tex.bgra[1], tex.bgra[0]);

#ifdef DEBUG_TEXTUREREADING

	std::cout << "Fragment Texture Reading: ";
	std::cout << "UV: " << fragUV << "  ||  " << "Color: " << diffuse << std::endl;

#endif // DEBUG_TEXTUREREADING

#ifdef FLATSHADER	

	Vec3f n = cross((i[1].worldPos - i[0].worldPos), (i[2].worldPos - i[0].worldPos));
	n.normalize();
	float intensity = dot(n, worldLightDir);

	if (intensity < .2f) return { -1, -1, -1, -1 };

	return { diffuse.x/(float)255 * intensity , diffuse.y/(float)255 * intensity , diffuse.z/(float)255 * intensity, 1};

#else

	Vec3f linearBarycoord = barycoord * FragW;

	Vec3f vertexWorldSpaceNormals[3] = { i[0].worldNromal, i[1].worldNromal, i[2].worldNromal };
	Vec3f worldNormalDir = GetFragNormalByVertNormal(vertexWorldSpaceNormals, linearBarycoord).normalize();

	float intensity = dot(worldNormalDir, worldLightDir);

	return { worldNormalDir.x , worldNormalDir.y, worldNormalDir.z, 1 };
	/*return { diffuse.x / (float)255 * intensity , diffuse.y / (float)255 * intensity , diffuse.z / (float)255 * intensity, 1 };*/

#endif // FLATSHADER

#ifdef DEBUG_FRAG
	std::cerr << "----------Each Fragment----------" << std::endl;;
	std::cerr << "Fragment WorldNormalDir = " << worldNormalDir << std::endl;
	std::cerr << "WorldLightDir = " << worldLightDir.normalize() << std::endl;
	std::cerr << "Light Intensity = " << intensity << std::endl;
#endif // DEBUG_FRAG

}

Vec3f Rasterizer::ObjectToWorldNormal(Vec3f normal)
{
	mat<4, 1, float> colVector;
	colVector[0][0] = normal.x;
	colVector[1][0] = normal.y;
	colVector[2][0] = normal.z;
	colVector[3][0] = 1;
	colVector = Matrix_M.invert_transpose() * colVector;
	return Vec3f(colVector[0][0], colVector[1][0], colVector[2][0]);
}

bool Rasterizer::IsInTriangle(Vec2i frag, Vec3f* triangleVertex)
{
	//					 A
	//				   / | \
	//				  /  |  \
	//				 /   |   \
	//				B    P    \
	//				 \		   \
	//				    \       \
	//						\    \
	//							\ \
	//							  C

	//�ж�frag�������Ƿ����������ڲ�
	Vec3f P(frag.x + .5, frag.y + .5, 1.f);

	Vec3f AP = P - Vec3f(triangleVertex[0].x, triangleVertex[0].y, 0.f);
	Vec3f BP = P - Vec3f(triangleVertex[1].x, triangleVertex[1].y, 0.f);
	Vec3f CP = P - Vec3f(triangleVertex[2].x, triangleVertex[2].y, 0.f);

	Vec3f AB = Vec3f((triangleVertex[1] - triangleVertex[0]).x, (triangleVertex[1] - triangleVertex[0]).y, 0.f);
	Vec3f BC = Vec3f((triangleVertex[2] - triangleVertex[1]).x, (triangleVertex[2] - triangleVertex[1]).y, 0.f);
	Vec3f CA = Vec3f((triangleVertex[0] - triangleVertex[2]).x, (triangleVertex[0] - triangleVertex[2]).y, 0.f);

	if ((cross(AB, AP).z > 0 && cross(BC, BP).z > 0 && cross(CA, CP).z > 0) || (cross(AB, AP).z < 0 && cross(BC, BP).z < 0 && cross(CA, CP).z < 0))
		return true;//ͬ�������������ڣ�����true
	return false;
}

Vec3f Rasterizer::GetFragNormalByVertNormal(Vec3f* triangleVertexNormals, Vec3f barycoord)
{
	Vec3f ret;
	ret = triangleVertexNormals[0] * barycoord[0] + triangleVertexNormals[1] * barycoord[1] + triangleVertexNormals[2] * barycoord[2];
	return ret;
}

float Rasterizer::GetFragHomogeneousDepth(float* triangleVertexDepths, Vec3f barycoord)
{
	mat<4, 1, float> colBarycoord;
	colBarycoord[0][0] = barycoord.x;
	colBarycoord[1][0] = barycoord.y;
	colBarycoord[2][0] = barycoord.z;
	colBarycoord[3][0] = 1;
	return (triangleVertexDepths[0] * barycoord[0] + triangleVertexDepths[1] * barycoord[1] + triangleVertexDepths[2] * barycoord[2]);
}

float Rasterizer::GetFragW(float* triangleVertexW, Vec3f barycoord)
{
	mat<4, 1, float> colBarycoord;
	colBarycoord[0][0] = barycoord.x;
	colBarycoord[1][0] = barycoord.y;
	colBarycoord[2][0] = barycoord.z;
	colBarycoord[3][0] = 1;
	return (triangleVertexW[0] * barycoord[0] + triangleVertexW[1] * barycoord[1] + triangleVertexW[2] * barycoord[2]);
}

void Rasterizer::DrawFrag(Vec2f pos, color4 color)
{
	float posx = (pos.x / screen.width) * 2 - 1.f;
	float posy = (pos.y / screen.height) * 2 - 1.f;
	glBegin(GL_POINTS);
	glPointSize(1);
	glColor3f(color.r, color.g, color.b);
	glVertex2f(posx, posy);
	glEnd();
}

