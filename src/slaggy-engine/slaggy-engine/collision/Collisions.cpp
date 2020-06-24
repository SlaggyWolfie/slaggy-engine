#include "Collisions.hpp"

#include <glm/glm.hpp>
#include <glm/ext/matrix_transform.hpp>

#include <core/Entity.hpp>
#include <core/Transform.hpp>

#include <collision/Sphere.hpp>
#include <collision/AABB.hpp>
#include <collision/OBB.hpp>

namespace slaggy
{
	bool Collisions::checkRadius(Shape& one, Shape& other)
	{
		return glm::distance(one.center(), other.center()) <= one.radius() + other.radius();
	}

	bool Collisions::checkAABB(BB& one, BB& other)
	{
		return one.max().x > other.min().x && one.min().x < other.max().x
			&& one.max().y > other.min().y && one.min().y < other.max().y
			&& one.max().z > other.min().z && one.min().z < other.max().z;
	}

	bool Collisions::intersection(Sphere& one, Sphere& other)
	{
		return checkRadius(one, other);
		//return glm::distance(one.center(), other.center()) <= one.radius() + other.radius();
	}

	bool Collisions::intersection(Sphere& sphere, AABB& aabb)
	{
		//very simple and incorrect
		//return glm::distance(sphere.center(), aabb.center()) <= sphere.radius() + aabb.radius();

		//Sphere check to prevent overexertion
		const bool radiusCheck = checkRadius(sphere, aabb);
		if (!radiusCheck) return false;

		//Better
		const glm::vec3& sphereCenter = sphere.center();
		const float sphereRadius = sphere.radius();

		//Arvo's algorithm
		float distance, squareDistance = 0;
		//find the square of the distance
		//from the sphere to the box
		for (int i = 0; i < 3; i++)
		{
			if (sphereCenter[i] < aabb.min()[i])
			{
				distance = sphereCenter[i] - aabb.min()[i];
				squareDistance += distance * distance;
			}
			else if (sphereCenter[i] > aabb.max()[i])
			{
				distance = sphereCenter[i] - aabb.max()[i];
				squareDistance += distance * distance;
			}
		}
		return squareDistance <= sphereRadius * sphereRadius;
	}

	bool Collisions::intersection(Sphere& sphere, OBB& obb)
	{
		//very simple and incorrect
		//return glm::distance(sphere.center(), obb.center()) <= sphere.radius() + obb.radius();

		//Sphere check to prevent overexertion
		const bool radiusCheck = checkRadius(sphere, obb);
		if (!radiusCheck) return false;

		//Better
		//const OBB& box = obb;
		const glm::vec3& sphereCenter = sphere.center();
		const float sphereRadius = sphere.radius();

		//Arvo's algorithm
		float distance, squareDistance = 0;
		//find the square of the distance
		//from the sphere to the box
		for (int i = 0; i < 3; i++)
		{
			if (sphereCenter[i] < obb.min()[i])
			{
				distance = sphereCenter[i] - obb.min()[i];
				squareDistance += distance * distance;
			}
			else if (sphereCenter[i] > obb.max()[i])
			{
				distance = sphereCenter[i] - obb.max()[i];
				squareDistance += distance * distance;
			}
		}
		return squareDistance <= sphereRadius * sphereRadius;
	}

	bool Collisions::intersection(AABB& one, AABB& other)
	{
		//Sphere check to prevent overexertion
		const bool radiusCheck = checkRadius(one, other);
		if (!radiusCheck) return false;

		return checkAABB(one, other);
	}

