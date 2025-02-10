#ifndef SHADER_H
#define SHADER_H

#include<glad/glad.h>
#include<string>
#include<fstream>
#include<sstream>
#include<iostream>

using namespace std;

class Shader {
public:

	//id of the shaderProgram
	unsigned int ID;

	Shader(const char* vertexShaderSourcePath, const char* fragmentShaderSourcePath) {
		string vertexShaderCode;
		string fragmentShaderCode;

		ifstream vertexShaderSourceFile;
		ifstream fragmentShaderSourceFile;

		vertexShaderSourceFile.exceptions(ifstream::failbit | ifstream::badbit);
		fragmentShaderSourceFile.exceptions(ifstream::failbit | ifstream::badbit);

		try {
			vertexShaderSourceFile.open(vertexShaderSourcePath);
			fragmentShaderSourceFile.open(fragmentShaderSourcePath);

			stringstream vertexShaderSourceStream;
			stringstream fragmentShaderSourceStream;

			vertexShaderSourceStream << vertexShaderSourceFile.rdbuf();
			fragmentShaderSourceStream << fragmentShaderSourceFile.rdbuf();

			vertexShaderSourceFile.close();
			fragmentShaderSourceFile.close();

			vertexShaderCode = vertexShaderSourceStream.str();
			fragmentShaderCode = fragmentShaderSourceStream.str();
		}
		catch (ifstream::failure e) {
			cout << "ERROR: shader file couldn't be read successfully " << e.what() << endl;
		}

		const char* vShaderCode = vertexShaderCode.c_str();
		const char* fShaderCode = fragmentShaderCode.c_str();

		unsigned int vertexShader = glCreateShader(GL_VERTEX_SHADER);
		glShaderSource(vertexShader, 1, &vShaderCode, NULL);
		glCompileShader(vertexShader);

		checkCompileErrors(vertexShader, "VERTEX");

		unsigned int fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
		glShaderSource(fragmentShader, 1, &fShaderCode, NULL);
		glCompileShader(fragmentShader);

		checkCompileErrors(fragmentShader, "FRAGMENT");

		ID = glCreateProgram();
		glAttachShader(ID, vertexShader);
		glAttachShader(ID, fragmentShader);
		glLinkProgram(ID);
		checkCompileErrors(ID, "PROGRAM");

		glDeleteShader(vertexShader);
		glDeleteShader(vertexShader);
	}

	void use()
	{
		glUseProgram(ID);
	}

private:
	void checkCompileErrors(unsigned int shader, string type) {
		int success;
		char infoLog[512];
		if (type == "VERTEX" || type == "FRAGMENT") {
			glGetShaderiv(shader, GL_COMPILE_STATUS, &success);
			if (!success) {
				glGetShaderInfoLog(shader, 512, NULL, infoLog);
				cout << "ERROR: error when compiling vertex shader" << infoLog << endl;
			}
		}
		else {
			glGetProgramiv(ID, GL_LINK_STATUS, &success);
			if (!success) {
				glGetShaderInfoLog(shader, 512, NULL, infoLog);
				cout << "ERROR: error when linking program" << infoLog << endl;
			}
		}
	}
};

#endif