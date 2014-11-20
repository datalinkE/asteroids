#include "Touch.h"
#include "Logger.hpp"

using namespace glm;
using namespace Geometry;

static void divide_by_w(vec4 &vector)
{
	vector[0] /= vector[3];
	vector[1] /= vector[3];
	vector[2] /= vector[3];
}

Touch::Touch(float normalized_x, float normalized_y, mat4 *inverted_view_projection_matrix)
{
	// We'll convert these normalized device coordinates into world-space
	// coordinates. We'll pick a point on the near and far planes, and draw a
	// line between them. To do this transform, we need to first multiply by
	// the inverse matrix, and then we need to undo the perspective divide.
	vec4 near_point_ndc(normalized_x, normalized_y, 0.0f, 1);
	vec4 far_point_ndc(normalized_x, normalized_y,  -100.0f, 1);

	vec4 near_point_world = (*inverted_view_projection_matrix) * near_point_ndc;
	vec4 far_point_world =  (*inverted_view_projection_matrix) * far_point_ndc;

	// Why are we dividing by W? We multiplied our vector by an inverse
	// matrix, so the W value that we end up is actually the *inverse* of
	// what the projection matrix would create. By dividing all 3 components
	// by W, we effectively undo the hardware perspective divide.
	divide_by_w(near_point_world);
	divide_by_w(far_point_world);

	// We don't care about the W value anymore, because our points are now
	// in world coordinates.
	vec3 near_point_ray(near_point_world[0], near_point_world[1], near_point_world[2]);
	vec3 far_point_ray(far_point_world[0], far_point_world[1], far_point_world[2]);

	vec3 vector_between = far_point_ray - near_point_ray;

	mLineOfSight.point = vec3(near_point_ray[0], near_point_ray[1], near_point_ray[2]);
	mLineOfSight.vector = vec3(vector_between[0], vector_between[1], vector_between[2]);
}

vec3 Touch::atPlane(const Plane& plane)
{
	return intersect(mLineOfSight, plane);
}

Touch::~Touch()
{
	//DLOG();
}

