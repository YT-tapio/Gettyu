#pragma once

class Effect;

class HitEffect
{
private:

	VECTOR pos_;		// ƒ|ƒWƒVƒ‡ƒ“
	VECTOR scale_;		// ‘å‚«‚³
	VECTOR rot_;		// ‰ñ“]

	std::shared_ptr<Effect> effect_;

public:

	HitEffect();

	~HitEffect();

	void SetDeltaTime();

	void Update();

};