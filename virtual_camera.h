#pragma once
#include"DxLib.h"

struct VirtualCameraName
{
	static const int kNothing = 0;	//何もない
	static const int kSphere = 1;	//球体上のカメラの処理
	static const int kSuperAttack = 2;	//必殺技のカメラ
	static const int kSuperAttackFirst = 3;
	static const int kSuperAttackSecond = 4;
	static const int kSuperAttackThird = 5;
};


class BaseVirtualCamera
{
private:

	//このバーチャルカメラの名前
	int name_;

protected:

	VECTOR pos_;				//カメラの位置
	VECTOR target_pos_;		//見る位置

public:


	BaseVirtualCamera(const VECTOR& pos,const int name);

	virtual ~BaseVirtualCamera();

	//カメラの場所
	void SetPos(const VECTOR& pos) { pos_ = pos; }

	//見る位置
	void SetTargetPos(const VECTOR& pos) { target_pos_ = pos; }

	const VECTOR GetPos() const { return pos_; }


	const VECTOR GetTargetPos() const { return target_pos_; }


	const int GetCameraName() const { return name_; }

};
