#pragma once

struct MousePoint
{
	int x;
	int y;
};

class Brain
{
private:

	const int kMaxMouseDiff = 10.0f;

	ChangeType change_type_;

	VECTOR velocity_ = { 0,0,0 };
	VECTOR direction_ = { 0,0,0 };

	VECTOR pos_;
	VECTOR next_pos_;

	bool is_change_;

	float vertical_rad_ = 0.0f;
	float side_rad_ = 0.0f;

	float side_distance_ = 0.0f;
	float distance_ = 30.0f;

	float sensitivity_ = 10.5f;

	MousePoint now_mouse_pos_;
	MousePoint before_mouse_pos_;

	MousePoint dead_zone_;

	void MakeVertical(const VECTOR& pos);

	bool CheckMousePoint(MousePoint now_point, MousePoint before_point);

public:

	Brain();


	~Brain();

	
	void Update(const VECTOR& target_pos);


	/// <summary>
	/// ƒJƒƒ‰‚ª‹…‘Ìã‚É‰ñ‚éˆ—
	/// </summary>
	void SphereUpdate(const VECTOR& target_pos);


	void ChangeCamera();

	
	void SetRad(const VECTOR& target_pos, const VECTOR& player_pos);


	void SetPos(const VECTOR& pos, const VECTOR& next_pos, const ChangeType& change_type);

	const bool GetIsChange() const { return is_change_; }

	/// <summary>
	/// ‰¡‚Ì‰ñ“]—Ê‚ğæ“¾‚·‚é
	/// </summary>
	const float GetSideRad() const { return side_rad_; }


	const VECTOR GetVelocity() const { return velocity_; }

};