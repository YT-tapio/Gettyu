#pragma once

class BaseObject;


class StillObject : public BaseObject
{
private:

	VECTOR scale_;

protected:



public:

	StillObject(VECTOR position,int model_handle,const float& scale);

	~StillObject() override;


	void Init(VECTOR position) override;


	void Update() override;


	void Draw() override;

};
