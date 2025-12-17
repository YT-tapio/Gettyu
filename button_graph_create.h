#pragma once

class NormalSubScreen;
class Font;

class ButtonGraph
{
private:

	

	Font* font_;

	NormalSubScreen* start_screen_;
	NormalSubScreen* exit_screen_;
	NormalSubScreen* input_type_screen_;
	NormalSubScreen* retry_screen_;
	NormalSubScreen* go_title_screen_;

	ButtonGraph();

	void MakeStart();

	void MakeInputType();

	void MakeExit();

	void MakeRetryScreen();

	void MakeGoTitle();

public:

	static ButtonGraph& GetInstance()
	{
		static ButtonGraph instance;	// 静的変数としてインスタンスを定義
		return instance;
	}

	//コピーコンストラクタと代入演算子を消去
	ButtonGraph(const ButtonGraph&) = delete;
	ButtonGraph& operator = (const ButtonGraph&) = delete;
	
	// 画像の生成
	void MakeGraph();

	// 明示的に消去
	void DeleteGraph();

	// スタート
	const int GetStartHandle() const;

	const int GetInputTypeHandle() const;

	// ゲーム終了
	const int GetExitHandle() const;

	const int GetRetryHandle() const;

	// タイトルに戻る
	const int GetGoTitleHandle() const;
};