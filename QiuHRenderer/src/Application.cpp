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

	Screen screen(1920,1080);

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

	//Model* model = new Model("res/obj/african_head.obj");
	Model* model = new Model("res/obj/March7th.obj");

	Vec3f light_dir(0, 0, -1);

	Rasterizer r(screen);
	r.SetTransform(Vec3f(0, 0, 0), Vec3f(10, 30, 45), Vec3f(1, 1, 1));
	r.SetCamera(Vec3f(0, 0, 25));
	r.SetPerspective();
	int temp = 0;

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
	r.ExeRenderPipeline(model, light_dir);

#else

	/* Loop until the user closes the window */
	while (!glfwWindowShouldClose(window))
	{

		temp += 5;
		r.SetTransform(Vec3f(0, -12.5, 0), Vec3f(0, temp, 0), Vec3f(1, 1, 1));
		r.SetPerspective();

		/* Render here */
		glClear(GL_COLOR_BUFFER_BIT);
		

		r.ExeRenderPipeline(model, light_dir);

		/* Swap front and back buffers */
		glfwSwapBuffers(window);

		/* Poll for and process events */
		glfwPollEvents();
	}


#endif //DEBUG_SINGLEFRAME

	glfwTerminate();

	return 0;
}