// Assembled by SeungHoon00
#pragma once

#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <cstring>
#include <stdlib.h>		  // srand, rand
#include <thread>         // std::this_thread::sleep_for
#include <chrono>         // std::chrono::seconds
#include "math.h"
#include <iostream>
#include <vector>
#include <string>
#include "GL\glut.h"
#include <glm\vec2.hpp>
#include <glm\vec3.hpp>
#include <functional>
#include <glm\common.hpp>
#include <glm\gtc\constants.hpp>
#include "SOIL2/SOIL2.h"

class DigitalCanvas2D
{
public:
	int width_ = 640;
	int height_ = 480;
	GLFWwindow* window_ = nullptr;

public:
	DigitalCanvas2D(const std::string& _title, const int& _width, const int& _height)
		: width_(_width), height_(_height)
	{
		if (!glfwInit()) reportErrorAndExit(__FUNCTION__, "glfw initialization");

		window_ = glfwCreateWindow(_width, _height, _title.c_str(), NULL, NULL);
		if (!window_) reportErrorAndExit(__FUNCTION__, "Window initialization");

		glfwMakeContextCurrent(window_);

		// Initialize GLEW
		glewExperimental = true; // Needed for core profile
		if (glewInit() != GLEW_OK) reportErrorAndExit(__FUNCTION__, "glew initialization");

		const float aspect_ratio = (float)width_ / (float)height_;
		glViewport(0, 0, width_, height_);
		glOrtho(-aspect_ratio, aspect_ratio, -1.0, 1.0, -1.0, 1.0);

		// anti-aliasing
		glfwWindowHint(GLFW_SAMPLES, 32);
		glEnable(GL_LINE_SMOOTH);
		glHint(GL_POLYGON_SMOOTH_HINT, GL_NICEST);
		glHint(GL_LINE_SMOOTH_HINT, GL_NICEST);
		glHint(GL_POLYGON_SMOOTH_HINT, GL_NICEST);
		glEnable(GL_BLEND);
		glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	}

	void reportErrorAndExit(const std::string& function_name, const std::string& message)
	{
		std::cout << "Error: " << function_name <<" "<< message << std::endl;

		glfwTerminate();
		getchar(); // pause to read error message
		exit(1);
	}
	
	float getRadian(const float& degree) // degree to radian
	{
		return degree / 180.0f * glm::half_pi<float>() * 2.0;
	}

	void drawLine(const glm::vec3& color0, const glm::vec2& position0, const glm::vec3& color1, const glm::vec2& position1)
	{
		glBegin(GL_LINES);
		{
			glColor3fv(&color0[0]);
			glVertex2fv(&position0[0]);
			glColor3fv(&color1[0]);
			glVertex2fv(&position1[0]);
		}
		glEnd();
	}

	void beginTransformation()
	{
		glPushMatrix();
	}

	void endTransformation()
	{
		glPopMatrix();
	}

	void translate(const float& pos_x, const float& pos_y)
	{
		glTranslatef(pos_x, pos_y, 0.0);
	}

	void rotate(const float& degree)
	{
		glRotatef(degree, 0.0, 0.0, 1.0);
	}

	void scale(const float& scale_x, const float& scale_y)
	{
		glScalef(scale_x, scale_y, 1.0);
	}

	void drawFilledBox(const glm::vec3& color, const float& width, const float& height)
	{
		const glm::vec2 left_bottom(-0.5*width, - 0.5*height);

		glColor3fv(&color[0]);
		glBegin(GL_QUADS);
		{
			glVertex2f(left_bottom.x, left_bottom.y);
			glVertex2f(left_bottom.x + width, left_bottom.y);
			glVertex2f(left_bottom.x + width, left_bottom.y + height);
			glVertex2f(left_bottom.x, left_bottom.y + height);
		}
		glEnd();
	}

	void drawFilledCircle(const glm::vec3& color, const float& radius, const int& num_segments = 100)
	{
		const float d_theta = glm::half_pi<float>() * 4.0 / (float)num_segments;

		glColor3fv(&color[0]);

		glBegin(GL_TRIANGLE_FAN);
		{
			glVertex2f(0.0f, 0.0f);

			float theta = 0;
			for (int i = 0; i < num_segments; ++i)
			{
				glVertex2f(radius * cos(theta), radius * sin(theta));

				theta += d_theta;
			}
			glVertex2f(radius, 0.0f);

		}
		glEnd();
	}

	void drawCircle(const glm::vec3& color, const float& radius, const int& num_segments = 100)
	{
		const float d_theta = glm::half_pi<float>() * 4.0 / (float)num_segments;

		glColor3fv(&color[0]);

		glBegin(GL_LINE_LOOP);
		{
			float theta = 0;
			for (int i = 0; i < num_segments; ++i)
			{
				glVertex2f(radius * cos(theta), radius * sin(theta));

				theta += d_theta;
			}
		}
		glEnd();
	}

	bool isKeyPressed(const int& key)
	{
		return glfwGetKey(window_, key);
	}

	template <typename Functor>
	void render(const Functor& draw_func)
	{
		glfwMakeContextCurrent(window_);

		glClearColor(1, 1, 1, 1);			 // while background
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		glMatrixMode(GL_MODELVIEW);

		draw_func();

		glfwSwapBuffers(window_); // double buffering

		glfwPollEvents();
	}

	template <typename Functor>
	void show(const Functor& draw_func)
	{
		while (!glfwWindowShouldClose(window_))
		{
			render(draw_func);
		}

		glfwTerminate();
	}
};