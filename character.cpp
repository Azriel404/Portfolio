#include "character.h"


/*====================
	Charcterクラス
====================*/

Character::Character(ID2D1HwndRenderTarget* pRenderTarget, ImageManager* imageManager, Field* field, int r) :
	pRT(pRenderTarget), img(imageManager), field(field), resource(r)
{

	myChar = {};				// 配列を初期化
	TransferToBitmap();			// 画像をビットマップに変換
	DetermineCharacterCost();	// キャラクターのコストを決定する

}

Character::~Character() {

}

// 画像をビットマップに変換
void Character::TransferToBitmap() {

	images[HERO] = img->TransferFromFile(L"src/images/hero.png");
	images[WARRIOR] = img->TransferFromFile(L"src/images/warrior.png");
	images[KNIGHT] = img->TransferFromFile(L"src/images/night.png");
	images[PRIEST] = img->TransferFromFile(L"src/images/priest.png");
	images[MAGE] = img->TransferFromFile(L"src/images/mage.png");

}


void Character::DetermineCharacterCost() {

	cost[HERO] = 300;
	cost[WARRIOR] = 150;
	cost[KNIGHT] = 200;
	cost[PRIEST] = 400;
	cost[MAGE] = 500;

}


// 生成処理
void Character::Generate(float mouseXpos, float mouseYpos, int isSelect) {

	auto& canDropArea = field->GetCanDropArea();

	D2D1_POINT_2F center;
	float distance;
	float radius = 30.0f;


	for (int i = 0; i < canDropArea.size(); i++) {

		for (int j = 0; j < canDropArea[0].size(); j++) {

			center = field->GetCenterPosition(i, j);
			center.y -= 15.0f;
			distance = sqrt((mouseXpos - center.x) * (mouseXpos - center.x) + (mouseYpos - center.y) * (mouseYpos - center.y));

			if (distance <= radius && canDropArea[i][j].frag) {

				switch (isSelect) {

				case HERO:

					if (CanGenerate(resource, cost[HERO])) {
						myChar[i][j] = new Hero(pRT, images[HERO], center, i, j);
						resource -= cost[HERO];
						canDropArea[i][j].frag = false;		// キャラクターを設置したため、フラグ変数をfalseへ
					}


					break;


				case WARRIOR:

					if (CanGenerate(resource, cost[WARRIOR])) {
						myChar[i][j] = new Warrior(pRT, images[WARRIOR], center, i, j);
						resource -= cost[WARRIOR];
						canDropArea[i][j].frag = false;		// キャラクターを設置したため、フラグ変数をfalseへ
					}

					break;


				case KNIGHT:

					if (CanGenerate(resource, cost[KNIGHT])) {
						myChar[i][j] = new Knight(pRT, images[KNIGHT], center, i, j);
						resource -= cost[KNIGHT];
						canDropArea[i][j].frag = false;		// キャラクターを設置したため、フラグ変数をfalseへ
					}

					break;


				case PRIEST:

					if (CanGenerate(resource, cost[PRIEST])) {
						myChar[i][j] = new Priest(pRT, images[PRIEST], center, i, j, field);
						resource -= cost[PRIEST];
						canDropArea[i][j].frag = false;		// キャラクターを設置したため、フラグ変数をfalseへ
					}

					break;


				case MAGE:

					if (CanGenerate(resource, cost[MAGE])) {
						myChar[i][j] = new Mage(pRT, images[MAGE], center, i, j);
						resource -= cost[MAGE];
						canDropArea[i][j].frag = false;		// キャラクターを設置したため、フラグ変数をfalseへ
					}

					break;

				}

			}

		}

	}

}


bool Character::CanGenerate(int resource, int cost) {

	if (resource - cost >= 0) {
		return true;
	}

	else {
		return false;
	}

}





