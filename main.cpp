#include<iostream>
#include<vector>
#include<memory>
#include"DxLib.h"
#include"EffekseerForDxLib.h"
#include"game.h"

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow)
{
    
    std::shared_ptr<Game>game = std::make_shared<Game>();

    game->Awake();
    game->Loop();
    game->End();

    return 0;


}