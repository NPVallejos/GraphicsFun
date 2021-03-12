# Graphics Pipeline Explained

1. The graphics pipeline takes as input a set of 3D coordinates.

2. It transforms these to colored 2D pixels on your screen.

3. Each step in the pipeline take the output of the previous step as input.

4. Each step in the graphics pipeline is highly specialized (they have one function) and thus can be easily executed in parallel.

5. Because of their parallel nature, graphics cards have thousands of small processing cores to quickly process data within the pipeline.

6. Each processing core run small programs on the GPU for each step of the pipeline.

7. These small programs are called **shaders**.
---
# What are Shaders?

1. Small programs that are run on the GPU
2. Written in the **OpenGL Shading Language (GLSL)**
3. For DirectX, they are written in **High Level Shader Language (HLSL)**
---
# Stages of the Pipeline in terms of Shaders (Editable shaders are in blue)

![Graphics Pipeline Image](/pipeline.png)

1. Vertex Shader:
	- Takes as input a single **vertex** - a collection of data per 3D coordinate
	- Vertex data is represented using **vertex attributes** that can contain 3D position and color value
	- Transform 3D coordinates into different 3D coordinates
2. Primitive Assembly:
	- Takes as input all the vertices from the vertex shader
	- Forms a primitive shape; a triangle in this case
3. Geometry Shader:
	- Takes as input a collection of vertices that for a primitive
	- Has the ability to generate other shapes by emitting new vertices to form new primitives
	- In this case it forms two new triangles using 1 new vertex
4. Rasterization:
	- Maps the resulting primitives to the corresponding pixels on the final screen
	- Results in **fragments** for the next shader
	- **Clipping** is performed which discards all fragments that are outside your view = increases performance
5. Fragment Shader:
	- A **Fragment** is all the data required for OpenGL to render a single pixel
	- Calculates the final color of a pixel
	- **This is the stage where all the advanced OpenGL effects occur**
	- This shader contains data about the 3D scene that it can use to calculate the final pixel color **(like lights, shadows, color of the light, etc.)**
6. Tests and Blending:
	- Checks the corresponding depth (and stencil) value of the fragment and uses those to check if the resulting fragment is in front or behind other objects and should be discarded accordingly
	- Also checks for **alpha** values (opacity value) and **blends** the objects accordingly
	- Thus **final pixel color** is determined here
---
# Vertex Shader


#### Vertex Buffer Object = VBO
![Vertex Buffer Object](/vertex_attribute_pointer.png)

1. Coordinate Value = **vertex attribute**
2. Vertex Attribute = **4 bytes** = **sizeof(float)**
3. This is how you tell OpenGL to interpret vertex data:
	```
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);
	```

#### Element Buffer Object = EBO
1. Holds our indices array
```
float vertices[] = {
     0.5f,  0.5f, 0.0f,  // top right
     0.5f, -0.5f, 0.0f,  // bottom right
    -0.5f, -0.5f, 0.0f,  // bottom left
    -0.5f,  0.5f, 0.0f   // top left
};
unsigned int indices[] = {  // note that we start from 0!
    0, 1, 3,   // first triangle
    1, 2, 3    // second triangle
};
```
2. The indices array basically tells OpenGL which vertexes to draw. Here we only use three vertexes to draw two triangles. EBO's are great because they allow us to reuse vertexes to make shapes.

#### Vertex Array Object = VAO
![Vertex Array Object](/vertex_array_objects_ebo.png)
1. VAO's contain our VBO's and the last bound EBO
2. This is how you use VBO's:
```
	unsigned int VAO;
	glGenVertexArrays(1, &VAO);

	// Bind the Vertex Array Object first, then bind and set VBO's and EBO's, and then configure vertex attributes(s)
	// Each bound VBO is stored in our VAO
	// The last bound EBO is stored in our VAO 
	glBindVertexArray(VAO);
```
