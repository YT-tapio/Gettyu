#include"virtual_camera.h"


BaseVirtualCamera::BaseVirtualCamera(const VECTOR& pos,const int name)
	:pos_(pos)
	,target_pos_(pos)
	,name_(name)
{

}


BaseVirtualCamera::~BaseVirtualCamera()
{

}