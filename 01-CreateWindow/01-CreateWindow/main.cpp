#include <GL/glew.h>
#include <GLFW/glfw3.h>

#include <iostream>

void framebuffer_size_callback(GLFWwindow* window, int width, int height);
void processInput(GLFWwindow *window);

int main()
{
	glfwInit(); // Initialize GLFW
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3); // Set major version of OpenGL
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3); // Set minor version of OpenGl
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE); // Use core profile of OpenGL


#ifdef MACOS
	glfwWindowHint(GLFW_OPENGL_FOWARD_COMPAT, GL_TRUE); // Need it for macOS
#endif

	// Create the window object and check if this is valid before continue
	GLFWwindow* window = glfwCreateWindow(800, 600, "Window", NULL, NULL);
	if (window == NULL)
	{
		std::cout << "Failed to create GLFW Window" << std::endl;
		glfwTerminate();
		return -1;
	}

	// Creates the window context
	glfwMakeContextCurrent(window);

	// Setting the coordinates, We do have to tell GLFW we want to call this function on 
	// every window resize by registering it
	glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);

	// Main render loop
	while (!glfwWindowShouldClose(window))
	{
		//Call processInput() method
		processInput(window);

		// Rendering commands
		glClearColor(0.2f, 0.3f, 0.3f, 1.0f); // Set the clear color
		glClear(GL_COLOR_BUFFER_BIT); // Clear the passed buffer

		// Checks if any events are triggered (like keyboard input or mouse movement events), updates the window state, and calls the corresponding functions (which we can set via callback methods). 
		glfwSwapBuffers(window);

		//  Will swap the color buffer (a large buffer that contains color values for each pixel in GLFW's window) that has been used to draw in during this iteration and show it as output to the screen.
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
	if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
	{
		glfwSetWindowShouldClose(window, true);
	}
}