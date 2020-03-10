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

	//light stores direction, diffuse, specular, ambient
	struct Light
	{
		glm::vec3 direction;
		glm::vec3 diffuse;
		glm::vec3 specular;
		glm::vec3 ambient;
	};

	//vertex stores position, normal, uv, tangent
	struct Vertex
	{
		glm::vec3 position;
		glm::vec4 normal;
		glm::vec2 uv;
		glm::vec4 tangent;
	}; 

	//random seed
	srand(time(nullptr));
	

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

	//mesh data (quads) - old
	/*glm::vec3 verticies[] =
	{
		glm::vec3(-0.5f, 0.5f, 0.0f),
		glm::vec3(0.5f, 0.5f, 0.0f),
		glm::vec3(-0.5f, -0.5f, 0.0f), 
		glm::vec3(0.5f, -0.5f, 0.0f),
		

	};
	int verticiesCount = 6;*/

	//aie::OBJMesh soulSpearMesh; //imported into project way before adam said not to... goodbye soulSpear, you will not be missed
	//soulSpearMesh.load("..\\Models\\soulspear.obj", false, false);

	//creation of trooper obj
	aie::OBJMesh TrooperMesh;
	TrooperMesh.load("..\\Models\\Alien_Medium.obj", false, false);

	//creation of wall corner obj
	aie::OBJMesh WallMesh;
	WallMesh.load("..\\Models\\Wall_Corner.obj", false, false);
	
	//creation of floor obj
	aie::OBJMesh FloorMesh;
	FloorMesh.load("..\\Models\\Floor_Full.obj", false, false);

	//creation of generator obj
	aie::OBJMesh GeneratorMesh;
	GeneratorMesh.load("..\\Models\\Cold_Generator.obj", false, false);

	//random floats used in update
	float randomNumberOne = rand() % 5;
	float randomNumberTwo = rand() % 5;
	float randomNumberThree = rand() % 5;
	float randomNumberFour = rand() % 5;
	
	//float textureCoords[] =
	//{    //texture coords (old)
		//	1.0f, 1.0f,
			//0.0f, 0.0f,
			//0.0f, 1.0f,
			//1.0f, 0.0f,
	//};

	//verticies of a square
	Vertex verticies[] =
	{//POSITION						//NORMAL							//TEXTURE COORDS
		{glm::vec3(-0.5f, 0.5f, 0.0f), glm::vec4(0.0f,0.0f, -1.0f, 0.0f), glm::vec2(1.0f,0.0f), glm::vec4(0.0f,0.0f, 1.0f, 0.0f)},
		{glm::vec3(-0.5f, -0.5f, 0.0f),   glm::vec4(0.0f,0.0f, -1.0f, 0.0f), glm::vec2(0.0f,0.0f), glm::vec4(0.0f,0.0f, 1.0f, 0.0f)},
		{glm::vec3(0.5f, 0.5f, 0.0f),  glm::vec4(0.0f,0.0f, -1.0f, 0.0f), glm::vec2(1.0f,1.0f), glm::vec4(0.0f,0.0f, 1.0f, 0.0f) }, //top right
		{glm::vec3(0.5f, -0.5f, 0.0f), glm::vec4(0.0f,0.0f, -1.0f, 0.0f), glm::vec2(0.0f,1.0f), glm::vec4(0.0f,0.0f, 1.0f, 0.0f) },//bottom right
	};
	int verticiesCount = 24; //24
	
	//create and "load" mesh
	uint VAO;
	uint VBO;
	uint IBO;

	//creation of index buffer
	int index_buffer[]{ 0,2,1,3,1,2 };

	//setup VAO, VBO and IBO
	glGenVertexArrays(1, &VAO);
	glGenBuffers(1, &VBO);
	glGenBuffers(1, &IBO);

	//setup VBO
	glBindVertexArray(VAO);
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, verticiesCount * sizeof(Vertex), &verticies[0], GL_STATIC_DRAW);

	//setup IBO
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, IBO);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, 6 * sizeof(int), index_buffer, GL_STATIC_DRAW);

	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), 0);
	
	//useage as lighting (tutorial)
	glEnableVertexAttribArray(1);
	glVertexAttribPointer(1, 4, GL_FLOAT, GL_TRUE, sizeof(Vertex), (void*)16);

	//useage as texture
	glEnableVertexAttribArray(2);
	glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)(3 * sizeof(float)));

	//useage as tangent
	glEnableVertexAttribArray(3);
	glVertexAttribPointer(3, 4, GL_FLOAT, GL_TRUE, sizeof(Vertex), (void*)16);

	//setup of light
	Light myLight =
	{
		//direction					//diffuse						//specular					//ambient
		(glm::vec3(0.0f,1.0f,0.0f)), (glm::vec3(0.9f, 0.3f, 0.3f)), (glm::vec3(0.75f,0.75f,0.75f)),(glm::vec3(0.1f, 0.0f, 0.2f))
	};

	// setup of my second light
		Light mySecondLight =
	{
		//direction					//diffuse						//specular					//ambient
		(glm::vec3(0.0f, 0.0f, 1.0f)), (glm::vec3(0.2f, 0.9f, 0.4f)), (glm::vec3(0.2f,0.2f,0.4f)),(glm::vec3(0.1f,0.0f,0.2f))
	};

	//texture code (texture code for alien texture)
	uint alienTexture;
	int x, y, n;
	//load alien texture
	unsigned char* data = stbi_load("../Textures/Alien_Albedo.png", &x, &y, &n, 0); //slice.jpg Alien_Albedo.png UV_Grid_Sm.jpg

	//bind texture to unsigned int alienTexture
	glGenTextures(1, &alienTexture);
	glBindTexture(GL_TEXTURE_2D, alienTexture);
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
		printf("loading Alien texture failed");
	}
	//free up data
	stbi_image_free(data);

	//texture code (texture code for wall piece)
	uint wallTexture;
	//load wall corner texture
	unsigned char* data2 = stbi_load("../Textures/Wall_Window_Corner_Albedo.png", &x, &y, &n, 0); //slice.jpg Alien_Albedo.png UV_Grid_Sm.jpg

	//bind texture to unsigned int wallTexture
	glGenTextures(1, &wallTexture);
	glBindTexture(GL_TEXTURE_2D, wallTexture);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, x, y, 0, GL_RGB, GL_UNSIGNED_BYTE, data2);

	//check if the texture loaded
	if (data)
	{
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, x, y, 0, GL_RGB, GL_UNSIGNED_BYTE, data2);
		//glGenerateMipmap(GL_TEXTURE_2D);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	}
	else
	{
		printf("loading wall texture failed");
	}
	//freeup data2
	stbi_image_free(data2);

	//texture code (texture code for floor piece)
	uint floorTexture;
	//load wallFloor texture
	unsigned char* data3 = stbi_load("../Textures/WallFloor.png", &x, &y, &n, 0); //slice.jpg Alien_Albedo.png UV_Grid_Sm.jpg

	//bind texture to unsigned int alienTexture
	glGenTextures(1, &floorTexture);
	glBindTexture(GL_TEXTURE_2D, floorTexture);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, x, y, 0, GL_RGB, GL_UNSIGNED_BYTE, data3);

	//check if the texture loaded
	if (data)
	{
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, x, y, 0, GL_RGB, GL_UNSIGNED_BYTE, data3);
		//glGenerateMipmap(GL_TEXTURE_2D);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	}
	else
	{
		printf("loading floor texture failed");
	}

	//freeup data3
	stbi_image_free(data3);

	//setup the generator texture
	uint generatorTexture;
	//load Generator Texture
	unsigned char* data4 = stbi_load("../Textures/Cold_Generator_Albedo.png", &x, &y, &n, 0); //slice.jpg Alien_Albedo.png UV_Grid_Sm.jpg

	//bind texture to unsigned int generatorTexture
	glGenTextures(1, &generatorTexture);
	glBindTexture(GL_TEXTURE_2D, generatorTexture);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, x, y, 0, GL_RGB, GL_UNSIGNED_BYTE, data4);

	//check if the texture loaded
	if (data)
	{
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, x, y, 0, GL_RGB, GL_UNSIGNED_BYTE, data4);
		//glGenerateMipmap(GL_TEXTURE_2D);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	}
	else
	{
		printf("loading generator texture failed");
	}
	//free up data4
	stbi_image_free(data4);


	//setup for shadow buffer? (i attempted shadows but it didnt work, so i commented it out and left it where it would be)
	/*uint frameBufferObject;
	uint frameBufferObjectDepth;

	glGenFramebuffers(1, &frameBufferObject);
	glBindFramebuffer(GL_FRAMEBUFFER, frameBufferObject);

	glGenTextures(1, &frameBufferObjectDepth);
	glBindTexture(GL_TEXTURE_2D, frameBufferObjectDepth);

	//texture uses a 16 bit depth component format
	glTexImage2D(GL_TEXTURE_2D, 0, GL_DEPTH_COMPONENT16, 1024, 1024, 0, GL_DEPTH_COMPONENT, GL_FLOAT, 0);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);

	//attached as a depth attachment to capture depth not colour
	glFramebufferTexture(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, frameBufferObjectDepth, 0);

	//no colour targets are used
	glDrawBuffer(GL_NONE);

	GLenum status = glCheckFramebufferStatus(GL_FRAMEBUFFER);
	if(status != GL_FRAMEBUFFER_COMPLETE)
	{ 
		printf("Framebuffer Error!\n");
	}
	
	glBindFramebuffer(GL_FRAMEBUFFER, 0);

	//creation of light projection matrix
	glm::mat4 lightProjection = glm::ortho<float>(-10, 10, -10, 10, -10, 10);*/

	//CAMERA STUFFS

	//setup an instance of the camera
	Camera myCamera;

	//set the camera's initial perspective and view direction (and dont have the near plane be 0)
	myCamera.SetPerspective(1.507f, 16 / 9.0f, 1.0f, 10000.0f);
	myCamera.SetLookAt(glm::vec3(-340, 350, 340), glm::vec3(0, 100, 0), glm::vec3(0, 1, 0));

	//create model matrix (and model matrix with opposite rotation)
	glm::mat4 model = glm::mat4(1);
	glm::mat4 modelOppositeRotation = glm::rotate(model, 3.2f, glm::vec3(0, 1, 0));

	

	//SHADERS ***
	uint vertex_shader_ID = 0;
	uint fragment_shader_ID = 0;
	uint shader_program_ID = 0;

	//load shader from file to string
	std::string shader_data;
	//std::ifstream in_file_stream("..\\Shaders\\simple_vertex.glsl", std::ifstream::in);
	std::ifstream in_file_stream("..\\Shaders\\phong.vert", std::ifstream::in); //phong.vert is the name of my most up to date vertex shader

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
	std::ifstream in_file_stream_frag("..\\Shaders\\phong.frag", std::ifstream::in); //phong.frag is the name of my most up to date fragment shader

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

	//background colour (blue)
	//glClearColor(0.3, 0.5, 1.0, 1.0);

	//background colour (white)
	//glClearColor(1, 1, 1, 1);

	//bool colourChange = false;
	//iterator stuffs (used with random numbers)
	float iterator = 0;
	float timer = 1;

	//deltaTime setup
	float deltaTime = 0;
	float currentFrame = 0;
	float previousFrame = 0;
	//bool press = true;

	//make the shape wireframe
	//glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);

	//glPolygonMode(GL_BACK, GL_LINE);

	//glEnable(GL_CULL_FACE);
	//glCullFace(GL_BACK);
	//glEnable(GL_DEPTH_WRITEMASK);
	//glEnable(GL_DEPTH);
	glEnable(GL_DEPTH_TEST);
	//glDepthFunc(GL_LESS);
	//glFrontFace(GL_CW);
	
	

	//game loop
	
	while (glfwWindowShouldClose(NewWindow) == false && glfwGetKey(NewWindow, GLFW_KEY_ESCAPE) != GLFW_PRESS)
	{
		//get deltatime
		currentFrame = glfwGetTime();
		deltaTime = currentFrame - previousFrame;
		previousFrame = currentFrame;

		//shadow code? (i attempted shadows but it didnt work, so i commented it out and left it where it would be)
		//creation of lightview
		/*glm::mat4 lightView = glm::lookAt(myLight.direction, glm::vec3(0), glm::vec3(0, 1, 0));
		glm::mat4 lightView2 = glm::lookAt(mySecondLight.direction, glm::vec3(0), glm::vec3(0, 1, 0));

		glm::mat4 lightMatrix = lightProjection * lightView;
		glm::mat4 lightMatrix2 = lightProjection * lightView2;

		//offset matrix
		glm::mat4 textureSpaceOffSet(
			0.5f, 0.0f, 0.0f, 0.0f,
			0.0f, 0.5f, 0.0f, 0.0f,
			0.0f, 0.0f, 0.5f, 0.0f,
			0.5f, 0.5f, 0.5f, 1.0f
		);

		lightMatrix = lightMatrix * textureSpaceOffSet;
		lightMatrix2 = lightMatrix2 * textureSpaceOffSet;*/

		/*for (int i = 0; i < sizeof(verticies); i++)
		{
		//amazing lag machine
			std::cout << verticies[i].x << " " << verticies[i].y << verticies[i].z << " " << verticies[i].z << " \n";
		}*/

		//pink color
		//glm::vec4 color = glm::vec4(1.0f, 0.3f, 0.7f, 0.0f);
		//empty color
		glm::vec4 color = glm::vec4(0.0f, 0.0f, 0.0f, 0.0f);

		//rotate the second light
		mySecondLight.direction =  glm::normalize(glm::vec3(glm::cos(currentFrame * 1), glm::sin(currentFrame * 1), 0));

		//myLight.diffuse = glm::vec3(one, two, three);

		//random color
		glm::vec4 color2 = glm::vec4(randomNumberOne * 0.2f, randomNumberTwo * 0.2f, randomNumberThree * 0.2f, 1.0f);
		//glm::vec3 tempcolor = glm::vec3(one * 0.2f, two * 0.2f, three * 0.2f);

		//make four new random numbers each time the iterator is greater than the timer
		if (iterator > timer)
		{
			randomNumberOne = rand() % 5;
			randomNumberTwo = rand() % 5;
			randomNumberThree = rand() % 5;
			randomNumberFour = rand() % 5;
			iterator = timer - iterator;
		}

		//glm::vec4 color = glm::vec4(1);

		//update the camera
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		myCamera.Update(deltaTime, NewWindow);
		
		
		//soulSpearMesh.draw();

		//create pvm and assign uniforms
		auto pvm = myCamera.ProjectionTransform * myCamera.ViewTransform /* model*/;
		glUseProgram(shader_program_ID);
		auto uniform_location = glGetUniformLocation(shader_program_ID, "projection_view_matrix");
		glUniformMatrix4fv(uniform_location, 1, false, glm::value_ptr(pvm)); //camera get projection view
		
		uniform_location = glGetUniformLocation(shader_program_ID, "NormalMatrix");
		glUniformMatrix3fv(uniform_location, 1, false, glm::value_ptr(glm::mat3(glm::inverseTranspose(myCamera.WorldTransform))));

		/*uniform_location = glGetUniformLocation(shader_program_ID, "lightMatrix");
		glUniformMatrix4fv(uniform_location, 1, false, glm::value_ptr(glm::mat4(lightMatrix)));

		uniform_location = glGetUniformLocation(shader_program_ID, "lightMatrix2");
		glUniformMatrix4fv(uniform_location, 1, false, glm::value_ptr(glm::mat4(lightMatrix2)));*/

		uniform_location = glGetUniformLocation(shader_program_ID, "Ia");
		glUniform3fv(uniform_location, 1, glm::value_ptr(myLight.ambient));

		uniform_location = glGetUniformLocation(shader_program_ID, "Id");
		glUniform3fv(uniform_location, 1, glm::value_ptr(myLight.diffuse));

		uniform_location = glGetUniformLocation(shader_program_ID, "Is");
		glUniform3fv(uniform_location, 1, glm::value_ptr(myLight.specular));

		//put in Ka, Kd, Ks, specularpower into uniform (custom vec 3s)
		uniform_location = glGetUniformLocation(shader_program_ID, "Ka");
		glUniform3fv(uniform_location, 1, glm::value_ptr(glm::vec3(0.8f, 0.4f, 0.4f)));

		uniform_location = glGetUniformLocation(shader_program_ID, "Kd");
		glUniform3fv(uniform_location, 1, glm::value_ptr(glm::vec3(0.9f, 0.3f, 0.4f)));

		uniform_location = glGetUniformLocation(shader_program_ID, "Ks");
		glUniform3fv(uniform_location, 1, glm::value_ptr(glm::vec3(0.4f, 0.1f, 0.4f)));


		uniform_location = glGetUniformLocation(shader_program_ID, "cameraPosition");
		glUniform3fv(uniform_location, 1, glm::value_ptr(glm::vec3(glm::inverse(myCamera.WorldTransform)[3])));

		uniform_location = glGetUniformLocation(shader_program_ID, "LightDirection");
		glUniform3fv(uniform_location, 1, glm::value_ptr(myLight.direction));

		uniform_location = glGetUniformLocation(shader_program_ID, "CameraColor");
		glUniform3fv(uniform_location, 1, glm::value_ptr(color));

		uniform_location = glGetUniformLocation(shader_program_ID, "model_matrix");
		glUniformMatrix4fv(uniform_location, 1, false, glm::value_ptr(model));


		uniform_location = glGetUniformLocation(shader_program_ID, "Ia2");
		glUniform3fv(uniform_location, 1, glm::value_ptr(mySecondLight.ambient));

		uniform_location = glGetUniformLocation(shader_program_ID, "Id2");
		glUniform3fv(uniform_location, 1, glm::value_ptr(mySecondLight.diffuse));

		uniform_location = glGetUniformLocation(shader_program_ID, "Is2");
		glUniform3fv(uniform_location, 1, glm::value_ptr(mySecondLight.specular));

		uniform_location = glGetUniformLocation(shader_program_ID, "LightDirection2");
		glUniform3fv(uniform_location, 1, glm::value_ptr(mySecondLight.direction));

		uniform_location = glGetUniformLocation(shader_program_ID, "CameraColor2");
		glUniform3fv(uniform_location, 1, glm::value_ptr(color2));

		//uniform_location = glGetUniformLocation(shader_program_ID, "color");
		//glUniform4fv(uniform_location, 1, glm::value_ptr(tempcolor));

		

			//add to the iterator
			iterator += 1 * deltaTime;
		//int vertexColorLocation = glGetUniformLocation(shader_program_ID, "color");
		//glUniform4f(vertexColorLocation, two * 0.2f, 0.3f, one * 0.1f, 1.0f);

		//shadow pass: bind shadow map target and clear depth
			/*glBindFramebuffer(GL_FRAMEBUFFER, frameBufferObject);
			glViewport(0, 0, 1024, 1024);
			glClear(GL_DEPTH_BUFFER_BIT);

			int loc = glGetUniformLocation(shader_program_ID, "lightMatrix");
			glUniformMatrix4fv(loc, 1, GL_FALSE, &(lightMatrix[0][0]));

			loc = glGetUniformLocation(shader_program_ID, "lightMatrix2");
			glUniformMatrix4fv(loc, 1, GL_FALSE, &(lightMatrix2[0][0]));

			loc = glGetUniformLocation(shader_program_ID, "shadowMap");
			glUniform1i(loc, 0);

			glActiveTexture(GL_TEXTURE0);
			glBindTexture(GL_TEXTURE_2D, frameBufferObjectDepth);*/

			//glUseProgram(shader_program_ID);

			

		

		
		
		//bind textures and draw mesh
		glBindTexture(GL_TEXTURE_2D, wallTexture);
		WallMesh.draw();

		//bind textures and draw mesh
		glBindTexture(GL_TEXTURE_2D, floorTexture);
		FloorMesh.draw();


		
		//change model matrix for generator, bind generator texture and draw mesh
		uniform_location = glGetUniformLocation(shader_program_ID, "model_matrix");
		model[3] = glm::vec4(20, 0, -100, 1);
		glUniformMatrix4fv(uniform_location, 1, false, glm::value_ptr(model));
		glBindTexture(GL_TEXTURE_2D, generatorTexture);
		GeneratorMesh.draw();

		//change modelOppositeRotation's position for trooperm bind texture and draw mesh
		modelOppositeRotation[3] = glm::vec4(0, 0, 120, 1);
		glUniformMatrix4fv(uniform_location, 1, false, glm::value_ptr(modelOppositeRotation));
		glBindTexture(GL_TEXTURE_2D, alienTexture);
		TrooperMesh.draw();

		//set model matrix position back to 0
		model[3] = glm::vec4(0, 0, 0, 1);
		glUniformMatrix4fv(uniform_location, 1, false, glm::value_ptr(model));

		//bind the vertex array
		glBindVertexArray(VAO);
		//glDrawArrays(GL_TRIANGLES, 0, verticiesCount); //six becomes whatever veritcies are above (uncomment when drawing shapes again)
		//draw the square
		glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);

		//swap buffers
		glfwSwapBuffers(NewWindow);
		glfwPollEvents();
	}


	//destroy the window, terminate glfw and close the program
	glfwDestroyWindow(NewWindow);
	glfwTerminate();
	return 0;
}

//below is extra code that was kept for some reason

//triangular prism (does not use index buffer)
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

//OLD random colour code
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

//attempt at recreating rectangular prism with ibo (and stopped mid way through)
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