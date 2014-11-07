#pragma once

#include <glm/glm.hpp>

namespace Geometry
{
	struct Line
	{
		glm::vec3 point;
		glm::vec3 vector;
	};

	struct Plane
	{
		glm::vec3 point;
		glm::vec3 normal;
	};

	struct Sphere
	{
		glm::vec3 center;
		float radius;
	};

	inline glm::vec3 intersect(const Line& line, const Plane& plane)
	{
		float top = glm::dot(plane.point, plane.normal) - glm::dot(line.point, plane.normal);
		float bot = glm::dot(plane.normal, line.vector);
		float t = top / bot;

		//TODO: not sure if I should handle case bot == 0
		//it could mean parallel line or line at plane

		return line.point + t * line.vector;
	}
}
