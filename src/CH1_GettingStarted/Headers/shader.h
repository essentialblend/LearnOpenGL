#ifndef SHADER_H
#define SHADER_H

#include <GLAD/include/glad/glad.h>

#include <string>
#include <iostream>
#include <sstream>
#include <fstream>

class Shader
{
public:
	unsigned int programID;

	/*Constructor to create a shader program.*/
	Shader(const char* vertexPath, const char* fragmentPath)
	{
		std::string vertexCode;
		std::string fragmentCode;
		std::ifstream vShaderFile;
		std::ifstream fShaderFile;

		/*Ensure ifstream exceptions*/
		vShaderFile.exceptions(std::ifstream::failbit | std::ifstream::badbit);
		fShaderFile.exceptions(std::ifstream::failbit | std::ifstream::badbit);

		/*Now try to open files.*/
		try
		{
			vShaderFile.open(vertexPath);
			fShaderFile.open(fragmentPath);
			std::stringstream vShaderStream, fShaderStream;
			/*Read file's buffer contents into stream.*/
			vShaderStream << vShaderFile.rdbuf();
			fShaderStream << fShaderFile.rdbuf();

			/*Close file handlers*/
			vShaderFile.close();
			fShaderFile.close();

			/*Convert stream into string*/
			vertexCode = vShaderStream.str();
			fragmentCode = fShaderStream.str();
		}
		catch (std::ifstream::failure exception)
		{
			std::cout << "ERROR :: SHADER :: FILE_READ_UNSUCCESSFUL" << std::endl;
		}

		const char* vShaderCodeFinal = vertexCode.c_str();
		const char* fShaderCodeFinal = fragmentCode.c_str();

		unsigned int vertexShaderObj, fragmentShaderObj;
		/*Build and compile our vertex shader program.*/
		vertexShaderObj = glCreateShader(GL_VERTEX_SHADER);
		glShaderSource(vertexShaderObj, 1, &vShaderCodeFinal, NULL);
		glCompileShader(vertexShaderObj);

		/*Check for vertex and fragment shader compile errors.*/
		int errorCheckSuccess;
		char infoLog[512];
		glGetShaderiv(vertexShaderObj, GL_COMPILE_STATUS, &errorCheckSuccess);

		if (!errorCheckSuccess)
		{
			glGetShaderInfoLog(vertexShaderObj, 512, NULL, infoLog);
			std::cout << "ERROR:: SHADER:: VERTEX:: COMPILATION FAILED!!\n" << infoLog << std::endl;
		}

		/*Build and compile our fragment shader program.*/
		fragmentShaderObj = glCreateShader(GL_FRAGMENT_SHADER);
		glShaderSource(fragmentShaderObj, 1, &fShaderCodeFinal, NULL);
		glCompileShader(fragmentShaderObj);

		glGetShaderiv(fragmentShaderObj, GL_COMPILE_STATUS, &errorCheckSuccess);
		if (!errorCheckSuccess)
		{
			glGetShaderInfoLog(fragmentShaderObj, 512, NULL, infoLog);
			std::cout << "ERROR:: SHADER:: FRAGMENT:: COMPILATION FAILED!!\n" << infoLog << std::endl;
		}

		/*Build our shader program, attach our vertex, fragment shaders, link the program and check for linking errors.*/
		programID = glCreateProgram();
		glAttachShader(programID, vertexShaderObj);
		glAttachShader(programID, fragmentShaderObj);
		glLinkProgram(programID);

		glGetProgramiv(programID, GL_LINK_STATUS, &errorCheckSuccess);
		if (!errorCheckSuccess)
		{
			glGetShaderInfoLog(fragmentShaderObj, 512, NULL, infoLog);
			std::cout << "ERROR:: SHADER:: PROGRAM:: LINKING FAILED!!\n" << infoLog << std::endl;
		}

		glDeleteShader(vertexShaderObj);
		glDeleteShader(fragmentShaderObj);
	}

	void use()
	{
		glUseProgram(programID);
	}
	
	void setBool(const std::string& name, bool boolValue) const
	{
		glUniform1i(glGetUniformLocation(programID, name.c_str()), (int)boolValue);
	}
	
	void setInt(const std::string& name, int intValue) const
	{
		glUniform1i(glGetUniformLocation(programID, name.c_str()), intValue);
	}
	
	void setFloat(const std::string& name, float floatValue) const
	{
		glUniform1f(glGetUniformLocation(programID, name.c_str()), floatValue);
	}
};
#endif