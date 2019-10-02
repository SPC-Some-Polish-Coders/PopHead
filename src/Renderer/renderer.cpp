#include <GL/glew.h>
#include "renderer.hpp"
#include "GameObjects/gameObject.hpp"
#include "Logs/logs.hpp"
#include "Utilities/math.hpp"
#include "Map/map.hpp"
#include "buffers.hpp"
#include "openglErrors.hpp"

namespace
{
const char* vertexShaderSource = "#version 330 core\n"
	"layout (location = 0) in vec3 aPos;\n"
	"void main()\n"
	"{\n"
	"   gl_Position = vec4(aPos.x, aPos.y, aPos.z, 1.0);\n"
	"}\0";

const char* fragmentShaderSource = "#version 330 core\n"
	"out vec4 FragColor;\n"
	"void main()\n"
	"{\n"
	"   FragColor = vec4(1.0f, 1.5f, 0.2f, 1.0f);\n"
	"}\n\0";
}

namespace ph {

Renderer::Renderer()
{
	glewExperimental = GL_TRUE;
	if(glewInit() != GLEW_OK)
		PH_LOG_ERROR("GLEW wasn't initialized correctly!");
}

void Renderer::setUpModernOpenGlTest()
{
	// build and compile our shader program
	// ------------------------------------
	// vertex shader
	unsigned vertexShader = glCreateShader(GL_VERTEX_SHADER);
	GLCheck( glShaderSource(vertexShader, 1, &vertexShaderSource, NULL) );
	GLCheck( glCompileShader(vertexShader) );
	// check for shader compile errors
	int success;
	char infoLog[512];
	GLCheck( glGetShaderiv(vertexShader, GL_COMPILE_STATUS, &success) );
	if(!success)
	{
		GLCheck( glGetShaderInfoLog(vertexShader, 512, NULL, infoLog) );
		PH_LOG_ERROR(std::string("ERROR::SHADER::VERTEX::COMPILATION_FAILED\n") + infoLog);
	}
	// fragment shader
	unsigned fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
	GLCheck( glShaderSource(fragmentShader, 1, &fragmentShaderSource, NULL) );
	GLCheck( glCompileShader(fragmentShader) );
	// check for shader compile errors
	GLCheck( glGetShaderiv(fragmentShader, GL_COMPILE_STATUS, &success) );
	if(!success)
	{
		GLCheck( glGetShaderInfoLog(fragmentShader, 512, NULL, infoLog) );
		PH_LOG_ERROR(std::string("ERROR::SHADER::FRAGMENT::COMPILATION_FAILED\n") + infoLog);
	}
	// link shaders
	shaderProgram = glCreateProgram();
	GLCheck( glAttachShader(shaderProgram, vertexShader) );
	GLCheck( glAttachShader(shaderProgram, fragmentShader) );
	GLCheck( glLinkProgram(shaderProgram) );
	// check for linking errors
	GLCheck( glGetProgramiv(shaderProgram, GL_LINK_STATUS, &success) );
	if(!success) {
		GLCheck( glGetProgramInfoLog(shaderProgram, 512, NULL, infoLog) );
		PH_LOG_ERROR(std::string("ERROR::SHADER::PROGRAM::LINKING_FAILED\n") + infoLog);
	}
	GLCheck( glDeleteShader(vertexShader) );
	GLCheck( glDeleteShader(fragmentShader) );

	float vertices[] = {
		-0.5f, -0.5f, 0.0f,
		 0.5f, -0.5f, 0.0f,
		 0.0f,  0.5f, 0.0f 
	};

	unsigned int indices[] = {0, 1, 2};

	GLCheck( glGenVertexArrays(1, &vao) );
	GLCheck( glBindVertexArray(vao) );

	VertexBuffer vbo(vertices, 3 * 3 * sizeof(float));

	/*unsigned int vbo;
	glGenBuffers(1, &vbo);
	glBindBuffer(GL_ARRAY_BUFFER, vbo);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);*/

	GLCheck( glEnableVertexAttribArray(0) );
	GLCheck( glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), ( void*)0) );

	IndexBuffer ibo(indices, 3);

	GLCheck( glBindBuffer(GL_ARRAY_BUFFER, 0) );
}

void Renderer::drawModernOpenGlTest()
{
	GLCheck( glUseProgram(shaderProgram) );
	GLCheck( glBindVertexArray(vao) );
	GLCheck( glDrawElements(GL_TRIANGLES, 3, GL_UNSIGNED_INT, 0) );
}

}
