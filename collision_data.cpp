#include"collision_data.h"

inline CollisionData CollisionDataUpdate(const CollisionData& data, const VECTOR& vel)
{
	CollisionData next_data = data;

	next_data.pos = VAdd(data.pos, vel);

	return next_data;
}