#define GLEW_STATIC
#define STB_IMAGE_IMPLEMENTATION

#include "stb_image.h"
#include <GL/glew.h>
#include <GLFW/glfw3.h>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include <iostream>

#include "Shader.h"
#include "Camera.h"
#include "Model.h"

#define WIDTH 800
#define HEIGHT 600
#define WINDOW_TITLE "Window"

float deltaTime = 0.0f;
float lastFrame = 0.0f;

float lastX = WIDTH / 2, lastY = HEIGHT / 2;
bool firstMouse = true;
bool isMouseCaptured = true;
GLuint mouseStatus = GLFW_CURSOR_DISABLED;

glm::vec3 lightPos(0.0f, 0.0f, 5.0f);

Camera camera(glm::vec3(0.0f, 0.0f, 3.0f));

void framebuffer_size_callback(GLFWwindow* window, int width, int height);
void processInput(GLFWwindow *window);
void mouse_callback(GLFWwindow* window, double xpos, double ypos);
void scroll_callback(GLFWwindow* window, double xoffset, double yoffset);
GLuint load_texture(const char *path);
unsigned int loadCubemap(std::vector<std::string> faces);

int main()
{
	glewExperimental = GL_TRUE;

	if (!glfwInit()) {
		std::cerr << "GL initialization failed" << std::endl;
		return 1;
	}
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3); // Set major version of OpenGL
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3); // Set minor version of OpenGl
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE); // Use core profile of OpenGL

#ifdef __APPLE__
	glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE); // Need it for macOS
#endif
														// Create the window object and check if this is valid before continue
	GLFWwindow* window = glfwCreateWindow(WIDTH, HEIGHT, WINDOW_TITLE, NULL, NULL);
	if (window == NULL)
	{
		std::cout << "Failed to create GLFW Window" << std::endl;
		glfwTerminate();
		return -1;
	}

	// Creates the window context
	glfwMakeContextCurrent(window);

	// Initialize GLEW, NOTE: This must be after create context
	glewInit();

	// Setting the coordinates, We do have to tell GLFW we want to call this function on 
	// every window resize by registering it
	glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);

	glfwSetInputMode(window, GLFW_CURSOR, mouseStatus);
	glfwSetCursorPosCallback(window, mouse_callback);
	glfwSetScrollCallback(window, scroll_callback);

	glEnable(GL_DEPTH_TEST);

	Shader shader("../shaders/model.vs", "../shaders/model.fs");
	Shader normalShader("../shaders/normal_visualization.vs", "../shaders/normal_visualization.fs", "../shaders/normal_visualization.gs");

    Model ourModel("../models/nanosuit/nanosuit.obj");
    
// Main render loop                                                  
	while (!glfwWindowShouldClose(window))
	{
		//Call processInput() method
		processInput(window);

		float currentFrame = (float)glfwGetTime();
		deltaTime = currentFrame - lastFrame;
		lastFrame = currentFrame;

		// Rendering commands
		glClearColor(0.1f, 0.1, 0.1f, 1.0f); // Set the clear color
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT); // Clear the passed buffer
		
        glm::mat4 model;
        glm::mat4 view;
        glm::mat4 projection;

        projection = glm::perspective(glm::radians(camera.Zoom), (float)WIDTH / (float)HEIGHT, 0.1f, 100.0f);
        // draw scene as normal
        shader.use();
        view = camera.GetViewMatrix();
        //model = glm::translate(model, glm::vec3(0.0f, -1.75f, 0.0f));
		//model = glm::scale(model, glm::vec3(0.2f, 0.2f, 0.2f));
        shader.setMat4("model", model);
        shader.setMat4("view", view);
        shader.setMat4("projection", projection);
        shader.setVec3("cameraPos", camera.Position);
		shader.setFloat("time", glfwGetTime()); 

        ourModel.Draw(shader);

		normalShader.use();
        normalShader.setMat4("projection", projection);
        normalShader.setMat4("view", view);
        normalShader.setMat4("model", model);

        ourModel.Draw(normalShader);

        //  Will swap the color buffer (a large buffer that contains color values for each pixel in GLFW's window) that has been used to draw in during this iteration and show it as output to the screen.
		glfwSwapBuffers(window);
		glfwPollEvents();
	}

    // Clean and delete all resources allocated.
	glfwTerminate();
	return 0;
}

void framebuffer_size_callback(GLFWwindow* window, int width, int height)
{
	// Behind the scenes OpenGL uses the data specified via glViewport to transform the 2D coordinates it processed 
	// to coordinates on your screen. For example, a processed point of location (-0.5,0.5) would (as its final
	// transformation) be mapped to (200,450) in screen coordinates. Note that processed coordinates in OpenGL are 
	// between -1 and 1 so we effectively map from the range (-1 to 1) to (0, 800) and (0, 600).
	glViewport(0, 0, width, height);
}

/*
We also want to have some form of input control in GLFW and we can achieve this several of GLFW's input functions.
We'll be using GLFW's glfwGetKey function that takes the window as input together with a key.
The function returns whether this key is currently being pressed.
*/
void processInput(GLFWwindow *window)
{
	float cameraSpeed = 2.5f * deltaTime;
	if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
		glfwSetWindowShouldClose(window, true);
	if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
		camera.ProcessKeyboard(FORWARD, deltaTime);
	if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
		camera.ProcessKeyboard(BACKWARD, deltaTime);
	if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
		camera.ProcessKeyboard(LEFT, deltaTime);
	if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
		camera.ProcessKeyboard(RIGHT, deltaTime);
    if (glfwGetKey(window, GLFW_KEY_T) == GLFW_PRESS)
    {
        mouseStatus = isMouseCaptured ? GLFW_CURSOR_NORMAL : GLFW_CURSOR_DISABLED;
        isMouseCaptured = !isMouseCaptured;
        glfwSetInputMode(window, GLFW_CURSOR, mouseStatus);
    }
}

void mouse_callback(GLFWwindow* window, double xpos, double ypos)
{
	if (firstMouse)
	{
		lastX = (float)xpos;
		lastY = (float)ypos;
		firstMouse = false;
	}

	float xoffset = (float)xpos - lastX;
	float yoffset = (float)ypos - lastY;

	lastX = (float)xpos;
	lastY = (float)ypos;

	camera.ProcessMouseMovement(xoffset, yoffset);
}

void scroll_callback(GLFWwindow* window, double xoffset, double yoffset)
{
	camera.ProcessMouseScroll((float)yoffset);
}