#pragma once
#include"DxLib.h"
//#include"object_base.h"

class ObjectBase;
class Animation;
struct AnimationData;
enum class AnimationType;

class CharacterDance : public ObjectBase
{
private:

	std::shared_ptr<Animation> animation_;
	AnimationType type_;

	/// <summary>
	/// matrixのセッティングを行う
	/// </summary>
	void Setting();

public:

	/// <summary>
	/// 
	/// </summary>
	/// <param name="pos"></param>
	/// <param name="rot"></param>
	/// <param name="scale"></param>
	/// <param name="path"></param>
	/// <param name="data">アニメーションモデル抜きのデータが来るので、コンストラクタでの時に自分のモデルを入れてあげる</param>
	CharacterDance(const VECTOR& pos, const VECTOR& rot, const VECTOR& scale, const char* path,AnimationData& data);

	~CharacterDance() override;

	void SetDeltaTime() override;

	void Init() override;

	void Update()override;

	void Draw() override;

	void Debug() override;
};