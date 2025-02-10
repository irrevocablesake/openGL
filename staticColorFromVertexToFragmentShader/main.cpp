#include<glad/glad.h>
#include<GLFW/glfw3.h>

#include<iostream>

using namespace std;

struct windowProperties {
	const int WIDTH = 800;
	const int HEIGHT = 800;
	const char* TITLE = "shader Communication";
} windowProperties;

void windowResize(GLFWwindow* window, int width, int height) {
	glViewport(0, 0, width, height);
}

void processEvent(GLFWwindow* window) {
	if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS) {
		glfwSetWindowShouldClose(window, true);
	}
}

const char* vertexShaderSource = "#version 460 core\n"
"layout ( location = 0 ) in vec4 aPos;\n"
"out vec3 fragColor;"
"void main()\n"
"{\n"
"   gl_Position = vec4( aPos.x, aPos.y, aPos.z, 1.0f );\n"
"   fragColor = vec3( 0.3, 0.0,0.0);\n"
"}\n\0";

const char* fragmentShaderSource = "#version 460 core\n"
"out vec4 outColor;\n"
"in vec3 fragColor;\n"
"void main()\n"
"{\n"
"   outColor = vec4( fragColor.r, fragColor.g, fragColor.b, 1.0 );\n"
"}\n\0";

int main() {
	glfwInit();
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 6);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

	GLFWwindow* window = glfwCreateWindow(
		windowProperties.WIDTH,
		windowProperties.HEIGHT,
		windowProperties.TITLE,
		NULL,
		NULL
	);

	if (window == NULL) {
		cout << "ERROR: couldn't create GLFW window" << endl;
		glfwTerminate();
		return -1;
	}

	glfwMakeContextCurrent(window);
	glfwSetFramebufferSizeCallback(window, windowResize);

	if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {
		cout << "ERROR: couldn't load dynamic pointers to openGL" << endl;
		glfwTerminate();
		return -1;
	}

	const float vertices[] = {
		-0.5f, -0.5f, 0.0f,
		0.5f, -0.5f, 0.0f,
		0.0f, 0.5f, 0.0f
	};

	unsigned int vertexShader = glCreateShader(GL_VERTEX_SHADER);
	glShaderSource(vertexShader, 1, &vertexShaderSource, NULL);
	glCompileShader(vertexShader);

	int success;
	char infoLog[512];
	glGetShaderiv(vertexShader, GL_COMPILE_STATUS, &success);

	if (!success) {
		glGetShaderInfoLog(vertexShader, 512, NULL, infoLog);
		cout << "ERROR: couldn't compile vertex shader: " << infoLog << endl;
		glfwTerminate();
		return -1;
	}

	unsigned int fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
	glShaderSource(fragmentShader, 1, &fragmentShaderSource, NULL);
	glCompileShader(fragmentShader);

	glGetShaderiv(fragmentShader, GL_COMPILE_STATUS, &success);
	if (!success) {
		glGetShaderInfoLog(fragmentShader, 512, NULL, infoLog);
		cout << "ERROR: couldn't compile fragment shader: " << infoLog << endl;
		glfwTerminate();
		return -1;
	}

	unsigned int shaderProgram;
	shaderProgram = glCreateProgram();
	glAttachShader(shaderProgram, vertexShader);
	glAttachShader(shaderProgram, fragmentShader);

	glLinkProgram(shaderProgram);

	glGetProgramiv(shaderProgram, GL_LINK_STATUS, &success);
	if (!success) {
		glGetProgramInfoLog(shaderProgram, 512, NULL, infoLog);
		cout << "ERROR: couldn't link the shader program " << infoLog << endl;
		glfwTerminate();
		return -1;
	}

	glDeleteShader(vertexShader);
	glDeleteShader(fragmentShader);

	unsigned int VAO;
	glGenVertexArrays(1, &VAO);
	glBindVertexArray(VAO);

	unsigned int VBO;
	glGenBuffers(1, &VBO);
	glBindBuffer(GL_ARRAY_BUFFER, VBO);

	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);

	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindVertexArray(0);

	while (!glfwWindowShouldClose(window)) {

		processEvent(window);

		glClearColor(0.3f, 0.4f, 0.6f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT);
		
		glUseProgram(shaderProgram);
		glBindVertexArray(VAO);
		glDrawArrays(GL_TRIANGLES, 0, 3);

		glfwSwapBuffers(window);
		glfwPollEvents();
	}

	glfwTerminate();
	return 0;
}