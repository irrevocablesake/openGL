#include<glad/glad.h>
#include<GLFW/glfw3.h>

#define GLM_ENABLE_EXPERIMENTAL

#include<glm/glm.hpp>
#include<glm/gtc/matrix_transform.hpp>
#include<glm/gtc/type_ptr.hpp>
#include<glm/gtx/string_cast.hpp>

#include "stb_image.h"

#include<iostream>

using namespace std;

int main() {

	//--------------------------------
	// TRANSLATION
	
	//position vector
	glm::vec4 vec(1.0f, 0.0f, 0.0f, 1.0f);
	cout << "position vector:" << vec.x << " " << vec.y << " " << vec.z << " " << vec.w << endl;

	//translation value
	glm::vec3 translateBy(1.0f, 1.0f, 0.0f);
	cout << "translate by:" << translateBy.x << " " << translateBy.y << " " << translateBy.z << endl;

	//creating a transformation matrix with desired translation effect
	glm::mat4 transformationMatrix = glm::mat4(1.0f);
	transformationMatrix = glm::translate(transformationMatrix, translateBy );

	//translating the vector
	vec = transformationMatrix * vec;

	//output vector with new location
	cout << "translation output: " << vec.x << " " << vec.y << " " << vec.z << " " << vec.w << endl << endl;



	//------------------------------
	//ROTATION

	//position vector
	vec = glm::vec4(1.0f, 0.0f, 0.0f, 1.0f);
	
	//rotation angle in radians
	float rotateBy = glm::radians(90.f);

	//rotation axis
	glm::vec3 axis = glm::vec3(0.0, 0.0, 1.0);

	//creating a transformation matrix with desired rotation effect
	transformationMatrix = glm::mat4(1.0f);
	transformationMatrix = glm::rotate(transformationMatrix, rotateBy, axis);

	//rotating the vector
	vec = transformationMatrix * vec;

	//output vector with new rotation
	cout << "rotation output: " << vec.x << " " << vec.y << " " << vec.z << " " << vec.w << endl << endl;

	//------------------------------
	//SCALING

	//position vector
	vec = glm::vec4(1.0f, 0.0f, 0.0f, 1.0f);

	//rotation angle in radians
	glm::vec3 scaleBy = glm::vec3( 0.5, 0.5, 0.5 );

	//creating a transformation matrix with desired scaling effect
	transformationMatrix = glm::mat4(1.0f);
	transformationMatrix = glm::scale( transformationMatrix, scaleBy);

	//scaling the vector
	vec = transformationMatrix * vec;

	//output vector with new rotation
	cout << "scaling output: " << vec.x << " " << vec.y << " " << vec.z << " " << vec.w << endl << endl;

	return 0;
}