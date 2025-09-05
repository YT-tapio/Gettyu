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

	EffectManager(const char* file_path, float size, int play_interval);					// コンストラクタ
	~EffectManager();					// デストラクタ
	void Initialize();					// 初期化
	void Load();						// 読み込み
	void Update();	// 更新
	void Draw();						// 描画
	void SetOnDisp(bool flag) { on_disp_ = flag; }
	void SetDeltaTime(const float& delta_time) { delta_time_ = delta_time; }

private:

	// 定数
	//const int	EffectParticleLimit = 20000;				// 画面に表示できる最大パーティクル数
	const char* file_path_;		// エフェクトのファイルパ


	float size_;					// エフェクトのサイズ
	int	 play_interval_;					// エフェクトを再生する周期

	float	EffectMoveSpeed = 0.0f;					// エフェクトが移動する速度

	bool on_disp_;
	EffectPlayType play_type_;

	float delta_time_;

	// 変数
	int resource_handle_;	// エフェクトのリソース用
	int playing_handle_;	// 再生中のエフェクトハンドル

	// 今回の動作でにみ必要な変数
	int		playCount;			// 周期敵に再生するためのカウント
};
