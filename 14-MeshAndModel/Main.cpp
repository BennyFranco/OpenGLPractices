#define GLEW_STATIC
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

glm::vec3 lightPos(0.0f, 0.0f, 5.0f);

Camera camera(glm::vec3(0.0f, 0.0f, 3.0f));

void framebuffer_size_callback(GLFWwindow* window, int width, int height);
void processInput(GLFWwindow *window);
void mouse_callback(GLFWwindow* window, double xpos, double ypos);
void scroll_callback(GLFWwindow* window, double xoffset, double yoffset);

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

	glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
	glfwSetCursorPosCallback(window, mouse_callback);
	glfwSetScrollCallback(window, scroll_callback);

	glEnable(GL_DEPTH_TEST);

#ifdef __APPLE__
	Shader ourShader("../shaders/model.vs", "../shaders/model.fs");
	Shader lightShader("./shaders/lamp.vs", "./shaders/lamp.fs");
#else
	Shader ourShader("../shaders/model.vs", "../shaders/model.fs");
	Shader lightShader("../shaders/lamp.vs", "../shaders/lamp.fs");
#endif

	Model ourModel("../models/nanosuit/nanosuit.obj");

	glm::vec3 pointLightPositions[] = {
		glm::vec3(0.7f,  0.2f,  2.0f),
		glm::vec3(2.3f, -3.3f, -4.0f),
		glm::vec3(-4.0f,  2.0f, -12.0f),
		glm::vec3(0.0f,  0.0f, -3.0f)
	};

	// Main render loop                                                  
	while (!glfwWindowShouldClose(window))
	{
		//Call processInput() method
		processInput(window);

		// Rendering commands
		glClearColor(0.05f, 0.05f, 0.05f, 1.0f); // Set the clear color
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT); // Clear the passed buffer

		float currentFrame = (float)glfwGetTime();
		deltaTime = currentFrame - lastFrame;
		lastFrame = currentFrame;

		ourShader.use();
		
		ourShader.setVec3("viewPos", camera.Position);

		// Directional light values
		ourShader.setVec3("dirLight.direction", -0.2f, -1.0f, -0.3f);
		ourShader.setVec3("dirLight.ambient", 0.2f, 0.2f, 0.2f);
		ourShader.setVec3("dirLight.diffuse", 0.5f, 0.5f, 0.5f);
		ourShader.setVec3("dirLight.specular", 1.0f, 1.0f, 1.0f);

		for (GLuint i = 0; i < 4; i++)
		{

			std::string structName = "pointLights["+std::to_string(i)+"].";
			ourShader.setFloat(structName + "constant", 1.0f);
			ourShader.setFloat(structName + "linear", 0.09f);
			ourShader.setFloat(structName + "quadratic", 0.032f);
			ourShader.setVec3(structName + "position", pointLightPositions[i]);
			ourShader.setVec3(structName + "diffuse", 0.5f, 0.5f, 0.5f);
			ourShader.setVec3(structName + "specular", 1.0f, 1.0f, 1.0f);
		}

		// SpotLight values
		/*ourShader.setVec3("light.position", camera.Position); // No necessery for directional light
		ourShader.setVec3("light.direction", camera.Front);
		ourShader.setFloat("light.cutOff", glm::cos(glm::radians(12.5f)));
		ourShader.setFloat("light.outerCutOff", glm::cos(glm::radians(17.5f)));*/

		ourShader.setFloat("material.shininess", 32.0f);

		glm::mat4 view;
		view = camera.GetViewMatrix();

		glm::mat4 projection;
		projection = glm::perspective(glm::radians(camera.Zoom), static_cast<float>(WIDTH / HEIGHT), 0.1f, 100.0f);
		
		ourShader.setMat4("projection", projection);
		ourShader.setMat4("view", view);

		glm::mat4 model;
		model = glm::translate(model, glm::vec3(0.0f, -1.75f, 0.0f));
		model = glm::scale(model, glm::vec3(0.2f, 0.2f, 0.2f));
		ourShader.setMat4("model", model);
		ourModel.Draw(ourShader);

		/*lightShader.use();
		lightShader.setMat4("projection", projection);
		lightShader.setMat4("view", view);

		for (GLuint i = 0; i < 4; i++)
		{
			glm::mat4 model;
			model = glm::translate(model, pointLightPositions[i]);
			model = glm::scale(model, glm::vec3(0.2f));
			lightShader.setMat4("model", model);

			glDrawArrays(GL_TRIANGLES, 0, 36);
		}*/

		//glBindVertexArray(VAO);
		//glBindVertexArray(lightVAO);

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
	{
		glfwSetWindowShouldClose(window, true);
	}
	if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
		camera.ProcessKeyboard(FORWARD, deltaTime);
	if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
		camera.ProcessKeyboard(BACKWARD, deltaTime);
	if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
		camera.ProcessKeyboard(LEFT, deltaTime);
	if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
		camera.ProcessKeyboard(RIGHT, deltaTime);
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
