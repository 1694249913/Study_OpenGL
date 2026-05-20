#include<glad/glad.h>
#include<GLFW/glfw3.h>
#include<iostream>

void framebuffer_size_callback(GLFWwindow* window, int width, int height);
void processInput(GLFWwindow* window);
int main()
{
	glfwInit();//初始化GLFW
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);//配置GLFW
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);//同上设置主次版本号-3
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);//告诉GLFW使用核心模式

	GLFWwindow* window = glfwCreateWindow(800, 600, "StartLearnOpenGL", NULL, NULL);
	if (window == NULL)
	{
		std::cout << "Faild to Create GLFW window(创建窗口失败！)" << std::endl;
		glfwTerminate();
		return -1;
	}
	glfwMakeContextCurrent(window);//将窗口的上下文设置为当前线程的主上下文
	glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);
	if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
	{
		//给GLAD传入了用来加载系统相关的OpenGL函数指针地址的函数。
	    //GLFW给的是glfwGetProcAddress，它根据我们编译的系统定义了正确的函数
		std::cout << "Faild to initialize GLAD ( GLAD初始化失败！)" << std::endl;
		return -1;
	}

	glViewport(0, 0, 800, 600);//设置视口尺寸（前两个参数用来控制视口左下角的位置）

	while (!glfwWindowShouldClose(window))//每次循环的开始前检查一次GLFW是否被要求退出
	{
		//处理输入指令
		processInput(window);

		//处理渲染指令
		glClearColor(0.2f, 0.3f, 0.3f, 1.0f);//设置清空屏幕所用的颜色
		glClear(GL_COLOR_BUFFER_BIT);//清空屏幕的颜色缓冲
		
		//检查并调用事件，交换缓冲
		glfwSwapBuffers(window);//函数会交换颜色缓冲（它是一个储存着GLFW窗口每一个像素颜色值的大缓冲）
		glfwPollEvents();//检查有没有触发什么事件
	}
	glfwTerminate();//正确释放/删除之前的分配的所有资源
	return 0;

}
void processInput(GLFWwindow* window)
{
	if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
		glfwSetWindowShouldClose(window,true);
}
void framebuffer_size_callback(GLFWwindow* window, int width, int height)
{
	glViewport(0, 0, width, height);
}