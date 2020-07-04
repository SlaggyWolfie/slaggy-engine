#include "LoggingNoRenderingProgram.hpp"

#include <iostream>

#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtx/string_cast.hpp>
#include <glm/gtc/type_ptr.hpp>

#include <core/Entity.hpp>
#include <engine/Shader.hpp>
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
	int LoggingNoRenderingProgram::run()
	{
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

		for (unsigned i = 0; i < 3; ++i)
		{
			const auto type = static_cast<TreeType>(i);
			for (unsigned j = 1; j < 10; ++j)
			{
				tests.emplace_back(type, j);
			}
		}

		// sim setup
		const unsigned simulationFrames = 2000;
		const unsigned objectAmount = 1000;
		Random::setSeed(7777777);

		int repetition = 0;
		for (const auto& test : tests)
		{
			repetition++;

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

			while (fixedFrames < simulationFrames)
			{
				// time
				auto currentFrame = glfwGetTime();
				const double deltaTime = currentFrame - lastFrame;
				lastFrame = currentFrame;
				lag += deltaTime;

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

				frames++;

				// reset every second
				currentFrame = glfwGetTime();
				if (currentFrame - timer > 1)
				{
					timer++;
					if (frames != 1) std::cout << frames;
					else
					{
						const float lowFrames = float(fixedTargetFramerate) / float(fixedUpdates);

						if (lowFrames < 1)
						{
							log.success = false;
							fixedFrames = -1;
						}
					}

					std::cout << "Fixed Updates: " << fixedUpdates << " | Second: " << static_cast<int>(currentFrame) << std::endl;
					fixedUpdates = 0, frames = 0;
				}
			}

			log = Log::end();
			std::cout << "Test #" << repetition << " End: (" << log.treeType << ", maximum depth " << log.maxDepth << ")";
			if (log.success) std::cout << std::endl;
			else std::cout << " - Early End" << std::endl;
		}

		glfwTerminate();
		return 0;
	}

	void LoggingNoRenderingProgram::createObject(
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
}
