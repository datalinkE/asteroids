#pragma once

#include <glm/glm.hpp>
#include "Logger.hpp"

namespace Geometry
{
    static float eps = std::numeric_limits<float>::epsilon() * 200;

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

	struct Circle
	{
		glm::vec2 center;
		float radius;
	};

	struct Rect
	{
	    glm::vec2 leftBot;
	    glm::vec2 rightTop;
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

	inline int size_of_circle_in_vertices(int num_points)
	{
	    return 1 + (num_points + 1);
	}

	inline void gen_circle(float* out, float radius, int num_points)
	{
		int offset = 0;
	    out[offset++] = 0.0f; //center_x;
	    out[offset++] = 0.0f; //center_y;

	    //TODO:
	    out[offset++] = 0.0f; //texture1;
	    out[offset++] = 0.0f; //texture2;

	    int i;
	    for (i = 0; i <= num_points; i++)
	    {
	        float angle_in_radians = ((float) i / (float) num_points)
	                               * ((float) M_PI * 2.0f);
	        out[offset++] = radius * cos(angle_in_radians);
	        out[offset++] = radius * sin(angle_in_radians);

		    //TODO:
		    out[offset++] = 0.0f; //texture1;
		    out[offset++] = 0.0f; //texture2;
	    }
	}
}
