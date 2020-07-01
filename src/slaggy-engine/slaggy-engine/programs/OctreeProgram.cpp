#include "OctreeProgram.hpp"

#include <iostream>

#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtx/string_cast.hpp>
#include <glm/gtc/type_ptr.hpp>

#include <core/Entity.hpp>
#include <engine/Shader.hpp>
#include <engine/Camera.hpp>

#include <collision/spatial/Octree.hpp>

namespace slaggy
{
	int OctreeProgram::run()
	{
		// Initialize GLFW context with OpenGL version 3.3 using the Core OpenGL profile
		glfwInit();

		//-----Setup-----//
		glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
		glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
		glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

		// MacOS-specific code
#ifdef __APPLE__
		glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
#endif

		// Create window
		GLFWwindow* window = glfwCreateWindow(INITIAL_SCREEN_WIDTH, INITIAL_SCREEN_HEIGHT, "My OpenGL Window!", nullptr, nullptr);
		if (window == nullptr)
		{
			std::cout << "Failed to create GLFW window." << std::endl;
			glfwTerminate();
			return INIT_ERROR;
		}

		// Set viewport size within window and assign resize function
		//glViewport(0, 0, INITIAL_SCREEN_WIDTH, INITIAL_SCREEN_HEIGHT);
		glfwMakeContextCurrent(window);
		glfwSetWindowUserPointer(window, this);

		auto framebufferResize = [](GLFWwindow* window, const int w, const int h)
		{
			static_cast<OctreeProgram*>(glfwGetWindowUserPointer(window))->
				framebuffer_size_callback(window, w, h);
		};

		auto mouse = [](GLFWwindow* window, const double x, const double y)
		{
			static_cast<OctreeProgram*>(glfwGetWindowUserPointer(window))->
				mouse_callback(window, x, y);
		};

		auto scroll = [](GLFWwindow* window, const double x, const double y)
		{
			static_cast<OctreeProgram*>(glfwGetWindowUserPointer(window))->
				scroll_callback(window, x, y);
		};

		glfwSetFramebufferSizeCallback(window, framebufferResize);
		glfwSetCursorPosCallback(window, mouse);
		glfwSetScrollCallback(window, scroll);

		glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
		glfwSetCursorPos(window, lastMousePosition.x, lastMousePosition.y);

		// Initialize GLAD � btw, black magic with this case as noted above
		if (!gladLoadGLLoader(GLADloadproc(glfwGetProcAddress)))
		{
			std::cout << "Failed to initialize GLAD." << std::endl;
			return INIT_ERROR;
		}

		// > configure global OpenGL state
		glEnable(GL_DEPTH_TEST);
		glEnable(GL_BLEND);
		glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
		//glEnable(GL_CULL_FACE);

		//stbi_set_flip_vertically_on_load(1);

		//----- end of setup -----//

		const glm::mat4 identity(1);
		glm::mat4 view, projection;
		glm::mat4 model = view = projection = identity;

		//camera = new Camera(glm::vec3(0, 0, 12));
		camera = new Camera(glm::vec3(0));
		camera->forward = (glm::vec3(1, 0, 0));
		camera->updateForward();

		Octree octree;
		octree.build(glm::vec3(0), glm::vec3(5), 0, 3, { });

		//Octree o;
		//o.build(glm::vec3(0), glm::vec3(0.1f), 0, 0, { });

		// Program Loop (Render Loop)
		while (!glfwWindowShouldClose(window))
		{
			const auto currentFrame = float(glfwGetTime());
			deltaTime = currentFrame - lastFrame;
			lastFrame = currentFrame;

			// Pre-input calculations | Critical
			view = camera->viewMatrix();
			projection = glm::perspective(glm::radians(camera->fov),
				float(INITIAL_SCREEN_WIDTH) / float(INITIAL_SCREEN_HEIGHT), 0.1f, 100.0f);

			process_input(window);

			// rendering
			glClearColor(_clearColor.r, _clearColor.g, _clearColor.b, _clearColor.a);
			glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

			// draw something
			//o.render(glm::vec3(0), view, projection);
			octree.renderWithChildren(view, projection);

			// double buffering, and poll IO events
			glfwSwapBuffers(window);
			glfwPollEvents();
		}

		// Clean-up!
		// something

		delete camera;

		glfwTerminate();
		return 0;
	}

	void OctreeProgram::framebuffer_size_callback(GLFWwindow* window, const int width, const int height) const
	{
		glViewport(0, 0, width, height);
	}

	void OctreeProgram::process_input(GLFWwindow* window)
	{
		if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
			glfwSetWindowShouldClose(window, true);

		if (glfwGetKey(window, GLFW_KEY_1) == GLFW_PRESS)
			_clearColor = _defaultClearColor;

		if (glfwGetKey(window, GLFW_KEY_2) == GLFW_PRESS)
			_clearColor = { 0.7f, 0, 0, 1 };

		if (glfwGetKey(window, GLFW_KEY_UP) == GLFW_PRESS)
		{
			mix_ratio += 0.01f;
			mix_ratio = std::min(mix_ratio, 1.0f);
		}

		if (glfwGetKey(window, GLFW_KEY_DOWN) == GLFW_PRESS)
		{
			mix_ratio -= 0.01f;
			mix_ratio = std::max(mix_ratio, 0.0f);
		}

		unsigned int direction = 0;
		if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS) direction |= Camera::movement::FORWARD;
		if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS) direction |= Camera::movement::BACKWARD;
		if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS) direction |= Camera::movement::LEFT;
		if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS) direction |= Camera::movement::RIGHT;
		const bool shiftPressed = glfwGetKey(window, GLFW_KEY_LEFT_SHIFT) == GLFW_PRESS;

		if (direction)
			camera->processMovement(Camera::movement(direction), shiftPressed, deltaTime);
	}

	void OctreeProgram::mouse_callback(GLFWwindow*, const double x, const double y)
	{
		if (firstMouse)
		{
			lastMousePosition = glm::vec2(x, y);
			firstMouse = false;
		}

		const glm::vec2 mousePosition(x, y);
		glm::vec2 offset = mousePosition - lastMousePosition;
		lastMousePosition = mousePosition;

		// > reversed since y-coordinates range from bottom to top
		offset.y *= -1;

		camera->processOrientation(offset);
	}

	void OctreeProgram::scroll_callback(GLFWwindow*, const double, const double yOffset) const
	{
		camera->processZoom(float(yOffset));
	}
}