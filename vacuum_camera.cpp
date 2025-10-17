#include"DxLib.h"
#include"vacuum_camera.h"

VacuumCamera::VacuumCamera(int name)
	: BaseVirtualCamera(VGet(0, 0, 0),name)
{

}

VacuumCamera::~VacuumCamera()
{

}