// 更新処理
void Character::Update() {

	for (int i = 0; i < myChar.size(); i++) {

		for (int j = 0; j < myChar[0].size(); j++) {

			if (GetCharacterFlag(i, j)) {

				myChar[i][j]->Attack();
				myChar[i][j]->Update();

			}

		}

	}

}


// 描画処理
void Character::Draw() {

	for (int i = 0; i < myChar.size(); i++) {

		for (int j = 0; j < myChar[0].size(); j++) {

			if (myChar[i][j] != NULL) {

				myChar[i][j]->Draw();

			}

		}

	}

}


// アクセッサ
std::array<std::array<Person*, 3>, 5>& Character::GetCharacters() {

	return myChar;

}


bool Character::GetCharacterFlag(int row, int column) {

	if (myChar[row][column] != nullptr) {		// nullptrではないか？　=　データが存在するか？

		return true;

	}
	else {

		return false;

	}
}


std::array<ID2D1Bitmap*, 5>& Character::GetCharacterImages() {

	return images;

}


std::array<int, 5>& Character::GetCost() {

	return cost;

}


void Character::AddResource(int r) {

	resource += r;

}


int Character::GetResource() {

	return resource;

}


/*==================
	Personクラス
==================*/

Person::Person(ID2D1HwndRenderTarget* pRenderTarget, ID2D1Bitmap* img, D2D1_POINT_2F center, int r, int c) {

	image.pBitmap = img;		// ビットマップへのポインタ
	image.size = 40.0f;			// ビットマップの表示サイズ
	image.x = center.x - image.size / 2.0f;		// ビットマップの左上のX座標
	image.y = center.y - image.size / 2.0f;		// ビットマップの左上のY座標
	hitbox = D2D1::RectF(center.x - image.size / 2.0f, center.y - image.size / 2.0f, center.x + image.size / 2.0f, center.y + image.size / 2.0f);	// 当たり判定
	pos = center;		// 自キャラの座標
	row = r;
	column = c;
	flag = true;		// 表示フラグ

	status = {};		// 空配列で初期化
	pRT = pRenderTarget;	// レンダーターゲットのセット

}

Person::~Person() {}		// デストラクタ(仮想)
void Person::Attack() {}	// Attack(仮想)
void Person::Update() {}
void Person::Draw() {}

D2D1_RECT_F Person::GetHitBox() {

	return hitbox;

}


std::vector<AttackInfo>& Person::GetAttackQue() {

	return attack;

}


Status* Person::GetStatus() {

	return &status;

}



/*================
	Heroクラス
================*/

Hero::Hero(ID2D1HwndRenderTarget* pRT, ID2D1Bitmap* img, D2D1_POINT_2F center, int row, int column) :
	Person(pRT, img, center, row, column)
{

	// ユニーク変数の初期化
	job = "Hero";
	status = {
		50,		// HP
		30,		// MP
		50,		// 攻撃力
		50,		// 防御力
		50,		// 敏捷性
	};

	pB = new Brush(pRT);

	attackDuration = 60.0f;
	attackHitBox = 30.0f;
	cooldownDuration = 20.0f;		// 最初に短めのクールダウンを設ける

}


Hero::~Hero() {

}


void Hero::Attack() {

	if (cooldownDuration <= 0.0f) {

		attack.push_back(
			{
				job,
				D2D1::RectF(pos.x, pos.y - attackHitBox / 2.0f, pos.x + (attackHitBox * 2.0f), pos.y + attackHitBox),
				D2D1::RectF(pos.x, pos.y - attackHitBox / 2.0f, pos.x + (attackHitBox * 2.0f), pos.y + attackHitBox),
				true,
				attackDuration
			}
		);

		cooldownDuration = 3.0f * 60.0f;

	}

	// クールダウンが残っていれば...
	else {

		cooldownDuration--;		// クールダウンを減らす

	}

}


