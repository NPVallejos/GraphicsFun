#include "glad/glad.h"
#include "GLFW/glfw3.h"

int main()
{
	/** Initialize GLFW */
	glfwInit();

	/** 
	* This function sets hints for the next call to 
	* glfwCreateWindow.
	* ------------------------------------------------------
	* You can think of this as initializing a struct using a
	* macro to specify the struct attribute that we are
	* assigning a value to.
	* ------------------------------------------------------
	* @param GLFW_CONTEXT_VERSION_MAJOR - enum option to configure 
	* @param 3 - integer that sets the value of this enum
	*/
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
	//glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);

	return 0;
}