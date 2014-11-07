#pragma once

#include "Geometry.hpp"

class Touch
{
public:
	Touch(float normalized_x, float normalized_y, glm::mat4 *inverted_view_projection_matrix);

	glm::vec3 atPlane(const Geometry::Plane& plane);

	virtual ~Touch();
private:
	Geometry::Line mLineOfSight;
};

