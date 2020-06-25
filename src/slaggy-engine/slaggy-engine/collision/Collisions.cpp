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
	bool Collisions::sphereTest(Shape& lhs, Shape& rhs)
	{
		return glm::distance(lhs.center(), rhs.center()) <= lhs.radius() + rhs.radius();
	}

	bool Collisions::aabbTest(BB& lhs, BB& rhs)
	{
		const glm::vec3 lmin = lhs.min();
		const glm::vec3 lmax = lhs.max();
		const glm::vec3 rmin = rhs.min();
		const glm::vec3 rmax = rhs.max();

		return lmax.x > rmin.x && lmin.x < rmax.x
			&& lmax.y > rmin.y && lmin.y < rmax.y
			&& lmax.z > rmin.z && lmin.z < rmax.z;
	}

	bool Collisions::intersection(Sphere& one, Sphere& other)
	{
		return sphereTest(one, other);
	}

	bool Collisions::intersection(Sphere& sphere, AABB& aabb)
	{
		const bool radiusCheck = sphereTest(sphere, aabb);
		if (!radiusCheck) return false;

		const glm::vec3& sphereCenter = sphere.center();
		const float sphereRadius = sphere.radius();

		// Arvo's algorithm
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
		const bool radiusCheck = sphereTest(sphere, obb);
		if (!radiusCheck) return false;

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

	bool Collisions::intersection(AABB& lhs, AABB& rhs)
	{
		if (!sphereTest(lhs, rhs)) return false;

		return aabbTest(lhs, rhs);
	}

	bool Collisions::intersection(AABB& aabb, OBB& obb)
	{
		const bool radiusCheck = sphereTest(aabb, obb);
		if (!radiusCheck) return false;

		// TODO ???
		bool aabbCheck = aabbTest(aabb, obb);
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

	bool Collisions::intersection(OBB& lhs, OBB& rhs)
	{
		bool radiusCheck = sphereTest(lhs, rhs);
		if (!radiusCheck) return false;

		// TODO ???
		const bool aabbCheck = aabbTest(lhs, rhs);
		if (!aabbCheck) return false;

		const glm::vec3 lhsCenter = lhs.center(); // object's pos = collider center
		glm::mat4 lhsTransform = glm::scale(lhs.transform(), lhs.halfSize()); // scaling for halfsize
		const glm::vec3 rhsCenter = rhs.center();
		glm::mat4 rhsTransform = glm::scale(rhs.transform(), rhs.halfSize()); // scaling for halfsize

		for (int a = 0; a < 3; a++)
		{
			glm::vec3 l = glm::vec3(lhsTransform[a]); // one axis to project on
			float tl = std::abs(glm::dot(l, rhsCenter) - glm::dot(l, lhsCenter)); // center distance
			float ra = std::abs(glm::dot(l, glm::vec3(lhsTransform[0])))
				+ std::abs(glm::dot(l, glm::vec3(lhsTransform[1])))
				+ std::abs(glm::dot(l, glm::vec3(lhsTransform[2])));
			float rb = std::abs(glm::dot(l, glm::vec3(rhsTransform[0])))
				+ std::abs(glm::dot(l, glm::vec3(rhsTransform[1])))
				+ std::abs(glm::dot(l, glm::vec3(rhsTransform[2])));
			float penetration = ra + rb - tl;
			if (penetration <= 0)
			{ // no overlap

				//std::cout << "1 No Collision Between OBBs" << std::endl;
				return false;
			}
		}

		for (int b = 0; b < 3; b++)
		{
			glm::vec3 l = glm::vec3(rhsTransform[b]); // rhs axis to project on
			float tl = std::abs(glm::dot(l, rhsCenter) - glm::dot(l, lhsCenter)); // center distance
			float ra = std::abs(glm::dot(l, glm::vec3(lhsTransform[0])))
				+ std::abs(glm::dot(l, glm::vec3(lhsTransform[1])))
				+ std::abs(glm::dot(l, glm::vec3(lhsTransform[2])));
			float rb = std::abs(glm::dot(l, glm::vec3(rhsTransform[0])))
				+ std::abs(glm::dot(l, glm::vec3(rhsTransform[1])))
				+ std::abs(glm::dot(l, glm::vec3(rhsTransform[2])));
			float penetration = ra + rb - tl;
			if (penetration <= 0)
			{ // no overlap
				//std::cout << "2 No Collision Between OBBs" << std::endl;
				return false;
			}
		}
		for (int a = 0; a < 3; a++)
		{
			glm::vec3 aAxis = glm::vec3(lhsTransform[a]);
			for (int b = 0; b < 3; b++) {
				glm::vec3 bAxis = glm::vec3(rhsTransform[b]);
				if (aAxis != bAxis) {
					glm::vec3 l = glm::cross(aAxis, bAxis); // has flaw when axis are same, result in (0,0,0), solved by if
					float tl = std::abs(glm::dot(l, rhsCenter) - glm::dot(l, lhsCenter)); // center distance
					float ra = std::abs(glm::dot(l, glm::vec3(lhsTransform[0])))
						+ std::abs(glm::dot(l, glm::vec3(lhsTransform[1])))
						+ std::abs(glm::dot(l, glm::vec3(lhsTransform[2])));
					float rb = std::abs(glm::dot(l, glm::vec3(rhsTransform[0])))
						+ std::abs(glm::dot(l, glm::vec3(rhsTransform[1])))
						+ std::abs(glm::dot(l, glm::vec3(rhsTransform[2])));
					float penetration = ra + rb - tl;
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
