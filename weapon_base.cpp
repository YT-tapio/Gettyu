#include"DxLib.h"
#include"weapon_base.h"


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

void WeaponBase::Update()
{
	
}


void WeaponBase::Draw()
{
	

	
	MATRIX model_mat = MMult(MGetScale(scale_), mat_);

	//元のmatから自立させなきゃいけない

	collision_data_.pos = MV1GetFramePosition(model_, bone_path_);
	
	
	if (local_)
	{
		mat_ = model_mat;
		MV1SetMatrix(model_, model_mat);
	}
	else
	{
		//ここの中でmatrixを作る
		MATRIX all_mat = MMult(mat_, MGetTranslate(VGet(0, 10, 0)));
		MV1SetMatrix(model_, all_mat);
	}

	
	
	
	
	//model_ = MV1LoadModel("data/model/weapon/use_path/Bug_Net3.mv1");
	/*
	MV1SetScale(model_, scale_);
	MV1SetPosition(model_, VGet(0, 10, 0));

	*/
	
	//デバック用

	//DrawSphere3D(VGet(0, 0, 0), 1.f, 20, GetColor(0, 0, 25), GetColor(0, 0, 25), FALSE);
	MV1DrawModel(model_);
	
	int frame_num = MV1GetFrameNum(model_);
	DrawFormatString(100, 100, GetColor(255, 255, 255), "%d", frame_num);

	//当たり判定を表示
	DrawSphere3D(collision_data_.pos, collision_data_.r, 20, GetColor(30 * bone_path_, (255 - 50 * bone_path_), 255),
		GetColor(30 * bone_path_, (255 - 50 * bone_path_), 255), FALSE);


}