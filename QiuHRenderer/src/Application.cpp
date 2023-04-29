#include <GLFW/glfw3.h>

#include "Rasterization/Draw.h"

#include "Rasterization/Rasterizer.h"

//#define DEBUG_SINGLEFRAME

//#define RENDERTEST_BOW
//#define RENDERTEST_BAT
#define RENDERTEST_MARCH7TH
//#define RENDERTEST_AFRICANHEAD

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

	//对象信息
#ifdef RENDERTEST_AFRICANHEAD

	Model* model = new Model("res/obj/african_head.obj");
	TGAImage* tex = new TGAImage("res/texture/african_head_diffuse.tga");

#endif // RENDERTEST_AFRICANHEAD

#ifdef RENDERTEST_BAT

	Model* model = new Model("res/obj/bat.obj");
	TGAImage* tex = new TGAImage("res/texture/bat.tga");

#endif // RENDERTEST_BAT

#ifdef RENDERTEST_BOW

	Model* model = new Model("res/obj/bow.obj");
	TGAImage* tex = new TGAImage("res/texture/bow.tga");

#endif // RENDERTEST_BOW

#ifdef RENDERTEST_MARCH7TH

	Model* model = new Model("res/obj/March7th.obj");
	TGAImage* tex = new TGAImage("res/texture/bow.tga");

#endif // RENDERTEST_MARCH7TH



	//场景信息
	Vec3f light_dir(0, 0, -1);
	Vec3f CameraPos;

	//声明光栅化器
	Rasterizer r(screen);
	r.SetTransform(Vec3f(0, 0, 0), Vec3f(10, 30, 45), Vec3f(1, 1, 1));
	
	r.SetPerspective();
	int temp = 0;


#ifdef DEBUG_SINGLEFRAME

	glClear(GL_COLOR_BUFFER_BIT);
	r.ExeRenderPipeline(modelBow, light_dir);

#else

	/* Loop until the user closes the window */
	while (!glfwWindowShouldClose(window))
	{
		//渲染设置
		temp += 5;
#ifdef RENDERTEST_AFRICANHEAD

		r.SetTransform(Vec3f(0, 0, 0), Vec3f(0, temp, 0), Vec3f(1, 1, 1));
		r.SetCamera(Vec3f(0, 0, 4));

#endif // RENDERTEST_AFRICANHEAD

#ifdef RENDERTEST_BAT

		r.SetTransform(Vec3f(-5, 2.5, 0), Vec3f(45, 0, 30), Vec3f(1, 1, 1));
		r.SetCamera(Vec3f(0, 0, 25));

#endif // RENDERTEST_BAT

#ifdef RENDERTEST_BOW

		r.SetTransform(Vec3f(0, -12.5, 0), Vec3f(0, temp, 0), Vec3f(1, 1, 1));
		r.SetCamera(Vec3f(0, 0, 25));

#endif // RENDERTEST_BOW

#ifdef RENDERTEST_MARCH7TH

		r.SetTransform(Vec3f(0, -12.5, 0), Vec3f(0, temp, 0), Vec3f(1, 1, 1));
		r.SetCamera(Vec3f(0, 0, 24));

#endif // RENDERTEST_MARCH7TH
		
		

		/* Render here */
		glClear(GL_COLOR_BUFFER_BIT);


		r.SetPerspective();

		r.ExeRenderPipeline(model, tex, light_dir);

		/* Swap front and back buffers */
		glfwSwapBuffers(window);

		/* Poll for and process events */
		glfwPollEvents();
	}


#endif //DEBUG_SINGLEFRAME

	glfwTerminate();

	return 0;
}