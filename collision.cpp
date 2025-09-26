#include"DxLib.h"
#include"collision_data.h"
#include"collision.h"

bool SphereCapsuleCollision(const CollisionData& one, const CollisionData& two)
{
	bool flag = FALSE;

	//各当たり判定によって変える

	//SphereとSphere
	if (one.name == CollisionName::kSphere && two.name == CollisionName::kSphere)
	{
		//二つの距離(dist)をとる

		//2点間の距離をとる
		VECTOR vel = VSub(two.pos, one.pos);

		//それの大きさを出します
		float dist = VSize(vel);

		//そのサイズが二つの半径分だと当たったとします

		if (dist <= (one.r + two.r))
		{
			flag = TRUE;
		}

	}






	return flag;
}