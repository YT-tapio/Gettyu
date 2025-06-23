#include"player.h"
#include"keyconfig.h"
#include"weapon.h"

#define _USE_MATH_DEFINES
#include <math.h>


Player::Player(VECTOR pos, int model,int pad_num)
	: model_(model)
	, pad_input_num_(pad_num)
	, weapon_(nullptr)
	, now_type_(AnimationType::kNothing)
	, target_rot_(0.0f)
	, before_rot_(0.0f)
{
	//model_ = model;
	//pad_input_num_ = pad_num;
	Init(pos);
}

Player::~Player()
{

}


void Player::Init(VECTOR pos)
{
	/*
	if (!(now_type_ == kNothing))
	{
		animation_.Detach(now_type_);
		now_type_ = kNothing;
	}
	*/
	
	now_type_ = AnimationType::kIdle;

	//animation_.Attach(now_type_);

	before_type_ = AnimationType::kNothing;
	before_before_type_ = AnimationType::kNothing;

	//int
	frame_num_ = 0;

	//float
	pos_ = pos;
	fall_speed_ = 0.0f;
	velocity_ = VGet(0, 0, 0);
	direction_ = VGet(0, 0, 0);
	rotation_ = VGet(0, 0, 0);
	before_rot_ = 0.0f;


	//bool
	is_ground_ = TRUE;
}


void Player::Draw()
{
	//MV1SetPosition(model_, pos_);

	MATRIX pos_matrix = MGetTranslate(pos_);
	MATRIX rotation_matrix = MGetRotY(rotation_.y);

	model_matrix_ = MMult(MMult(
		MGetRotY(rotation_.y), MGetScale(VGet(0.01f, 0.01f, 0.01f))),pos_matrix);

	//MV1SetRotationXYZ(model_, rotation_);
	DrawSphere3D(VGet(pos_.x, pos_.y + 15, pos_.z), 0.5f, 5, GetColor(0, 255, 0), GetColor(0, 255, 0), FALSE);

	MV1SetMatrix(model_, model_matrix_);
	
	MV1DrawModel(model_);

	if (weapon_ != nullptr)
	{
		weapon_->Draw();
	}
	

	//animation_.Draw(now_type_);



	//TestFunc();
}


void Player::AddAnim(const AnimationData& animation_data)
{
	//アニメーションを追加
	animation_.Add(animation_data);
}


void Player::InputState()
{
	GetHitKeyStateAll(key_input_);

	GetJoypadXInputState(pad_input_num_, &pad_input_);
}


void Player::AttachWeapon(const TCHAR* frame_path, int model,float scale)
{
	if (weapon_ != nullptr)
	{
		weapon_ = nullptr;
	}
	

	MATRIX pos_matrix = MGetTranslate(pos_);
	MATRIX rotation_matrix = MGetRotY(rotation_.y);

	model_matrix_ = MMult(MMult(
		MGetRotY(rotation_.y), MGetScale(VGet(0.01f, 0.01f, 0.01f))), pos_matrix);

	MV1SetMatrix(model_, model_matrix_);
	
	frame_num_ = MV1SearchFrame(model_, frame_path);

	MATRIX frame_mat = 
		MV1GetFrameLocalWorldMatrix(model_, frame_num_);

	weapon_ = new Weapon(
		frame_mat, model,scale, 
		MV1GetFramePosition(model_, frame_num_));

}


void Player::Update(const VECTOR& pos, const float& rotation)
{

	InputState();

	InputMovement(pos, rotation);
	if (AnimationType::kAttack > now_type_)
	{
		pos_ = VAdd(pos_, velocity_);
	}
	

	

	//武器を持たない設定にしているときは処理を回さない
	if (weapon_ != nullptr)
	{
		auto test = GetFrameMatrix();

		weapon_->SetMatrix(test);
		weapon_->SetPos(MV1GetFramePosition(model_, frame_num_));
	}



}

