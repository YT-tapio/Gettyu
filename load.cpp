#include"DxLib.h"
#include"load.h"
#include"Draw2D.h"

LoadUI::LoadUI()
{
	const char* kPath = "data/image/now_loading.png";
	handle_ = LoadGraph(kPath);

	if (handle_ == -1)
	{
		printfDx("ì«Ç›çûÇ›ÉGÉâÅ[\n");
	}

}

LoadUI::~LoadUI()
{

}

void LoadUI::Draw()
{
	Draw2D::ExtendGraph(kPos, static_cast<int>(kOriginalSize.x * kScale.x), static_cast<int>(kOriginalSize.y * kScale.y), handle_, TRUE);
}