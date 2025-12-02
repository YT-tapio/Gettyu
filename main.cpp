#include<iostream>
#include<vector>
#include<memory>
#include"DxLib.h"
#include"scene_manager.h"

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow)
{

    std::shared_ptr<SceneManager>scene_manager = std::make_shared<SceneManager>();

    scene_manager->Update();
    
    scene_manager->End();

    return 0;
}