/* This file is part of TiL NX!

Copyright (C) 2018/2019 Manuel Rodríguez Matesanz
>    This program is free software: you can redistribute it and/or modify
>    it under the terms of the GNU General Public License as published by
>    the Free Software Foundation, either version 3 of the License, or
>    (at your option) any later version.
>
>    This program is distributed in the hope that it will be useful,
>    but WITHOUT ANY WARRANTY; without even the implied warranty of
>    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
>    GNU General Public License for more details.
>
>    You should have received a copy of the GNU General Public License
>    along with this program.  If not, see <http://www.gnu.org/licenses/>.
>    See LICENSE for information.
*/

#pragma once
#ifndef _SPRITE_HPP_
#define _SPRITE_HPP_

#include "SDL_Helper.hpp"

class Sprite
{
protected:
	SDL_Texture * m_sprite;
	int m_x;
	int m_y;
	int m_numFrames;
	int m_sizePerFrameX;
	int m_sizePerFrameY;
	int m_currentFrameX;
	int m_currentFrameY;
	bool m_multipleFrames;
	bool m_active;
	bool m_animated;

public:
	Sprite(int _x, int _y, SDL_Helper * _helper, char * _sprite, int m_numFrames, int _sizePerFrameX, int _sizePerFrameY, int _currentFrameX, int _currentFrameY, bool _multipleFrames, bool _animated);
	~Sprite() {};
	virtual void MoveX(int _value);
	virtual void MoveY(int _value);
	virtual void MoveToCoord(int _x, int _y);

	virtual int GetX();
	virtual void SetX(int _value);
	virtual int GetY();
	virtual void SetY(int _value);

	virtual SDL_Texture * GetSprite();
	virtual void SetSprite(SDL_Texture * _sprite);

	virtual void Draw(SDL_Helper * _helper);
	virtual void Update();
	virtual void End(SDL_Helper * _helper);

	virtual void SetNumFrames(int _value);
	virtual int GetNumFrames();
	virtual void SetFrameSize(int _value, bool _xAxis);
	virtual int GetFrameSize(bool _xAxis);

	virtual bool Touched(touchPosition * _touch);
	virtual bool IsActive();
	virtual void SetActive(bool _value);
};
#endif