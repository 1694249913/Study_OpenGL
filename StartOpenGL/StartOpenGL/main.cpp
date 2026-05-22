#include<glad/glad.h>
#include<GLFW/glfw3.h>
#include<iostream>

const char* vertexShaderSource = "#version 330 core\n"
"layout(location=0)in vec3 aPos;\n"//layout (location = 0)设定了输入变量的位置值
"layout (location = 1) in vec3 aColor;\n" // 颜色变量的属性位置值为 1
//"out vec4 vertexColor; \n" // 为片段着色器指定一个颜色输出
"out vec3 ourColor;\n"
"void main()\n"
"{\n"//gl_Position设置的值会成为该顶点着色器的输出(本次直接输出，不做处理)
"gl_Position=vec4(aPos.x,aPos.y,aPos.z,1.0);\n"
//"vertexColor = vec4(0.5, 0.0, 0.0, 1.0);\n" // 把输出变量设置为暗红色"
"ourColor = aColor; \n"// 将ourColor设置为我们从顶点数据那里得到的输入颜色
"}\0";
const char* fragmentShaderSource = "#version 330 core\n"
"out vec4 FragColor;\n"
"in vec3 ourColor;\n"
//"in vec4 vertexColor; \n"// 从顶点着色器传来的输入变量（名称相同、类型相同）
//"uniform vec4 ourColor;\n" // 在OpenGL程序代码中设定这个变量
"void main()\n"
"{\n"//输出最终颜色rgba
//"  FragColor = vec4(1.0f, 0.5f, 0.2f, 1.0f);\n"
//"FragColor=vertexColor;\n"
" FragColor = vec4(ourColor,1.0);\n"
"}\0";

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
	glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);//设置窗口大小的回调函数
	if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
	{
		//给GLAD传入了用来加载系统相关的OpenGL函数指针地址的函数。
	    //GLFW给的是glfwGetProcAddress，它根据我们编译的系统定义了正确的函数
		std::cout << "Faild to initialize GLAD ( GLAD初始化失败！)" << std::endl;
		return -1;
	}
	//---------顶点着色器
	unsigned int vertexShader;//顶点着色器的引用ID
	vertexShader = glCreateShader(GL_VERTEX_SHADER);//创建顶点着色器
	//将源码附加到着色器对象上（要编译的着色器对象作为第一个参数。第二参数指定了传递的源码字符串数量，这里只有一个。第三个参数是顶点着色器真正的源码），并编译
	glShaderSource(vertexShader, 1, &vertexShaderSource, NULL);
	glCompileShader(vertexShader);//编译

	//检查编译是否成功
	int  success;
	char infoLog[512];//储存错误消息的容器
	glGetShaderiv(vertexShader, GL_COMPILE_STATUS, &success);//检查是否编译成功
	if (!success)//未成功
	{
		//获取错误消息
		glGetShaderInfoLog(vertexShader, 512, NULL, infoLog);
		std::cout << "ERROR::SHADER::VERTEX::COMPILATION_FAILED（顶点着色器编译失败！）\n" << infoLog << std::endl;
	}

	// ----片元着色器
	unsigned int fragmentShader;//片段着色器的引用ID
	fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);//创建片元着色器
	glShaderSource(fragmentShader, 1, &fragmentShaderSource, NULL);
	glCompileShader(fragmentShader);
	if (!success)//未成功
	{
		//获取错误消息
		glGetShaderInfoLog(fragmentShader, 512, NULL, infoLog);
		std::cout << "ERROR::SHADER::FRAGMENT::COMPILATION_FAILED（片元着色器编译失败！）\n" << infoLog << std::endl;
	}

	//-------着色器程序
	unsigned int shaderProgram;
	shaderProgram = glCreateProgram();

	//附加着色器->链接程序
	glAttachShader(shaderProgram, vertexShader);
	glAttachShader(shaderProgram, fragmentShader);
	glLinkProgram(shaderProgram);
	//检查连接是否成功
	glGetProgramiv(shaderProgram, GL_LINK_STATUS, &success);
	if (!success) {
		glGetProgramInfoLog(shaderProgram, 512, NULL, infoLog);
		std::cout << "ERROR::PROGRAM::LINKED_FAILED（着色器程序链接失败！）\n" << infoLog << std::endl;
	}

	//链接到程序对象后，删除着色器对象
	glDeleteShader(vertexShader);
	glDeleteShader(fragmentShader);
	//{//三角形绘制--------------------------------------------
	//	//顶点数据数组，存储了位置信息
	//	float vertices[] = {
	//	   -0.5f, -0.5f, 0.0f,
	//		0.5f, -0.5f, 0.0f,
	//		0.0f,  0.5f, 0.0f
	//	};
	//	unsigned int VBO;
	//	glGenBuffers(1, &VBO);//生成一个带有缓冲ID的VBO对象
	//	unsigned int VAO;
	//	glGenVertexArrays(1, &VAO);//生成VAO
	//	glBindVertexArray(VAO);
	//	glBindBuffer(GL_ARRAY_BUFFER, VBO);//将新创建的缓冲绑定到GL_ARRAY_BUFFER目标上
	//	//把用户定义的数据复制到当前绑定缓冲的函数。
	//	//它的第一个参数是目标缓冲的类型：顶点缓冲对象当前绑定到GL_ARRAY_BUFFER目标上。
	//	//第二个参数指定传输数据的大小(以字节为单位)；用一个简单的sizeof计算出顶点数据大小就行。
	//	//第三个参数是我们希望发送的实际数据。
	//	//第四个参数指定了我们希望显卡如何管理给定的数据。它有三种形式：
	//	/*
	//		GL_STATIC_DRAW ：数据不会或几乎不会改变。
	//		GL_DYNAMIC_DRAW：数据会被改变很多。
	//		GL_STREAM_DRAW ：数据每次绘制时都会改变。
	//	*/
	//	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
	//}
	
	
	//正方形绘制---------------------------------
	//正方形顶点，只包含四个顶点
	float vertices[] = {
	     0.5f, 0.5f, 0.0f, 1.0f, 0.0f, 0.0f,  // 右上角
	     0.5f, -0.5f, 0.0f,0.0f, 1.0f, 0.0f,  // 右下角
	    -0.5f, -0.5f, 0.0f,0.0f, 0.0f, 1.0f, // 左下角
	    -0.5f, 0.5f, 0.0f  ,1.0f, 0.5f, 0.0f, // 左上角
	};
	unsigned int indices[] = {
		// 注意索引从0开始! 
		// 此例的索引(0,1,2,3)就是顶点数组vertices的下标，
		// 这样可以由下标代表顶点组合成矩形

		0, 1, 3, // 第一个三角形
		1, 2, 3  // 第二个三角形
	};

	unsigned int EBO,VAO, VBO;
	glGenVertexArrays(1, &VAO);
	glGenBuffers(1, &VBO);
	glGenBuffers(1, &EBO);
	glBindVertexArray(VAO);

	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
	//与VBO类似，我们先绑定EBO然后用glBufferData把索引复制到缓冲里
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);
	//---------------------------------------------------------------------

	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)0);
	/*
	 第一个参数指定我们要配置的顶点属性。我们在顶点着色器中使用layout(location = 0)定义了position顶点属性的位置值(Location)，
	 	   它可以把顶点属性的位置值设置为0。因为我们希望把数据传递到这一个顶点属性中，所以这里我们传入0。
	 第二个参数指定顶点属性的大小。顶点属性是一个vec3，它由3个值组成，所以大小是3。
	 第三个参数指定数据的类型，这里是GL_FLOAT(GLSL中vec*都是由浮点数值组成的)。
	 第四个参数定义我们是否希望数据被标准化(Normalize)。如果我们设置为GL_TRUE，所有数据都会被映射到0（对于有符号型signed数据是-1）到1之间。我们把它设置为GL_FALSE。
	 第五个参数叫做步长(Stride)，它告诉我们在连续的顶点属性组之间的间隔。
	 	  由于下个组位置数据在3个float之后，我们把步长设置为3 * sizeof(float)。
	 	  要注意的是在两个顶点属性之间没有空隙时，我们也可以设置为0来让OpenGL决定具体步长是多少（只有当数值是紧密排列时才可用）。
	 	  一旦我们有更多的顶点属性，我们就必须更小心地定义每个顶点属性之间的间隔。
	 最后一个参数的类型是void*，所以需要我们进行这个奇怪的强制类型转换。它表示位置数据在缓冲中起始位置的偏移量(Offset)。
	 	  由于位置数据在数组的开头，所以这里是0。
    */
	glEnableVertexAttribArray(0);//以顶点属性位置值作为参数，启用顶点属性
	// 颜色属性
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)(3 * sizeof(float)));
	glEnableVertexAttribArray(1);
	glBindBuffer(GL_ARRAY_BUFFER, 0);//解绑
	glBindVertexArray(0);//解绑
	while (!glfwWindowShouldClose(window))//每次循环的开始前检查一次GLFW是否被要求退出
	{
		//处理输入指令
		processInput(window);

		//处理渲染指令
		glClearColor(0.2f, 0.3f, 0.3f, 1.0f);//设置清空屏幕所用的颜色
		glClear(GL_COLOR_BUFFER_BIT);//清空屏幕的颜色缓冲
		glUseProgram(shaderProgram);//激活程序对象
		/*float timeValue = glfwGetTime();
		float greenValue = (sin(timeValue) / 2.0f) + 0.5f;
		int vertexColorLocation = glGetUniformLocation(shaderProgram, "ourColor");
		
		glUniform4f(vertexColorLocation, 0.0f, greenValue, 0.0f, 1.0f);*/
		//glUseProgram(shaderProgram);
		glBindVertexArray(VAO);//动态绑定VAO

		//glDrawArrays(GL_TRIANGLES, 0, 3);//将提供的定点绘制为三角形

		/*第一个参数指定了我们绘制的模式，这个和glDrawArrays的一样。
		 第二个参数是我们打算绘制顶点的个数，这里填6，也就是说我们一共需要绘制6个顶点。
		 第三个参数是索引的类型，这里是GL_UNSIGNED_INT。
		 最后一个参数里我们可以指定EBO中的偏移量（或者传递一个索引数组，但是这是当你不在使用索引缓冲对象的时候），
		 但是我们会在这里填写0。*/
		//用glDrawElements来替换glDrawArrays函数，表示我们要从索引缓冲区渲染三角形。
		//使用glDrawElements时，我们会使用当前绑定的索引缓冲对象中的索引进行绘制
		glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
		

		//检查并调用事件，交换缓冲
		glfwSwapBuffers(window);//函数会交换颜色缓冲（它是一个储存着GLFW窗口每一个像素颜色值的大缓冲）
		glfwPollEvents();//检查有没有触发什么事件
	}

	glDeleteVertexArrays(1, &VAO);
	glDeleteBuffers(1, &VBO);
	glDeleteBuffers(1, &EBO);
	glDeleteProgram(shaderProgram);

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