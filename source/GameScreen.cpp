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
	this->m_score = 0;
}

GameScreen::~GameScreen()
{
	this->m_helper->SDL_DestroyTexture(this->m_background);

	this->m_gameBGM->End(this->m_helper);
	delete(m_gameBGM);

	this->m_helper->SDL_DestroySound(m_tapSFX);

}

void GameScreen::Start(SDL_Helper * helper)
{
	this->m_helper = helper;
	this->m_helper->SDL_LoadImage(&this->m_background, IMG_BACKGROUND);
	this->m_gameBGM = new MusicSound(this->m_helper, SND_BGM_GAME, true, 1);
	this->m_helper->SDL_LoadSound(&this->m_tapSFX, SND_SFX_TAP);

	this->m_gameBGM->Play(this->m_helper);
}

void GameScreen::EndGame()
{
	this->m_changeScene = true;
}

void GameScreen::Draw()
{
	this->m_helper->SDL_DrawImage(this->m_background, 0, 0);
}

void GameScreen::Update()
{
	if (this->m_changeScene)
		NextScene();
}

void GameScreen::CheckInputs(u64 kDown, u64 kHeld)
{

	if (kDown & KEY_TOUCH)
	{
		u32 i;
		hidTouchRead(&touch, i);

	}

	if (kDown & KEY_PLUS)
		EndGame();
}

// * We go to the next scene = GameScreen
void GameScreen::NextScene()
{	
	SceneManager::Instance()->LoadScene(SceneManager::SCENES::TITLE);
}
