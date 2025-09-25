#include"normal_enemy.h"


NormalEnemy::NormalEnemy(const char* path,const VECTOR& pos,const VECTOR& scale,const VECTOR& dir)
	:BaseEnemy(MV1LoadModel(path),pos,scale,dir)
{
	collision_data_.name = CollisionName::kCapsule;
	collision_data_.pos = pos;
	collision_data_.r = 3.f;
	collision_data_.ver = 0.0;
}

NormalEnemy::~NormalEnemy()
{

}


void NormalEnemy::Init(const VECTOR& pos, const VECTOR scale)
{
	pos_ = pos;
	dir_ = VGet(0.f, 0.f, 0.f);
	velocity_ = VGet(0.f, 0.f, 0.f);
	scale_ = scale;

	is_get_ = FALSE;
	delta_time_ = 0.0f;
}

void NormalEnemy::Update(std::shared_ptr<Player> player)
{
	// ÉÅÉÇë„ÇÌÇË
	// ïﬂÇ‹ÇÈÇ©Ç«Ç§Ç©ÇÃèàóùÇÇ∑ÇÈplayerë§Ç…thisÇëóÇÍÇŒÇÊÇ≥ÇªÇ§Ç‚ÇÀ
	
	player->IsHitEnemy(this);



	if (!is_get_)
	{
		VECTOR vel = VGet(0.f, 0.f, 0.f);
		vel = VScale(VNorm(dir_), 2.0f);
		velocity_ = VScale(vel, delta_time_);

		collision_data_.pos = VAdd(collision_data_.pos, velocity_);

		pos_ = collision_data_.pos;
		pos_.y = collision_data_.pos.y - collision_data_.r;
		//ìñÇΩÇËîªíËÇÃà íuÇçXêV

	}



	
}
