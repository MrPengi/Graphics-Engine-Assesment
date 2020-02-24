#include <fstream>
#include <sstream>
#include <ctime>
#include <iostream>
#include "glm.hpp"
#include "ext.hpp"
#include "gl_core_4_5.h"
#include "glfw3.h"
#include "Camera.h"
#include "OBJMesh.h"
#define STB_IMAGE_IMPLEMENTATION
#include "..\Dependencies\stb_image.h"

using uint = unsigned int;



int main()
{
	//og
	/*struct Vertex
	{
		glm::vec3 position;
		glm::vec2 uv;
	};*/

	struct Light
	{
		glm::vec3 direction;
		glm::vec3 diffuse;
		glm::vec3 specular;
		glm::vec3 ambient;
	};

	//changed for lighing tutorial
	struct Vertex
	{
		glm::vec3 position;
		glm::vec4 normal;
		glm::vec2 uv;
	}; 


	srand(time(nullptr));
	//initalise everything

	//if a new window does not open close the program
	if (glfwInit() == false)
	{
		return -1;
	}

	//create a new window
	GLFWwindow* NewWindow = glfwCreateWindow(1280, 720, "Display Window", nullptr, nullptr);


	//if NewWindow is null somehow terminate the window and close the program
	if (NewWindow == nullptr)
	{
		glfwTerminate();
		return -2;
	}

	glfwMakeContextCurrent(NewWindow);

	//if loading the openGL fails destroy the window, terminate glfw and terminate the program 
	if (ogl_LoadFunctions() == ogl_LOAD_FAILED)
	{
		glfwDestroyWindow(NewWindow);
		glfwTerminate();
		return -3;
	}
	
	//print  gl data to terminal
	auto major = ogl_GetMajorVersion();
	auto minor = ogl_GetMinorVersion();
	printf("GL: %i.%i\n", major, minor);

	//mesh data (quads)
	/*glm::vec3 verticies[] =
	{
		glm::vec3(-0.5f, 0.5f, 0.0f),
		glm::vec3(0.5f, 0.5f, 0.0f),
		glm::vec3(-0.5f, -0.5f, 0.0f), 
		glm::vec3(0.5f, -0.5f, 0.0f),
		

	};
	int verticiesCount = 6;*/

	//aie::OBJMesh soulSpearMesh;
	//soulSpearMesh.load("..\\Models\\soulspear.obj", false, false);

	aie::OBJMesh TrooperMesh;
	TrooperMesh.load("..\\Models\\Alien_Small.obj", false, false);

	float one = rand() % 5;
	float two = rand() % 5;
	float three = rand() % 5;
	float four = rand() % 5;
	
	//float textureCoords[] =
	//{    //texture coords
		//	1.0f, 1.0f,
			//0.0f, 0.0f,
			//0.0f, 1.0f,
			//1.0f, 0.0f,
	//};

	//Vertex verticies[] =
	//{
	//	{glm::vec3(-0.5f, 0.5f, 0.0f), glm::vec2(1.0f,0.0f)},
	//	{glm::vec3(-0.5f, -0.5f, 0.0f),   glm::vec2(0.0f,0.0f)},
	//	{glm::vec3(0.5f, 0.5f, 0.0f),  glm::vec2(1.0f,1.0f) }, //top right
	//	{glm::vec3(0.5f, -0.5f, 0.0f), glm::vec2(0.0f,1.0f) },//bottom right
	//
	//	//{glm::vec3(0.5f, 0.5f, 0.0f), glm::vec2(1.0f,1.0f)},
	//	//{glm::vec3(-0.5f, -0.5f, 0.0f),  glm::vec2(0.0f,1.0f)},//3 (base left)
	//	
	//	//{glm::vec3(-0.5f, 0.5f, 0.0f),glm::vec2(0.0f,0.5f) },
	//	//{glm::vec3(0.0f, 0.5f, 0.5f),glm::vec2(0.5f,1.0f) },
	//	//{glm::vec3(0.5f, 0.5f, 0.0f), glm::vec2(0.5f,0.0f) },//9 (top side)
	//	//
	//	//{glm::vec3(0.5f, -0.5f, 0.0f),glm::vec2(1.0f,0.0f)},
	//	//{glm::vec3(0.0f, -0.5f, 0.5f),glm::vec2(0.0f,1.0f)},
	//	//{glm::vec3(-0.5f, -0.5f, 0.0f), glm::vec2(0.3f,0.0f)},//12 (bottom side)
	//	//
	//	//{glm::vec3(-0.5f, 0.5f, 0.0f), glm::vec2(1.0f,0.0f)},
	//	//{glm::vec3(0.0f, 0.5f, 0.5f),glm::vec2(1.0f,0.3f)},
	//	//{glm::vec3(0.5f, -0.5f, 0.0f), glm::vec2(0.0f,0.0f)},//15 (top to bottom right)
	//	//
	//	//{glm::vec3(0.5f, -0.5f, 0.0f),glm::vec2(1.0f,0.0f)},
	//	//{glm::vec3(0.5f, 0.5f, 0.0f), glm::vec2(1.0f,1.0f)},
	//	//{glm::vec3(0.0f, 0.5f, 0.5f), glm::vec2(0.0f,0.0f)},//18 (top to bottom left)
	//	//
	//	//{glm::vec3(-0.5f, -0.5f, 0.0f),glm::vec2(0.0f,1.0f)},
	//	//{glm::vec3(0.0f, -0.5f, 0.5f),glm::vec2(0.0f,0.0f)},
	//	//{glm::vec3(0.0f, 0.5f, 0.5f), glm::vec2(1.0f,0.0f)},//21 (top to bottom left)
	//	//
	//	//{glm::vec3(0.5f, -0.5f, 0.0f),glm::vec2(1.0f,0.0f)},
	//	//{glm::vec3(0.0f, 0.5f, 0.5f),glm::vec2(0.0f,1.0f)},
	//	//{glm::vec3(0.0f, -0.5f, 0.5f), glm::vec2(0.0f,0.0f)}//24 (top to bottom left)
	//
	//};

	Vertex verticies[] =
	{
		{glm::vec3(-0.5f, 0.5f, 0.0f), glm::vec4(0.0f,0.0f, 0.0f, 1.0f), glm::vec2(1.0f,0.0f)},
		{glm::vec3(-0.5f, -0.5f, 0.0f),   glm::vec4(0.0f,0.0f, 0.0f, 1.0f), glm::vec2(0.0f,0.0f)},
		{glm::vec3(0.5f, 0.5f, 0.0f),  glm::vec4(0.0f,0.0f, 0.0f, 1.0f), glm::vec2(1.0f,1.0f) }, //top right
		{glm::vec3(0.5f, -0.5f, 0.0f), glm::vec4(0.0f,0.0f, 0.0f, 1.0f), glm::vec2(0.0f,1.0f) },//bottom right
	};
	int verticiesCount = 24; //24
	
	//create and "load" mesh
	uint VAO;
	uint VBO;
	uint IBO;

	int index_buffer[]{ 0,2,1,3,1,2 };

	glGenVertexArrays(1, &VAO);
	glGenBuffers(1, &VBO);
	glGenBuffers(1, &IBO);

	
	
	
	glBindVertexArray(VAO);
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, verticiesCount * sizeof(Vertex), &verticies[0], GL_STATIC_DRAW);

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, IBO);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, 6 * sizeof(int), index_buffer, GL_STATIC_DRAW);

	
	//glBindVertexArray(0);
	//glBindBuffer(GL_ARRAY_BUFFER, 0);
	


	
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), 0);
	
	//useage as lighting (tutorial)
	glEnableVertexAttribArray(1);
	glVertexAttribPointer(1, 4, GL_FLOAT, GL_TRUE, sizeof(Vertex), (void*)16);

	//useage as texture
	glEnableVertexAttribArray(2);
	glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)(3 * sizeof(float)));


	//setup of light
	Light myLight =
	{
		(glm::vec3(0.0f,0.0f,1.0f)), (glm::vec3(0.0f,1.0f,0.0f)), (glm::vec3(0.3f,0.5f,0.0f)), (glm::vec3(0.25f, 0.25f, 1.0f))
	};


	//texture code
	uint testTexture;
	int x, y, n;
	unsigned char* data = stbi_load("../Textures/UV_Grid_Sm.jpg", &x, &y, &n, 0); //slice.jpg Alien_Albedo.png UV_Grid_Sm.jpg

	glGenTextures(1, &testTexture);
	glBindTexture(GL_TEXTURE_2D, testTexture);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, x, y, 0, GL_RGB, GL_UNSIGNED_BYTE, data);

	//check if the texture loaded
	if (data)
	{
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, x, y, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
		//glGenerateMipmap(GL_TEXTURE_2D);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	}
	else
	{
		printf("loading texture failed");
	}

	stbi_image_free(data);

	//CAMERA STUFFS

	//setup an instance of the camera
	Camera myCamera;

	//set the camera's initial perspective and view direction
	myCamera.SetPerspective(1.507f, 16 / 9.0f, 0.0f, 7.0f);
	myCamera.SetLookAt(glm::vec3(0, 0, -1), glm::vec3(0), glm::vec3(0, 1, 0));

	//glm::mat4 projection = glm::perspective(1.507f, 16 / 9.0f, 0.0f, 7.0f);
	//glm::mat4 view = glm::lookAt(glm::vec3(0, 0, 1), glm::vec3(0), glm::vec3(0, 1, 0));
	glm::mat4 model = glm::mat4(1);

	

	//SHADERS ***
	uint vertex_shader_ID = 0;
	uint fragment_shader_ID = 0;
	uint shader_program_ID = 0;

	//load shader from file to string
	std::string shader_data;
	//std::ifstream in_file_stream("..\\Shaders\\simple_vertex.glsl", std::ifstream::in);
	std::ifstream in_file_stream("..\\Shaders\\phong.vert", std::ifstream::in);

	//load the source into a string for complication
	std::stringstream string_stream;
	if (in_file_stream.is_open())
	{
		string_stream << in_file_stream.rdbuf();
		shader_data = string_stream.str();
		in_file_stream.close();
	}

	//allocate for shader program
	vertex_shader_ID = glCreateShader(GL_VERTEX_SHADER);
	//convert to raw cha*
	const char* dataOfShader = shader_data.c_str();
	//send in shader location
	glShaderSource(vertex_shader_ID, 1, (const GLchar**)&dataOfShader, 0);
	//build
	glCompileShader(vertex_shader_ID);

	//did it work
	GLint success = GL_FALSE;
	glGetShaderiv(vertex_shader_ID, GL_COMPILE_STATUS, &success);
	if (success == GL_FALSE)
	{
		printf("Vertex shader failed");
	}

	//frag shader ROUND 2
	//std::ifstream in_file_stream_frag("..\\Shaders\\simple_frag.glsl", std::ifstream::in); //or simple frag2 (for texture)
	std::ifstream in_file_stream_frag("..\\Shaders\\phong.frag", std::ifstream::in);

	//load the source into a string for complication
	std::stringstream frag_string_stream;
	if (in_file_stream_frag.is_open())
	{
		frag_string_stream << in_file_stream_frag.rdbuf();
		shader_data = frag_string_stream.str();
		in_file_stream_frag.close();
	}

	//allocate for shader program
	fragment_shader_ID = glCreateShader(GL_FRAGMENT_SHADER);
	//convert to raw cha*
	dataOfShader = shader_data.c_str();
	//send in shader location
	glShaderSource(fragment_shader_ID, 1, (const GLchar**)&dataOfShader, 0);
	//build
	glCompileShader(fragment_shader_ID);

	success = GL_FALSE;
	glGetShaderiv(fragment_shader_ID, GL_COMPILE_STATUS, &success);
	if (success == GL_FALSE)
	{
		printf("Fragment shader failed");
	}


	//link shaders
	shader_program_ID = glCreateProgram();

	glAttachShader(shader_program_ID, vertex_shader_ID);
	glAttachShader(shader_program_ID, fragment_shader_ID);

	glLinkProgram(shader_program_ID);

	success = GL_FALSE;
	glGetProgramiv(shader_program_ID, GL_LINK_STATUS, &success);
	if (!success)
	{
		printf("linking failed\n");

		GLint logLength = 0;
		glGetShaderiv(shader_program_ID, GL_INFO_LOG_LENGTH, &logLength);

		char* log = new char[logLength];

		glGetShaderInfoLog(shader_program_ID, logLength, 0, log);

		std::string errorMessage(log);
		errorMessage += "SHADER_FAILED_TO_COMPILE";
		printf(errorMessage.c_str());
		delete[] log;
	}


	/*for (int i = 0; i < 6; ++i)
	{
		verticies[i] = (glm::vec3)(pvm * glm::vec4(verticies[i], 1));
	}*/

	//background colour (blue
	//glClearColor(0.3, 0.5, 1.0, 1.0);
	glClearColor(1, 1, 1, 1);

	//bool colourChange = false;

	float iterator = 0;
	float timer = 1;

	float deltaTime = 0;
	float currentFrame = 0;
	float previousFrame = 0;
	//bool press = true;

	//make the shape wireframe
	//glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);

	//glPolygonMode(GL_BACK, GL_LINE);

	//glEnable(GL_CULL_FACE);
	//glCullFace(GL_BACK);
	//glEnable(GL_DEPTH_TEST);
	//glFrontFace(GL_CW);

	//game loop
	while (glfwWindowShouldClose(NewWindow) == false && glfwGetKey(NewWindow, GLFW_KEY_ESCAPE) != GLFW_PRESS)
	{
		//get deltatime
		currentFrame = glfwGetTime();
		deltaTime = currentFrame - previousFrame;
		previousFrame = currentFrame;

		/*for (int i = 0; i < sizeof(verticies); i++)
		{
		//amazing lag machine
			std::cout << verticies[i].x << " " << verticies[i].y << verticies[i].z << " " << verticies[i].z << " \n";
		}*/

		//pink
		//glm::vec4 color = glm::vec4(1.0f, 0.3f, 0.7f, 0.0f);

		myLight.direction = glm::normalize(glm::vec3(glm::cos(deltaTime * 300), glm::sin(deltaTime * 300), 0));

		//myLight.diffuse = glm::vec3(one, two, three);

		//random color
		glm::vec4 color = glm::vec4(one * 0.2f, two * 0.2f, three * 0.2f, 1.0f);

		//glm::vec4 color = glm::vec4(1);

		//update the camera
		myCamera.Update(deltaTime, NewWindow);

		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		
		//glm::mat4 pvm = myCamera.projection * myCamera.ViewTransform * model;
		//myCamera.UpdateProjectionViewTransform(model);



		//if (glfwGetKey(NewWindow, GLFW_KEY_SPACE) == GLFW_PRESS)
		//{
		//	press != press;
		//}

		//if (press)
		//{
			//model = glm::rotate(model, 0.016f, glm::vec3(1, 1, 1));
		//}

		/*if (!press)
		{
			model = glm::rotate(model, 0.016f, glm::vec3(-1, -1, -1));
		}*/
		TrooperMesh.draw();
		
		//soulSpearMesh.draw();
		
		auto pvm = myCamera.ProjectionTransform * myCamera.ViewTransform * model;
		glUseProgram(shader_program_ID);
		auto uniform_location = glGetUniformLocation(shader_program_ID, "projection_view_matrix");
		glUniformMatrix4fv(uniform_location, 1, false, glm::value_ptr(pvm));
		
		uniform_location = glGetUniformLocation(shader_program_ID, "NormalMatrix");
		glUniformMatrix4fv(uniform_location, 1, false, glm::value_ptr(glm::mat3(glm::inverseTranspose(myCamera.WorldTransform))));


		uniform_location = glGetUniformLocation(shader_program_ID, "Ia");
		glUniform3fv(uniform_location, 1, glm::value_ptr(myLight.ambient));

		uniform_location = glGetUniformLocation(shader_program_ID, "Id");
		glUniform3fv(uniform_location, 1, glm::value_ptr(myLight.diffuse));

		uniform_location = glGetUniformLocation(shader_program_ID, "Is");
		glUniform3fv(uniform_location, 1, glm::value_ptr(myLight.specular));


		uniform_location = glGetUniformLocation(shader_program_ID, "LightDirection");
		glUniform3fv(uniform_location, 1, glm::value_ptr(myLight.direction));

		//uniform_location = glGetUniformLocation(shader_program_ID, "model_matrix");
		//glUniformMatrix4fv(uniform_location, 1, false, glm::value_ptr(model));

		uniform_location = glGetUniformLocation(shader_program_ID, "color");
		glUniform4fv(uniform_location, 1, glm::value_ptr(color));

		


		 if (iterator > timer)
			{
				one = rand() % 5;
				two = rand() % 5;
				three = rand() % 5;
				four = rand() % 5;
				iterator = timer - iterator;
			}

			iterator += 1 * deltaTime;
		int vertexColorLocation = glGetUniformLocation(shader_program_ID, "color");
		glUniform4f(vertexColorLocation, two * 0.2f, 0.3f, one * 0.1f, 1.0f);

		glBindTexture(GL_TEXTURE_2D, testTexture);

		glBindVertexArray(VAO);
		//glDrawArrays(GL_TRIANGLES, 0, verticiesCount); //six becomes whatever veritcies are above (uncomment when drawing shapes again)

		glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);



		
		

		glfwSwapBuffers(NewWindow);
		glfwPollEvents();
	}


	//destroy the window, terminate glfw and close the program
	glfwDestroyWindow(NewWindow);
	glfwTerminate();
	return 0;
}

