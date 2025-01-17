#include "game_system.h"




GameSystem::GameSystem(ID2D1HwndRenderTarget* pRenderTarget, ImageManager* image, AudioManager* audio) :
	pRenderTarget(pRenderTarget),
	image(image), audio(audio)
{
	time = 0;
	score = 0;
	waves = 1;
	resources = 1000;		// 初期資源の量
	brush = new Brush(pRenderTarget);
	field = new Field(pRenderTarget);
	enemy = new EnemyManager(pRenderTarget, field, image);
	character = new Character(pRenderTarget, image, field, resources);
	command = new Command(pRenderTarget, field->GetGameArea(), character->GetCharacterImages());
	base = new Base(pRenderTarget, brush);
	font = new Font(pRenderTarget, brush, character, command);

	bg = image->TransferFromFile(L"src/images/background_field.png");
	effect = image->TransferFromFile(L"src/images/boom.png");

}


GameSystem::~GameSystem() {
	// インスタンスへのポインタを持つ変数を宣言した場合、適切にメモリを開放することを忘れないように
	delete brush;
	delete field;
	delete enemy;
	delete command;
	delete base;
	delete font;
	delete character;


	brush = nullptr;
	field = nullptr;
	enemy = nullptr;
	command = nullptr;
	base = nullptr;
	font = nullptr;
	character = nullptr;
}


void GameSystem::Update() {

	enemy->Update();
	character->Update();
	resources = character->GetResource();		// resoucesの値を同期する
	EnemyOutOfScreen();
	CollisionCheck();

}


void GameSystem::Draw() {

	/*==== 背景の描画 ====*/
	pRenderTarget->DrawBitmap(
		bg,
		D2D1::RectF(0.0f, -125.0f, pRenderTarget->GetSize().width, pRenderTarget->GetSize().height)
	);


	/*==== ゲームフィールドの描画 ====*/
	field->DrawField(
		brush->GetBrushColor(White),
		brush->GetBrushColor(Black),
		brush->GetBrushColor(Black)
	);


	/*==== ドロップエリアの描画 ====*/
	if (command->GetIsSelected() != 999) {

		field->DrawCanDropArea(
			brush->GetBrushColor(Yellow),
			brush->GetBrushColor(Black)
		);

	}

	/*==== コマンドの描画 ====*/
	command->Draw(brush->GetBrushColor(Gray), brush->GetBrushColor(DodgeBlue));


	/*==== 敵の描画 ====*/
	enemy->Draw();


	/*==== テキストの描画 ====*/
	font->DrawALL(time, score, waves, resources);


	/*==== 自キャラの描画 ====*/
	character->Draw();

	image->Draw();		// 画像レンダリング

	if (hit != 0) {
		pRenderTarget->DrawBitmap(
			effect,
			D2D1::RectF(
				hitpoint.x,
				hitpoint.y,
				hitpoint.x + 20.0f,
				hitpoint.y + 20.0f
			)

		);
		hit--;
	}


	/*==== 自拠点のHPバーの描画 ====*/

	base->ShowHP();

}


void GameSystem::NowTime(int nowTime) {

	time = nowTime;

}


void GameSystem::CollisionCheck() {

	myCharacters = character->GetCharacters();
	enemies = enemy->GetEnemies();

	for (int i = 0; i < myCharacters.size(); i++) {
		for (int j = 0; j < myCharacters[0].size(); j++) {

			if (character->GetCharacterFlag(i, j)) {

				for (auto& enemy : enemies) {

					if (enemy != nullptr) {

						D2D1_RECT_F hitbox = myCharacters[i][j]->GetHitBox();
						eInfo& e = enemy->GetInfo();

						//-------------------------------------------------------------------------------------------


						// 体と体がぶつかったか
						if (hitbox.right >= e.hitbox.left && hitbox.left <= e.hitbox.right &&
							hitbox.top <= e.hitbox.bottom && hitbox.bottom >= e.hitbox.top) {
							enemy->isBumping = true;
							//audio->PlayWav(Hit);
														// もしぶつかっているならば、
							enemy->Attack(myCharacters[i][j]->GetStatus());
						}


						//-------------------------------------------------------------------------------------------


						// 敵と攻撃がぶつかったか
						if (!myCharacters[i][j]->GetAttackQue().empty()) {
							auto it = myCharacters[i][j]->GetAttackQue().begin();
							while (it != myCharacters[i][j]->GetAttackQue().end()) {
								auto& currentAttack = *it;
								D2D1_RECT_F AttackHitBox = currentAttack.hitbox;

								// 攻撃が当たった
								if (AttackHitBox.right >= e.hitbox.left && AttackHitBox.left <= e.hitbox.right &&
									AttackHitBox.top <= e.hitbox.bottom && AttackHitBox.bottom >= e.hitbox.top) {
									enemy->frag = false;
									score += 100;
									hit = 10;
									character->AddResource(200);
									hitpoint = D2D1::Point2F(AttackHitBox.right, AttackHitBox.top);
									it = myCharacters[i][j]->GetAttackQue().erase(it);

								}
								else {
									++it;
								}
							}
						}


						//-------------------------------------------------------------------------------------------

					}

				}







			}

		}
	}

}


void GameSystem::EnemyOutOfScreen() {

	for (auto& e : enemy->GetEnemies()) {

		// 敵が画面外に移動
		if (e != nullptr && e->frag) {

			if (e->GetInfo().image.x <= 0.0f) {

				e->GetInfo().image.x = -100.0f;
				e->GetInfo().hitbox = D2D1::RectF(0.0f, 0.0f, 0.0f, 0.0f);
				e->GetInfo().dx = 0.0f;

				e->frag = false;

				base->DecrementHP();	// baseのHPを減らす

			}

		}

	}

}




bool GameSystem::ClearCheck() {

	return enemy->ClearCheck();

}


void GameSystem::ContinueGame() {

	waves++;		// ウェーブを増やす

	int min = 1 + waves;	// 敵の最小数
	if (min >= 100) {
		min = 100;			// 100以上にはならないようにする
	}

	int max = min + 2 * waves;		// 敵の最大数
	if (max >= 100) {
		max = 100;				// 101以上にはならないようにする
	}

	enemy->Generate(min, max);

}



Brush* GameSystem::GetBrushInstance() {

	return brush;

}


Field* GameSystem::GetFieldInstance() {

	return field;

}


Command* GameSystem::GetCommandInstance() {

	return command;

}


Character* GameSystem::GetCharacterInstance() {

	return character;

}

Font* GameSystem::GetFontInstance() {

	return font;

}


Base* GameSystem::GetBaseInstance() {

	return base;

}


int GameSystem::GetScore() {

	return score;

}


int GameSystem::GetWaves() {

	return waves;

}