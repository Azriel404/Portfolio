#include "enemy.h"

/*=====================
		private
=====================*/

EnemyManager::EnemyManager(ID2D1HwndRenderTarget* pRT, Field* field, ImageManager* image) :
	pRT(pRT), field(field), image(image)
{

	ImportImages();
	Generate(1, 5);

}



EnemyManager::~EnemyManager() {

	// enemies�z��̒��g�����ׂč폜����
	ResetArray();

	// enemyimages�z��̒��g�����ׂč폜����
	for (auto& img : enemyimages) {

		img->Release();
		img = nullptr;

	}

}


/*==== �G�摜�̓ǂݍ��� ====*/
void EnemyManager::ImportImages() {

	enemyimages[0] = image->TransferFromFile(L"src/images/alien_blue.png");
	enemyimages[1] = image->TransferFromFile(L"src/images/alien_green.png");
	enemyimages[2] = image->TransferFromFile(L"src/images/alien_pink.png");
	enemyimages[3] = image->TransferFromFile(L"src/images/alien_purple.png");
	enemyimages[4] = image->TransferFromFile(L"src/images/alien_yellow.png");

}


/*==== enemies�z��̒��g������������====*/
void EnemyManager::ResetArray() {

	for (auto& array : enemies) {

		if (array != nullptr) {		// �v�f�����݂����...

			delete array;			// �폜����

		}

		array = nullptr;			// nullptr������

	}

}



/*==== �G�̐��� ====*/
void EnemyManager::Generate(int min, int max) {


	// �܂��A�z��̒��g�����ׂč폜����
	ResetArray();

	eNum = 0;		// �G�̐�����x0�ɂ��Ă���


	// ���ɗ�����p���āA�G�̐������߂�
	eNum = GenerateRandomInt(min, max);
	int row = 0;

	for (int i = 0; i < eNum; i++) {

		row = GenerateRandomInt(0, field->GetRow() - 1);		// �ǂ̍s�ɏo�������邩
		D2D1_POINT_2F pos = field->GetCenterPosition(row, 0);
		ID2D1Bitmap* eImage = enemyimages[GenerateRandomInt(0, enemyimages.size() - 1)];
		float width = pRT->GetSize().width;

		enemies[i] = new Enemy(pos, eImage, width, pRT);

	}

}


void EnemyManager::Update() {

	for (int i = 0; i < eNum; i++) {

		enemies[i]->Move();

	}

}


void EnemyManager::Draw() {

	for (int i = 0; i < eNum; i++) {

		enemies[i]->Draw();

	}

}


bool EnemyManager::ClearCheck() {

	int count = 0;

	for (auto& e : enemies) {

		if (e != nullptr && !e->frag) {
			count++;
		}

	}

	if (count == eNum) {
		return true;
	}
	else {
		count = 0;
		return false;
	}

}


std::array<Enemy*, 100>& EnemyManager::GetEnemies() {

	return enemies;

}


int EnemyManager::GetEnemyNumber() {

	return eNum;

}





/*
	Enemy�N���X
*/

Enemy::Enemy(D2D1_POINT_2F pos, ID2D1Bitmap* image, float width, ID2D1HwndRenderTarget* pRT) : pRT(pRT)
{
	// ��{���̐ݒ�
	float spawnX = 50.0f + GenerateRandomFloat(width, width + (width / 5));		// �G�̏����o���ʒu��X���W
	info.image.size = 30.0f;					// �G�̑傫��
	info.image.x = pos.x + spawnX;				// �G��x���W 
	info.image.y = pos.y - info.image.size;		// �G��y���W
	info.image.pBitmap = image;					// �G�̉摜�f�[�^
	info.hitbox = D2D1::RectF(					// �G�̓����蔻��
		info.image.x - info.image.size / 2.0f,
		info.image.y - info.image.size / 2.0f,
		info.image.x + info.image.size / 2.0f,
		info.image.y + info.image.size / 2.0f
	);
	info.hp = 5;								// �G�̗̑�
	info.dx = GenerateRandomFloat(0.5f, 2.0f);	// �G�̈ړ���
	frag = true;								// �G�̐����t���O
	isBumping = false;							// �G�ɂԂ����Ă��邩�̃t���O

}


Enemy::~Enemy() {


}




void Enemy::Move() {

	// �G�ƂԂ����Ă��Ȃ���΁A�ړ�
	if (frag) {

		if (!isBumping) {

			info.image.x -= info.dx;
			info.hitbox.left -= info.dx;		// �����蔻����ړ�
			info.hitbox.right -= info.dx;

		}

	}

	/*==== �`��t���O��false�̎��̏��� ====*/
	else if (!frag) {

		info.image.x = -100.0f;
		info.hitbox = D2D1::RectF(0.0f, 0.0f, 0.0f, 0.0f);
		info.dx = 0.0f;
		isBumping = false;

	}

}

void Enemy::Draw() {

	pRT->DrawBitmap(
		info.image.pBitmap,
		D2D1::RectF(
			info.image.x,
			info.image.y,
			info.image.x + info.image.size,
			info.image.y + info.image.size
		),
		1.0f,
		D2D1_BITMAP_INTERPOLATION_MODE_LINEAR,		// �k�����̕�ԃ��[�h
		D2D1::RectF(
			0.0f,									// �`�挳�̍����X���W
			0.0f,								    // �`�挳�̍����Y���W
			info.image.pBitmap->GetSize().width,	// �`�挳�̉E����X���W
			info.image.pBitmap->GetSize().height	// �`�挳�̉E����Y���W
		)

	);

}


void Enemy::Attack(Status* stat) {

	if (isBumping && cooldownDuration <= 0.0f) {

		//(stat->HP) -= (stat->DEF) - info.atk;
		(stat->HP) -= 20;

		if ((stat->HP) <= 0) {
			(stat->HP) = 0;
		}


		// �U��������A�������ɉ�����
		float stepback = 30.0f;
		info.image.x += stepback;
		info.hitbox.left += stepback;
		info.hitbox.right += stepback;

		isBumping = false;
		cooldownDuration = 2.0f * 60.0f;

	}

	else if (cooldownDuration >= 0.0f) {

		cooldownDuration--;

	}

}


void Enemy::Bumping() {

	isBumping = !isBumping;

}



eInfo& Enemy::GetInfo() {

	return info;

}


int Enemy::GetHP() {

	if (info.hp <= 0) {

		info.hp = 0;

	}

	return info.hp;

}
