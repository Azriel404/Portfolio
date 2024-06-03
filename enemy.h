#pragma once

#include <array>
#include <vector>
#include <d2d1.h>
#include "random_number.h"
#include "field.h"
#include "image_manager.h"
#include "character.h"

#pragma comment(lib, "d2d1.lib")

typedef struct EnemyInformation {

	ImageInfo image;
	D2D1_RECT_F hitbox;

	int hp;
	int atk;
	int def;

	float dx;
	bool flag;

} eInfo;

class Enemy {
private:

	eInfo info;

	float attackDuration;		// �U����������
	float cooldownDuration;		// �N�[���_�E������ 

	ID2D1HwndRenderTarget* pRT;



public:

	bool frag;			// �������Ă��邩�̃t���O
	bool isBumping;		// �G�ƏՓ˂��Ă��邩�̃t���O

	Enemy(D2D1_POINT_2F, ID2D1Bitmap*, float, ID2D1HwndRenderTarget*);
	~Enemy();

	void Move();
	void Draw();
	void Attack(Status*);
	void Bumping();

	int GetHP();
	eInfo& GetInfo();

};



class EnemyManager {

private:

	ID2D1HwndRenderTarget* pRT;
	Field* field;
	ImageManager* image;

	int eNum;

	std::array<ID2D1Bitmap*, 5> enemyimages;
	std::array<Enemy*, 100> enemies;

	void ImportImages();
	void ResetArray();

public:

	EnemyManager(ID2D1HwndRenderTarget*, Field*, ImageManager*);
	~EnemyManager();

	void Generate(int, int);
	void Update();
	void Draw();

	bool ClearCheck();

	std::array<Enemy*, 100>& GetEnemies();
	int GetEnemyNumber();

};