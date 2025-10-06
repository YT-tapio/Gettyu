#pragma once
//いろんな入力を図る

//入力されているなどの状態を表すもの
enum InputState
{
	kOff,			// 押されていない状態
	kPush,		// 押した瞬間の状態
	kOn,			// 押し続けている状態
	kRelease	// 離した瞬間の状態
};

struct InputType
{
	//操作タイプ

	int atai = 0;

	char key[256] = {};
	int mouse = 0;
	int mouse_x = 0;
	int mouse_y = 0;
	int log = 0;

	float left_stick_rad = 0;
	float right_stick_rad = 0;
	XINPUT_STATE pad = {};
	InputState input_state = InputState::kOff;
};

struct StickType
{
	static const int kRight = 0;
	static const int kLeft = 1;
};

struct Control
{
	static const int kX = 0;
	static const int kY = 1;
};



class Input
{

private:


	/*------定数------*/

	const float kMaxPadStickNum = 32767;
	const float kPadStickDeadZone = 10000;
	const float kMouseDeadZone = 10;

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

	/// <summary>
	/// パーセントを作ります
	/// </summary>
	/// <param name="value">調べたい値</param>
	/// <param name="min">最低値(デッドゾーンなどがある場合)</param>
	/// <param name="max">最大値</param>
	/// <returns></returns>
	float MakePercent(float value, float min, float max);

public:

	// コンストラクタ
	Input(const int num);
	

	// デストラクタ
	~Input();

	
	// 入力更新
	void Update();

	void SetTypeState(const  InputType& now_input,const InputType& before_input);

	void ResetMousePoint();

	// キー入力を見る
	InputState CheckInputKey(int key_code);

	// マウス入力を見る
	InputState CheckInputMouse(int mouse);

	// パッド(ボタン)入力を見る
	InputState CheckInputPadButton(int pad_config);

	//マウスの移動量
	float GetMouseVertical();

	float GetMouseRad();

	// パッド(スティック)の入力量を返す(直線の長さ) // 左右 // 最大380くらい
	float GetPadStickVertical(int type);

	// スティック入力の角度を返す
	float GetPadStickRad(int type);

	// 
	float GetPadStickPercent(int type, int control);

	float GetMousePercent(int control);

	float GetStickSpin(int type);

	const InputType GetNowTypeState() const { return now_type_state_; }
	const InputType GetBeforeTypeState() const { return before_type_state_; }


	const int GetPadNom() const { return num_; }

	void Draw();

};