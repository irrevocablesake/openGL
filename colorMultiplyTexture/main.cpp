#include<glad/glad.h>
#include<GLFW/glfw3.h>

#include "stb_image.h"

#include<iostream>

using namespace std;

struct windowProperties {
	const int WIDTH = 800;
	const int HEIGHT = 800;
	const char* TITLE = "triangle";
} windowProperties;

void windowResize(GLFWwindow* window, int width, int height) {
	glViewport(0, 0, width, height);
}

void processInput(GLFWwindow* window) {
	if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS) {
		glfwSetWindowShouldClose(window, true);
	}
}

const char* vertexShaderSource = "#version 460 core\n"
"layout ( location = 0 ) in vec3 aPos;\n"
"layout ( location = 1 ) in vec2 aTexture;\n"
"layout ( location = 2 ) in vec3 aColor;\n"
"out vec2 outTextureCoord;\n"
"out vec3 outColor;\n"
"void main()\n"
"{\n"
"    gl_Position = vec4( aPos.x, aPos.y, aPos.z, 1.0);\n"
"    outTextureCoord = aTexture;\n"
"    outColor = aColor;\n"
"}\0";

const char* fragmentShaderSource = "#version 460 core\n"
"out vec4 fragColor;\n"
"in vec2 outTextureCoord;\n"
"uniform sampler2D ourTexture;\n"
"in vec3 outColor;\n"
"void main()\n"
"{\n"
"    fragColor = texture( ourTexture, outTextureCoord) * vec4( outColor, 1.0);\n"
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
		cout << "ERROR: coulnd't create a GLFW window" << endl;
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

	unsigned int vertexShader = glCreateShader(GL_VERTEX_SHADER);
	glShaderSource(vertexShader, 1, &vertexShaderSource, NULL);
	glCompileShader(vertexShader);

	int success;
	char infoLog[512];
	glGetShaderiv(vertexShader, GL_COMPILE_STATUS, &success);
	if (!success) {
		glGetShaderInfoLog(vertexShader, 512, NULL, infoLog);
		cout << "ERROR: vertex shader compilation failed " << infoLog << endl;
	}

	unsigned int fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
	glShaderSource(fragmentShader, 1, &fragmentShaderSource, NULL);
	glCompileShader(fragmentShader);

	glGetShaderiv(fragmentShader, GL_COMPILE_STATUS, &success);
	if (!success) {
		glGetShaderInfoLog(fragmentShader, 512, NULL, infoLog);
		cout << "ERROR: fragment shader compilation failed " << infoLog << endl;
	}

	unsigned int shaderProgram = glCreateProgram();
	glAttachShader(shaderProgram, vertexShader);
	glAttachShader(shaderProgram, fragmentShader);
	glLinkProgram(shaderProgram);

	glGetProgramiv(shaderProgram, GL_LINK_STATUS, &success);
	if (!success) {
		glGetProgramInfoLog(shaderProgram, 512, NULL, infoLog);
		cout << "ERROR: program linking failed  " << infoLog << endl;
	}

	glDeleteShader(vertexShader);
	glDeleteShader(fragmentShader);



	unsigned int texture;
	glGenTextures(1, &texture);

	glBindTexture(GL_TEXTURE_2D, texture);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

	int width, height, nrChannels;
	unsigned char* data = stbi_load(
		"D:\\programming\\3D graphics\\opengl\\resources\\texture\\image\\container.jpg",
		&width,
		&height,
		&nrChannels,
		0
	);

	if (data) {
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
		glGenerateMipmap(GL_TEXTURE_2D);
	}
	else {
		cout << "ERROR: Failed to load texture" << endl;
		cout << stbi_failure_reason();
	}

	stbi_image_free(data);

	float vertices[] = {
		//position data		 //texture data  //color data
		-0.5f, -0.5f, 0.0f,	 0.0f, 0.0f,  1.0f, 0.0f, 0.0f,
		0.5f, -0.5f, 0.0f,	 1.0f, 0.0f,  0.0f, 1.0f, 0.0f,
		0.0f, 0.5f, 0.0f,	 0.5f, 1.0f,  0.0f, 0.0f, 1.0f
	};

	unsigned int VAO;
	glGenVertexArrays(1, &VAO);

	glBindVertexArray(VAO);

	unsigned int VBO;
	glGenBuffers(1, &VBO);

	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);

	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(3 * sizeof(float)));
	glEnableVertexAttribArray(1);

	glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(5 * sizeof(float)));
	glEnableVertexAttribArray(2);

	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindVertexArray(0);

	while (!glfwWindowShouldClose(window)) {

		processInput(window);

		glClearColor(0.3f, 0.1f, 0.3f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT);

		glBindTexture(GL_TEXTURE_2D, texture);

		glUseProgram(shaderProgram);
		glBindVertexArray(VAO);
		glDrawArrays(GL_TRIANGLES, 0, 3);

		glfwSwapBuffers(window);
		glfwPollEvents();
	}

	glDeleteVertexArrays(1, &VAO);
	glDeleteBuffers(1, &VBO);

	glfwTerminate();
	return 0;
}