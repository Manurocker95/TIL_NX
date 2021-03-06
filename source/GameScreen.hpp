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
#ifndef _GAME_SCREEN_HPP_
#define _GAME_SCREEN_HPP_
#include <string>
#include "Scene.hpp"
#include "MusicSound.hpp"
#include "SfxSound.hpp"
#include "Circle.hpp"
#include "Text.hpp"
#include "Settings.h"

class GameScreen : public Scene
{

private:

	Circle * m_circles [MAX_CIRCLES];
	Circle * m_draggedCircle;

	Text * m_scoreText;
	Text * m_roundsText;
	SDL_Texture * m_background;

	MusicSound * m_gameBGM;
	SfxSound * m_tapSFX;

	bool m_dragging;
	bool m_muted;
	int m_score;
	int m_spawned;
	int m_maxSpawn;
	int m_solved;
	int m_rounds;
	float m_speedMultiplier;

	unsigned int m_currentTime;
	unsigned int m_lastTime;
	unsigned int m_timeToSpawn;
public:

	GameScreen();												// Constructor
	~GameScreen();												// Destructor
	void Start(SDL_Helper * helper) override;					// initialize
	void Draw() override;										// Draw
	void CheckInputs(u64 kDown, u64 kHeld, u64 kUp) override;	// CheckInput
	void Update() override;										// Update
	void NextScene() override;
	void EndGame();
	void AddScore(Circle * _circle);
	void Spawn();
};

#endif