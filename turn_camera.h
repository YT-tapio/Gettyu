#pragma once



class TurnCamera
{
private:


	VECTOR position_;
	ChangeType change_type;

public:

	TurnCamera(const VECTOR& pos);

	~TurnCamera();


	void Init(const VECTOR& pos);


	void Update();


};
