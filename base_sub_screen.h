#pragma once
#include<iostream>
#include"DxLib.h"

enum class AlphaColorType
{
	kBlack,
	kWhite
};

class BaseSubScreen
{
private:

	int handle_;

	VECTOR center_pos_;

	int screen_width_;			//描画する際のやつ
	int screen_height_;			//描画する際のやつ
	
	int param_;

	bool is_blend_;

	AlphaColorType color_type_;

protected:

	bool is_disp_;


public:


	BaseSubScreen(const VECTOR& pos,const int screen_width, const int screen_height, const int width,const int height,bool alpha,AlphaColorType color_type, const int param,bool is_blend);

	virtual ~BaseSubScreen();

	/// <summary>
	/// 自分の画面を映し出す際のカメラのセットアップ
	/// </summary>
	void SetUpCamera();

	/// <summary>
	/// もともとカメラの設定に戻す
	/// </summary>
	void SetUpOrignalCamera();

	//起動てきなの,このscreenを使うという意思表示
	void Up();

	//シャットダウン的なの,このscreenを使わないという意思表示
	void Down();

	virtual void Update() = 0;

	void Draw();

	void Debug();

	void SetIsDisp(const bool& flag);

	const int GetHandle() const { return handle_; }

	const bool GetIsDisp() const { return is_disp_; }
};