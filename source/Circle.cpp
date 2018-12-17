#include "Circle.hpp"
#include "Settings.h"

Circle::Circle(CIRCLE_TYPE _type, int _x, int _y, SDL_Helper * _helper, char * _sprite, int _sizePerFrameX, int _sizePerFrameY) : Sprite (_x, _y, _helper, _sprite, 1, _sizePerFrameX, _sizePerFrameY, 0, 0, false, false)
{
	this->m_type = _type;
	this->m_dragging = false;
	this->m_falling = false;
	this->m_value = Circle::CIRCLE_VALUE::NOTHING;
	this->m_speed = 1.f;
}


Circle::~Circle()
{

}

void Circle::UpdateDrag(touchPosition * _pos)
{
	if (!this->m_active)
	{
		return;
	}
	this->m_x = _pos->px - (m_sizePerFrameX / 2);
	this->m_y = _pos->py - (m_sizePerFrameY / 2);
}

void Circle::OnDrag(touchPosition  *_pos)
{
	if (!this->m_active)
	{
		return;
	}
	this->m_x = _pos->px - (m_sizePerFrameX / 2);
	this->m_y = _pos->py - (m_sizePerFrameX / 2);
}

void  Circle::OnDrop()
{
	if (!this->m_active)
	{
		return;
	}
	this->m_dragging = false;
}

void Circle::Update()
{
	if (!this->m_active)
	{
		return;
	}

	if (!this->m_dragging && this->m_falling)
	{
		this->m_y += CIRCLE_DISPLACEMENT * m_speed;
	}	
	
	if (this->m_y > SWITCH_SCREEN_HEIGHT + this->m_sizePerFrameY)
		this->m_value = CIRCLE_VALUE::WRONG;
	else
		CheckCorrect();
}

void Circle::CheckCorrect()
{
	if (this->m_type == CIRCLE_TYPE::RED)
	{
		if (this->m_x <= MIN_POS_RED)
		{
			this->m_value = CIRCLE_VALUE::CORRECT;
		}
		else if (this->m_x >= MIN_POS_BLUE)
		{
			this->m_value = CIRCLE_VALUE::WRONG;
		}
	}
	else // BLUE
	{
		if (this->m_x <= MIN_POS_RED)
		{
			this->m_value = CIRCLE_VALUE::WRONG;
		}
		else if (this->m_x >= MIN_POS_BLUE)
		{
			this->m_value = CIRCLE_VALUE::CORRECT;
		}
	}
}

Circle::CIRCLE_VALUE Circle::GetValue()
{
	return this->m_value;
}

void Circle::SetValue(Circle::CIRCLE_VALUE _value)
{
	this->m_value = _value;
}

void Circle::ResetValue()
{
	this->m_value = Circle::CIRCLE_VALUE::NOTHING;
	this->m_falling = false;
}

void Circle::SetSpeed(float _value)
{
	this->m_speed = _value;
}

bool Circle::GetFalling()
{
	return this->m_falling;
}

void Circle::SetFalling(bool _value)
{
	this->m_falling = _value;
}