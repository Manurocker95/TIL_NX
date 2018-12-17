#pragma once
#ifndef _CIRCLE_HPP_
#define _CIRCLE_HPP_

#include "Sprite.hpp"

class Circle : public Sprite
{
public:

	enum CIRCLE_TYPE
	{
		RED,
		BLUE
	};

	enum CIRCLE_VALUE
	{
		NOTHING,
		CORRECT,
		WRONG
	};

private:

	CIRCLE_TYPE m_type;
	bool m_dragging;
	float m_speed;
	CIRCLE_VALUE m_value;

public:

	Circle(CIRCLE_TYPE _type, int _x, int _y, SDL_Helper * _helper, char * _sprite, int _sizePerFrameX, int _sizePerFrameY);
	~Circle();

	void Update() override;
	void OnDrag(touchPosition * _pos);
	void UpdateDrag(touchPosition * _pos);
	void OnDrop();
	void CheckCorrect();
	CIRCLE_VALUE GetValue();
	void SetValue(CIRCLE_VALUE _value);
	void ResetValue();
	void SetSpeed(float _value);
};

#endif
