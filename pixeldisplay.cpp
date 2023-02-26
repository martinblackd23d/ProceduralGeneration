#include "pixeldisplay.h"
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include "shader.h"
#include <iostream>
#include <vector>
#include "stb_image.h"

Shader* gshaders;
GLFWwindow* gwindow;
unsigned int gVAO;
unsigned int gtexture1;
int gxImage;
int gyImage;


void framebuffer_size_callback(GLFWwindow* window, int width, int height) {
	glViewport(0, 0, width, height);
}

void processInput(GLFWwindow* window) {
	if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS) {
		glfwSetWindowShouldClose(window, true);
	}
}

int initGLFW(int* status, GLFWwindow** window) {
	glfwInit();
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 6);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
	//glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE)

	*window = glfwCreateWindow(1000, 1000, "TerrainGen", NULL, NULL);
	if (window == NULL) {
		std::cout << "Failed to create GLFW window" << std::endl;
		glfwTerminate();
		*status = -1;
		return -1;
	};
	glfwMakeContextCurrent(*window);
	if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {
		std::cout << "Failed to initialize GLAD" << std::endl;
		*status = -1;
		return -1;
	};
	glViewport(0, 0, 1000, 1000);
	glfwSetFramebufferSizeCallback(*window, framebuffer_size_callback);
	return 0;
}

void createTexture(unsigned int* texture1, int xImage, int yImage, unsigned char* data) {
	//createTexture(xImage, yImage);
	glGenTextures(1, texture1);
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, *texture1);
	//if (data == NULL) {
	//	std::cout << "Failed to allocate memory" << std::endl;
	//}


	//int width, height, nrChannels;
	//unsigned char* data = stbi_load("temp.png", &width, &height, &nrChannels, 0);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);

	if (data) {
		glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, xImage, yImage, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
		//glGenerateMipmap(GL_TEXTURE_2D);
	}
	else {
		std::cout << "Failed to load image data" << std::endl;
	}
	//stbi_image_free(data);
	//delete[] data;
}

void sceneSetup(Shader* shaders, unsigned int* VAO, unsigned int *texture1, int xImage, int yImage, unsigned char* data) {

	float vertices[] = {
		1.0f, 1.0f, 0.0f,	1.0f, 1.0f,
		1.0f, -1.0, 0.0f,	1.0f, 0.0f,
		-1.0f, -1.0f, 0.0f,	0.0f, 0.0f,
		-1.0f, 1.0f, 0.0f,	0.0f, 1.0f
	};
	unsigned int indices[] = {
		0, 1, 3,
		1, 2, 3
	};
	float texCoords1[] = {
		0.0f, 0.0f,
		1.0f, 1.0f,
		0.0f, 1.0f
	};


	unsigned int VBO, EBO;
	glGenVertexArrays(1, VAO);
	glGenBuffers(1, &VBO);
	glGenBuffers(1, &EBO);

	glBindVertexArray(*VAO);

	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);

	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)(3 * sizeof(float)));
	glEnableVertexAttribArray(1);

	createTexture(texture1, xImage, yImage, data);

	(*shaders).use();

	glUniform1i(glGetUniformLocation((*shaders).ID, "texture1"), 0);
};

void updateBuffer() {
	return;
}

void renderFrame(Shader *shaders, int xImage, int yImage, unsigned int VAO, unsigned int* texture1) {
	updateBuffer();

	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, *texture1);

	shaders->use();
	glBindVertexArray(VAO);

	/*std::vector<int> textureData;
	for (int i = 0; i < xImage * yImage * 3; i++) {
		textureData[i] = pixelBuffer[i];
	}
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_1D, texture);

	(*shaders).use();
	(*shaders).setInt("width", globalwidth);
	(*shaders).setInt("height", globalheight);
	(*shaders).setInt("xImage", xImage);
	(*shaders).setInt("yImage", yImage);
	(*shaders).setIntArray("data", xImage * yImage * 3 * sizeof(int), pixelBuffer);*/


	glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);

};

void mainLoop(GLFWwindow* window, Shader *shaders, int xImage, int yImage, unsigned int VAO, unsigned int* texture1) {
	processInput(window);

	glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT);

	

	renderFrame(shaders, xImage, yImage, VAO, texture1);

	glfwSwapBuffers(window);
	glfwPollEvents();
}

int init(int xImage, int yImage, unsigned char* data) {
	int status = 0;
	GLFWwindow* window;
	initGLFW(&status, &window);
	if (status == -1) {
		return -1;
	}
	Shader shaders("vertexshader.c", "fragmentshader.c");

	unsigned int VAO;
	unsigned int texture1;
	sceneSetup(&shaders, &VAO, &texture1, xImage, yImage, data);
	gwindow = window;
	gshaders = &shaders;
	gxImage = xImage;
	gyImage = yImage;
	gVAO = VAO;
	gtexture1 = texture1;
	return 0;
}

void gterminate() {
	glfwTerminate();
}

int display(unsigned char* data) {
	if (!glfwWindowShouldClose(gwindow)) {
		mainLoop(gwindow, gshaders, gxImage, gyImage, gVAO, &gtexture1);
		return 0;
	}
	else {
		gterminate();
		return -1;
	}
}