	bool Collisions::intersection(AABB& aabb, OBB& obb)
	{
		//std::cout << "Collision Between OBBs" << std::endl;
		//std::cout << "Test Collision Between OBBs" << std::endl;

		//Sphere check to prevent overexertion
		const bool radiusCheck = checkRadius(aabb, obb);
		if (!radiusCheck) return false;

		//AABB check to prevent overexertion
		bool aabbCheck = checkAABB(aabb, obb);
		if (!aabbCheck) return false;

		const glm::vec3 oneCenter = aabb.center(); // object's pos = collider center
		glm::mat4 oneTransform = glm::scale(aabb.entity().getBehavior<Transform>()->transformationMatrix(), aabb.halfSize()); // scaling for halfsize
		const glm::vec3 otherCenter = obb.center();
		glm::mat4 otherTransform = glm::scale(obb.transform(), obb.halfSize()); // scaling for halfsize

		for (int a = 0; a < 3; a++)
		{
			glm::vec3 l = glm::vec3(oneTransform[a]); // one axis to project on
			float tl = std::abs(glm::dot(l, otherCenter) - glm::dot(l, oneCenter)); // center distance
			float ra = std::abs(glm::dot(l, glm::vec3(oneTransform[0]))) + std::abs(glm::dot(l, glm::vec3(oneTransform[1]))) + std::abs(glm::dot(l, glm::vec3(oneTransform[2])));
			float rb = std::abs(glm::dot(l, glm::vec3(otherTransform[0]))) + std::abs(glm::dot(l, glm::vec3(otherTransform[1]))) + std::abs(glm::dot(l, glm::vec3(otherTransform[2])));
			float penetration = (ra + rb) - tl;
			if (penetration <= 0)
			{ // no overlap

				//std::cout << "1 No Collision Between AABB and OBB" << std::endl;
				return false;
			}
		}

		for (int b = 0; b < 3; b++)
		{
			glm::vec3 l = glm::vec3(otherTransform[b]); // other axis to project on
			float tl = std::abs(glm::dot(l, otherCenter) - glm::dot(l, oneCenter)); // center distance
			float ra = std::abs(glm::dot(l, glm::vec3(oneTransform[0]))) + std::abs(glm::dot(l, glm::vec3(oneTransform[1]))) + std::abs(glm::dot(l, glm::vec3(oneTransform[2])));
			float rb = std::abs(glm::dot(l, glm::vec3(otherTransform[0]))) + std::abs(glm::dot(l, glm::vec3(otherTransform[1]))) + std::abs(glm::dot(l, glm::vec3(otherTransform[2])));
			float penetration = (ra + rb) - tl;
			if (penetration <= 0)
			{ // no overlap
				//std::cout << "2 No Collision Between AABB and OBB" << std::endl;
				return false;
			}
		}
		for (int a = 0; a < 3; a++)
		{
			glm::vec3 aAxis = glm::vec3(oneTransform[a]);
			for (int b = 0; b < 3; b++) {
				glm::vec3 bAxis = glm::vec3(otherTransform[b]);
				if (aAxis != bAxis) {
					glm::vec3 l = glm::cross(aAxis, bAxis); // has flaw when axis are same, result in (0,0,0), solved by if
					float tl = std::abs(glm::dot(l, otherCenter) - glm::dot(l, oneCenter)); // center distance
					float ra = std::abs(glm::dot(l, glm::vec3(oneTransform[0]))) + std::abs(glm::dot(l, glm::vec3(oneTransform[1]))) + std::abs(glm::dot(l, glm::vec3(oneTransform[2])));
					float rb = std::abs(glm::dot(l, glm::vec3(otherTransform[0]))) + std::abs(glm::dot(l, glm::vec3(otherTransform[1]))) + std::abs(glm::dot(l, glm::vec3(otherTransform[2])));
					float penetration = (ra + rb) - tl;
					if (penetration <= 0)
					{ // no overlap
						//std::cout << "3 No Collision Between AABB and OBB" << std::endl;
						return false;
					}
				}
			}
		}

		//std::cout << "Collision Between AABB and OBB" << std::endl;
		return true;
	}