//triangular prism
//glm::vec3 verticies[] =
//{
//	glm::vec3(-0.5f, 0.5f, 0.0f),
//	glm::vec3(0.5f, 0.5f, 0.0f),
//	glm::vec3(-0.5f, -0.5f, 0.0f), //3 (base left)
//
//	glm::vec3(0.5f, 0.5f, 0.0f),
//	glm::vec3(-0.5f, -0.5f, 0.0f),
//	glm::vec3(0.5f, -0.5f, 0.0f), //6 (base right)
//
//	glm::vec3(-0.5f, 0.5f, 0.0f),
//	glm::vec3(0.0f, 0.5f, 0.5f),
//	glm::vec3(0.5f, 0.5f, 0.0f), //9 (top side)
//
//	glm::vec3(0.5f, -0.5f, 0.0f),
//	glm::vec3(0.0f, -0.5f, 0.5f),
//	glm::vec3(-0.5f, -0.5f, 0.0f), //12 (bottom side)
//
//	glm::vec3(0.5f, -0.5f, 0.0f),
//	glm::vec3(0.0f, 0.5f, 0.5f),
//	glm::vec3(-0.5f, 0.5f, 0.0f), //15 (top to bottom right)
//
//	glm::vec3(0.5f, 0.5f, 0.0f),
//	glm::vec3(-0.5f, -0.5f, 0.0f),
//	glm::vec3(0.0f, 0.5f, 0.5f), //18 (top to bottom left)
//
//	glm::vec3(-0.5f, -0.5f, 0.0f),
//	glm::vec3(0.0f, 0.5f, 0.5f),
//	glm::vec3(0.0f, -0.5f, 0.5f), //21 (top to bottom left)
//
//	glm::vec3(0.5f, -0.5f, 0.0f),
//	glm::vec3(0.0f, 0.5f, 0.5f),
//	glm::vec3(0.0f, -0.5f, 0.5f) //24 (top to bottom left)
//
//};
//int verticiesCount = 24;

