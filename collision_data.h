#pragma once

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