#pragma once
#include <EffekseerForDXLib.h> // 先ほど指定したエフェクシアファイルをインクルード

// エフェクト管理クラス
class EffectManager
{
public:

	EffectManager();					// コンストラクタ
	~EffectManager();					// デストラクタ
	void Initialize();					// 初期化
	void Load();						// 読み込み
	void Update(VECTOR playPosition);	// 更新
	void Draw();						// 描画


private:

	// 定数
	const int	EffectParticleLimit = 20000;				// 画面に表示できる最大パーティクル数
	const char* EffectFilePath = "data/effect/Simple_Distortion.efkefc";		// エフェクトのファイルパ
	const float EffectSize = 10.0f;					// エフェクトのサイズ
	const int	EffectPlayInterval = 300;					// エフェクトを再生する周期
	const float	EffectMoveSpeed = 0.2f;					// エフェクトが移動する速度

	// 変数
	int effectResourceHandle;	// エフェクトのリソース用
	int playingEffectHandle;	// 再生中のエフェクトハンドル

	// 今回の動作でにみ必要な変数
	int		playCount;			// 周期敵に再生するためのカウント
};
