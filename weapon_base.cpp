#include<iostream>
#include<list>
#include"DxLib.h"
#include"weapon_base.h"
#include"situation.h"

WeaponBase::WeaponBase()
{
	//最初はまだmatをただ初期化だけしといて、後からmatを入れる
	velocity_ = VGet(0, 0, 0);
	pos_ = VGet(0, 0, 0);
	mat_ = MGetTranslate(VGet(0, 0, 0));
	local_ = TRUE;

	collision_data_.pos = pos_;

	//model_ = MV1LoadModel("data/model/weapon/use_path/Bug_Net3.mv1");
}

WeaponBase::~WeaponBase()
{

}

void WeaponBase::Update(EnemyBase* enemy, const float spin_rad)
{
	
}


bool WeaponBase::IsInRange(const VECTOR& vel, float range)
{
	bool flag = FALSE;

	//どんくらいの範囲にいるか

	float size = VSize(vel);

	if (size <= range)
	{
		flag = TRUE;
	}

	return flag;

}

void WeaponBase::Draw(float delta_time)
{
	
	static float timer = 0.0f;
	
	MATRIX model_mat = MMult(MGetScale(scale_), mat_);

	//元のmatから自立させなきゃいけない
	//当たり判定の位置を更新
	collision_data_.pos = MV1GetFramePosition(model_, bone_path_);
	
	
	//デバック用

	MV1DrawModel(model_);
	
	int frame_num = MV1GetFrameNum(model_);
	if (FALSE)
	{
		//DrawFormatString(100, 100, GetColor(255, 255, 255), "%d", frame_num);
		//DrawFormatString(0, 0, GetColor(255, 255, 255), "weapon_collision_pos:: x:%.2f,x:%.2f,x:%.2f", collision_data_.pos.x, collision_data_.pos.y, collision_data_.pos.z);
		//当たり判定を表示
		DrawSphere3D(collision_data_.pos, collision_data_.r, 20, GetColor(30 * bone_path_, (255 - 50 * bone_path_), 255),
			GetColor(30 * bone_path_, (255 - 50 * bone_path_), 255), FALSE);
	}

	
	
	if (FALSE)
	{
		timer += delta_time;

		if (timer >= 0.0f)
		{
			rem_poss_.push_back(collision_data_.pos);
			timer = 0.0f;
		}

		int i = 0;
		for (auto& rem_pos : rem_poss_)
		{
			DrawSphere3D(rem_pos, 1, 20, GetColor(255, 0, 255),
				GetColor(255, 0, 255), TRUE);
			i++;
		}


		if (i > 15)
		{
			//rem_poss_.clear();
		}
	}
	


}


void WeaponBase::SetWeaponName(int name)
{

}
void WeaponBase::SetDeltaTime(float delta_time)
{
	delta_time_ = delta_time;
}