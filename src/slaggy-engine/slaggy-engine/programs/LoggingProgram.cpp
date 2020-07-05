#include "LoggingProgram.hpp"

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
	int LoggingProgram::run()
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
			static_cast<LoggingProgram*>(glfwGetWindowUserPointer(window))->
				framebuffer_size_callback(window, w, h);
		};

		auto mouse = [](GLFWwindow* window, const double x, const double y)
		{
			static_cast<LoggingProgram*>(glfwGetWindowUserPointer(window))->
				mouse_callback(window, x, y);
		};

		auto scroll = [](GLFWwindow* window, const double x, const double y)
		{
			static_cast<LoggingProgram*>(glfwGetWindowUserPointer(window))->
				scroll_callback(window, x, y);
		};

		//glfwSetFramebufferSizeCallback(window, framebufferResize);
		//glfwSetCursorPosCallback(window, mouse);
		//glfwSetScrollCallback(window, scroll);

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
		//glfwSwapInterval(0);

		// create test sets	
		enum class TreeType : unsigned { OCTREE = 0, KDTREE = 1, BSPTREE = 2 };
		std::unordered_map<TreeType, std::string> treeTypes =
		{
			{ TreeType::OCTREE, "Octree" },
			{ TreeType::KDTREE, "K-d Tree" },
			{ TreeType::BSPTREE, "BSP Tree" }
		};

		std::vector<std::pair<TreeType, unsigned>> tests;
		tests.emplace_back(TreeType::OCTREE, 0);
		tests.emplace_back(TreeType::OCTREE, 3);
		tests.emplace_back(TreeType::OCTREE, 7);
		tests.emplace_back(TreeType::KDTREE, 3);
		tests.emplace_back(TreeType::KDTREE, 7);
		tests.emplace_back(TreeType::BSPTREE, 3);
		tests.emplace_back(TreeType::BSPTREE, 7);

		//for (unsigned i = 0; i < 3; ++i)
		//{
		//	const auto type = static_cast<TreeType>(i);
		//	for (unsigned j = 1; j < 10; ++j)
		//	{
		//		tests.emplace_back(type, j);
		//	}
		//}

		// sim setup
		const unsigned simulationFrames = 2000;
		const unsigned objectAmount = 1000;
		Random::setSeed(7777777);

		int repetition = 0;
		for (const auto& test : tests)
		{
			repetition++;
			if (glfwWindowShouldClose(window)) break;

			// Tree setup
			std::unique_ptr<SpatialPartitioningTree> tree = nullptr;

			switch (test.first)
			{
			case TreeType::OCTREE: tree = std::make_unique<Octree>(); break;
			case TreeType::KDTREE: tree = std::make_unique<KDTree>(); break;
			case TreeType::BSPTREE: tree = std::make_unique<BSPTree>(); break;
			default: return -1;
			}

			tree->initialize(glm::vec3(0), glm::vec3(5), test.second);

			// logging setup
			Log& log = Log::start();
			log.treeType = treeTypes[test.first];
			log.maxDepth = test.second;
			std::cout << "Test #" << repetition << " Start: (" << log.treeType << ", maximum depth " << log.maxDepth << ")" << std::endl;

			// containers
			std::vector<std::unique_ptr<Entity>> objects;
			std::vector<Shape*> shapeColliders;
			std::vector<OctreeMovement*> movers;

			// Program Loop (Render Loop)
			const unsigned fixedTargetFramerate = 50;
			const float fixedTimerPerFrame = 1.0f / fixedTargetFramerate;

			unsigned fixedFrames = 0, frames = 0, fixedUpdates = 0;
			double lag = 0;

			double lastFrame = glfwGetTime();
			double timer = lastFrame;

			// Camera & Rendering setup
			camera = new Camera(glm::vec3(0, 0, 20));
			glm::mat4 view = glm::mat4(1);
			glm::mat4 projection = glm::mat4(1);

			while (fixedFrames < simulationFrames && !glfwWindowShouldClose(window))
			{
				// time
				auto currentFrame = glfwGetTime();
				deltaTime = currentFrame - lastFrame;
				lastFrame = currentFrame;
				lag += deltaTime;

				// input
				process_exit_input(window);
				//process_input(window);

				// fixed update
				while (lag >= fixedTimerPerFrame)
				{
					if (fixedFrames < objectAmount)
						createObject(objects, shapeColliders, movers, *dynamic_cast<AABB*>(tree.get()),
							glm::vec3(0), 4, 0.1f, 0.2f);

					for (auto mover : movers) mover->fixedUpdate();

					tree->startSplit(shapeColliders);

					double collisionTestTime = glfwGetTime();

						auto collisions = tree->collisions();

					collisionTestTime = glfwGetTime() - collisionTestTime;

					CollisionManager::resolve(collisions);

					fixedFrames++;
					fixedUpdates++;
					lag -= fixedTimerPerFrame;

					log.snapshot.frame = fixedFrames;
					// usually in seconds
					// but now measuring in ns because it's too small
					log.snapshot.calculationTime = collisionTestTime * 1000000; 
					log.snapshot.collisionTests = collisions.size();

					log.takeSnapshot();
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
				tree->renderNodes(view, projection);

				for (auto shapeCollider : shapeColliders)
					shapeCollider->render(glm::vec3(0, 0, 1), view, projection);

				frames++;

				// reset every second
				currentFrame = glfwGetTime();
				if (currentFrame - timer > 1)
				{
					timer++;
					std::cout << "\tFPS: ";
					if (frames != 1) std::cout << frames;
					else
					{
						const float lowFrames = float(fixedTargetFramerate) / float(fixedUpdates);
						std::cout << lowFrames;

						if (lowFrames < 1)
						{
							log.success = false;
							fixedFrames = -1;
						}
					}

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

			log = Log::end();
			std::cout << "Test #" << repetition << " End: (" << log.treeType << ", maximum depth " << log.maxDepth << ")";
			if (log.success) std::cout << std::endl;
			else std::cout << " - Early End" << std::endl;
		}

		glfwTerminate();
		return 0;
	}

	void LoggingProgram::createObject(
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

	void LoggingProgram::framebuffer_size_callback(GLFWwindow* window, const int width, const int height) const
	{
		glViewport(0, 0, width, height);
	}

	void LoggingProgram::process_exit_input(GLFWwindow* window)
	{
		if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
			glfwSetWindowShouldClose(window, true);
	}
	
	void LoggingProgram::process_input(GLFWwindow* window)
	{
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

	void LoggingProgram::mouse_callback(GLFWwindow*, const double x, const double y)
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

	void LoggingProgram::scroll_callback(GLFWwindow*, const double, const double yOffset) const
	{
		camera->processZoom(float(yOffset));
	}
}
