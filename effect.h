#pragma once

class Effect
{
private:

	VECTOR pos_;
	int handle_;			//エフェクトのデータの格納
	int play_handle_;	//再生中のえふぇくとのデータを格納

	float size_;			//エフェクトの大きさ

	bool is_play_;

public:

	Effect(const char* file_path, const VECTOR& pos, float size);

	~Effect();



	void SetPos(const VECTOR& pos) { pos_ = pos; }

	void SetIsPlay(bool flag) { if (flag != is_play_) is_play_ = flag; }

	const VECTOR GetPos() const { return pos_; }

	const bool GetIsPlay() const { return is_play_; }

};



/**
@brief    再生中の3D表示のエフェクトの角度を設定する。
@param    playingEffectHandle    再生中のエフェクトのハンドル
@param    x    X軸角度(ラジアン)
@param    y    Y軸角度(ラジアン)
@param    y    Y軸角度(ラジアン)
@return    0:成功、-1:失敗
@note
回転の方向は時計回りである。
回転の順番は Z軸回転 → X軸回転 → Y軸回転である。
※エフェクトが既に再生終了していても成功を返す。
*/
//int SetRotationPlayingEffekseer3DEffect(int playingEffectHandle, float x, float y, float z);

