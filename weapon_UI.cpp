#include"weapon_UI.h"

WeaponUI::WeaponUI()
{
	//screenの設定
	sub_screen_ = std::make_shared<NormalSubScreen>(VGet(0, 0, 0), 0, 0, TRUE, AlphaColorType::kBlack, 15);

	//ここでいろんなデーターダウンロード
	x_button_.handle				= LoadGraph("data/UI/X_ButtonUI.png");
	y_button_.handle				= LoadGraph("data/UI/Y_ButtonUI.png");

	//posの設定
	x_button_.pos					= VGet(50.f, 50.f, 0.f);
	y_button_.pos					= VGet(150.f, 50.f, 0.f);

	//元の画像のサイズ
	x_button_.original_width	= 1920.f;
	x_button_.original_height	= 1080.f;
	y_button_.original_width	= 1920.f;
	y_button_.original_height	= 1080.f;

	x_button_.width					= 50.f;
	x_button_.height				= 50.f;
	y_button_.width					= 50.f;
	y_button_.height				= 50.f;

	//さいず
	OffsetGraphSize(x_button_);
	OffsetGraphSize(y_button_);


	if (x_button_.handle == -1 || y_button_.handle == -1)
	{
		printfDx("読み込み失敗\n");
	}

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


void WeaponUI::Update()
{

}


void WeaponUI::Draw()
{
	GraphDraw(x_button_);
	GraphDraw(y_button_);
}

