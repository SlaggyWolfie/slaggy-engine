#include "Geometry.hpp"

#include <glm/ext/matrix_transform.hpp>

#include <core/Entity.hpp>

#include <physics/volumes/Sphere.hpp>
#include <physics/volumes/AABB.hpp>
#include <physics/volumes/OBB.hpp>
#include <physics/Plane.hpp>

namespace slaggy
{
	float Geometry::epsilon = 0.00001f;
	
	bool Geometry::sphereTest(const Shape& lhs, const Shape& rhs)
	{
		return glm::distance(lhs.center(), rhs.center()) <= lhs.radius() + rhs.radius();
	}

	bool Geometry::aabbTest(const AABB& lhs, const AABB& rhs)
	{
		const glm::vec3 lmin = lhs.min();
		const glm::vec3 lmax = lhs.max();
		const glm::vec3 rmin = rhs.min();
		const glm::vec3 rmax = rhs.max();

		return lmax.x > rmin.x && lmin.x < rmax.x
			&& lmax.y > rmin.y && lmin.y < rmax.y
			&& lmax.z > rmin.z && lmin.z < rmax.z;
	}

	bool Geometry::intersects(const Plane& lhs, const Plane& rhs)
	{
		const glm::vec3 direction = glm::cross(lhs.normal, rhs.normal);
		return magnitudeSqr(direction) > epsilon;
	}

	bool Geometry::intersects(const Sphere& one, const Sphere& other)
	{
		return sphereTest(one, other);
	}

	bool Geometry::intersects(const Sphere& sphere, const Plane& plane)
	{
		const float sphereRadius = sphere.radius();
		const glm::vec3 sphereCenter = sphere.center();

		const glm::vec3 closestPoint = plane.closestPointTo(sphereCenter);
		const float distanceSq = distanceSqr(sphereCenter, closestPoint);

		return sphereRadius * sphereRadius > distanceSq;
	}

	bool Geometry::intersects(const AABB& aabb, const Plane& plane)
	{
		const float length = glm::dot(aabb.size(), Geometry::fabsf(plane.normal));

		const float dot = glm::dot(plane.normal, aabb.center());
		const float dist = dot - plane.distance();

		return std::fabsf(dist) <= length;
	}

	bool Geometry::intersects(const OBB& obb, const Plane& plane)
	{
		const glm::mat3 rotationalMatrix = glm::mat3(obb.transformationMatrix());
		const glm::vec3 projectedPlaneNormal = glm::vec3
		(
			glm::dot(plane.normal, rotationalMatrix[0]),
			glm::dot(plane.normal, rotationalMatrix[1]),
			glm::dot(plane.normal, rotationalMatrix[2])
		);

		const float length = glm::dot(obb.size(), Geometry::fabsf(projectedPlaneNormal));

		const float dot = glm::dot(plane.normal, obb.center());
		const float dist = dot - plane.distance();

		return std::fabsf(dist) <= length;
	}

	bool Geometry::intersects(const Sphere& sphere, const AABB& aabb)
	{
		const bool radiusCheck = sphereTest(sphere, aabb);
		if (!radiusCheck) return false;

		// Jim Arvo

		const float sr = sphere.radius();
		const glm::vec3 sc = sphere.center();
		const glm::vec3 bmin = aabb.min();
		const glm::vec3 bmax = aabb.max();

		float distance, squareDistance = 0;

		//find the square of the distance
		//from the sphere to the box
		for (unsigned i = 0; i < 3; i++)
		{
			if (sc[i] < bmin[i])
			{
				distance = sc[i] - bmin[i];
				squareDistance += distance * distance;
			}
			else if (sc[i] > bmax[i])
			{
				distance = sc[i] - bmax[i];
				squareDistance += distance * distance;
			}
		}

		return squareDistance <= sr * sr;
	}

	bool Geometry::intersects(const Sphere& sphere, const OBB& obb)
	{
		const glm::vec3 spherePosition = sphere.center();
		const glm::vec3 closest = obb.closestPointTo(spherePosition);
		const float radius = sphere.radius();

		return distanceSqr(spherePosition, closest) < radius * radius;
	}

	bool Geometry::intersects(const AABB& lhs, const AABB& rhs)
	{
		return sphereTest(lhs, rhs) && aabbTest(lhs, rhs);
	}

	bool Geometry::intersects(const AABB& aabb, const OBB& obb)
	{
		const bool radiusCheck = sphereTest(aabb, obb);
		if (!radiusCheck) return false;

		return satTest(aabb, obb);
	}

	bool Geometry::intersects(const OBB& lhs, const OBB& rhs)
	{
		const bool radiusCheck = sphereTest(lhs, rhs);
		if (!radiusCheck) return false;

		return satTest(lhs, rhs);
	}