void Player::InputMovement(const VECTOR& pos, const float& rotation)
{
	VECTOR velocity = { 0.0f,0.0f,0.0f };

	float speed = 0.0f;

	direction_ = VGet(0, 0, 0);

	/*(PadConfig::kLeftButton)*/

	CheckDirection(pos, rotation);


	if (key_input_[KeyConfig::kDashKey])
	{
		speed = kDashSpeed;

		now_type_ = AnimationType::kFastRun;
	}
	else if (key_input_[KeyConfig::kWalkKey])
	{
		speed = kWalkSpeed;

		now_type_ = AnimationType::kWalk;
	}
	else
	{
		speed = kNormalSpeed;

		now_type_ = AnimationType::kSlowRun;
	}

	velocity = VScale(direction_, speed);

	JumpAction(velocity);


	

	if (VSize(velocity) != 0)
	{
		direction_ = VNorm(velocity);
	}
	else
	{
		now_type_ = AnimationType::kIdle;
	}

	if (!is_ground_)
	{
		if (velocity_.y > 0)
		{
			now_type_ = AnimationType::kJumpUp;
		}
		else if(velocity_.y < 0)
		{
			now_type_ = AnimationType::kJumpDown;
		}
		
	}


	//velocity_ = VScale(velocity, delta_time_);

	/*---デバッグ用---*/
	if (key_input_[KEY_INPUT_1])
	{
		now_type_ = AnimationType::kIdle;
	}

	if (key_input_[KEY_INPUT_2])
	{
		now_type_ = AnimationType::kWalk;
	}

	if (key_input_[KEY_INPUT_3])
	{
		now_type_ = AnimationType::kSlowRun;
	}

	if (key_input_[KEY_INPUT_Q])
	{
		now_type_ = AnimationType::kSwordSlash;
	}

	/*
	if (now_type_ != AnimationType::kIdle)
	{
		now_type_ = AnimationType::kIdle;
	}
	*/


	if (before_type_ != now_type_ && !(animation_.GetBlendFlag()))
	{

		if (!(before_type_ == AnimationType::kNothing))
		{
			animation_.InitBlend(now_type_, before_type_);
		}

		animation_.Attach(now_type_);

		before_before_type_ = before_type_;
		before_type_ = now_type_;

		animation_.SetBlend(TRUE);

	}

	animation_.Update(now_type_);
	if (animation_.GetBlendFlag())
	{
		animation_.Update(before_type_);
	}



	velocity_ = VScale(velocity,delta_time_);


}


void Player::CheckDirection(const VECTOR& pos, const float& rotation)
{
	
	float constant = 0.0f;

	MakeLine(constant, pos);

	VECTOR direction = VGet(0, 0, 0);



	//どちらが前かの判別
	//原点からの距離を見る

	float my_scale = sqrt((pos_.x * pos_.x) + (pos_.z * pos_.z));
	float other_scale = sqrt((pos.x * pos.x) + (pos.z * pos.z));

	//キーを何個入力したか
	int input_count = 0;

	//回転量
	float rot = 0.0f;

	bool flag = FALSE;

	if (pos_.x > pos.x)
	{
		direction = VGet(1, 0, 0);
	}
	else
	{
		direction = VGet(-1, 0, 0);
	}


	/*--------プレイヤーの操作--------*/

	//前
	if (key_input_[KeyConfig::kUpKey] || pad_input_.ThumbLY > PadConfig::kUpStick)
	{
		direction_ = VAdd(direction_, VGet(direction.x, 0, direction.x * constant));
		//rotation_ = VGet(0, rotation, 0);

		/*---例外処理(行列使ったらこんなことしなくて済んだかも)---*/

		if (!(key_input_[KeyConfig::kDownKey]))
		{
			rot += (static_cast<float>((M_PI / 180) * 0));

			input_count++;

		}

	}

	//後ろ
	if (key_input_[KeyConfig::kDownKey] || pad_input_.ThumbLY < PadConfig::kDownStick)
	{
		direction_ = VAdd(direction_, VGet(-1.0f * (direction.x), 0, -1.0f * (direction.x * constant)));

		/*---例外処理---*/
		if (!key_input_[KeyConfig::kUpKey])
		{
			if (key_input_[KeyConfig::kRightKey] || pad_input_.ThumbLX < PadConfig::kRightStick)
			{
				rot += (static_cast<float>((M_PI / 180) * 180));
			}
			else if(key_input_[KeyConfig::kLeftKey] || pad_input_.ThumbLX > PadConfig::kLeftStick)
			{
				rot +=  -1 * (static_cast<float>((M_PI / 180) *  180));
			}
			else
			{
				//前回を参照する
				if (before_rot_ > static_cast<float>((M_PI / 180) * 0))
				{
					rot += (static_cast<float>((M_PI / 180) * 180));
				}
				else
				{
					rot += -1 * (static_cast<float>((M_PI / 180) * 180));
				}
			}



			input_count++;
		}

	}


	//右
	if (key_input_[KeyConfig::kRightKey] || pad_input_.ThumbLX < PadConfig::kRightStick)
	{
		direction_ = VAdd(direction_, VGet(direction.x * constant, 0, -direction.x));

		/*---例外処理---*/
		if (!key_input_[KeyConfig::kLeftKey])
		{
			rot += (static_cast<float>((M_PI / 180) * 90));
			input_count++;
		}
		//rotation_ = VAdd(rotation_,VGet(0, rotation + static_cast<float>((M_PI / 180) * 90), 0));
	}

	//左
	if (key_input_[KeyConfig::kLeftKey] || pad_input_.ThumbLX > PadConfig::kLeftStick)
	{
		direction_ = VAdd(direction_, VGet(-(direction.x * constant), 0, direction.x));

		/*---例外処理---*/
		if (!key_input_[KeyConfig::kRightKey])
		{
			rot += -1 * static_cast<float>((M_PI / 180) * 90);

			input_count++;
		}

		//rotation_ = VAdd(rotation_, VGet(0, rotation - static_cast<float>((M_PI / 180) * 90), 0));
	}


	//正規化
	if (VSquareSize(direction_) > 0)
	{
		direction_ = VNorm(direction_);
	}

	SetLightDirection(VGet(direction.x, 0, direction.x * constant));

	if (input_count != 0)
	{
		target_rot_ = (rotation + (rot / input_count));
		before_rot_ = rot / input_count;
	}

	CheckReverseRot(rotation_.y, target_rot_);
	
}


