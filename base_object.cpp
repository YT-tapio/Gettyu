#include"base_object.h"


ObjectBase::ObjectBase(const VECTOR& pos, int model_handle)
	: pos_(pos)
	, model_(model_handle)
	, mat_(MGetTranslate(pos_))
{

};


ObjectBase::~ObjectBase()
{

}


