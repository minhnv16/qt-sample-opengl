#include <GL/glew.h>
#include <GLFW/glfw3.h>

#include <iostream>
#include <string>
#include "glm/glm.hpp"
#include "glm/gtc/matrix_transform.hpp"
#include "glm/gtc/type_ptr.hpp"
#include <vector>

namespace l6 {

#define PI 3.14159265359
unsigned int aSelectedVecColorLocation;
unsigned int aNormalVecColorLocation;
unsigned int selectedId;


glm::vec4 vecSelectedColor = glm::vec4(1.f, 0.f, 0.f, 1.f);
glm::vec4 vecNormalColor = glm::vec4(0.f, 0.f, 1.f, 1.f);

GLuint shaderProgram;
GLuint VBO;


struct Position {
	float fX = 0.f;
	float fY = 0.f;
	float fZ = 0.f;
	Position() {};
	Position(float x, float y, float z) {
		fX = x; fY = y; fZ = z;
	}
};

struct VertexAtt {
	Position posData;
	float fSelected = 0;
	int nObjectId = 0;

	VertexAtt() {};
	VertexAtt(const Position& pos, const float& selected, const int& objId = 0) {
		posData = pos;
		fSelected = selected;
		nObjectId = objId;
	}
};


VertexAtt vertices[] = {
	//location						//selected		//obj id
	VertexAtt(Position(0.f,0.f,0.f),			0,	-1),

	VertexAtt(Position(0.25f, 0.0f, 0.0f),		1,	0),
	VertexAtt(Position(0.25f, 0.25f, 0.0f),		1,	0),
	VertexAtt(Position(0.5f,  0.25f, 0.0f),		1,	0),
	VertexAtt(Position(0.5f,  0.0f, 0.0f),		1,	0),
	VertexAtt(Position(0.375f,  0.125f, 0.0f),  1,	0),

	VertexAtt(Position(-0.25f, 0.0f, 0.0f),		0,	1),
	VertexAtt(Position(-0.25f, 0.25f, 0.0f),	0,	1),
	VertexAtt(Position(-0.5f,  0.25f, 0.0f),	0,	1),
	VertexAtt(Position(-0.5f,  0.0f, 0.0f),		0,	1),
	VertexAtt(Position(-0.375f,  0.125f, 0.0f),	0,	1),


	VertexAtt(Position(-0.25f, -0.1f, 0.0f),		2,	2),
	VertexAtt(Position(-0.25f, -0.25f, 0.0f),		2,	2),
	VertexAtt(Position(-0.5f,  -0.25f, 0.0f),		2,	2),
	VertexAtt(Position(-0.5f,  -0.1f, 0.0f),		2,	2),

	VertexAtt(Position(0.25f, -0.1f, 0.0f),			2,	3),
	VertexAtt(Position(0.25f, -0.25f, 0.0f),		2,	3),
	VertexAtt(Position(0.5f,  -0.25f, 0.0f),		2,	3),
	VertexAtt(Position(0.5f,  -0.1f, 0.0f),			2,	3),


	VertexAtt(Position(0.0f, 0.0f, 0.0f),			2,	4),
	VertexAtt(Position(0.0f, 0.25f, 0.0f),			2,	4),
	VertexAtt(Position(0.1f, 0.25f, 0.0f),			2,	4),
	VertexAtt(Position(0.1f, 0.0f, 0.0f),			2,	4),
};


GLuint elements[] = {
		1,2,3,4,5,
		0,
		6,7,8, 9, 10,
		0,
		11, 12, 13, 14,
		0,
		15,16,17,18,
		0,
		19,20,21,22,
		};


struct Object {
	VertexAtt* pPointer;
	int nNumOfVertex;
public:
	Object(VertexAtt* p = NULL, const int &n = 0) {
		pPointer = p;
		nNumOfVertex = n;
	}
};

std::vector<Object> g_vecObj;


void framebuffer_size_callback(GLFWwindow* window, int width, int height) {
	glViewport(0, 0, width, height);
}
bool get_area_centroid(const Object &obj,float &a, float &x, float &y) {
	a = 0.0f;

	if (obj.nNumOfVertex < 3) {
		return false;
	}
	std::vector<Position> vec_pos;
	for (int i = 0; i < obj.nNumOfVertex; i++) {
		VertexAtt vi = obj.pPointer[i];		
		vec_pos.push_back(vi.posData);
	}

	vec_pos.push_back(obj.pPointer[0].posData);

	float sum = 0.0f;
	for (int i = 0; i < vec_pos.size()-1; i++) {
		Position pos_i = vec_pos[i];
		Position pos_j = vec_pos[i+1];
		sum += (pos_i.fX * pos_j.fY - pos_j.fX * pos_i.fY);
	}
	a = 0.5 * sum;

	float sum_x = 0.0f, sum_y = 0.0f;
	for (int i = 0; i < vec_pos.size() - 1; i++) {
		Position pos_i = vec_pos[i];
		Position pos_j = vec_pos[i + 1];
		sum_x += ((pos_i.fX + pos_j.fX) * (pos_i.fX * pos_j.fY - pos_j.fX * pos_i.fY));
		sum_y += ((pos_i.fY + pos_j.fY) * (pos_i.fX * pos_j.fY - pos_j.fX * pos_i.fY));
	}
	x = sum_x / (6 * a);
	y = sum_y / (6 * a);

	return true;

}

void rotate_point(const float cx, const float cy, float angle, float &x, float &y)
{
	float s = sin(angle);
	float c = cos(angle);

	// translate point back to origin:
	x -= cx;
	y -= cy;

	// rotate point
	float xnew = x * c - y * s;
	float ynew = x * s + y * c;

	// translate point back:
	x = xnew + cx;
	y = ynew + cy;

}

void rotate_my_obj(Object obj, const float cx, const float cy, float angle) {

	for (int i = 0; i < obj.nNumOfVertex; i++) {
		rotate_point(cx, cy, angle, obj.pPointer[i].posData.fX, obj.pPointer[i].posData.fY);
	}
}
void transition_my_obj(Object obj, const float cx, const float cy) {
	for (int i = 0; i < obj.nNumOfVertex; i++) {
		obj.pPointer[i].posData.fX += cx;
		obj.pPointer[i].posData.fY += cy;
	}
}

void key_callback_learn(GLFWwindow* window, int key, int scancode, int action, int mode) {
	//std::cout << "key :" << key << std::endl;
	Object obj;
	if (action == GLFW_PRESS) {
		if (key >= 320 && key <= 324) {
			selectedId = key - 320;
			if (selectedId <= g_vecObj.size()) {
			}
			else {
				return;
			}
		}
		obj = g_vecObj[selectedId];

		if (key == 32) {
			glm::vec4 tmp = vecSelectedColor;
			vecSelectedColor = vecNormalColor;
			vecNormalColor = tmp;
			glUniform4fv(aSelectedVecColorLocation, 1, glm::value_ptr(vecSelectedColor));
			glUniform4fv(aNormalVecColorLocation, 1, glm::value_ptr(vecNormalColor));

		}

		if (key >= 262 && key <= 265) {
			glm::vec2 trans;
			switch (key)
			{
			case 262://key right				
				trans = glm::vec2(0.1f, 0.0f);
				break;
			case 263://key lef
				trans = glm::vec2(-0.1f, 0.0f);
				break;
			case 264://key down
				trans = glm::vec2(0.0f, -0.1f);
				break;
			case 265://key up
				trans = glm::vec2(0.0f, 0.1f);
				break;
			default:
				break;
			}
			transition_my_obj(obj, trans.x, trans.y);
			int ofset = obj.pPointer - vertices;
			glBindBuffer(GL_ARRAY_BUFFER, VBO);
			glBufferSubData(GL_ARRAY_BUFFER, ofset * sizeof(VertexAtt), obj.nNumOfVertex * sizeof(VertexAtt), obj.pPointer);

		}

		if (key == 65 || key == 68) {

			float angle = 0.f;
			switch (key) {
			case 65: //A key
				angle = PI / 4.0f;
				break;
			case 68: //D key
				angle = -PI / 4.0f;
				break;
			default:
				break;
			}


			glm::mat4 transform = glm::mat4(1.0f);
			float a, x, y;
			get_area_centroid(obj, a, x, y);

			rotate_my_obj(obj, x, y, angle);

			int ofset = obj.pPointer - vertices;
			glBindBuffer(GL_ARRAY_BUFFER, VBO);
			glBufferSubData(GL_ARRAY_BUFFER, ofset * sizeof(VertexAtt), obj.nNumOfVertex * sizeof(VertexAtt), obj.pPointer);



		}
		if (key == 83 || key == 87) {
			switch (key) {
			case 83:	//S key
				break;
			case 87: //W key
				break;
			default:
				break;
			}

		}
	}
}

int main() {

	int test = sizeof(Position);
	test = sizeof(VertexAtt);

	g_vecObj.push_back(Object(&vertices[1], 5));
	g_vecObj.push_back(Object(&vertices[6], 5));
	g_vecObj.push_back(Object(&vertices[11], 4));
	g_vecObj.push_back(Object(&vertices[15], 4));
	g_vecObj.push_back(Object(&vertices[19], 4));

	GLFWwindow* window = 0;
	if (GLFW_FALSE == glfwInit()) {
		return -1;
	}
	
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);


