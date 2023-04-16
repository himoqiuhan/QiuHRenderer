#include <GLFW/glfw3.h>

#include "Rasterization/Draw.h"
#include "VertexData/ModelLoader.h"
#include "Support/Math/Matrix.h"

#include "Support/Information/BasicInformation.h"

int main(void)
{
	GLFWwindow* window;

	Screen screen;

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




	//------------------试验田--------------------
	Transform transform;
	transform.transition = Vec3f(0, 0, 0);
	transform.rotate = Vec3f(0, 0, 0);
	transform.scale = Vec3f(1, 1, 1);

	Matrix  MatModel = GetModelMatrix(transform);
	Camera camera(screen);
	camera.position = Vec3f(0, 0, 2);
	Matrix MatView = GetViewMatrix(camera);
	Matrix MatPerspective = GetPerspectiveMatrix(camera);

	Matrix Matrix_MVP = MatPerspective * MatView * MatModel;
	//------------------试验田--------------------





	/* Loop until the user closes the window */
	while (!glfwWindowShouldClose(window))
	{
		/* Render here */
		glClear(GL_COLOR_BUFFER_BIT);

		for (int i = 0; i < model->nfaces(); i++)
		{
			std::vector<int> face = model->face(i);
			Vec2i screen_coords[3];
			Vec3f world_coords[3];
			for (int j = 0; j < 3; j++) {
				mat<4, 1, float> colVector;
				colVector[0][0] = model->vert(face[j]).x;
				colVector[1][0] = model->vert(face[j]).y;
				colVector[2][0] = model->vert(face[j]).z;
				colVector[3][0] = 1;
				world_coords[j] = Vec3f((Matrix_MVP * colVector)[0][0], (Matrix_MVP * colVector)[1][0], (Matrix_MVP * colVector)[2][0]);
				world_coords[j] = world_coords[j] / (Matrix_MVP * colVector)[3][0];


				/*std::cout << std::endl;
				std::cout << "Result Vector Col Matrix: " << std::endl;
				for (int i = 0; i < 4; i++)
				{
					std::cout << (Matrix_MVP * colVector)[i][0] << std::endl;
					std::cout << std::endl;
				}*/



				screen_coords[j] = Vec2i((world_coords[j].x + 1.) * 640/2, (world_coords[j].y + 1.) * 480/2);//世界空间转化为屏幕空间
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
		

		/* Swap front and back buffers */
		glfwSwapBuffers(window);

		/* Poll for and process events */
		glfwPollEvents();
	}

	glfwTerminate();
	return 0;
}