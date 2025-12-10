#pragma once

//‚±‚±‚Évibration‚Ìstruct‚ğ‚¢‚Á‚Ï‚¢‘‚«‚İ‚Ü‚·
//power‚ªæ
struct VibrationData
{
	int power = 0;		//U“®
	int time  = 0;		//ŠÔ
};

//‹zû‚µ‚Ä‚¢‚é‚Æ‚«‚ÌU“®
const VibrationData kVacuumVibration	= { 700,500 };

//•KE‹Z‚Ì”š”­‚Ü‚Å‚ÌU“®
const VibrationData kQuakeVibration		= { 500,300 };

//•KE‹Z‚É‚æ‚é”š”­‚ÌU“®
const VibrationData kBombVibration		= { 1000,100 };

// ƒQƒbƒg‚µ‚½‚ÌU“®
const VibrationData kGetVibration		= { 500,1200 };

const VibrationData kHitEnemyVibration	= { 450,300 };