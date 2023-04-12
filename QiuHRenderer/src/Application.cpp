#include <GLFW/glfw3.h>

#include "Rasterization/Draw.h"
#include "VertexData/ModelLoader.h"

int main(void)
{
	GLFWwindow* window;

	/* Initialize the library */
	if (!glfwInit())
		return -1;

	/* Create a windowed mode window and its OpenGL context */
	window = glfwCreateWindow(640, 480, "Hello World", NULL, NULL);
	if (!window)
	{
		glfwTerminate();
		return -1;
	}

	/* Make the window's context current */
	glfwMakeContextCurrent(window);

	Vector2f Triangle[3] = { Vector2f(50,50),Vector2f(100,100), Vector2f(100,50) };
	Vector3f TriangleColor(1, 1, 1);

	Model* model = new Model("res/obj/african_head.obj");

	Vec3f light_dir(0, 0, -1);

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
				world_coords[j] = model->vert(face[j]);
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