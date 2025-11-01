#include"weapon_UI.h"
#include"screen.h"
#include"weapon_checker.h"
WeaponUI::WeaponUI()
{

	//もともとの画面の比率

	float all_screen_size = (kGameWidth + kGameHeight);

	float kScreenWidthPercent = kGameWidth / all_screen_size;
	float kScreenHeightPercent = kGameHeight / all_screen_size;


	int sub_screen_width = 1000;
	int sub_screen_height = 1000;

	

	sub_screen_width = sub_screen_width * kScreenWidthPercent;
	sub_screen_height = sub_screen_height * kScreenHeightPercent;


	//screenの設定
	sub_screen_ = std::make_shared<NormalSubScreen>(VGet(1000, 200,0.f), kGameWidth, kGameHeight, static_cast<int>(sub_screen_width), static_cast<int>(sub_screen_height), TRUE, AlphaColorType::kBlack, 0);
	circle_gauss_ = std::make_shared<NormalSubScreen>(VGet(1000, 200, 0.f), kGameWidth, kGameHeight, static_cast<int>(sub_screen_width), static_cast<int>(sub_screen_height), TRUE, AlphaColorType::kBlack, 0);

	//ここでいろんなデーターダウンロード
	x_button_.handle				= LoadGraph("data/UI/X_ButtonUI.png");
	y_button_.handle				= LoadGraph("data/UI/Y_ButtonUI.png");

	//posの設定
	x_button_.pos					= VGet(800.f, 200.f, 0.f);
	y_button_.pos					= VGet(1000.f, 400.f, 0.f);

	//元の画像のサイズ
	x_button_.original_width		= 1920.f;
	x_button_.original_height		= 1080.f;
	y_button_.original_width		= 1920.f;
	y_button_.original_height		= 1080.f;


	//どのくらいの大きさにしたいか
	x_button_.width					= 300.f;
	x_button_.height				= 300.f;
	y_button_.width					= 300.f;
	y_button_.height				= 300.f;

	//さいず
	OffsetGraphSize(x_button_);
	OffsetGraphSize(y_button_);


	//ぼかしたサークルの位置
	circle_gauss_pos_ = VGet(x_button_.pos.x, x_button_.pos.y, 0.f);
	circle_gauss_r_ = 150.f;

	if (x_button_.handle == -1 || y_button_.handle == -1)
	{
		printfDx("読み込み失敗\n");
	}

	sub_screen_->SetIsDisp(TRUE);
	circle_gauss_->SetIsDisp(TRUE);


}

WeaponUI::~WeaponUI()
{
	DeleteGraph(x_button_.handle);
	DeleteGraph(y_button_.handle);
}

void WeaponUI::OffsetGraphSize(UIGraphData& graph_data)
{
	//もともとwidth,heightに入れておく
	graph_data.width = graph_data.width * (graph_data.original_width / (graph_data.original_width + graph_data.original_height));
	graph_data.height = graph_data.height * (graph_data.original_height / (graph_data.original_width + graph_data.original_height));
}

void WeaponUI::GraphDraw(UIGraphData graph_data)
{
	DrawExtendGraph(static_cast<int>(graph_data.pos.x - graph_data.width), static_cast<int>(graph_data.pos.y - graph_data.height)
		, static_cast<int>(graph_data.pos.x + graph_data.width), static_cast<int>(graph_data.pos.y + graph_data.height), graph_data.handle, TRUE);
}

void WeaponUI::SetCirclePos()
{
	switch (WeaponChecker::GetInstance().GetName())
	{
	case WeaponName::kBat:

		circle_gauss_pos_ = x_button_.pos;

		break;


	case WeaponName::kBugNet:

		circle_gauss_pos_ = y_button_.pos;

		break;


	}


}


/*--------public---------*/

void WeaponUI::Update()
{
	//武器の種類によって変える
	SetCirclePos();


	circle_gauss_->Up();


	DrawCircle(static_cast<int>(circle_gauss_pos_.x), static_cast<int>(circle_gauss_pos_.y), static_cast<int>(circle_gauss_r_), GetColor(255, 255, 240), TRUE);

	circle_gauss_->Down();

	gausser_->Update(VGet(kGameWidth * 0.5f, kGameHeight * 0.5f, 0.f), kGameWidth, kGameHeight, circle_gauss_->GetHandle(), 8, 10000);

	sub_screen_->Up();			// screenを起動

	GraphDraw(x_button_);
	GraphDraw(y_button_);
	//DrawCircle(kGameWidth * 0.5f, kGameHeight * 0.5f, GetColor(0, 0, 0), TRUE);
	sub_screen_->Down();		// screenを使わない

	

}


void WeaponUI::Draw()
{

	circle_gauss_->Draw();
	sub_screen_->Draw();
}