	bool Geometry::satTest(const Box& lhs, const Box& rhs)
	{
		// Separating Axis Theorem

		const glm::vec3 lhsCenter = lhs.center(); // object's pos = collider center
		const glm::vec3 rhsCenter = rhs.center();
		const glm::mat4 lhsTransform = glm::scale(lhs.transformationMatrix(), lhs.halfSize()); // scaling for halfsize
		const glm::mat4 rhsTransform = glm::scale(rhs.transformationMatrix(), rhs.halfSize()); // scaling for halfsize

		for (unsigned a = 0; a < 3; a++)
		{
			const glm::vec3 l = glm::vec3(lhsTransform[a]); // one axis to project on
			const float tl = std::abs(glm::dot(l, rhsCenter) - glm::dot(l, lhsCenter)); // center distance

			const float ra = std::abs(glm::dot(l, glm::vec3(lhsTransform[0])))
				+ std::abs(glm::dot(l, glm::vec3(lhsTransform[1])))
				+ std::abs(glm::dot(l, glm::vec3(lhsTransform[2])));

			const float rb = std::abs(glm::dot(l, glm::vec3(rhsTransform[0])))
				+ std::abs(glm::dot(l, glm::vec3(rhsTransform[1])))
				+ std::abs(glm::dot(l, glm::vec3(rhsTransform[2])));

			const float penetration = ra + rb - tl;
			if (penetration <= 0)
			{ // no overlap

				return false;
			}
		}

		for (unsigned b = 0; b < 3; b++)
		{
			const glm::vec3 l = glm::vec3(rhsTransform[b]); // rhs axis to project on
			const float tl = std::abs(glm::dot(l, rhsCenter) - glm::dot(l, lhsCenter)); // center distance

			const float ra = std::abs(glm::dot(l, glm::vec3(lhsTransform[0])))
				+ std::abs(glm::dot(l, glm::vec3(lhsTransform[1])))
				+ std::abs(glm::dot(l, glm::vec3(lhsTransform[2])));

			const float rb = std::abs(glm::dot(l, glm::vec3(rhsTransform[0])))
				+ std::abs(glm::dot(l, glm::vec3(rhsTransform[1])))
				+ std::abs(glm::dot(l, glm::vec3(rhsTransform[2])));

			const float penetration = ra + rb - tl;
			if (penetration <= 0)
			{ // no overlap
				return false;
			}
		}

		for (unsigned a = 0; a < 3; a++)
		{
			const glm::vec3 aAxis = glm::vec3(lhsTransform[a]);
			for (unsigned b = 0; b < 3; b++)
			{
				const glm::vec3 bAxis = glm::vec3(rhsTransform[b]);
				if (aAxis != bAxis)
				{
					const glm::vec3 l = glm::cross(aAxis, bAxis); // has flaw when axis are same, result in (0,0,0), solved by if
					const float tl = std::abs(glm::dot(l, rhsCenter) - glm::dot(l, lhsCenter)); // center distance

					const float ra = std::abs(glm::dot(l, glm::vec3(lhsTransform[0])))
						+ std::abs(glm::dot(l, glm::vec3(lhsTransform[1])))
						+ std::abs(glm::dot(l, glm::vec3(lhsTransform[2])));

					const float rb = std::abs(glm::dot(l, glm::vec3(rhsTransform[0])))
						+ std::abs(glm::dot(l, glm::vec3(rhsTransform[1])))
						+ std::abs(glm::dot(l, glm::vec3(rhsTransform[2])));

					const float penetration = ra + rb - tl;
					if (penetration <= 0)
					{ // no overlap
						return false;
					}
				}
			}
		}

		return true;
	}

	float Geometry::distanceSqr(const glm::vec3& lhs, const glm::vec3& rhs)
	{
		return magnitudeSqr(lhs - rhs);
	}

	float Geometry::magnitudeSqr(const glm::vec3& v)
	{
		return v.x * v.x + v.y * v.y + v.z * v.z;
	}

	glm::vec3 Geometry::fabsf(const glm::vec3& v)
	{
		return glm::vec3
		(
			std::fabsf(v.x),
			std::fabsf(v.y),
			std::fabsf(v.z)
		);
	}

	void Geometry::reflectVelocity(const glm::vec3& centerLhs, const glm::vec3& centerRhs, glm::vec3& velocityLhs, glm::vec3& velocityRhs)
	{
		const glm::vec3 total = velocityLhs - velocityRhs;
		velocityLhs = velocityLhs - total;
		velocityRhs = velocityLhs + total;
		return;

		// https://en.wikipedia.org/wiki/Elastic_collision#Two-dimensional_collision_with_two_moving_objects
		// assuming masses are equal

		const glm::vec3 centerDistanceLR = centerLhs - centerRhs;
		const glm::vec3 centerDistanceRL = -centerDistanceLR;

		velocityLhs = velocityLhs - glm::dot(velocityLhs - velocityRhs, centerDistanceLR) / magnitudeSqr(centerDistanceLR) * centerDistanceLR;
		velocityRhs = velocityRhs - glm::dot(velocityRhs - velocityLhs, centerDistanceRL) / magnitudeSqr(centerDistanceRL) * centerDistanceRL;
	}

	//void Geometry::reflectVelocity(glm::vec3& lhs, glm::vec3& rhs)
	//{
	//	const glm::vec3 total = lhs - rhs;
	//	lhs = lhs - total;
	//	rhs = lhs + total;
	//}
}
