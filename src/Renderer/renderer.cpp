#include <GL/glew.h>
#include "renderer.hpp"
#include "GameObjects/gameObject.hpp"
#include "Logs/logs.hpp"
#include "Utilities/math.hpp"
#include "Map/map.hpp"

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

Renderer::Renderer(sf::RenderTarget& renderTarget)
	:mRenderTarget(renderTarget)
	,mCamera{ sf::Vector2f{0,0}, sf::Vector2f{16*40, 16*30} }
	,mStaticObjectsCamera{ sf::Vector2f{0,0}, sf::Vector2f{16*40, 16*30} }
	,mDebugRenderingMode(false)
{
	mCamera.setViewport({0.f, 0.f, 1.f, 1.f});

	glewExperimental = GL_TRUE;
	if(glewInit() != GLEW_OK)
		PH_LOG_ERROR("GLEW wasn't initialized correctly!");

}

void Renderer::startSceneRendering()
{
	mRenderCommand.clear();
	mRenderCommand.setView(mCamera.getView());
}

void Renderer::startUIRendering()
{
	mRenderCommand.setView(mStaticObjectsCamera.getView());
}

void Renderer::draw(const sf::Drawable& drawableObject)
{
	mRenderCommand.draw(drawableObject, sf::RenderStates::Default);
}

void Renderer::draw(const Map& map)
{
	map.draw(mRenderTarget, sf::RenderStates::Default, getProperCameraBounds());
}

void Renderer::setUpModernOpenGlTest()
{
	// build and compile our shader program
	// ------------------------------------
	// vertex shader
	unsigned vertexShader = glCreateShader(GL_VERTEX_SHADER);
	glShaderSource(vertexShader, 1, &vertexShaderSource, NULL);
	glCompileShader(vertexShader);
	// check for shader compile errors
	int success;
	char infoLog[512];
	glGetShaderiv(vertexShader, GL_COMPILE_STATUS, &success);
	if(!success)
	{
		glGetShaderInfoLog(vertexShader, 512, NULL, infoLog);
		PH_LOG_ERROR(std::string("ERROR::SHADER::VERTEX::COMPILATION_FAILED\n") + infoLog);
	}
	// fragment shader
	unsigned fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
	glShaderSource(fragmentShader, 1, &fragmentShaderSource, NULL);
	glCompileShader(fragmentShader);
	// check for shader compile errors
	glGetShaderiv(fragmentShader, GL_COMPILE_STATUS, &success);
	if(!success)
	{
		glGetShaderInfoLog(fragmentShader, 512, NULL, infoLog);
		PH_LOG_ERROR(std::string("ERROR::SHADER::FRAGMENT::COMPILATION_FAILED\n") + infoLog);
	}
	// link shaders
	shaderProgram = glCreateProgram();
	glAttachShader(shaderProgram, vertexShader);
	glAttachShader(shaderProgram, fragmentShader);
	glLinkProgram(shaderProgram);
	// check for linking errors
	glGetProgramiv(shaderProgram, GL_LINK_STATUS, &success);
	if(!success) {
		glGetProgramInfoLog(shaderProgram, 512, NULL, infoLog);
		PH_LOG_ERROR(std::string("ERROR::SHADER::PROGRAM::LINKING_FAILED\n") + infoLog);
	}
	glDeleteShader(vertexShader);
	glDeleteShader(fragmentShader);

	float vertices[] = {
		-0.5f, -0.5f, 0.0f,
		 0.5f, -0.5f, 0.0f,
		 0.0f,  0.5f, 0.0f 
	};

	glGenVertexArrays(1, &vao);
	glBindVertexArray(vao);

	unsigned int vbo;
	glGenBuffers(1, &vbo);
	glBindBuffer(GL_ARRAY_BUFFER, vbo);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), ( void*) 0);
	glEnableVertexAttribArray(0);

	glBindBuffer(GL_ARRAY_BUFFER, 0);
}

void Renderer::drawModernOpenGlTest()
{
	glUseProgram(shaderProgram);
	glBindVertexArray(vao);
	glDrawArrays(GL_TRIANGLES, 0, 3);
}

sf::FloatRect Renderer::getProperCameraBounds() const
{
	if(mDebugRenderingMode) {
		sf::Vector2f cameraCommonSize(640, 480);
		const sf::Vector2f cameraCommonLeftCornerPosition(
			mCamera.getCenter().x - cameraCommonSize.x / 2,
			mCamera.getCenter().y - cameraCommonSize.x / 2
		);
		return sf::FloatRect(cameraCommonLeftCornerPosition.x, cameraCommonLeftCornerPosition.y,
			cameraCommonSize.x, cameraCommonSize.y);
	}
	else
		return mCamera.getBounds();
}

}
