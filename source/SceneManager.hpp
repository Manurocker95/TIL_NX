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
#ifndef _SCENE_MANAGER_H_
#define _SCENE_MANAGER_H_

#include <string>
#include <fstream>
#include <sstream>
#include "Scene.hpp"


class SceneManager
{
public:

	enum SCENES 
	{ 
		SPLASH, 
		TITLE,
		GAME, 
		LOADING
	};

private:					
	static SceneManager * instance;			// Singleton Instance
	Scene * m_actualScene;					// Scene running
	SDL_Helper * m_helper;					// SDL Helper instance

	int m_bestScore;						// Best score
	bool m_out;								// if we ended or not the program

public:
	static SceneManager * Instance();		// Property to get the singleton instance
	void LoadScene(SCENES _scene);
	void SetActualScene(SCENES _scene);		// Method to set a new scene (E.G: SplashScreen -> GameScreen)
	void Start(SDL_Helper * helper);		// Method for initialization
	void Update();							// Method called every frame. It calls to scene Draw, Update and CheckInput functions
	void Draw();							// Method called every frame. It calls to scene Draw, Update and CheckInput functions
	void CheckInputs();						// Method called every frame. It calls to scene Draw, Update and CheckInput functions
	bool IsOut();							// Method that returns if we are exiting the game from scene (normal exiting)
	void ExitGame(int _score = -1);			// Method to exit the game (Out = true)
	void ReadData();						// Method for reading data from our save
	void SaveData(int _val);
	void Exit();
	
	SDL_Helper * GetHelper();

private:
	SceneManager() {};						// Private so that it can  not be called
};

#endif