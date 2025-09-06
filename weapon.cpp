#include"DxLib.h"
#include"weapon.h"

void Weapon::Update(Player* player)
{
	//必殺中にsituation_camera_numが既定の数字になると変わるようにしたいです
	
	
	if (!local_)
	{
		//その位置からの切り離しが必要
		//とりあえず、動き方を決めよう

		VECTOR offset_vel = VGet(0, 0, 0);	//初期化

		//上に投げているかのような処理を作る(offset_velのy座標をいじる)

		offset_vel = VGet(0, 1, 0);

		velocity_ = VAdd(velocity_, offset_vel);

	}

	
}


void Weapon::Draw()
{
	VECTOR scale_rot = VGet(0, 0, 0);
	VECTOR pos = VGet(0, 0, 0);
	scale_rot = VTransformSR(scale_rot, mat_);
	pos = VTransform(pos, mat_);

	
	//MATRIX pos_mat = MGetTranslate(pos_);
	//MATRIX scale_mat = MGetScale(scale_rot);
	//MATRIX rot_mat = MMult(MMult(MInverse(scale_mat), mat_), MInverse(pos_mat));
	//MATRIX model_mat = MMult(MGetScale(scale_), mat_);
	//MATRIX model_mat = MMult(mat_, MGetScale(scale_));
	MATRIX model_mat = MMult(MGetScale(scale_), mat_);

	


	if (local_)
	{
		//MV1SetMatrix(model_, mat_);
		MV1SetMatrix(model_, model_mat);
		//MV1SetScale(model_, scale_);
	}
	else
	{
		//ここの中でmatrixを作る
		MATRIX all_mat = MMult(MGetScale(scale_), 
			MGetTranslate(VAdd(pos_, velocity_)));


		MV1SetMatrix(model_, all_mat);
	}

	
	MV1DrawModel(model_);
}