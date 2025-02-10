#include<glad/glad.h>
#include<GLFW/glfw3.h>

#include "stb_image.h"
#include "shader.h"

#include<iostream>

#define GLM_ENABLE_EXPERIMENTAL

#include<glm/glm.hpp>
#include<glm/gtc/matrix_transform.hpp>
#include<glm/gtc/type_ptr.hpp>
#include<glm/gtx/string_cast.hpp>

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

	Shader shader("vertexShader.vs", "fragmentShader.fs");

	unsigned int texture;
	glGenTextures(1, &texture);

	glBindTexture(GL_TEXTURE_2D, texture);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

	stbi_set_flip_vertically_on_load(true);

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

	unsigned int texture2;
	glGenTextures(1, &texture2);

	glBindTexture(GL_TEXTURE_2D, texture2);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

	unsigned char* data2 = stbi_load(
		"D:\\programming\\3D graphics\\opengl\\resources\\texture\\image\\awesomeface.png",
		&width,
		&height,
		&nrChannels,
		0
	);

	if (data2) {
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, data2);
		glGenerateMipmap(GL_TEXTURE_2D);
	}
	else {
		cout << "ERROR: Failed to load texture" << endl;
		cout << stbi_failure_reason();
	}

	stbi_image_free(data2);

	unsigned int uniformLocation1 = glGetUniformLocation(shader.ID, "ourTexture");
	unsigned int uniformLocation2 = glGetUniformLocation(shader.ID, "ourTexture2");

	shader.use();

	glUniform1i(uniformLocation1, 0);
	glUniform1i(uniformLocation2, 1);


	float vertices[] = {
		//position data		 //texture data  //color data
		-0.5f, -0.5f, 0.0f,	 0.0f, 0.0f,  1.0f, 0.0f, 0.0f,
		0.5f, -0.5f, 0.0f,	 1.0f, 0.0f,  0.0f, 1.0f, 0.0f,
		0.5f, 0.5f, 0.0f,	 1.0f, 1.0f,  0.0f, 0.0f, 1.0f,

		-0.5f, -0.5f, 0.0f, 0.0f, 0.0f,  1.0f, 0.0f, 0.0f,
		-0.5f, 0.5f, 0.0f,0.0f, 1.0f,  0.0f, 1.0f, 0.0f,
		0.5f, 0.5f, 0.0f,1.0f, 1.0f,  0.0f, 0.0f, 1.0f

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


	glm::mat4 model = glm::mat4(1.0f);
	model = glm::rotate(model, glm::radians(-55.0f), glm::vec3(1.0f, 0.0f, 0.0f));

	glm::mat4 view = glm::mat4(1.0f);
	view = glm::translate(view, glm::vec3(0.0f, 0.0f, -3.0f));

	//projection matrix
	glm::mat4 projection = glm::perspective(
		glm::radians(45.0f),
		(float)windowProperties.WIDTH / (float)windowProperties.HEIGHT,
		0.1f,
		100.0f
	);

	shader.use();
	int modelUniformLocation = glGetUniformLocation(shader.ID, "model");
	glUniformMatrix4fv(modelUniformLocation, 1, GL_FALSE, glm::value_ptr(model));

	int viewUniformLocation = glGetUniformLocation(shader.ID, "view");
	glUniformMatrix4fv(viewUniformLocation, 1, GL_FALSE, glm::value_ptr(view));

	int projectionUniformLocation = glGetUniformLocation(shader.ID, "projection");
	glUniformMatrix4fv(projectionUniformLocation, 1, GL_FALSE, glm::value_ptr(projection));

	while (!glfwWindowShouldClose(window)) {

		processInput(window);

		glClearColor(0.3f, 0.1f, 0.3f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT);

		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, texture);

		glActiveTexture(GL_TEXTURE1);
		glBindTexture(GL_TEXTURE_2D, texture2);

		shader.use();
		glBindVertexArray(VAO);
		glDrawArrays(GL_TRIANGLES, 0, 6);

		glfwSwapBuffers(window);
		glfwPollEvents();
	}

	glDeleteVertexArrays(1, &VAO);
	glDeleteBuffers(1, &VBO);

	glfwTerminate();
	return 0;
}