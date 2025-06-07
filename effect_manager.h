#pragma once
#include <EffekseerForDXLib.h> // 先ほど指定したエフェクシアファイルをインクルード

enum EffectPlayType
{
	kStart,
	kPlay,
	kEnd
};

// エフェクト管理クラス
class EffectManager
{
public:

	EffectManager();					// コンストラクタ
	~EffectManager();					// デストラクタ
	void Initialize();					// 初期化
	void Load();						// 読み込み
	void Update(const VECTOR& playPosition);	// 更新
	void Draw();						// 描画
	void SetOnDisp(bool flag) { on_disp_ = flag; }


private:

	// 定数
	const int	EffectParticleLimit = 20000;				// 画面に表示できる最大パーティクル数
	const char* EffectFilePath = "data/effect/Simple_Distortion.efkefc";		// エフェクトのファイルパ
	const float EffectSize = 1.0f;					// エフェクトのサイズ
	const int	EffectPlayInterval = 120;					// エフェクトを再生する周期
	const float	EffectMoveSpeed = 0.0f;					// エフェクトが移動する速度

	bool on_disp_;
	EffectPlayType play_type_;

	// 変数
	int effectResourceHandle;	// エフェクトのリソース用
	int playingEffectHandle;	// 再生中のエフェクトハンドル

	// 今回の動作でにみ必要な変数
	int		playCount;			// 周期敵に再生するためのカウント
};