void Hero::Update() {

	// 攻撃キューに攻撃がなかったら...
	if (attack.empty()) {
		return;		// 何もしない
	}

	// 攻撃キューに攻撃があったら...
	auto it = attack.begin();			// イテレータを初期化
	while (it != attack.end()) {		// 配列に要素がある限り繰り返す
		auto& currentAttack = *it;

		if (currentAttack.displayTime <= 0.0f || currentAttack.attack.left >= pRT->GetSize().width) {

			it = attack.erase(it);  // currentAttackを削除し、イテレータを更新

		}
		else {

			currentAttack.displayTime--;		// 描画時間を減らす
			++it;  // イテレータを次に進める

		}

	}

}


void Hero::Draw() {

	pRT->DrawBitmap(
		image.pBitmap,
		D2D1::RectF(
			image.x,
			image.y,
			image.x + image.size,
			image.y + image.size
		)
	);


	if (!attack.empty()) {

		for (auto& currentAttack : attack) {

			pRT->FillRectangle(currentAttack.attack, pB->GetBrushColor(Red));

		}

	}

}

/*===================
	Warriorクラス
===================*/

Warrior::Warrior(ID2D1HwndRenderTarget* pRT, ID2D1Bitmap* img, D2D1_POINT_2F center, int row, int column) :
	Person(pRT, img, center, row, column)
{

	// ユニーク変数の初期化
	job = "Warrior";
	status = {
		70,
		20,
		80,
		30,
		30
	};

	pB = new Brush(pRT);

	attackDuration = 60.0f;
	attackHitBox = 20.0f;
	cooldownDuration = 20.0f;		// 最初に短めのクールダウンを設ける

}


Warrior::~Warrior() {

}

void Warrior::Attack() {

	if (cooldownDuration <= 0.0f) {

		attack.push_back(
			{
				job,
				D2D1::RectF(pos.x, pos.y - attackHitBox / 2.0f, pos.x + (attackHitBox * 2.0f), pos.y + attackHitBox),
				D2D1::RectF(pos.x, pos.y - attackHitBox / 2.0f, pos.x + (attackHitBox * 2.0f), pos.y + attackHitBox),
				true,
				attackDuration
			}
		);

		cooldownDuration = 4.0f * 60.0f;

	}

	// クールダウンが残っていれば...
	else {

		cooldownDuration--;		// クールダウンを減らす

	}

}

void Warrior::Update() {

	// 攻撃キューに攻撃がなかったら...
	if (attack.empty()) {
		return;		// 何もしない
	}

	// 攻撃キューに攻撃があったら...
	auto it = attack.begin();			// イテレータを初期化
	while (it != attack.end()) {		// 配列に要素がある限り繰り返す
		auto& currentAttack = *it;

		if (currentAttack.displayTime <= 0.0f || currentAttack.attack.left >= pRT->GetSize().width) {

			it = attack.erase(it);  // currentAttackを削除し、イテレータを更新

		}
		else {

			currentAttack.displayTime--;		// 描画時間を減らす
			++it;  // イテレータを次に進める

		}

	}

}


void Warrior::Draw() {

	pRT->DrawBitmap(
		image.pBitmap,
		D2D1::RectF(
			image.x,
			image.y,
			image.x + image.size,
			image.y + image.size
		)
	);


	if (!attack.empty()) {

		for (auto& currentAttack : attack) {

			pRT->FillRectangle(currentAttack.attack, pB->GetBrushColor(Red));

		}

	}
}


/*==================
	Knightクラス
==================*/

Knight::Knight(ID2D1HwndRenderTarget* pRT, ID2D1Bitmap* img, D2D1_POINT_2F center, int row, int column) :
	Person(pRT, img, center, row, column)
{

	// ユニーク変数の初期化
	job = "Knight";
	status = {
		70,
		40,
		30,
		80,

	};

	pB = new Brush(pRT);

	attackDuration = 30.0f;
	attackHitBox = 30.0f;
	cooldownDuration = 20.0f;		// 最初に短めのクールダウンを設ける

}

