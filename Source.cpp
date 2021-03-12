#include "glad/glad.h"
#include "GLFW/glfw3.h"
#include <iostream>

void framebuffer_size_callback(GLFWwindow* window, int width, int height);
void toggleWireframeMode();
void keyCallback(GLFWwindow* window, int key, int scancode, int action, int mods);

bool bWireframeModeOn = false;

int main()
{
	/*********************************************************************************************************/
	/* Initialize GLFW, Create Window Object, Set Current Context, Set Resize Window Callback, Set Viewport. */
	/*********************************************************************************************************/
	
	/** 1. Initialize GLFW Library */
	glfwInit();
	
	/**
	* ------------------------------------------------------
	* 2. This function sets hints for the next call to
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

#ifndef __APPLE__
	glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
#endif

	/** 3. Creates Window Object & set it to be the current context. */
	GLFWwindow* window = glfwCreateWindow(800, 600, "GraphicsFun", nullptr, nullptr);
	if (window == nullptr)
	{
		std::cout << "Failed to create GLFW window" << std::endl;
		glfwTerminate();
		return -1;
	}
	glfwMakeContextCurrent(window);

	/**
	* The moment a user resizes the window the viewport should be adjusted as well =>
	* Register a callback function on the window that gets called each time the window is
	* resized
	* ------------------------------------------------------
	* void framebuffer_size_callback(GLFWwindow * window, int width, int height);
	* ------------------------------------------------------
	*/
	glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);

	/** Initialize GLAD which manages function pointers for OpenGL */
	{
		if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
		{
			std::cout << "Failed to initialize GLAD" << std::endl;
			return -1;
		}
	}

	/** 
	* Set the actual size of the window rectangle for you rendering 
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
	* glViewport(x, y, width, height)
	*/
	glViewport(0, 0, 800, 600);

	/************************************************************************/
	/*				Build, compile, and link our shader program             */
	/************************************************************************/
	
	/** 1. Setup Vertex Shader - we just set the position of each vertex nothing crazy */
	const char* vertexShaderSource = "#version 330 core\n"
		"layout (location = 0) in vec3 aPos;\n"
		"void main()\n"
		"{\n"
		"   gl_Position = vec4(aPos.x, aPos.y, aPos.z, 1.0);\n"
		"}\0";
	unsigned int vertexShader = glCreateShader(GL_VERTEX_SHADER);
	glShaderSource(vertexShader, 1, &vertexShaderSource, NULL);
	glCompileShader(vertexShader);
	// Check for shader compile errors
	int success;
	char infoLog[512];
	glGetShaderiv(vertexShader, GL_COMPILE_STATUS, &success);
	if (!success)
	{
		glGetShaderInfoLog(vertexShader, 512, NULL, infoLog);
		std::cout << "ERROR::SHADER::VERTEX::COMPILATION_FAILED\n" << infoLog << std::endl;
	}

	/** 2. Setup Fragment Shader - Just sets the color of our pixels to be orange */
	const char* fragmentShaderSource = "#version 330 core\n"
		"out vec4 FragColor;\n"
		"void main()\n"
		"{\n"
		"   FragColor = vec4(1.0f, 0.5f, 0.2f, 1.0f);\n"
		"}\n\0";
	unsigned int fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
	glShaderSource(fragmentShader, 1, &fragmentShaderSource, NULL);
	glCompileShader(fragmentShader);
	// Check for shader compile errors
	glGetShaderiv(fragmentShader, GL_COMPILE_STATUS, &success);
	if (!success)
	{
		glGetShaderInfoLog(fragmentShader, 512, NULL, infoLog);
		std::cout << "ERROR::SHADER::FRAGMENT::COMPILATION_FAILED\n" << infoLog << std::endl;
	}

	/** 3. Create Shader Program + Link our Two Shaders by attaching to a single Shader Program - OpenGL configures other shaders for us (Primitive Assembly | Geometry Shader | Rasterization | Tests and Blending) */
	unsigned int shaderProgram = glCreateProgram();
	glAttachShader(shaderProgram, vertexShader);
	glAttachShader(shaderProgram, fragmentShader);
	glLinkProgram(shaderProgram);
	// check for linking errors
	glGetProgramiv(shaderProgram, GL_LINK_STATUS, &success);
	if (!success)
	{
		glGetProgramInfoLog(shaderProgram, 512, NULL, infoLog);
		std::cout << "ERROR::SHADER::PROGRAM::LINKING_FAILED\n" << infoLog << std::endl;
	}

	/** 4. Delete shaders after linking to free up resources */
	glDeleteShader(vertexShader);
	glDeleteShader(fragmentShader);

	/** Set up vertex data (and buffer(s)) and configure vertex attributes */
	float vertices[] = {
		 0.5f,  0.5f, 0.0f,  // top right
		 0.5f, -0.5f, 0.0f,  // bottom right
		-0.5f, -0.5f, 0.0f,  // bottom left
		-0.5f,  0.5f, 0.0f   // top left 
	};

	/** Setup indices for our Element Buffer Objects */
	unsigned int indices[] = {
		0, 1, 3, // First Triangle
		1, 2, 3	 // Second Triangle
	};

	/************************************************************************/
	/* Vertex Array Object, Vertex Buffer Objects, & Element Buffer Objects */
	/************************************************************************/

	/** 1. Tell OpenGL how many VAO's, VBO's, and EBO's to generate */
	// glGenBuffers(<How many to generate>, <what we are generating>) -> only generating 1 VBO and 1 EBO
	// glGenVertexArrays(<How many to generate>, <what we are generating>) -> only generating 1 VAO
	unsigned int VBO, VAO, EBO;
	glGenVertexArrays(1, &VAO);
	glGenBuffers(1, &VBO);
	glGenBuffers(1, &EBO);
	
	/** 1. bind the Vertex Array Object first, then bind and set VBO's and EBO's, and then configure vertex attributes(s). */
	glBindVertexArray(VAO);
	
	/** 2. then copy our vertices array in a VBO for OpenGL to use */
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
	
	/** 3. copy our index array into our EBO for OpenGL to use */
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

	/** 4. Tell OpenGL how it should interpret the vertex data per attribute */
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);

	/** 5. (optional) unbind VBO: note that this is allowed, the call to glVertexAttribPointer registered VBO as the vertex attribute's bound vertex buffer object so afterwards we can safely unbind */
	glBindBuffer(GL_ARRAY_BUFFER, 0);

	/** 6. (optional) You can unbind the VAO afterwards so other VAO calls won't accidentally modify this VAO, but this rarely happens. Modifying other 
	* VAOs requires a call to glBindVertexArray anyways so we generally don't unbind VAOs (nor VBOs) when it's not directly necessary. */
	glBindVertexArray(0);

	/************************************************************************/
	/*						  Set Key Callback Function						*/
	/************************************************************************/
	glfwSetKeyCallback(window, keyCallback);

	/** Start the render loop - each iteration is called a 'frame' */
	while (!glfwWindowShouldClose(window))
	{
		/** Render Commands */
		// Sets the macro GL_COLOR_BUFFER_BIT to a turquoise
		glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
		// Clear the screen and redraw using GL_COLOR_BUFFER_BIT value
		glClear(GL_COLOR_BUFFER_BIT);

		// draw our first triangle
		glUseProgram(shaderProgram);
		glBindVertexArray(VAO); // seeing as we only have a single VAO there's no need to bind it every time, but we'll do so to keep things a bit more organized
		
		// Draw a triangle with our VBO's
		//glDrawArrays(GL_TRIANGLES, 0, 3);

		// Draw triangles with our EBO which is stored in our VAO
		glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);

		/** Swap Buffers:
		*	1. Back Buffer = Rendering Calculations Balogne
		*	2. Front Buffer = Visual representation of render after calculations are completed (what you see on da screen)
		*	Purpose: Prevents artifacts/flickering b/c rendering happens in the back buffer before it is shown to the user
		*/
		glfwSwapBuffers(window);
		
		/** Poll IO events(keys pressed / released, mouse moved etc.) */
		glfwPollEvents();
	}

	// optional: de-allocate all resources once they've outlived their purpose:
	// ------------------------------------------------------------------------
	glDeleteVertexArrays(1, &VAO);
	glDeleteBuffers(1, &VBO);
	glDeleteBuffers(1, &EBO);
	glDeleteProgram(shaderProgram);

	// glfw: terminate, clearing all previously allocated GLFW resources.
	// ------------------------------------------------------------------
	glfwTerminate();


	return 0;
}

/** Toggles wireframe mode on/off */
void toggleWireframeMode()
{
	bWireframeModeOn = !bWireframeModeOn;
	if (bWireframeModeOn)
	{
		glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
	}
	else
	{
		glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
	}
}

/** 
* Callback function that resets the viewport whenever the window is resized 
*/
void framebuffer_size_callback(GLFWwindow* window, int width, int height)
{
	glViewport(0, 0, width, height);
}

/** 
* Checks for user input (keyboard, mouse, controller events).
*/
void keyCallback(GLFWwindow* window, int key, int scancode, int action, int mods)
{
	// Escape - close window
	if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS)
	{
		glfwSetWindowShouldClose(window, true);
	}
	// 'w' - toggle wireframe mode
	if (key == GLFW_KEY_W && action == GLFW_PRESS)
	{
		toggleWireframeMode();
	}
}