void Player::CheckReverseRot(float& now_rot, float target_rot)
{
	//同じときは早期リターン
	if (now_rot == target_rot) { return; }
	
	//ここで180の値を宣言
	float simple_reverce_num = (static_cast<float>(M_PI / 180) * 180);

	//target_rotが反対にあるんかの確認を行います
	//ここで反対のアングルを確認(-180よりもした、もしくは180よりも上に行く場合はここで値の調整などを行う)
	if (FALSE)
	{
		


		//フラグを用意(180を超えるもしくは-180を下回った時の確認をするためのフラグ)
		bool over_plus = FALSE;		//180を超える場合
		bool over_minus = FALSE;		//-180を下回る場合


		//now_rotに180度を足すとどうなるかを判定

		//180度足した時(右周り)の場所を確認
		float reverse_plus_rot = now_rot + simple_reverce_num;
		float reverse_minus_rot = now_rot - simple_reverce_num;

		//180,-180を超えるかの確認(これからは-180を下回ることも超えると書きます)

		//先に超えた分の値を保管しとくやつを宣言と思ったけどいらないかも
		float over_plus_rot = 0.0f;
		float over_minus_rot = 0.0f;



		//180
		if (reverse_plus_rot >= simple_reverce_num)
		{
			over_plus = TRUE;

			//いったん値を出します(超過分の値を出し,180からひけばok)
			if (TRUE)
			{
				//めっちゃいるやんけ//反転した時の値出しですこれ重要
				over_plus_rot =
					(reverse_plus_rot - simple_reverce_num
						- simple_reverce_num);
			}

		}


		//-180
		if (reverse_minus_rot <= -simple_reverce_num)
		{
			if (!over_plus)
			{
				over_minus = TRUE;
			}

			//いったん値を出します(超過分の値を出し,-180をたせばok必要)
			//上記の通り
			if (TRUE)
			{
				//めっちゃいるやんけ//反転した時の値出しですこれ重要
				over_minus_rot =
					(reverse_minus_rot + simple_reverce_num
						+ simple_reverce_num);
			}

		}

		float diff = 0.0f;

		//target_rotが範囲内にいるかのフラグ
		bool on_plus_target = FALSE;
		bool on_minus_target = FALSE;

		//とりあえず普通の処理はできた

		//ここから例外処理
		//もし、反対にしたとき180,-180を超えるのが確認できているときに
		if (over_plus)
		{
			//180をもし超えているときに調整する値
			// (初期値は引っかからないように360,-360を超えるようにする)
			float offset_target_rot = simple_reverce_num * 3;

			if (target_rot > simple_reverce_num)
			{
				offset_target_rot = target_rot - (simple_reverce_num * 2);
			}

			if (target_rot < -(simple_reverce_num))
			{
				offset_target_rot = target_rot + (simple_reverce_num * 2);
			}



			//もし、target_rotがその間にいたら
			if ((now_rot < target_rot &&
				target_rot <= simple_reverce_num) ||
				(-simple_reverce_num <= offset_target_rot &&
					offset_target_rot <= over_plus_rot)
				)
			{
				diff = (static_cast<float>((M_PI / 180) * 4));
				/*このifの中で計算しないとめんどそう(180を超えた時の)*/
				//未来を先取り
				float future_rot = now_rot + (diff * (delta_time_ * 10));

				if (future_rot > simple_reverce_num)
				{
					float reverce_future_rot = (future_rot - simple_reverce_num)
						- simple_reverce_num;

					now_rot = reverce_future_rot;
				}
				else
				{
					now_rot += (diff * (delta_time_ * 10));
				}


				//180をターゲットがこえていないとき
				if (target_rot > (static_cast<float>(M_PI / 180) * 0) &&
					target_rot <= simple_reverce_num)
				{
					if (target_rot < now_rot)
					{
						now_rot = target_rot;
					}
				}
				else if (now_rot < static_cast<float>((M_PI / 180) * 0))//超えていてとき、現在の回転も180を超えているとき
				{
					if (offset_target_rot < now_rot)
					{
						now_rot = offset_target_rot;
					}
				}

				on_plus_target = TRUE;
			}
			else  //反対のrotが180を超えているけど、そこにターゲットがいないとき
			{
				diff = -1 * (static_cast<float>((M_PI / 180) * 4));
				now_rot += (diff * (delta_time_ * 10));

				//ターゲットより低くならないように調整
				if (now_rot < target_rot)
				{
					now_rot = target_rot;
				}


			}



		}

		//-180
		if (over_minus)
		{
			//180をもし超えているときに調整する値
			// (初期値は引っかからないように360,-360を超えるようにする)
			float offset_target_rot = -(simple_reverce_num * 3);

			if (target_rot > simple_reverce_num)
			{
				offset_target_rot = target_rot - (simple_reverce_num * 2);
			}

			if (target_rot < -simple_reverce_num)
			{
				offset_target_rot = target_rot + (simple_reverce_num * 2);
			}



			//その間にtarget_rotがいると
			if ((now_rot > target_rot &&
				target_rot >= -simple_reverce_num) ||
				(simple_reverce_num >= offset_target_rot &&
					offset_target_rot >= over_minus_rot))
			{
				diff = -1 * (static_cast<float>((M_PI / 180) * 4));

				//未来を先取り
				float future_rot = now_rot + (diff * (delta_time_ * 10));

				//-180を超えてしまうとき
				if (future_rot < (-1 * simple_reverce_num))
				{
					float future_reverce_rot = (future_rot +
						simple_reverce_num + simple_reverce_num);

					now_rot = future_reverce_rot;
				}
				else
				{
					now_rot += (diff * (delta_time_ * 10));
				}



				//-180をターゲットが超えていないとき
				if (target_rot < (static_cast<float>(M_PI / 180) * 0) &&
					target_rot >= -simple_reverce_num)
				{
					if (target_rot > now_rot)
					{
						now_rot = target_rot;
					}
				}
				else if (now_rot > static_cast<float>((M_PI / 180) * 0)) //超えているときに現在のrotが+なら
				{
					if (offset_target_rot > now_rot)
					{
						now_rot = offset_target_rot;
					}
				}


				on_minus_target = TRUE;
			}
			else  //反対のrotが-180を超えているけど、そこにターゲットがいないとき
			{
				diff = (static_cast<float>((M_PI / 180) * 4));
				now_rot += (diff * (delta_time_ * 10));

				//ターゲットより低くならないように調整
				if (now_rot < target_rot)
				{
					now_rot = target_rot;
				}
			}



		}




		//now_rotがtarget_rotよりも大きいとき-を代入

		if (FALSE)
		{
			if (now_rot > target_rot)
			{
				diff = -1 * (static_cast<float>((M_PI / 180) * 4));
			}

			//now_rotがtarget_rotよりも小さいとき+を代入
			if (now_rot < target_rot)
			{
				diff = (static_cast<float>((M_PI / 180) * 4));
			}

		}


		//どちらかを表示
		if (TRUE)
		{
			if (over_plus)
			{
				printfDx("plus\n");
			}

			if (over_minus)
			{
				printfDx("minus\n");
			}

		}
	}

	/*---------------------------新しい処理----------------------------*/

	// まずは今の座標から目標の座標までの距離を求める
	// その距離が180度を越えるような大きさだと例外の処理を進める

	// 今からからターゲットまでの回転の距離
	float rot_distance = 0.0f;

	//回転量
	float rot_num = (static_cast<float>((M_PI / 180) * 4)) * (delta_time_ * 10);

	// 同じときは先にはじくようにしているので大丈夫
	// どちらが小さいかを見て小さいほうから大きいほうを引く
	if (now_rot < target_rot)
	{
		rot_distance = now_rot - target_rot;
	}
	else
	{
		rot_distance = target_rot - now_rot;
	}

	// rot_distanceの絶対値が180より大きいなら
	if (fabs(rot_distance) > simple_reverce_num)
	{
		//現在の回転量がマイナスなら
		if (now_rot < static_cast<float>((M_PI / 180) * 0))
		{
			now_rot -= rot_num;

			//-180を超えるとき
			if (now_rot < -simple_reverce_num)
			{
				//-180からどんだけ超えているのかを確認
				float over_num = now_rot + simple_reverce_num;

				//超過したときの+の値を代入
				now_rot = simple_reverce_num + over_num;

			}

		}
		else  //+なら
		{
			now_rot += rot_num;

			//180を超えるとき
			if (now_rot > simple_reverce_num)
			{
				//180からどんだけ超えているかを確認
				float over_num = now_rot - simple_reverce_num;

				//超過したときの-の値を代入
				now_rot = -simple_reverce_num + over_num;

			}
		}
	}
	else  //普通の処理
	{
		if (now_rot < target_rot)
		{
			now_rot += rot_num;

			if (now_rot > target_rot)
			{
				now_rot = target_rot;
			}
		}
		else
		{
			now_rot -= rot_num;

			if (now_rot < target_rot)
			{
				now_rot = target_rot;
			}
		}
	}
	

}