Knight::~Knight() {

}


void Knight::Attack() {

	if (cooldownDuration <= 0.0f) {

		attack.push_back(
			{
				job,
				D2D1::RectF(pos.x, pos.y - attackHitBox / 2.0f, pos.x + (attackHitBox * 2.0f), pos.y + attackHitBox),
				D2D1::RectF(pos.x, pos.y - attackHitBox / 2.0f, pos.x + (attackHitBox * 2.0f), pos.y + attackHitBox),
				true,
				attackDuration
			}
		);

		cooldownDuration = 1.5f * 60.0f;

	}

	// クールダウンが残っていれば...
	else {

		cooldownDuration--;		// クールダウンを減らす

	}

}

void Knight::Update() {

	// 攻撃キューに攻撃がなかったら...
	if (attack.empty()) {
		return;		// 何もしない
	}

	// 攻撃キューに攻撃があったら...
	auto it = attack.begin();			// イテレータを初期化
	while (it != attack.end()) {		// 配列に要素がある限り繰り返す
		auto& currentAttack = *it;

		if (currentAttack.displayTime <= 0.0f) {

			it = attack.erase(it);  // currentAttackを削除し、イテレータを更新

		}
		else {

			currentAttack.displayTime--;		// 描画時間を減らす
			++it;  // イテレータを次に進める

		}

	}

}


void Knight::Draw() {

	pRT->DrawBitmap(
		image.pBitmap,
		D2D1::RectF(
			image.x,
			image.y,
			image.x + image.size,
			image.y + image.size
		)
	);


	if (!attack.empty()) {

		for (auto& currentAttack : attack) {

			pRT->FillRectangle(currentAttack.attack, pB->GetBrushColor(Red));

		}

	}
}


/*==================
	Priestクラス
==================*/

Priest::Priest(ID2D1HwndRenderTarget* pRT, ID2D1Bitmap* img, D2D1_POINT_2F center, int row, int column, Field* f) :
	Person(pRT, img, center, row, column), f(f)
{

	// ユニーク変数の初期化
	job = "Priest";
	status = {
		50,
		70,
		10,
		40,
		50
	};

	pB = new Brush(pRT);

	attackDuration = 60.0f;
	attackHitBox = 20.0f;
	cooldownDuration = 20.0f;		// 最初に短めのクールダウンを設ける

}


Priest::~Priest() {

}



void Priest::Attack() {

	if (cooldownDuration <= 0.0f) {

		D2D1_RECT_F healArea;
		D2D1_POINT_2F p1 = f->GetCenterPosition(0, column);

		// 自分の列の前後を回復する
		healArea = D2D1::RectF(
			f->GetCenterPosition(row, 0).x - attackHitBox,
			f->GetCenterPosition(row, 0).y - attackHitBox,
			f->GetCenterPosition(row, 2).x + attackHitBox,
			f->GetCenterPosition(row, 2).y + attackHitBox
		);


		attack.push_back(
			{
				job,
				healArea,
				healArea,
				true,
				attackDuration
			}
		);

		cooldownDuration = 5.0f * 60.0f;

	}

	// クールダウンが残っていれば...
	else {

		cooldownDuration--;		// クールダウンを減らす

	}

}

void Priest::Update() {

	// 攻撃キューに攻撃がなかったら...
	if (attack.empty()) {
		return;		// 何もしない
	}

	// 攻撃キューに攻撃があったら...
	if (!attack.empty()) {
		auto it = attack.begin();			// イテレータを初期化

		auto& currentAttack = *it;			// 先頭の要素を取り出す	

		if (currentAttack.displayTime <= 0.0f) {

			it = attack.erase(it);  // currentAttackを削除し、イテレータを更新

		}
		else {

			it->isAttacking = false;			// 回復は一度だけ起きる : 攻撃中かのフラグをfalseにする
			currentAttack.displayTime--;		// 描画時間を減らす
			++it;  // イテレータを次に進める
		}

	}

}


