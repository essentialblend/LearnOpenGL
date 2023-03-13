#include <GLAD/include/glad/glad.h>
#include <GLFWLib/GLFW/glfw3.h>

/*OpenGL Mathematics headers.*/
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include "stb_image.h"

#include "Headers/shader.h"

#include <iostream>

void framebuffer_size_callback(GLFWwindow* window, int width, int height);
void processInput(GLFWwindow* windowObj);

const int WindowHeight = 600;
const int WindowWidth = 800;

int main()
{
	/* Initialize GLFW.*/
	glfwInit();
	
	/*Use glfwWindowHint to configure GLFW.*/
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	
	/*Limit to core profile.*/
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

	/*Create a window object with parameters.*/
	GLFWwindow* windowObj = glfwCreateWindow(WindowWidth, WindowHeight, "Learn OpenGL", NULL, NULL);
	if (windowObj == NULL)
	{
		std::cout << "Failed to create GLFW window!" << std::endl;
		glfwTerminate();
		return -1;
	}
	/*Make the object current on the thread.*/
	glfwMakeContextCurrent(windowObj);

	/*Check for GLAD problems.*/
	if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
	{
		std::cout << "Failed to initialize GLAD" << std::endl;
		return -1;
	}

	/* Call framebuffer_size_callback function everytime window is resized.*/
	glfwSetFramebufferSizeCallback(windowObj, framebuffer_size_callback);

	/*Vertices for the triangle.*/
	float vertices[] = {
		// positions			// colors			// tex-coords
		0.5f,  0.5f, 0.0f,		1.0f, 0.0f, 0.0f,	1.0f, 1.0f,		// top right
	    0.5f, -0.5f, 0.0f,		0.0f, 1.0f, 0.0f,	1.0f, 0.0f,		// bottom right
	   -0.5f, -0.5f, 0.0f,		0.0f, 0.0f, 1.0f,	0.0f, 0.0f,		// bottom left
	   -0.5f,  0.5f, 0.0f,		1.0f, 1.0f, 0.0f,	0.0f, 1.0f		// top left
	};

	/*Create our shader program.*/
	Shader shaderProgram("Shaders/vShader.glsl", "Shaders/fShader.glsl");
	

	GLuint indices[] =
	{
		0, 1, 3,	// first triangle
		1, 2, 3		// second triangle
	};

	/*Create a VBO, VAO.*/
	GLuint VBOObj;
	/*Create VAO.*/
	GLuint VAOObj;
	/*Create EBO.*/
	GLuint EBOObj;

	/*Set up VAO.*/
	glGenVertexArrays(1, &VAOObj);
	glBindVertexArray(VAOObj);

	/*Set up VBO*/
	/*Generate a buffer object and assign it to VBOObj.*/
	glGenBuffers(1, &VBOObj);
	/*Bind the type of buffer to the buffer object.*/
	glBindBuffer(GL_ARRAY_BUFFER, VBOObj);
	/*Call glBufferData to copy vertex data into our newly created VBOObj.*/
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

	/*Set up EBO*/
	glGenBuffers(1, &EBOObj);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBOObj);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

	/*Tell OpenGL how to interpret vertex position data.*/
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);

	/*Tell OpenGL how to interpret vertex color data. (Not needed?)*/ 
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(3 * sizeof(float)));
	glEnableVertexAttribArray(1);
	glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(6 * sizeof(float)));
	glEnableVertexAttribArray(2);
	

	/*TEXTURES CHAPTER.................*/

	GLuint textureObj;
	glGenTextures(1, &textureObj);
	glBindTexture(GL_TEXTURE_2D, textureObj);

	/*Give parameters for texture wrapping on the S and T axis.*/
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);

	/* If GL_CLAMP_BORDER, need to specify a border color.

		float borderColor[] = {1.0f, 1.0f, 0.0f, 1.0f};
		glTexParameterfv(GL_TEXTURE_2D, GL_TEXTURE_BORDER_COLOR, borderColor);
	*/

	/*Set mip-map filtering method. 1- GL_NEAREST_MIPMAP_NEAREST, GL_LINEAR_MIPMAP_NEAREST, GL_NEAREST_MIPMAP_LIENAR, GL_LINEAR_MIPMAP_LINEAR.*/
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

	int width, height, nrChannels;
	stbi_set_flip_vertically_on_load(true);
	unsigned char* imageData = stbi_load("../../resources/images/container.jpg", &width, &height, &nrChannels, 0);
	if (imageData)
	{
		glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
		
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, imageData);
		glGenerateMipmap(GL_TEXTURE_2D);
	}
	else
	{
		std::cout << "Failed to load texture." << std::endl;
	}
	stbi_image_free(imageData);

	GLuint textureObj2;
	glGenTextures(1, &textureObj2);
	glBindTexture(GL_TEXTURE_2D, textureObj2);
	/*Give parameters for texture wrapping on the S and T axis.*/
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);

	/* If GL_CLAMP_BORDER, need to specify a border color.

		float borderColor[] = {1.0f, 1.0f, 0.0f, 1.0f};
		glTexParameterfv(GL_TEXTURE_2D, GL_TEXTURE_BORDER_COLOR, borderColor);
	*/

	/*Set mip-map filtering method. 1- GL_NEAREST_MIPMAP_NEAREST, GL_LINEAR_MIPMAP_NEAREST, GL_NEAREST_MIPMAP_LIENAR, GL_LINEAR_MIPMAP_LINEAR.*/
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

	stbi_set_flip_vertically_on_load(true);
	imageData = stbi_load("../../resources/images/awesomeface.png", &width, &height, &nrChannels, 0);
	if (imageData)
	{
		glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, imageData);
		glGenerateMipmap(GL_TEXTURE_2D);
	}
	else
	{
		std::cout << "Failed to load texture." << std::endl;
	}
	stbi_image_free(imageData);

	shaderProgram.use();
	glUniform1i(glGetUniformLocation(shaderProgram.programID, "chosenTexture"), 0);
	// or set it via the texture class
	shaderProgram.setInt("chosenTexture2", 1);

	/*RENDER LOOP*/
	while (!glfwWindowShouldClose(windowObj))
	{
		processInput(windowObj);
		
		/*Render stuff happens here.*/
		glClearColor(0.2f, 0.5f, 0.6f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT);
		
		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, textureObj);
		glActiveTexture(GL_TEXTURE1);
		glBindTexture(GL_TEXTURE_2D, textureObj2);

		shaderProgram.use();
		glBindVertexArray(VAOObj);
		glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);

		//Rotate and scale the object using the vertex shader.
		glm::mat4 translMatrix = glm::mat4(1.0f);
		translMatrix = glm::translate(translMatrix, glm::vec3(sin(glfwGetTime()) * 0.5f, sin(glfwGetTime()) * 0.5f, 0.0f));
		translMatrix = glm::rotate(translMatrix, (float)glfwGetTime(), glm::vec3(0.0f, 0.0f, 1.0f));

		GLuint transLoc = glGetUniformLocation(shaderProgram.programID, "transformMatrix");
		glUniformMatrix4fv(transLoc, 1, GL_FALSE, glm::value_ptr(translMatrix));
		glfwSwapBuffers(windowObj);
		glfwPollEvents();
	}

	/*Delete VBO/VAO/EBO/Shader Program.*/
	glDeleteVertexArrays(1, &VAOObj);
	glDeleteBuffers(1, &VBOObj);
	glDeleteBuffers(1, &EBOObj);
	
	glfwTerminate();
	return 0;
}

void framebuffer_size_callback(GLFWwindow* window, int width, int height)
{
	glViewport(0, 0, width, height);
}

void processInput(GLFWwindow* windowObj)
{
	if (glfwGetKey(windowObj, GLFW_KEY_ESCAPE) == GLFW_PRESS)
	{
		glfwSetWindowShouldClose(windowObj, true);
	}
}