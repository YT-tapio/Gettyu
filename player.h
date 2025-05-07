#pragma once
#include"DxLib.h"

class Player
{
private:

	VECTOR pos_;	//ポジション
	int model_;			//モデル

public:

	
	Player(VECTOR pos, int model);

	~Player();


	void Init(VECTOR pos);


	void Draw();


	void Update();


	const VECTOR& GetPos() const { return pos_; }


};