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

	// enemies配列の中身をすべて削除する
	ResetArray();

	// enemyimages配列の中身をすべて削除する
	for (auto& img : enemyimages) {

		img->Release();
		img = nullptr;

	}

}


/*==== 敵画像の読み込み ====*/
void EnemyManager::ImportImages() {

	enemyimages[0] = image->TransferFromFile(L"src/images/alien_blue.png");
	enemyimages[1] = image->TransferFromFile(L"src/images/alien_green.png");
	enemyimages[2] = image->TransferFromFile(L"src/images/alien_pink.png");
	enemyimages[3] = image->TransferFromFile(L"src/images/alien_purple.png");
	enemyimages[4] = image->TransferFromFile(L"src/images/alien_yellow.png");

}


/*==== enemies配列の中身を初期化する====*/
void EnemyManager::ResetArray() {

	for (auto& array : enemies) {

		if (array != nullptr) {		// 要素が存在すれば...

			delete array;			// 削除する

		}

		array = nullptr;			// nullptrを入れる

	}

}



/*==== 敵の生成 ====*/
void EnemyManager::Generate(int min, int max) {


	// まず、配列の中身をすべて削除する
	ResetArray();

	eNum = 0;		// 敵の数も一度0にしておく


	// 次に乱数を用いて、敵の数を決める
	eNum = GenerateRandomInt(min, max);
	int row = 0;

	for (int i = 0; i < eNum; i++) {

		row = GenerateRandomInt(0, field->GetRow() - 1);		// どの行に出現させるか
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
	Enemyクラス
*/

Enemy::Enemy(D2D1_POINT_2F pos, ID2D1Bitmap* image, float width, ID2D1HwndRenderTarget* pRT) : pRT(pRT)
{
	// 基本情報の設定
	float spawnX = 50.0f + GenerateRandomFloat(width, width + (width / 5));		// 敵の初期出現位置のX座標
	info.image.size = 30.0f;					// 敵の大きさ
	info.image.x = pos.x + spawnX;				// 敵のx座標 
	info.image.y = pos.y - info.image.size;		// 敵のy座標
	info.image.pBitmap = image;					// 敵の画像データ
	info.hitbox = D2D1::RectF(					// 敵の当たり判定
		info.image.x - info.image.size / 2.0f,
		info.image.y - info.image.size / 2.0f,
		info.image.x + info.image.size / 2.0f,
		info.image.y + info.image.size / 2.0f
	);
	info.hp = 5;								// 敵の体力
	info.dx = GenerateRandomFloat(0.5f, 2.0f);	// 敵の移動量
	frag = true;								// 敵の生存フラグ
	isBumping = false;							// 敵にぶつかっているかのフラグ

}


Enemy::~Enemy() {


}




void Enemy::Move() {

	// 敵とぶつかっていなければ、移動
	if (frag) {

		if (!isBumping) {

			info.image.x -= info.dx;
			info.hitbox.left -= info.dx;		// 当たり判定も移動
			info.hitbox.right -= info.dx;

		}

	}

	/*==== 描画フラグがfalseの時の処理 ====*/
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
		D2D1_BITMAP_INTERPOLATION_MODE_LINEAR,		// 縮小時の補間モード
		D2D1::RectF(
			0.0f,									// 描画元の左上のX座標
			0.0f,								    // 描画元の左上のY座標
			info.image.pBitmap->GetSize().width,	// 描画元の右下のX座標
			info.image.pBitmap->GetSize().height	// 描画元の右下のY座標
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


		// 攻撃した後、少し後ろに下がる
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