void Priest::Draw() {

	if (!attack.empty()) {

		for (auto& currentAttack : attack) {

			pRT->FillRectangle(currentAttack.attack, pB->GetBrushColor(Red));

		}

	}

	pRT->DrawBitmap(
		image.pBitmap,
		D2D1::RectF(
			image.x,
			image.y,
			image.x + image.size,
			image.y + image.size
		)
	);

}


/*================
	Mageクラス
================*/

Mage::Mage(ID2D1HwndRenderTarget* pRT, ID2D1Bitmap* img, D2D1_POINT_2F center, int row, int column) :
	Person(pRT, img, center, row, column)
{

	// ユニーク変数の初期化
	job = "Mage";
	status = {
		40,
		80,
		80,
		30,
		40
	};

	pB = new Brush(pRT);

	attackDuration = 999.9f;
	attackHitBox = 20.0f;
	cooldownDuration = 20.0f;		// 最初に短めのクールダウンを設ける

}


Mage::~Mage() {

	delete pB;
	pB = nullptr;

}



void Mage::Attack() {

	// クールダウンが終了したら...
	if (cooldownDuration <= 0.0f) {

		attack.push_back(
			{
				job,
				D2D1::RectF(pos.x - attackHitBox / 2.0f, pos.y - attackHitBox / 2.0f, pos.x + attackHitBox / 2.0f, pos.y + attackHitBox / 2.0f),
				D2D1::RectF(pos.x - attackHitBox / 2.0f, pos.y - attackHitBox / 2.0f, pos.x + attackHitBox / 2.0f, pos.y + attackHitBox / 2.0f),
				true,
				attackDuration
			}
		);

		cooldownDuration = 60.0f;		// クールダウンを300フレーム(5秒)に設定

	}

	// クールダウンが残っていれば...
	else {

		cooldownDuration--;		// クールダウンを減らす

	}

}


void Mage::Update() {

	// 攻撃キューに攻撃がなかったら...
	if (attack.empty()) {
		return;		// 何もしない
	}

	// 攻撃キューに攻撃があったら...
	auto it = attack.begin();			// イテレータを初期化
	while (it != attack.end()) {		// 配列に要素がある限り繰り返す
		auto& currentAttack = *it;

		currentAttack.attack.left += 10.0f;   // 攻撃を動かす
		currentAttack.attack.right += 10.0f;
		currentAttack.hitbox.left += 10.0f;   // 当たり判定を動かす
		currentAttack.hitbox.right += 10.0f;

		if (currentAttack.displayTime <= 0.0f || currentAttack.attack.left >= pRT->GetSize().width) {

			it = attack.erase(it);  // currentAttackを削除し、イテレータを更新

		}
		else {

			currentAttack.displayTime--;		// 描画時間を減らす
			++it;  // イテレータを次に進める

		}

	}

}


void Mage::Draw() {

	pRT->DrawBitmap(
		image.pBitmap,
		D2D1::RectF(
			image.x,
			image.y,
			image.x + image.size,
			image.y + image.size
		)
	);


	if (!attack.empty()) {

		for (auto& currentAttack : attack) {

			pRT->FillRectangle(currentAttack.attack, pB->GetBrushColor(Red));

		}

	}

}



/*
	キャラ方針

	Hero:バランス型。器用貧乏。
	Warrior:攻撃型。防御がやや低く、攻撃速度もやや遅いが、高火力
	Knight:防御型。やや攻撃力は低めだが、高い防御力と相手を押し戻す攻撃で鉄壁の守りを実現
	Priest:支援型。攻撃手段はないが、周囲の味方をサポートする。体力の回復や、攻撃のCDを下げるなど
	Mage:遠距離型。高いMPと攻撃力を生かし、遠距離から魔法を打つ。しかし、体力と防御力が低め。

	ステータスはとりあえず　50　を基準とする。

*/