	int nWidth = 600, nHeight = 600;
	window = glfwCreateWindow(nWidth, nHeight, "Test OpenGL", NULL /* glfwGetPrimaryMonitor()*/, NULL);

	if (!window) {
		glfwTerminate();
		return -1;
	}

	glfwMakeContextCurrent(window);
	glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);
	glfwSetKeyCallback(window, key_callback_learn);

	glewExperimental = GL_TRUE;


	//init glew
	GLenum err = glewInit();

	if (GLEW_OK != err) {
		glfwTerminate();
		return -1;
	}



	unsigned int  VAO;
	glGenVertexArrays(1, &VAO);
	glBindVertexArray(VAO);


	//vertex buffer objects
	glGenBuffers(1, &VBO);
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);


	//Element Buffer Object
	GLuint EBO;
	glGenBuffers(1, &EBO);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(elements), elements, GL_STATIC_DRAW);

	//glBufferSubData(GL_ELEMENT_ARRAY_BUFFER, 0, sizeof(elements), elements);


	char const* VertexShaderSource = R"GLSL(
		#version 330 core
		in vec3 aPos;
		in float isSelected;
		out vec4 oColor; 

		uniform vec4 uSelectedColor;
		uniform vec4 uNormalColor;

		void main()
		{
			if(isSelected == 0){
				oColor = uSelectedColor; // vec4(1.0f, 0.0f, 0.0f, 1.f);
			}
			else if(isSelected == 1){
				oColor = uNormalColor; // vec4(0.0f, 1.0f, 0.0f, 1.f);
			}
			else {
				oColor = vec4(0.0f, 1.0f, 0.0f, 1.f);
			}
			gl_Position = vec4(aPos.x, aPos.y, aPos.z, 1.0);			
		}
		)GLSL";

	GLuint vertexShader;
	vertexShader = glCreateShader(GL_VERTEX_SHADER);
	glShaderSource(vertexShader, 1, &VertexShaderSource, NULL);
	glCompileShader(vertexShader);
	GLint compiled_status = -1;
	glGetShaderiv(vertexShader, GL_COMPILE_STATUS, &compiled_status);
	if (!compiled_status)
	{
		char infoLog[512] = { 0 };
		glGetShaderInfoLog(vertexShader, 512, NULL, infoLog);
		std::cout << "ERROR::SHADER::VERTEX::COMPILATION_FAILED\n" << infoLog << std::endl;
	}


	char const* FragmentShaderSource = R"GLSL(
		#version 330 core
		uniform vec4 vecColor;
		out vec4 FragColor;
		in vec4 oColor; 
		void main()
		{
			FragColor = oColor;
		}
		)GLSL";

	GLuint fragmentShader;
	fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
	glShaderSource(fragmentShader, 1, &FragmentShaderSource, NULL);
	glCompileShader(fragmentShader);
	compiled_status = -1;
	glGetShaderiv(fragmentShader, GL_COMPILE_STATUS, &compiled_status);
	if (!compiled_status)
	{
		char infoLog[512] = { 0 };
		glGetShaderInfoLog(vertexShader, 512, NULL, infoLog);
		std::cout << "ERROR::SHADER::FRAGMENT::COMPILATION_FAILED\n" << infoLog << std::endl;
	}


	shaderProgram = glCreateProgram();
	glAttachShader(shaderProgram, vertexShader);
	glAttachShader(shaderProgram, fragmentShader);
	glLinkProgram(shaderProgram);
	glGetProgramiv(shaderProgram, GL_LINK_STATUS, &compiled_status);
	if (!compiled_status) {
		char infoLog[512] = { 0 };
		glGetProgramInfoLog(shaderProgram, 512, NULL, infoLog);
		std::cout << "program \n" << compiled_status << std::endl;
	}


	//linking vertex attribute
	unsigned int aPosLocation = glGetAttribLocation(shaderProgram, "aPos");
	glVertexAttribPointer(aPosLocation, 3, GL_FLOAT, GL_FALSE, sizeof(VertexAtt), 0);
	glEnableVertexAttribArray(aPosLocation);


	unsigned int aPosSelected = glGetAttribLocation(shaderProgram, "isSelected");
	glVertexAttribPointer(aPosSelected, 1, GL_FLOAT, GL_FALSE, sizeof(VertexAtt), (void*)sizeof(Position));
	glEnableVertexAttribArray(aPosSelected);

	glBindVertexArray(0);


	aSelectedVecColorLocation = glGetUniformLocation(shaderProgram, "uSelectedColor");
	aNormalVecColorLocation = glGetUniformLocation(shaderProgram, "uNormalColor");

	glUseProgram(shaderProgram);
	glEnable(GL_PRIMITIVE_RESTART);
	//glPrimitiveRestartIndex(0);

	glUniform4fv(aSelectedVecColorLocation, 1, glm::value_ptr(vecSelectedColor));
	glUniform4fv(aNormalVecColorLocation, 1, glm::value_ptr(vecNormalColor));


	double lastTime = glfwGetTime();
	int nbFrames = 0;

	glfwSwapInterval(0);
	glBindVertexArray(VAO);

	while (!glfwWindowShouldClose(window))
	{
		double currentTime = glfwGetTime();
		nbFrames++;
		if (currentTime - lastTime >= 1.0) { // If last prinf() was more than 1 sec ago
			// printf and reset timer
			printf("%f ms/frame\n", 1000.0 / double(nbFrames));
			printf("fps=%d\n", nbFrames);

			nbFrames = 0;
			lastTime = currentTime;
		}

		glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT);

		glDrawElements(GL_LINE_LOOP, sizeof(elements) / sizeof(unsigned int), GL_UNSIGNED_INT, 0);

		glfwSwapBuffers(window);
		glfwPollEvents();
	}

	glDeleteBuffers(1, &VBO);
	glDeleteBuffers(1, &EBO);
	glDeleteProgram(shaderProgram);
	glfwTerminate();
	return 0;
}


}

