#include <GLFW/glfw3.h>

//#include "Rasterization/Draw.h"
//#include "VertexData/ModelLoader.h"
//#include "Support/Math/Matrix.h"
//
//#include "Support/Information/BasicInformation.h"

#include "Rasterization/Draw.h"

#include "Rasterization/Rasterizer.h"

//#define DEBUG_SINGLEFRAME
//#define LEGACY

int main(void)
{
	GLFWwindow* window;

	Screen screen(640,480);

	/* Initialize the library */
	if (!glfwInit())
		return -1;

	/* Create a windowed mode window and its OpenGL context */
	window = glfwCreateWindow(screen.width, screen.height, "QiuHRenderer", NULL, NULL);
	if (!window)
	{
		glfwTerminate();
		return -1;
	}

	/* Make the window's context current */
	glfwMakeContextCurrent(window);

	Model* model = new Model("res/obj/african_head.obj");

	Vec3f light_dir(0, 0, -1);

	Rasterizer r(screen);
	r.SetTransform(Vec3f(0, 0, 0), Vec3f(0, 0, 0), Vec3f(1, 1, 1));
	
	r.SetPerspective();

	////------------------试验田--------------------

#ifdef LEGACY

	Transform transform;
	transform.transition = Vec3f(0, 0, 0);
	transform.rotate = Vec3f(0, 0, 0);
	transform.scale = Vec3f(1, 1, 1);

	Matrix  MatModel = GetModelMatrix(transform);
	Camera camera;
	camera.position = Vec3f(0, 0, 2);
	Matrix MatView = GetViewMatrix(camera);
	Matrix MatPerspective = GetPerspectiveMatrix(camera);

	Matrix Matrix_MVP = MatPerspective * MatView * MatModel;

	std::cout << "|||||||||||||||||||摄像机基本信息： " << "position: " << camera.position << " LookAt: " << camera.g << " LookUp: " << camera.t << std::endl;

#endif // LEGACY

	////------------------试验田--------------------

	Vec3f CameraPos;



#ifdef DEBUG_SINGLEFRAME

	glClear(GL_COLOR_BUFFER_BIT);
	r.ExeRenderPipeline(model, light_dir, screen);

#else

	/* Loop until the user closes the window */
	while (!glfwWindowShouldClose(window))
	{
		/* Render here */
		glClear(GL_COLOR_BUFFER_BIT);

		
#ifdef LEGACY

		std::cout << "|||||||||||||||||||Legacy 单帧开始|||||||||||||||||||" << std::endl;

		for (int i = 0; i < model->nfaces(); i++)
		{
			std::vector<int> face = model->face(i);
			Vec3f screen_coords[3];
			Vec3f world_coords[3];
			for (int j = 0; j < 3; j++) {
				mat<4, 1, float> colVector;
				colVector[0][0] = model->vert(face[j]).x;
				colVector[1][0] = model->vert(face[j]).y;
				colVector[2][0] = model->vert(face[j]).z;
				colVector[3][0] = 1;
				world_coords[j] = Vec3f((Matrix_MVP * colVector)[0][0], (Matrix_MVP * colVector)[1][0], (Matrix_MVP * colVector)[2][0]);
				world_coords[j] = world_coords[j] / (Matrix_MVP * colVector)[3][0];

	//DEBUG PRINT ---VertexShader Testing---
				std::cout << "--屏幕空间坐标:  " << world_coords[j] << "--透视系数： " << (Matrix_MVP * colVector)[3][0] << std::endl;
				
				std::cout << std::endl;
	//Finished, No Wrong In Vertex Shader And v2f data streaming


				/*std::cout << std::endl;
				std::cout << "Result Vector Col Matrix: " << std::endl;
				for (int i = 0; i < 4; i++)
				{
					std::cout << (Matrix_MVP * colVector)[i][0] << std::endl;
					std::cout << std::endl;
				}*/


				screen_coords[j] = Vec3f((world_coords[j].x + 1.) * 640/2, (world_coords[j].y + 1.) * 480/2,world_coords[j].z);//世界空间转化为屏幕空间
			}
			//根据世界空间计算当前面的法线
			Vec3f n = cross((world_coords[2] - world_coords[0]),(world_coords[1] - world_coords[0]));
			n.normalize();
			float intensity = dot(n, light_dir);
			//Back-face culling 背面剔除
			if (intensity > 0)
			{
				DrawTriangle(screen_coords, Vec3f(intensity, intensity, intensity));
			}
		}

		std::cout << "|||||||||||||||||||Legacy 单帧结束|||||||||||||||||||" << std::endl;
		std::cout << std::endl;
#else

		r.ExeRenderPipeline(model, light_dir, screen);

#endif // LEGACY

		/* Swap front and back buffers */
		glfwSwapBuffers(window);

		/* Poll for and process events */
		glfwPollEvents();
	}


#endif //DEBUG_SINGLEFRAME

	glfwTerminate();

	return 0;
}