#ifdef _WIN32
#include <windows.h>

// Use glad to initialize OpenGL context on Windows
#include <glad/glad.h>
#include <GLFW/glfw3.h>


#else

#include <OpenGL/gl3.h>
#include <GLFW/glfw3.h>

#endif

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include "error.h"
#include "triangle.h"
#include "shader.h"
#include "disk.h"

#include <stdio.h>
#include <stdlib.h>
#include <chrono>
#include <ctime>  

static TrianglePtr tri_second;
static TrianglePtr tri_minute;
static TrianglePtr tri_hour;
static DiskPtr disk;
static ShaderPtr shd;

static void error(int code, const char* msg)
{
	printf("GLFW error %d: %s\n", code, msg);
	glfwTerminate();
	exit(1);
}

static void keyboard(GLFWwindow* window, int key, int scancode, int action, int mods)
{
	if (key == GLFW_KEY_Q && action == GLFW_PRESS)
		glfwSetWindowShouldClose(window, GLFW_TRUE);
}

static void resize(GLFWwindow* win, int width, int height)
{
	glViewport(0, 0, width, height);
}

static void initialize()
{
	glClearColor(0.78f, 0.63f, 0.78f, 1.0f);
	shd = Shader::Make();
	disk = Disk::Make(30);

	//criação dos ponteiros do relógio
	tri_second = Triangle::Make();
	tri_minute = Triangle::Make();
	tri_hour = Triangle::Make();

	//carregamento e compilação dos shaders
	shd->AttachVertexShader("shaders/vertex.glsl");
	shd->AttachFragmentShader("shaders/fragment.glsl");
	shd->Link();
	Error::Check("initialize");
}


static void get_time(float& hours, float& minutes, float&seconds)
{
	auto curr_clock = std::chrono::system_clock::now();
	std::time_t curr_time = std::chrono::system_clock::to_time_t(curr_clock);

	tm tm_local;
	localtime_s(&tm_local, &curr_time);

	hours = (float)tm_local.tm_sec;
	minutes = (float)tm_local.tm_min;
	seconds = (float)tm_local.tm_hour;
}


static void display(GLFWwindow* win)
{
	//pegar as horas, minutos e segundos do sistema

	float segundo_atual = 0.0f;
	float minuto_atual = 0.0f;
	float hora_atual =  0.0f;

	get_time(segundo_atual, minuto_atual, hora_atual);

	// 2. Calcular os ângulos para cada ponteiro
	float angulo_segundos = glm::radians(segundo_atual * 6.0f);
	float angulo_minutos = glm::radians(minuto_atual * 5.5f);
	float angulo_horas = glm::radians((hora_atual > 12 ? hora_atual - 12 : hora_atual) * 30.0f + minuto_atual * 0.5f);

	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	//ativação do shader
	shd->UseProgram();

	//matriz do circulo
	glm::mat4 mat_disk = glm::mat4(1.0f);
	shd->SetUniform("model", mat_disk);

	disk->Draw();

	//criação da matriz de transformação para a rotação do ponteiro dos segundos
	glm::mat4 mat_ptr_segundos = glm::mat4(1.0f); //matriz identidade

	mat_ptr_segundos = glm::rotate(mat_ptr_segundos, -angulo_segundos, glm::vec3(0.0f, 0.0f, 1.0f)); //rotaciona o modelo
	mat_ptr_segundos = glm::scale(mat_ptr_segundos, glm::vec3(0.25f, 1.0f, 1.0f));

	//envio da matriz de transformação para o shader
	shd->SetUniform("model", mat_ptr_segundos);

	//desenho do ponteiro dos segundos
	tri_second->Draw();

	//criação da matriz de transformação para a rotação do ponteiro dos minutos
	glm::mat4 mat_ptr_minutos = glm::mat4(1.0f); //matriz identidade

	mat_ptr_minutos = glm::rotate(mat_ptr_minutos, -angulo_minutos, glm::vec3(0.0f, 0.0f, 1.0f)); //rotaciona o modelo
	mat_ptr_minutos = glm::scale(mat_ptr_minutos, glm::vec3(0.65f, 0.75f, 1.0f)); //escala o modelo

	shd->SetUniform("model", mat_ptr_minutos);

	tri_minute->Draw();


	//criação da matriz de transformação para a rotação do ponteiro das horas

	glm::mat4 mat_ptr_hour = glm::mat4(1.0f); //matriz identidade

	mat_ptr_hour = glm::rotate(mat_ptr_hour, -angulo_horas, glm::vec3(0.0f, 0.0f, 1.0f)); //rotaciona o modelo
	mat_ptr_hour = glm::scale(mat_ptr_hour, glm::vec3(0.50f, 0.50f, 1.0f)); //escala o modelo

	shd->SetUniform("model", mat_ptr_hour);
	tri_hour->Draw();

	Error::Check("display");
}

int main()
{
	glfwInit();
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 1);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
	glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);       // required for mac os

	glfwSetErrorCallback(error);

	GLFWwindow* win = glfwCreateWindow(600, 400, "Clock", nullptr, nullptr);
	glfwSetFramebufferSizeCallback(win, resize);  // resize callback
	glfwSetKeyCallback(win, keyboard);            // keyboard callback

	glfwMakeContextCurrent(win);

#ifdef __glad_h_
	if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {
		printf("Failed to initialize GLAD OpenGL context\n");
		exit(1);
	}
#endif
#ifdef __glew_h__
	glewInit();
	if (glewInit() != GLEW_OK) {
		printf("Failed to initialize GLEW OpenGL context\n");
		exit(1);
	}
#endif

	printf("OpenGL version: %s\n", glGetString(GL_VERSION));


	initialize();

	while (!glfwWindowShouldClose(win)) {
		//idle(win);
		display(win);
		glfwSwapBuffers(win);
		glfwPollEvents();
	}
	glfwTerminate();
	return 0;
}
