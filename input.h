#pragma once
#include<iostream>
//いろんな入力を図る




const float kPadSpinMin = 50.f;



enum InputDeviceType
{
	kNothing,		// 初期化時の値
	kKey,			// キーボード
	kPad			// パッド
};


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

	char key[256]	= {};
	int mouse		= 0;
	int mouse_x		= 0;
	int mouse_y		= 0;
	int log			= 0;
	float wheel		= 0;

	float left_stick_rad = 0;
	float right_stick_rad = 0;
	XINPUT_STATE pad = {};
	InputState input_state = InputState::kOff;
};

struct StickType
{
	static const int kRight		= 0;
	static const int kLeft		= 1;
};

struct Control
{
	static const int kX = 0;
	static const int kY = 1;
};

class ConditionTimer;

class Input
{

private:


	/*------定数------*/

	const float kMaxPadStickNum		= 32767;
	const float kPadStickDeadZone	= 10000;
	const float kMouseDeadZone		= 10;

	/*-----変数-----*/

	InputDeviceType device_type_;

	std::shared_ptr<ConditionTimer> wheel_offset_timer_;

	// 現在の
	InputType now_type_state_;
	
	// 過去の
	InputType before_type_state_;
	
	// 識別番号
	int num_;

	bool is_active_;

	bool GetInputKey();

	bool GetInputPad();


	void DecideDeviceType();

	/// <summary>
	/// ホイールが動かされているかのチェック
	/// </summary>
	bool CheckChangeWheel(const float& next_wheel, const float& before_wheel);

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

	/// <summary>
	/// スティックがnumよりも先に行ってるかどうか
	/// </summary>
	/// <param name="pad_num"></param>
	/// <param name="num"></param>
	/// <param name="plus"></param>
	/// <returns></returns>
	bool CheckPadNum(short stick_num, int num, bool plus);

	bool CheckControlPadNum(int type, int control, int num, bool plus);

	// コンストラクタ
	Input();

public:

	
	static Input& GetInstance()
	{
		static Input instance;
		return instance;
	}

	Input(const Input&) = delete;
	Input& operator=(const Input&) = delete;

	void Awake(const int num);
	
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

	/// <summary>
	/// 引数以上に動いているかの判別
	/// </summary>
	/// <param name="num"></param>
	/// <returns></returns>
	bool GetPadMove(int type,int cotrol,int num);
	
	/// <summary>
	/// マウスの移動が発生したのかを検知する
	/// </summary>
	/// <returns></returns>
	bool GetMouseMove();

	/// <summary>
	/// 前回との差を返す
	/// </summary>
	/// <returns></returns>
	float GetWheelDifference();

	const int GetMousePosX() const { return now_type_state_.mouse_x; }
	const int GetMousePosY() const { return now_type_state_.mouse_y; }

	const InputType GetNowTypeState() const { return now_type_state_; }
	const InputType GetBeforeTypeState() const { return before_type_state_; }

	const InputDeviceType GetDeviceType() const { return device_type_; }

	const int GetPadNom() const { return num_; }

	void NoActive();

	void Active();

	void Debug();

	void Draw();

};