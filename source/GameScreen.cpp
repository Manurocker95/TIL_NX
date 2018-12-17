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
#include "Settings.h"
#include <cmath>


GameScreen::GameScreen() : Scene()
{
	this->m_changeScene = false;
	this->m_muted = false;
	this->m_dragging = false;
	this->m_score = 0;
	this->m_speedMultiplier = 1.f;
}

GameScreen::~GameScreen()
{
	this->m_helper->SDL_DestroyTexture(this->m_background);

	this->m_gameBGM->End(this->m_helper);
	delete(this->m_gameBGM);

	this->m_tapSFX->End(this->m_helper);
	delete(this->m_tapSFX);

	this->m_circle->End(this->m_helper);
	delete(this->m_helper);

	this->m_scoreText->End(this->m_helper);
	delete(this->m_scoreText);
}

void GameScreen::Start(SDL_Helper * helper)
{
	this->m_helper = helper;
	this->m_scoreText = new Text(helper, "Score: 0", 525, 20, 15, true, FONT_NORMAL, BLACK);
	this->m_helper->SDL_LoadImage(&this->m_background, IMG_BACKGROUND);
	this->m_circle = new Circle(Circle::CIRCLE_TYPE::BLUE, 200, 200, this->m_helper, IMG_BLUE_CIRCLE, 94, 93);
	this->m_gameBGM = new MusicSound(this->m_helper, SND_BGM_GAME, true, 1);
	this->m_tapSFX = new SfxSound(this->m_helper, SND_SFX_TAP, false, 2);
	this->m_gameBGM->Play(this->m_helper);
}

void GameScreen::EndGame()
{
	this->m_changeScene = true;
}

void GameScreen::Draw()
{
	this->m_helper->SDL_DrawImage(this->m_background, 0, 0);
	this->m_circle->Draw(this->m_helper);	
	this->m_scoreText->Draw(this->m_helper);
}

void GameScreen::Update()
{
	m_circle->Update();

	if (m_circle->GetValue() == Circle::CORRECT)
	{
		AddScore(m_circle);
	}
	else if (m_circle->GetValue() == Circle::WRONG)
	{
		this->m_circle->ResetValue();
		this->m_circle->SetActive(false);
		EndGame();
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
			this->m_circle->UpdateDrag(&this->touch);
			
			if (this->m_draggedCircle != NULL)
			{
				this->m_draggedCircle->UpdateDrag(&this->touch);
			}
			
		}
		else
		{
			if (this->m_circle->Touched(&this->touch))
			{
				this->m_tapSFX->Play(this->m_helper);
				this->m_draggedCircle = this->m_circle;
				this->m_dragging = true;
				this->m_circle->OnDrag(&this->touch);
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
	this->m_score += SCORE_TO_ADD;
	this->m_scoreText->SetText("Score: " + std::to_string(this->m_score));

	_circle->MoveToCoord(200, 200);
	_circle->ResetValue();
	
	this->m_dragging = false;
	this->m_draggedCircle->OnDrop();
	this->m_draggedCircle = NULL;
}