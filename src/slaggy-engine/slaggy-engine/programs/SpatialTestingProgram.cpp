#include "SpatialTestingProgram.hpp"

#include <iostream>

#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtx/string_cast.hpp>
#include <glm/gtc/type_ptr.hpp>

#include <core/Entity.hpp>
#include <engine/Shader.hpp>
#include <engine/Camera.hpp>
#include <utils/Random.hpp>

#include <OctreeMovement.hpp>
#include <collision/spatial/Octree.hpp>
#include <collision/colliders/SphereCollider.hpp>
#include <collision/CollisionManager.hpp>
#include "collision/spatial/KDTree.hpp"
#include "collision/spatial/BSPTree.hpp"

#include <Log.hpp>

namespace slaggy
{
	int SpatialTestingProgram::run()
	{
#pragma region init
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
			static_cast<SpatialTestingProgram*>(glfwGetWindowUserPointer(window))->
				framebuffer_size_callback(window, w, h);
		};

		auto mouse = [](GLFWwindow* window, const double x, const double y)
		{
			static_cast<SpatialTestingProgram*>(glfwGetWindowUserPointer(window))->
				mouse_callback(window, x, y);
		};

		auto scroll = [](GLFWwindow* window, const double x, const double y)
		{
			static_cast<SpatialTestingProgram*>(glfwGetWindowUserPointer(window))->
				scroll_callback(window, x, y);
		};

		glfwSetFramebufferSizeCallback(window, framebufferResize);
		glfwSetCursorPosCallback(window, mouse);
		glfwSetScrollCallback(window, scroll);

		glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
		glfwSetCursorPos(window, lastMousePosition.x, lastMousePosition.y);

		// Initialize GLAD — btw, black magic with this case as noted above
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
#pragma endregion 
		glfwSwapInterval(0);
				
		const glm::mat4 identity(1);
		glm::mat4 view, projection;
		glm::mat4 model = view = projection = identity;

		//camera = new Camera(glm::vec3(0, 0, 12));
		camera = new Camera(glm::vec3(0, 0, 20));
		//camera->forward = glm::vec3(1, 0, 0);
		//camera->updateForward();

		//Octree octree;
		BSPTree tree;
		tree.initialize(glm::vec3(0), glm::vec3(5), 7);

		//unsigned objectAmount = 0;
		std::vector<std::unique_ptr<Entity>> objects;
		std::vector<Shape*> shapeColliders;
		std::vector<OctreeMovement*> movers;

		//const unsigned frames = 2400;
		const unsigned simulationFrames = 2000;

		// Program Loop (Render Loop)
		const unsigned fixedTargetFramerate = 50;
		const float fixedTimerPerFrame = 1.0f / fixedTargetFramerate;

		unsigned fixedFrames = 0, frames = 0, fixedUpdates = 0;
		double lag = 0;

		double lastFrame = glfwGetTime();
		double timer = lastFrame;

		//while (!glfwWindowShouldClose(window))
		while (fixedFrames < simulationFrames && !glfwWindowShouldClose(window))
		{
			// time
			auto currentFrame = glfwGetTime();
			deltaTime = currentFrame - lastFrame;
			lastFrame = currentFrame;
			lag += deltaTime;

			// input
			process_input(window);

			// fixed update
			while (lag >= fixedTimerPerFrame)
			{
				if (fixedFrames < 10)
					createObject(objects, shapeColliders, movers, tree, glm::vec3(0), 4, 0.1f, 0.2f);

				for (auto mover : movers) mover->fixedUpdate();

				tree.startSplit(shapeColliders);
				CollisionManager::resolve(tree.collisions());

				fixedFrames++;
				fixedUpdates++;
				lag -= fixedTimerPerFrame;
			}

			// potential update here
			// potential late update here

			// rendering
			view = camera->viewMatrix();
			projection = glm::perspective(glm::radians(camera->fov),
				float(INITIAL_SCREEN_WIDTH) / float(INITIAL_SCREEN_HEIGHT), 0.1f, 100.0f);

			glClearColor(_clearColor.r, _clearColor.g, _clearColor.b, _clearColor.a);
			glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

			// draw something
			//o.render(glm::vec3(0), view, projection);
			tree.renderNodes(view, projection);

			for (auto shapeCollider : shapeColliders)
				shapeCollider->render(glm::vec3(0, 0, 1), view, projection);

			frames++;

			// reset every second
			currentFrame = glfwGetTime();
			if (currentFrame - timer > 1)
			{
				timer++;
				std::cout << "FPS: ";
				if (frames != 1) std::cout << frames;
				else std::cout << float(fixedTargetFramerate) / float(fixedUpdates);

				std::cout << " | Fixed Updates: "
					<< fixedUpdates << " | Second: " << static_cast<int>(currentFrame) << std::endl;
				fixedUpdates = 0, frames = 0;
			}

			// double buffering, and poll IO events
			glfwSwapBuffers(window);
			glfwPollEvents();
		}