//RANDOM(ISH) verticies for wack shapes (sometimes perfect triangles other times big cluster messes)
//float one = rand() % 5;
//float two = rand() % 5;
//float three = rand() % 5;
//float four = rand() % 5;
//
//glm::vec3 verticies[] =
//{
//	glm::vec3(one, two, three),
//	glm::vec3(two, one, four),
//	glm::vec3(two, three, one), //3 (base left)
//
//	glm::vec3(two, three, one),
//	glm::vec3(four, one, three),
//	glm::vec3(one, four, two), //6 (base right)
//
//	glm::vec3(four, one, three),
//	glm::vec3(three, one, two),
//	glm::vec3(four, one, two), //9 (top side)
//
//	glm::vec3(one, four, two),
//	glm::vec3(four, two, one),
//	glm::vec3(two, one, three), //12 (bottom side)
//
//	glm::vec3(three, one, two),
//	glm::vec3(four, three, one),
//	glm::vec3(one, two, three), //15 (top to bottom right)
//
//	glm::vec3(one, three, four),
//	glm::vec3(two, one, four),
//	glm::vec3(four, two, one), //18 (top to bottom left)
//
//	glm::vec3(one, four, two),
//	glm::vec3(one, three, two),
//	glm::vec3(four, one, two), //21 (top to bottom left)
//
//	glm::vec3(four, two, three),
//	glm::vec3(two, four, three),
//	glm::vec3(three, two, four) //24 (top to bottom left)
//
//};
//int verticiesCount = 24;

