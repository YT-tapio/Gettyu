#pragma once

class Font
{
private:

	int handle_;

public:



	/// <summary>
	/// フォントの読み込み
	/// </summary>
	/// <param name="file_path">フォントファイルののpath</param>
	/// <param name="font_path">フォントの名前</param>
	/// <param name="size"></param>
	/// <param name="thick"></param>
	/// <param name="font_type">DX_FONTTYPE_NORMAL　　　　　　: ノーマルフォント
	/// DX_FONTTYPE_EDGE　　　　　　　　	: エッジつきフォント
	/// DX_FONTTYPE_ANTIALIASING　　　　	: アンチエイリアスフォント
	/// DX_FONTTYPE_ANTIALIASING_4X4　　　	: アンチエイリアスフォント(4x4サンプリング)
	/// DX_FONTTYPE_ANTIALIASING_8X8　　	: アンチエイリアスフォント(8x8サンプリング)
	/// DX_FONTTYPE_ANTIALIASING_EDGE_4X4　 : アンチエイリアス＆エッジ付きフォント(4x4サンプリング)
	/// DX_FONTTYPE_ANTIALIASING_EDGE_8X8　 : アンチエイリアス＆エッジ付きフォント(8x8サンプリング) </param>
	Font(const char* file_path,const char* font_path, int size, int thick,int font_type);

	~Font();

	const int GetHandle() const { return handle_; }
};