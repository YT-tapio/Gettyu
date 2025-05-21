#pragma once
#include"DxLib.h"

class Player
{
private:

	VECTOR pos_;	//ポジション
	VECTOR velocity_;
	VECTOR direction_;
	int model_;			//モデル

	//入力するパッドの番号
	int pad_input_num_;

	//操作タイプ
	char key_input_[256] = {};
	XINPUT_STATE pad_input_ = {};

public:

	
	Player(VECTOR pos, int model,int pad_num);

	~Player();


	void Init(VECTOR pos);


	void Draw();


	void InputState();


	void Update();


	const VECTOR& GetPos() const { return pos_; }


};