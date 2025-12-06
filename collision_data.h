#pragma once
#include"DxLib.h"
enum class CollisionName
{
	kNothing,	// Õ“Ë‚È‚µ
	kSphere,	// ‹…‘Ì
	kCapsule	// ƒJƒvƒZƒ‹
};


struct CollisionData
{
	CollisionName name;	// Õ“Ë”»’è‚Ì¯•Ê
	VECTOR pos;				// 
	float r;						// ”¼Œa
	float ver;					// c‚Ì’·‚³
};

inline CollisionData CollisionDataUpdate(const CollisionData& data, const VECTOR& vel);
