#include"DxLib.h"
#include"weapon.h"

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

	if (TRUE)
	{
		//MV1SetMatrix(model_, mat_);
		MV1SetMatrix(model_, model_mat);
		//MV1SetScale(model_, scale_);
	}
	else
	{
		MV1SetPosition(model_, pos_);
	}

	
	MV1DrawModel(model_);
}