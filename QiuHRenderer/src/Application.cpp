#include <GLFW/glfw3.h>

#include "Rasterization/Draw.h"

#include "Rasterization/Rasterizer.h"

#include "vendor/imgui/imgui.h"
#include "vendor/imgui/imgui_impl_glfw.h"
#include "vendor/imgui/imgui_impl_opengl3.h"

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

	//������Ϣ
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



	//������Ϣ
	Vec3f light_dir(0, 0, -1);
	Vec3f CameraPos;

	//������դ����
	Rasterizer r(screen);
	r.SetTransform(Vec3f(0, 0, 0), Vec3f(10, 30, 45), Vec3f(1, 1, 1));
	
	r.SetPerspective();
	int temp = 0;

	//��ʼ��ImGui
	const char* glsl_version = "#version 130";
	ImGui::CreateContext();
	ImGui::StyleColorsDark();
	ImGui_ImplGlfw_InitForOpenGL(window, true);
	ImGui_ImplOpenGL3_Init(glsl_version);

	bool show_demo_window = true;
	bool show_another_window = false;
	ImVec4 clear_color = ImVec4(0.45f, 0.55f, 0.60f, 1.00f);


#ifdef DEBUG_SINGLEFRAME

	glClear(GL_COLOR_BUFFER_BIT);
	r.ExeRenderPipeline(modelBow, light_dir);

#else

	/* Loop until the user closes the window */
	while (!glfwWindowShouldClose(window))
	{
		//��Ⱦ����
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
		
		ImGui_ImplOpenGL3_NewFrame();
		ImGui_ImplGlfw_NewFrame();
		ImGui::NewFrame();

		/* Render here */
		glClear(GL_COLOR_BUFFER_BIT);

		r.SetPerspective();

		r.ExeRenderPipeline(model, tex, light_dir);

		//ImGui���ڵ�����
		{
			static float f = 0.0f;
			static int counter = 0;
			ImGui::Text("Hello, world!");                           // Display some text (you can use a format string too)
			ImGui::SliderFloat("float", &f, 0.0f, 1.0f);            // Edit 1 float using a slider from 0.0f to 1.0f    
			ImGui::ColorEdit3("clear color", (float*)&clear_color); // Edit 3 floats representing a color

			ImGui::Checkbox("Demo Window", &show_demo_window);      // Edit bools storing our windows open/close state
			ImGui::Checkbox("Another Window", &show_another_window);

			if (ImGui::Button("Button"))                            // Buttons return true when clicked (NB: most widgets return true when edited/activated)
				counter++;
			ImGui::SameLine();
			ImGui::Text("counter = %d", counter);

			ImGui::Text("Application average %.3f ms/frame (%.1f FPS)", 1000.0f / ImGui::GetIO().Framerate, ImGui::GetIO().Framerate);
		}

		//ImGui��Ⱦ
		ImGui::Render();
		ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());

		/* Swap front and back buffers */
		glfwSwapBuffers(window);

		/* Poll for and process events */
		glfwPollEvents();
	}


#endif //DEBUG_SINGLEFRAME

	// Cleanup ImGui
	ImGui_ImplOpenGL3_Shutdown();
	ImGui_ImplGlfw_Shutdown();
	ImGui::DestroyContext();

	glfwTerminate();

	return 0;
}