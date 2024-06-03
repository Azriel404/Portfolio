#include "game_system.h"




GameSystem::GameSystem(ID2D1HwndRenderTarget* pRenderTarget, ImageManager* image, AudioManager* audio) :
	pRenderTarget(pRenderTarget),
	image(image), audio(audio)
{
	time = 0;
	score = 0;
	waves = 1;
	resources = 1000;		// ���������̗�
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
	// �C���X�^���X�ւ̃|�C���^�����ϐ���錾�����ꍇ�A�K�؂Ƀ��������J�����邱�Ƃ�Y��Ȃ��悤��
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
	resources = character->GetResource();		// resouces�̒l�𓯊�����
	EnemyOutOfScreen();
	CollisionCheck();

}


void GameSystem::Draw() {

	/*==== �w�i�̕`�� ====*/
	pRenderTarget->DrawBitmap(
		bg,
		D2D1::RectF(0.0f, -125.0f, pRenderTarget->GetSize().width, pRenderTarget->GetSize().height)
	);


	/*==== �Q�[���t�B�[���h�̕`�� ====*/
	field->DrawField(
		brush->GetBrushColor(White),
		brush->GetBrushColor(Black),
		brush->GetBrushColor(Black)
	);


	/*==== �h���b�v�G���A�̕`�� ====*/
	if (command->GetIsSelected() != 999) {

		field->DrawCanDropArea(
			brush->GetBrushColor(Yellow),
			brush->GetBrushColor(Black)
		);

	}

	/*==== �R�}���h�̕`�� ====*/
	command->Draw(brush->GetBrushColor(Gray), brush->GetBrushColor(DodgeBlue));


	/*==== �G�̕`�� ====*/
	enemy->Draw();


	/*==== �e�L�X�g�̕`�� ====*/
	font->DrawALL(time, score, waves, resources);


	/*==== ���L�����̕`�� ====*/
	character->Draw();

	image->Draw();		// �摜�����_�����O

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


	/*==== �����_��HP�o�[�̕`�� ====*/

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


						// �̂Ƒ̂��Ԃ�������
						if (hitbox.right >= e.hitbox.left && hitbox.left <= e.hitbox.right &&
							hitbox.top <= e.hitbox.bottom && hitbox.bottom >= e.hitbox.top) {
							enemy->isBumping = true;
							//audio->PlayWav(Hit);
														// �����Ԃ����Ă���Ȃ�΁A
							enemy->Attack(myCharacters[i][j]->GetStatus());
						}


						//-------------------------------------------------------------------------------------------


						// �G�ƍU�����Ԃ�������
						if (!myCharacters[i][j]->GetAttackQue().empty()) {
							auto it = myCharacters[i][j]->GetAttackQue().begin();
							while (it != myCharacters[i][j]->GetAttackQue().end()) {
								auto& currentAttack = *it;
								D2D1_RECT_F AttackHitBox = currentAttack.hitbox;

								// �U������������
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

		// �G����ʊO�Ɉړ�
		if (e != nullptr && e->frag) {

			if (e->GetInfo().image.x <= 0.0f) {

				e->GetInfo().image.x = -100.0f;
				e->GetInfo().hitbox = D2D1::RectF(0.0f, 0.0f, 0.0f, 0.0f);
				e->GetInfo().dx = 0.0f;

				e->frag = false;

				base->DecrementHP();	// base��HP�����炷

			}

		}

	}

}




bool GameSystem::ClearCheck() {

	return enemy->ClearCheck();

}


void GameSystem::ContinueGame() {

	waves++;		// �E�F�[�u�𑝂₷

	int min = 1 + waves;	// �G�̍ŏ���
	if (min >= 100) {
		min = 100;			// 100�ȏ�ɂ͂Ȃ�Ȃ��悤�ɂ���
	}

	int max = min + 2 * waves;		// �G�̍ő吔
	if (max >= 100) {
		max = 100;				// 101�ȏ�ɂ͂Ȃ�Ȃ��悤�ɂ���
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