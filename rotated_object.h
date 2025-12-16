#pragma once

class ObjectBase;

class RotatedObject : public ObjectBase
{
private:
	float rotate_speed_;

public:

	RotatedObject(const VECTOR& pos, const VECTOR& rot, const VECTOR& scale, const char* path,const float& speed);

	~RotatedObject() override;

	void SetDeltaTime() override;

	void Init() override;

	void Update() override;

	void Draw() override;

	void Debug() override;
};