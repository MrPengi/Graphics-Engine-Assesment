#include <fstream>
#include <sstream>
#include <ctime>
#include <iostream>
#include "glm.hpp"
#include "ext.hpp"
#include "gl_core_4_5.h"
#include "glfw3.h"

using uint = unsigned int;

int main()
{
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

	glm::vec3 verticies[] =
{
	glm::vec3(-0.5f, 0.5f, 0.0f),
	glm::vec3(0.5f, 0.5f, 0.0f),
	glm::vec3(-0.5f, -0.5f, 0.0f), //3 (base left)

	glm::vec3(0.5f, 0.5f, 0.0f),
	glm::vec3(-0.5f, -0.5f, 0.0f),
	glm::vec3(0.5f, -0.5f, 0.0f), //6 (base right)

	glm::vec3(-0.5f, 0.5f, 0.0f),
	glm::vec3(0.0f, 0.5f, 0.5f),
	glm::vec3(0.5f, 0.5f, 0.0f), //9 (top side)

	glm::vec3(0.5f, -0.5f, 0.0f),
	glm::vec3(0.0f, -0.5f, 0.5f),
	glm::vec3(-0.5f, -0.5f, 0.0f), //12 (bottom side)

	glm::vec3(0.5f, -0.5f, 0.0f),
	glm::vec3(0.0f, 0.5f, 0.5f),
	glm::vec3(-0.5f, 0.5f, 0.0f), //15 (top to bottom right)

	glm::vec3(0.5f, 0.5f, 0.0f),
	glm::vec3(-0.5f, -0.5f, 0.0f),
	glm::vec3(0.0f, 0.5f, 0.5f), //18 (top to bottom left)

	glm::vec3(-0.5f, -0.5f, 0.0f),
	glm::vec3(0.0f, 0.5f, 0.5f),
	glm::vec3(0.0f, -0.5f, 0.5f), //21 (top to bottom left)

	glm::vec3(0.5f, -0.5f, 0.0f),
	glm::vec3(0.0f, 0.5f, 0.5f),
	glm::vec3(0.0f, -0.5f, 0.5f) //24 (top to bottom left)

};
int verticiesCount = 24;
	
	//create and "load" mesh
	uint VAO;
	uint VBO;
	uint IBO;

	int index_buffer[]{ 0,1,2,1,2,3 };

	glGenVertexArrays(1, &VAO);
	glGenBuffers(1, &VBO);
	glGenBuffers(1, &IBO);

	
	
	glBindVertexArray(VAO);
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, verticiesCount * sizeof(glm::vec3), &verticies[0], GL_STATIC_DRAW);

	//glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, IBO);
	//glBufferData(GL_ELEMENT_ARRAY_BUFFER, 6 * sizeof(int), index_buffer, GL_STATIC_DRAW);


	//glBindVertexArray(0);
	//glBindBuffer(GL_ARRAY_BUFFER, 0);

	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(glm::vec3), 0);

	glm::mat4 projection = glm::perspective(1.507f, 16 / 9.0f, 0.0f, 7.0f);
	glm::mat4 view = glm::lookAt(glm::vec3(0, 0, 1), glm::vec3(0), glm::vec3(0, 1, 0));
	glm::mat4 model = glm::mat4(1);

	


	//SHADERS ***
	uint vertex_shader_ID = 0;
	uint fragment_shader_ID = 0;
	uint shader_program_ID = 0;

	//load shader from file to string
	std::string shader_data;
	std::ifstream in_file_stream("..\\Shaders\\simple_vertex.glsl", std::ifstream::in);

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
	const char* data = shader_data.c_str();
	//send in shader location
	glShaderSource(vertex_shader_ID, 1, (const GLchar**)&data, 0);
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
	std::ifstream in_file_stream_frag("..\\Shaders\\simple_frag.glsl", std::ifstream::in);

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
	data = shader_data.c_str();
	//send in shader location
	glShaderSource(fragment_shader_ID, 1, (const GLchar**)&data, 0);
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
	glClearColor(0, 0, 0, 0);

	//bool colourChange = false;

	//float one = rand() % 10;
	//float two = /*rand() % 10*/ 0;
	//float three = /*rand() % 10*/ 0;
	//float four = /*rand() % 10*/ 0;

	/*float random = rand() % 10;
	float iterator = 0;*/

	float deltaTime = 0;
	float currentFrame = 0;
	float previousFrame = 0;
	//bool press = true;

	glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);

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

		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		
		glm::mat4 pvm = projection * view * model;

		//pink
		glm::vec4 color = glm::vec4(1.0f, 0.3f, 0.7f, 0.0f);

		//if (glfwGetKey(NewWindow, GLFW_KEY_SPACE) == GLFW_PRESS)
		//{
		//	press != press;
		//}

		//if (press)
		//{
			model = glm::rotate(model, 0.016f, glm::vec3(-1, -1, -1));
		//}

		/*if (!press)
		{
			model = glm::rotate(model, 0.016f, glm::vec3(-1, -1, -1));
		}*/
		
		
		

		glUseProgram(shader_program_ID);
		auto uniform_location = glGetUniformLocation(shader_program_ID, "projection_view_matrix");
		glUniformMatrix4fv(uniform_location, 1, false, glm::value_ptr(pvm));
		uniform_location = glGetUniformLocation(shader_program_ID, "model_matrix");
		glUniformMatrix4fv(uniform_location, 1, false, glm::value_ptr(model));
		uniform_location = glGetUniformLocation(shader_program_ID, "color");
		glUniform4fv(uniform_location, 1, glm::value_ptr(color));

		glBindVertexArray(VAO);
		glDrawArrays(GL_TRIANGLES, 0, verticiesCount); //six becomes whatever veritcies are above

		//glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);



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