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


#include "GameScreen.hpp"
#include "SceneManager.hpp"
#include "Colors.h"
#include "Filepaths.h"
#include <cmath>
#include <ctime>
#include <cstdlib>

GameScreen::GameScreen() : Scene()
{
	this->m_changeScene = false;
	this->m_muted = false;
	this->m_dragging = false;
	this->m_score = 0;
	this->m_speedMultiplier = 1.f;
	this->m_spawned = 0;
	this->m_solved = 0;
	this->m_maxSpawn = 1;
	this->m_rounds = 1;
	this->m_currentTime = SDL_GetTicks();
	srand(time(NULL));
}

GameScreen::~GameScreen()
{
	this->m_helper->SDL_DestroyTexture(this->m_background);

	this->m_gameBGM->End(this->m_helper);
	delete(this->m_gameBGM);

	this->m_tapSFX->End(this->m_helper);
	delete(this->m_tapSFX);

	for (int i = 0; i < MAX_CIRCLES; i++)
	{
		this->m_circles[i]->End(this->m_helper);
		delete(this->m_circles[i]);
	}

	this->m_scoreText->End(this->m_helper);
	delete(this->m_scoreText);	
	
	this->m_roundsText->End(this->m_helper);
	delete(this->m_roundsText);
}

void GameScreen::Start(SDL_Helper * helper)
{
	this->m_helper = helper;
	this->m_scoreText = new Text(helper, "Score: 0", 525, 20, 15, true, FONT_NORMAL, BLACK);
	this->m_roundsText = new Text(helper, "Rounds: 0", 155, 20, 15, true, FONT_NORMAL, BLACK);
	this->m_helper->SDL_LoadImage(&this->m_background, IMG_BACKGROUND);
	this->m_gameBGM = new MusicSound(this->m_helper, SND_BGM_GAME, true, 1);
	this->m_tapSFX = new SfxSound(this->m_helper, SND_SFX_TAP, false, 2);
	this->m_gameBGM->Play(this->m_helper);

	int pos = SPAWN_MIN_X;
	int type = 0;
	for (int i = 0; i < MAX_CIRCLES; i++)
	{
		pos = rand() % SPAWN_MAX_X + SPAWN_MIN_X;
		type = rand() % 2 + 1;
		this->m_circles[i] = (type == 1) ? new Circle(Circle::CIRCLE_TYPE::BLUE, pos, -94, this->m_helper, IMG_BLUE_CIRCLE, 94, 93) : new Circle(Circle::CIRCLE_TYPE::RED, pos, -94, this->m_helper, IMG_RED_CIRCLE, 94, 93);
	}

	m_timeToSpawn = rand() % MAX_TIME_TO_SPAWN + MIN_TIME_TO_SPAWN;

	this->m_spawned = rand() % this->m_maxSpawn + 1; // Between 1 and Max
	this->m_solved = this->m_spawned;

	for (int i = 0; i < m_spawned; i++)
		this->m_circles[i]->SetFalling(true);
}

void GameScreen::EndGame()
{
	this->m_changeScene = true;
}

void GameScreen::Draw()
{
	this->m_helper->SDL_DrawImage(this->m_background, 0, 0);
	for (int i = 0; i < MAX_CIRCLES; i++)
	{
		this->m_circles[i]->Draw(this->m_helper);
	}
	this->m_scoreText->Draw(this->m_helper);
	this->m_roundsText->Draw(this->m_helper);
}

void GameScreen::Update()
{
	if (m_spawned > 0 && m_solved > 0)
	{
		for (int i = 0; i < MAX_CIRCLES; i++)
		{
			this->m_circles[i]->Update();

			if (this->m_circles[i]->GetValue() == Circle::CORRECT)
			{
				AddScore(this->m_circles[i]);
				break;
			}
			else if (this->m_circles[i]->GetValue() == Circle::WRONG)
			{
				this->m_circles[i]->ResetValue();
				this->m_circles[i]->SetActive(false);
				EndGame();
				break;
			}
		}
	}
	else
	{
		this->m_currentTime = SDL_GetTicks();

		if (m_currentTime >= m_timeToSpawn)
			Spawn();
	}

	if (this->m_changeScene)
		NextScene();
}

void GameScreen::CheckInputs(u64 kDown, u64 kHeld, u64 kUp)
{
	if (kHeld & KEY_TOUCH)
	{
		u32 i;
		hidTouchRead(&this->touch, i);

		if (this->m_dragging)
		{			
			if (this->m_draggedCircle != NULL)
			{
				this->m_draggedCircle->UpdateDrag(&this->touch);
			}		
		}
		else
		{
			for (int i = 0; i < MAX_CIRCLES; i++)
			{
				if (this->m_circles[i]->Touched(&this->touch))
				{
					this->m_tapSFX->Play(this->m_helper);
					this->m_draggedCircle = this->m_circles[i];
					this->m_dragging = true;
					this->m_circles[i]->OnDrag(&this->touch);
					break;
				}
			}
		}
	}
	else if (kUp & KEY_TOUCH)
	{
		u32 i;
		hidTouchRead(&this->touch, i);

		if (this->m_dragging)
		{
			if (this->m_draggedCircle != NULL)
			{
				this->m_draggedCircle->OnDrop();
				this->m_draggedCircle = NULL;
			}
		}

		this->m_dragging = false;
	}

	if (kDown & KEY_PLUS)
		EndGame();
}

// * We go to the next scene = GameScreen
void GameScreen::NextScene()
{	
	SceneManager::Instance()->SaveData(m_score);
	SceneManager::Instance()->LoadScene(SceneManager::SCENES::TITLE);
}

void GameScreen::AddScore(Circle * _circle)
{
	--this->m_solved;
	this->m_score += SCORE_TO_ADD;
	this->m_scoreText->SetText("Score: " + std::to_string(this->m_score));

	_circle->MoveToCoord(200, -94);
	_circle->ResetValue();
	
	this->m_dragging = false;
	this->m_draggedCircle->OnDrop();
	this->m_draggedCircle = NULL;
}

void GameScreen::Spawn()
{
	this->m_spawned = rand() % this->m_maxSpawn + 1; // Between 1 and Max
	this->m_solved = this->m_spawned;

	int pos = 200;
	for (int i = 0; i < m_spawned; i++)
	{
		pos = rand() % SPAWN_MAX_X + SPAWN_MIN_X;
		this->m_circles[i]->MoveToCoord(pos, -94);
		this->m_circles[i]->SetFalling(true);
	}
		

	// For the next time
	this->m_timeToSpawn = rand() % MAX_TIME_TO_SPAWN + MIN_TIME_TO_SPAWN;

	++this->m_rounds;

	if (this->m_rounds % ROUNDS_MULTIPLIER == 0 && m_maxSpawn + 1 <= MAX_CIRCLES)
		++m_maxSpawn;


	m_roundsText->SetText("Rounds: " + std::to_string(this->m_rounds));
}