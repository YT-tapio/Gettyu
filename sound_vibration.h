#pragma once

class SoundVibration
{
private:

    const float kInitNum = 0.f;
    float num_; //オブジェクトが発しているサウンドの可視化

public:

    SoundVibration();

    ~SoundVibration();

    //毎回リセットすることを忘れずに
    void Reset();

    //何かあるたびにnumに足していく
    void Add(float num);

    //調整
    void Sub(float num);

    const float GetNum() const { return num_; }

};