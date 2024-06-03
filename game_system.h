#pragma once

/*
	enemy�⎩�L�����Ȃǂ��`
	move��update�Adraw���̏����𐷂荞��
*/
#include <Windows.h>
#pragma comment(lib, "winmm.lib")

#include "brush.h"
#include "field.h"
#include "enemy.h"
#include "command.h"
#include "base.h"
#include "font.h"
#include "image_manager.h"
#include "audio_manager.h"

#include "character.h"

class GameSystem {

private:

	int time;
	int score;
	int waves;
	int resources;

	std::array<std::array<Person*, 3>, 5 > myCharacters;
	std::array<Enemy*, 100> enemies;


	// �N���X�̃C���X�^���X���L�q
	ID2D1HwndRenderTarget* pRenderTarget;

	Brush* brush;
	Field* field;
	EnemyManager* enemy;
	Command* command;
	Base* base;
	Font* font;
	ImageManager* image;
	Character* character;
	AudioManager* audio;

	ID2D1Bitmap* bg;		// 
	ID2D1Bitmap* effect;		// 
	int hit;
	D2D1_POINT_2F hitpoint;


public:

	GameSystem(ID2D1HwndRenderTarget*, ImageManager*, AudioManager*);
	~GameSystem();

	void Update();
	void Draw();

	void NowTime(int);		// �^�C�}�[�̃Z�b�g

	void CollisionCheck();
	bool ClearCheck();
	void ContinueGame();

	void EnemyOutOfScreen();

	/*==== �C���X�^���X�̃Q�b�^�[ ====*/
	Brush* GetBrushInstance();
	Field* GetFieldInstance();
	Command* GetCommandInstance();
	Character* GetCharacterInstance();
	Font* GetFontInstance();
	Base* GetBaseInstance();
	int GetScore();
	int GetWaves();

};