void  Player::JumpAction(VECTOR& velocity)
{
	//重力
	fall_speed_ -= (kGravity * delta_time_);

	//地面にいるかの判定
	is_ground_ = CheckGround();


	if (is_ground_)
	{

		if (key_input_[KeyConfig::kJumpKey] || pad_input_.Buttons[PadConfig::kJumpButton])
		{
			//ジャンプの処理
			fall_speed_ = kJumpPower;
			is_ground_ = FALSE;
			now_type_ = AnimationType::kJumpUp;
		}
	}

	VECTOR fall_velocity = VGet(0, fall_speed_, 0);
	velocity = VAdd(velocity, fall_velocity);


}


bool Player::CheckGround()
{
	if (pos_.y <= 0.0f)
	{
		fall_speed_ = 0.0f;
		return TRUE;
		
	}
	else
	{
		return FALSE;
	}
}




void Player::TestFunc()
{
	frame_num_ = MV1GetFrameNum(model_);


	for (int i = 0; i < frame_num_; i++)
	{

		// フレーム名の描画
		DrawFormatString(0, i * 15, GetColor(255, 255, 255), "Name         %s", MV1GetFrameName(model_, i));

	}


}


void Player::MakeLine(float& constant, const VECTOR& pos)
{
	//直線のvector
	VECTOR  distance = VGet(pos.x - pos_.x, 0, pos.z - pos_.z);

	if (distance.x != 0.0f)
	{
		constant = distance.z / distance.x;
	}
	else
	{
		constant = distance.z;
	}

}

MATRIX Player::GetFrameMatrix()
{
	return MV1GetFrameLocalWorldMatrix(model_,frame_num_);
}