		// Clean-up!
		delete camera;

		glfwTerminate();
		return 0;
	}

	void SpatialTestingProgram::createObject(
		std::vector<std::unique_ptr<Entity>>& objectContainer,
		std::vector<Shape*>& colliderContainer,
		std::vector<OctreeMovement*>& movers,
		const AABB& bounds,
		const glm::vec3& spherePosition, const float sphereRadius,
		const float speed, const float objectHalfSize) const
	{
		glm::vec3 position = glm::vec3(
			Random::range(-1.0f, 1.0f, 2),
			Random::range(-1.0f, 1.0f, 2),
			Random::range(-1.0f, 1.0f, 2));

		position *= sphereRadius;
		position += spherePosition;

		// calc direction
		glm::vec3 velocity = glm::vec3(
			Random::range(-1.0f, 1.0f, 2),
			Random::range(-1.0f, 1.0f, 2),
			Random::range(-1.0f, 1.0f, 2));

		velocity *= speed;

		auto entity = new Entity;
		objectContainer.emplace_back(entity);

		Transform* transform = nullptr;
		entity->addComponent(transform);
		transform->translate(position);

		const glm::vec3 axis = glm::vec3(
			Random::range(-1.0f, 1.0f, 2),
			Random::range(-1.0f, 1.0f, 2),
			Random::range(-1.0f, 1.0f, 2));

		const auto angle = static_cast<float>(Random::range(0, 360));
		transform->rotate(glm::normalize(axis), angle);

		const auto sc = entity->addComponent<SphereCollider>();
		sc->setRadius(objectHalfSize);
		colliderContainer.push_back(sc);

		auto om = entity->addComponent<OctreeMovement>();
		om->velocity = velocity;
		om->setBounds(*sc, bounds);
		movers.push_back(om);
	}

	void SpatialTestingProgram::framebuffer_size_callback(GLFWwindow* window, const int width, const int height) const
	{
		glViewport(0, 0, width, height);
	}

	void SpatialTestingProgram::process_input(GLFWwindow* window)
	{
		if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
			glfwSetWindowShouldClose(window, true);

		if (glfwGetKey(window, GLFW_KEY_1) == GLFW_PRESS)
			_clearColor = _defaultClearColor;

		if (glfwGetKey(window, GLFW_KEY_2) == GLFW_PRESS)
			_clearColor = { 0.7f, 0, 0, 1 };

		unsigned int direction = 0;
		if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS) direction |= Camera::movement::FORWARD;
		if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS) direction |= Camera::movement::BACKWARD;
		if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS) direction |= Camera::movement::LEFT;
		if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS) direction |= Camera::movement::RIGHT;
		const bool shiftPressed = glfwGetKey(window, GLFW_KEY_LEFT_SHIFT) == GLFW_PRESS;

		if (direction)
			camera->processMovement(Camera::movement(direction), shiftPressed, static_cast<float>(deltaTime));
	}

	void SpatialTestingProgram::mouse_callback(GLFWwindow*, const double x, const double y)
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

	void SpatialTestingProgram::scroll_callback(GLFWwindow*, const double, const double yOffset) const
	{
		camera->processZoom(float(yOffset));
	}
}