//testing changing the shape per frame
//if (iterator > timer)
//{
//	one = rand() % 5;
//	two = rand() % 5;
//	three = rand() % 5;
//	four = rand() % 5;
//
//	glm::vec3 verticies[] =
//	{
//		glm::vec3(one, two, three),
//		glm::vec3(two, one, four),
//		glm::vec3(two, three, one), //3 (base left)
//
//		glm::vec3(two, three, one),
//		glm::vec3(four, one, three),
//		glm::vec3(one, four, two), //6 (base right)
//
//		glm::vec3(four, one, three),
//		glm::vec3(three, one, two),
//		glm::vec3(four, one, two), //9 (top side)
//
//		glm::vec3(one, four, two),
//		glm::vec3(four, two, one),
//		glm::vec3(two, one, three), //12 (bottom side)
//
//		glm::vec3(three, one, two),
//		glm::vec3(four, three, one),
//		glm::vec3(one, two, three), //15 (top to bottom right)
//
//		glm::vec3(one, three, four),
//		glm::vec3(two, one, four),
//		glm::vec3(four, two, one), //18 (top to bottom left)
//
//		glm::vec3(one, four, two),
//		glm::vec3(one, three, two),
//		glm::vec3(four, one, two), //21 (top to bottom left)
//
//		glm::vec3(four, two, three),
//		glm::vec3(two, four, three),
//		glm::vec3(three, two, four) //24 (top to bottom left)
//
//	};
//
//	//color = glm::vec4(one * 0.1f, two * 0.1f, three * 0.1f, 0.0f);
//
//	glBindVertexArray(VAO);
//	glBindBuffer(GL_ARRAY_BUFFER, VBO);
//	glBufferData(GL_ARRAY_BUFFER, verticiesCount * sizeof(glm::vec3), &verticies[0], GL_STATIC_DRAW);
//
//	iterator = timer - iterator;
//}
//iterator += 1 * deltaTime;

//random colour code
/*if (!colourChange)
		{

			random colour
			one = rand() % 100;
			 two = rand() % 100;
			 three = rand() % 100;
			 four = rand() % 100;
			 //colourChange = true;
			//glm::vec4 color = glm::vec4((0.1f * one), (0.1f * two), (0.1f * three), (0.1f * four));

			if (random == 1)
			{
				one = one + 0.1f;
			}
			if (random == 2)
			{
				two = two + 0.1f;
			}
			if (random == 3)
			{
				three = three + 0.1f;
			}
			if (random == 4)
			{
				four = four + 0.1f;
			}
			if (random == 5)
			{
				one = one - 0.1f;
			}
			if (random == 6)
			{
				two = two - 0.1f;
			}
			if (random == 7)
			{
				three = three - 0.1f;
			}
			if (random == 8)
			{
				four = four - 0.1f;
			}
			glm::vec4 color = glm::vec4(one, two, three, four);
		}*/

		/*if (colourChange)
		{
			iterator = iterator + 1 * ;
			if (iterator < 600000000)
			{
				iterator = iterator - 600000000;
				colourChange = false;
			}
		}*/

		//glm::vec4 color = glm::vec4((0.1f * one), (0.1f * two), (0.1f * three), (0.1f * four));