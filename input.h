#pragma once

//いろんな入力を図る

struct InputType
{
	//操作タイプ
	char key[256] = {};
	int mouse = 0;
	int mouse_x = 0;
	int mouse_y = 0;
	XINPUT_STATE pad = {};
};

//入力されているなどの状態を表すもの
enum InputState
{
	kOff,			// 押されていない状態
	kPush,		// 押した瞬間の状態
	kOn,			// 押し続けている状態
	kRelease	// 離した瞬間の状態
};

class Input
{

private:

	/*-----変数-----*/

	// 現在の
	InputType now_type_state_;
	
	// 過去の
	InputType before_type_state_;
	
	// 識別番号
	int num_;

	/// <summary>
	/// 2点間の距離ベクトル?を出す
	/// </summary>
	/// <param name="next_pos">行きたい場所</param>
	/// <param name="pos">今の場所</param>
	/// <returns></returns>
	VECTOR GetVerticalVector(const VECTOR& next_pos,const VECTOR& pos);

public:

	// コンストラクタ
	Input(int num);
	

	// デストラクタ
	~Input();

	
	// 入力更新
	void Update();


	// キー入力を見る
	InputState CheckInputKey(int key_code);

	// マウス入力を見る
	InputState CheckInputMouse(int mouse);

	// パッド(ボタン)入力を見る
	InputState CheckInputPadButton(int pad_config);

	// パッド(スティック)の入力量を返す(直線の長さ)
	float GetPadStickVertical();

	// スティック入力の角度を返す
	float GetPadStickRad();




	const InputType GetNowTypeState() const { return now_type_state_; }


	void Draw();

};