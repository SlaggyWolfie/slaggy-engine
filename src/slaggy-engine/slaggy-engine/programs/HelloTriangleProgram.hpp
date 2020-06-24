#pragma once
#ifndef HELLO_TRIANGLE_PROGRAM_HPP
#define HELLO_TRIANGLE_PROGRAM_HPP

#include "Program.hpp"

#include <array>

// ReSharper disable once CppUnusedIncludeDirective
#include <glad/glad.h>
#include <GLFW/glfw3.h>

class HelloTriangleProgram final : Program
{
public:
	const int INIT_ERROR = -1;
	const int INITIAL_SCREEN_WIDTH = 800;
	const int INITIAL_SCREEN_HEIGHT = 600;

	// green-ish color
	const std::array<float, 4> _defaultClearColor{ 0.2f, 0.3f, 0.3f, 1.0f };

	std::array<float, 4> _clearColor = _defaultClearColor;

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
		"FragColor = vec4(1.0f, 0.5f, 0.2f, 1.0f);\n"
		"}\0";

	const char* fragmentShader2Source = "#version 330 core\n"
		"out vec4 FragColor;\n"
		"void main()\n"
		"{\n"
		"FragColor = vec4(0.2f, 1.0f, 0.2f, 1.0f);\n"
		"}\0";

	void framebuffer_size_callback(GLFWwindow* window, int width, int height);
	unsigned int init_shader(const char* vertexShaderSource, const char* fragmentShaderSource);
	void process_input(GLFWwindow* window);
	void set_clear_color(const std::array<float, 4>& color);
	void set_clear_color(float r, float g, float b, float a);

	void draw_triangle(GLFWwindow* window, unsigned shaderProgram);
	void draw_polygon_ebo(GLFWwindow* window, unsigned shaderProgram);
	void ex_draw_two_triangles(GLFWwindow* window, unsigned shaderProgram);
	void ex_draw_two_triangles_2(GLFWwindow* window, unsigned shaderProgram);
	void ex_draw_two_triangles_3(GLFWwindow* window, unsigned shaderProgram);
	
	int run() override;
};
#endif //HELLO_TRIANGLE_PROGRAM_HPP