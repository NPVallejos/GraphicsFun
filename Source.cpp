#include "glad/glad.h"
#include "GLFW/glfw3.h"
#include <iostream>

void framebuffer_size_callback(GLFWwindow* window, int width, int height);

int main()
{
	/** Initialize GLFW */
	glfwInit();

	/** 
	* ------------------------------------------------------
	* This function sets hints for the next call to 
	* glfwCreateWindow.
	* ------------------------------------------------------
	* You can think of this as initializing a struct using a
	* macro to specify the struct attribute that we are
	* assigning a value to.
	* ------------------------------------------------------
	* @param GLFW_CONTEXT_VERSION_MAJOR - enum option to configure glfw version
	* @param 3 - integer that sets the glfw version number
	* ------------------------------------------------------
	*/
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
	/** Uncomment the following for MAC OS X */
	//glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);

	/** Create a window object */
	GLFWwindow* window = glfwCreateWindow(800, 600, "GraphicsFun", nullptr, nullptr);
	/** Check if window is valid */
	if (window == nullptr)
	{
		std::cout << "Failed to create GLFW window" << std::endl;
		glfwTerminate();
		return -1;
	}
	glfwMakeContextCurrent(window);

	/** Initialize GLAD which manages function pointers for OpenGL */
	if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
	{
		std::cout << "Failed to initialize GLAD" << std::endl;
		return -1;
	}

	/** 
	* Set the size of the rendering window 
	* ------------------------------------------------------
	* First two params set the location of the lower left corner of the window
	* Last two params set the width and height of the rendering window in pixels
	* ------------------------------------------------------
	* This data specifies how to transform 2D coordinates to screen coordinates
	* Ex: A processed point of location (-0.5, 0.5) would be mapped to (200, 450) in
	* screen coordinates
	* ------------------------------------------------------
	* We effectively map from the range (-1 to 1) to (0, 800) and (0, 600)
	* ------------------------------------------------------
	* (-1, -1) is bottom left corner, (-1, 0) is bottom middle, (-1, 1) is bottom right
	*/
	glViewport(0, 0, 800, 600);

	/** 
	* The moment a user resizes the window the viewport should be adjusted as well =>
	* Register a callback function on the window that gets called each time the window is
	* resized
	* ------------------------------------------------------
	* void framebuffer_size_callback(GLFWwindow * window, int width, int height);
	* ------------------------------------------------------
	*/
	glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);

	/** Start the render loop - only ends when the window is closed */
	while (!glfwWindowShouldClose(window))
	{
		/**  All rendering commands draw to the back buffer and then output the final result to the front buffer: this prevents flickering/artifacts */
		glfwSwapBuffers(window);
		glfwPollEvents();
	}

	/** Clean up all allocated resources and exit all windows */
	glfwTerminate();

	return 0;
}

void framebuffer_size_callback(GLFWwindow* window, int width, int height)
{
	glViewport(0, 0, width, height);
}