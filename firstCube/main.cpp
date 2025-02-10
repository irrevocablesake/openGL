#include<glad/glad.h>
#include<GLFW/glfw3.h>
#include<glm/glm.hpp>
#include<glm/gtc/matrix_transform.hpp>
#include<glm/gtc/type_ptr.hpp>

#include "stb_image.h"

#include<iostream>

using namespace std;

struct windowProperties {
	const int WIDTH = 800;
	const int HEIGHT = 800;
	const char* TITLE = "First Cube";
} windowProperties;

void windowResize(GLFWwindow* window, int WIDTH, int HEIGHT) {
	glViewport(0, 0, WIDTH, HEIGHT);
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
		cout << "ERROR: couldn't create GLFW window" << endl;
		glfwTerminate();
		return -1;
	}

	glfwMakeContextCurrent(window);
	glfwSetFramebufferSizeCallback(window, windowResize);

	if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {
		cout << "ERROR: couldn't load GLAD Pointers" << endl;
		glfwTerminate();
		return -1;
	}

	while (!glfwWindowShouldClose(window)) {
		processInput(window);

		glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT);

		glfwSwapBuffers(window);
		glfwPollEvents();
	}

	glfwTerminate();
	return 0;
}