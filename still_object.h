#pragma once

class BaseObject;


class StillObject : public BaseObject
{
private:



protected:



public:

	StillObject(VECTOR position,int model_handle);

	~StillObject() override;


	void Init(VECTOR position) override;


	void Update() override;


	void Draw() override;

};
