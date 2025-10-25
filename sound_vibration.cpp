#include"sound_vibration.h"

SoundVibration::SoundVibration()
    :num_(kInitNum)
{

}

SoundVibration::~SoundVibration()
{
    Reset();
}


void SoundVibration::Reset()
{
    num_ = kInitNum;
}


void SoundVibration::Add(float num)
{
    num_ = num_ + num;
}

void SoundVibration::Sub(float num)
{
    num_ = num_ - num;
}