	bool Collisions::intersection(OBB& one, OBB& other)
	{
		//std::cout << "Collision Between OBBs" << std::endl;
		//std::cout << "Test Collision Between OBBs" << std::endl;

		//Sphere check to prevent overexertion
		bool radiusCheck = checkRadius(one, other);
		if (!radiusCheck) return false;

		//AABB check to prevent overexertion
		const bool aabbCheck = checkAABB(one, other);
		if (!aabbCheck) return false;

		const glm::vec3 oneCenter = one.center(); // object's pos = collider center
		glm::mat4 oneTransform = glm::scale(one.transform(), one.halfSize()); // scaling for halfsize
		const glm::vec3 otherCenter = other.center();
		glm::mat4 otherTransform = glm::scale(other.transform(), other.halfSize()); // scaling for halfsize

		for (int a = 0; a < 3; a++)
		{
			glm::vec3 l = glm::vec3(oneTransform[a]); // one axis to project on
			float tl = std::abs(glm::dot(l, otherCenter) - glm::dot(l, oneCenter)); // center distance
			float ra = std::abs(glm::dot(l, glm::vec3(oneTransform[0]))) + std::abs(glm::dot(l, glm::vec3(oneTransform[1]))) + std::abs(glm::dot(l, glm::vec3(oneTransform[2])));
			float rb = std::abs(glm::dot(l, glm::vec3(otherTransform[0]))) + std::abs(glm::dot(l, glm::vec3(otherTransform[1]))) + std::abs(glm::dot(l, glm::vec3(otherTransform[2])));
			float penetration = (ra + rb) - tl;
			if (penetration <= 0)
			{ // no overlap

				//std::cout << "1 No Collision Between OBBs" << std::endl;
				return false;
			}
		}

		for (int b = 0; b < 3; b++)
		{
			glm::vec3 l = glm::vec3(otherTransform[b]); // other axis to project on
			float tl = std::abs(glm::dot(l, otherCenter) - glm::dot(l, oneCenter)); // center distance
			float ra = std::abs(glm::dot(l, glm::vec3(oneTransform[0]))) + std::abs(glm::dot(l, glm::vec3(oneTransform[1]))) + std::abs(glm::dot(l, glm::vec3(oneTransform[2])));
			float rb = std::abs(glm::dot(l, glm::vec3(otherTransform[0]))) + std::abs(glm::dot(l, glm::vec3(otherTransform[1]))) + std::abs(glm::dot(l, glm::vec3(otherTransform[2])));
			float penetration = (ra + rb) - tl;
			if (penetration <= 0)
			{ // no overlap
				//std::cout << "2 No Collision Between OBBs" << std::endl;
				return false;
			}
		}
		for (int a = 0; a < 3; a++)
		{
			glm::vec3 aAxis = glm::vec3(oneTransform[a]);
			for (int b = 0; b < 3; b++) {
				glm::vec3 bAxis = glm::vec3(otherTransform[b]);
				if (aAxis != bAxis) {
					glm::vec3 l = glm::cross(aAxis, bAxis); // has flaw when axis are same, result in (0,0,0), solved by if
					float tl = std::abs(glm::dot(l, otherCenter) - glm::dot(l, oneCenter)); // center distance
					float ra = std::abs(glm::dot(l, glm::vec3(oneTransform[0]))) + std::abs(glm::dot(l, glm::vec3(oneTransform[1]))) + std::abs(glm::dot(l, glm::vec3(oneTransform[2])));
					float rb = std::abs(glm::dot(l, glm::vec3(otherTransform[0]))) + std::abs(glm::dot(l, glm::vec3(otherTransform[1]))) + std::abs(glm::dot(l, glm::vec3(otherTransform[2])));
					float penetration = (ra + rb) - tl;
					if (penetration <= 0)
					{ // no overlap
						//std::cout << "3 No Collision Between OBBs" << std::endl;
						return false;
					}
				}
			}
		}

		//std::cout << "Collision Between OBBs" << std::endl;
		return true;
	}
}
