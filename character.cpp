#include "character.h"


/*====================
	Charcter�N���X
====================*/

Character::Character(ID2D1HwndRenderTarget* pRenderTarget, ImageManager* imageManager, Field* field, int r) :
	pRT(pRenderTarget), img(imageManager), field(field), resource(r)
{

	myChar = {};				// �z���������
	TransferToBitmap();			// �摜���r�b�g�}�b�v�ɕϊ�
	DetermineCharacterCost();	// �L�����N�^�[�̃R�X�g�����肷��

}

Character::~Character() {

}

// �摜���r�b�g�}�b�v�ɕϊ�
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


// ��������
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
						canDropArea[i][j].frag = false;		// �L�����N�^�[��ݒu�������߁A�t���O�ϐ���false��
					}


					break;


				case WARRIOR:

					if (CanGenerate(resource, cost[WARRIOR])) {
						myChar[i][j] = new Warrior(pRT, images[WARRIOR], center, i, j);
						resource -= cost[WARRIOR];
						canDropArea[i][j].frag = false;		// �L�����N�^�[��ݒu�������߁A�t���O�ϐ���false��
					}

					break;


				case KNIGHT:

					if (CanGenerate(resource, cost[KNIGHT])) {
						myChar[i][j] = new Knight(pRT, images[KNIGHT], center, i, j);
						resource -= cost[KNIGHT];
						canDropArea[i][j].frag = false;		// �L�����N�^�[��ݒu�������߁A�t���O�ϐ���false��
					}

					break;


				case PRIEST:

					if (CanGenerate(resource, cost[PRIEST])) {
						myChar[i][j] = new Priest(pRT, images[PRIEST], center, i, j, field);
						resource -= cost[PRIEST];
						canDropArea[i][j].frag = false;		// �L�����N�^�[��ݒu�������߁A�t���O�ϐ���false��
					}

					break;


				case MAGE:

					if (CanGenerate(resource, cost[MAGE])) {
						myChar[i][j] = new Mage(pRT, images[MAGE], center, i, j);
						resource -= cost[MAGE];
						canDropArea[i][j].frag = false;		// �L�����N�^�[��ݒu�������߁A�t���O�ϐ���false��
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





// �X�V����
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


// �`�揈��
void Character::Draw() {

	for (int i = 0; i < myChar.size(); i++) {

		for (int j = 0; j < myChar[0].size(); j++) {

			if (myChar[i][j] != NULL) {

				myChar[i][j]->Draw();

			}

		}

	}

}


// �A�N�Z�b�T
std::array<std::array<Person*, 3>, 5>& Character::GetCharacters() {

	return myChar;

}


bool Character::GetCharacterFlag(int row, int column) {

	if (myChar[row][column] != nullptr) {		// nullptr�ł͂Ȃ����H�@=�@�f�[�^�����݂��邩�H

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
	Person�N���X
==================*/

Person::Person(ID2D1HwndRenderTarget* pRenderTarget, ID2D1Bitmap* img, D2D1_POINT_2F center, int r, int c) {

	image.pBitmap = img;		// �r�b�g�}�b�v�ւ̃|�C���^
	image.size = 40.0f;			// �r�b�g�}�b�v�̕\���T�C�Y
	image.x = center.x - image.size / 2.0f;		// �r�b�g�}�b�v�̍����X���W
	image.y = center.y - image.size / 2.0f;		// �r�b�g�}�b�v�̍����Y���W
	hitbox = D2D1::RectF(center.x - image.size / 2.0f, center.y - image.size / 2.0f, center.x + image.size / 2.0f, center.y + image.size / 2.0f);	// �����蔻��
	pos = center;		// ���L�����̍��W
	row = r;
	column = c;
	flag = true;		// �\���t���O

	status = {};		// ��z��ŏ�����
	pRT = pRenderTarget;	// �����_�[�^�[�Q�b�g�̃Z�b�g

}

Person::~Person() {}		// �f�X�g���N�^(���z)
void Person::Attack() {}	// Attack(���z)
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
	Hero�N���X
================*/

Hero::Hero(ID2D1HwndRenderTarget* pRT, ID2D1Bitmap* img, D2D1_POINT_2F center, int row, int column) :
	Person(pRT, img, center, row, column)
{

	// ���j�[�N�ϐ��̏�����
	job = "Hero";
	status = {
		50,		// HP
		30,		// MP
		50,		// �U����
		50,		// �h���
		50,		// �q����
	};

	pB = new Brush(pRT);

	attackDuration = 60.0f;
	attackHitBox = 30.0f;
	cooldownDuration = 20.0f;		// �ŏ��ɒZ�߂̃N�[���_�E����݂���

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

	// �N�[���_�E�����c���Ă����...
	else {

		cooldownDuration--;		// �N�[���_�E�������炷

	}

}


void Hero::Update() {

	// �U���L���[�ɍU�����Ȃ�������...
	if (attack.empty()) {
		return;		// �������Ȃ�
	}

	// �U���L���[�ɍU������������...
	auto it = attack.begin();			// �C�e���[�^��������
	while (it != attack.end()) {		// �z��ɗv�f���������J��Ԃ�
		auto& currentAttack = *it;

		if (currentAttack.displayTime <= 0.0f || currentAttack.attack.left >= pRT->GetSize().width) {

			it = attack.erase(it);  // currentAttack���폜���A�C�e���[�^���X�V

		}
		else {

			currentAttack.displayTime--;		// �`�掞�Ԃ����炷
			++it;  // �C�e���[�^�����ɐi�߂�

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
	Warrior�N���X
===================*/

Warrior::Warrior(ID2D1HwndRenderTarget* pRT, ID2D1Bitmap* img, D2D1_POINT_2F center, int row, int column) :
	Person(pRT, img, center, row, column)
{

	// ���j�[�N�ϐ��̏�����
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
	cooldownDuration = 20.0f;		// �ŏ��ɒZ�߂̃N�[���_�E����݂���

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

	// �N�[���_�E�����c���Ă����...
	else {

		cooldownDuration--;		// �N�[���_�E�������炷

	}

}

void Warrior::Update() {

	// �U���L���[�ɍU�����Ȃ�������...
	if (attack.empty()) {
		return;		// �������Ȃ�
	}

	// �U���L���[�ɍU������������...
	auto it = attack.begin();			// �C�e���[�^��������
	while (it != attack.end()) {		// �z��ɗv�f���������J��Ԃ�
		auto& currentAttack = *it;

		if (currentAttack.displayTime <= 0.0f || currentAttack.attack.left >= pRT->GetSize().width) {

			it = attack.erase(it);  // currentAttack���폜���A�C�e���[�^���X�V

		}
		else {

			currentAttack.displayTime--;		// �`�掞�Ԃ����炷
			++it;  // �C�e���[�^�����ɐi�߂�

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
	Knight�N���X
==================*/

Knight::Knight(ID2D1HwndRenderTarget* pRT, ID2D1Bitmap* img, D2D1_POINT_2F center, int row, int column) :
	Person(pRT, img, center, row, column)
{

	// ���j�[�N�ϐ��̏�����
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
	cooldownDuration = 20.0f;		// �ŏ��ɒZ�߂̃N�[���_�E����݂���

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

	// �N�[���_�E�����c���Ă����...
	else {

		cooldownDuration--;		// �N�[���_�E�������炷

	}

}

void Knight::Update() {

	// �U���L���[�ɍU�����Ȃ�������...
	if (attack.empty()) {
		return;		// �������Ȃ�
	}

	// �U���L���[�ɍU������������...
	auto it = attack.begin();			// �C�e���[�^��������
	while (it != attack.end()) {		// �z��ɗv�f���������J��Ԃ�
		auto& currentAttack = *it;

		if (currentAttack.displayTime <= 0.0f) {

			it = attack.erase(it);  // currentAttack���폜���A�C�e���[�^���X�V

		}
		else {

			currentAttack.displayTime--;		// �`�掞�Ԃ����炷
			++it;  // �C�e���[�^�����ɐi�߂�

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
	Priest�N���X
==================*/

Priest::Priest(ID2D1HwndRenderTarget* pRT, ID2D1Bitmap* img, D2D1_POINT_2F center, int row, int column, Field* f) :
	Person(pRT, img, center, row, column), f(f)
{

	// ���j�[�N�ϐ��̏�����
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
	cooldownDuration = 20.0f;		// �ŏ��ɒZ�߂̃N�[���_�E����݂���

}


Priest::~Priest() {

}



void Priest::Attack() {

	if (cooldownDuration <= 0.0f) {

		D2D1_RECT_F healArea;
		D2D1_POINT_2F p1 = f->GetCenterPosition(0, column);

		// �����̗�̑O����񕜂���
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

	// �N�[���_�E�����c���Ă����...
	else {

		cooldownDuration--;		// �N�[���_�E�������炷

	}

}

void Priest::Update() {

	// �U���L���[�ɍU�����Ȃ�������...
	if (attack.empty()) {
		return;		// �������Ȃ�
	}

	// �U���L���[�ɍU������������...
	if (!attack.empty()) {
		auto it = attack.begin();			// �C�e���[�^��������

		auto& currentAttack = *it;			// �擪�̗v�f�����o��	

		if (currentAttack.displayTime <= 0.0f) {

			it = attack.erase(it);  // currentAttack���폜���A�C�e���[�^���X�V

		}
		else {

			it->isAttacking = false;			// �񕜂͈�x�����N���� : �U�������̃t���O��false�ɂ���
			currentAttack.displayTime--;		// �`�掞�Ԃ����炷
			++it;  // �C�e���[�^�����ɐi�߂�
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
	Mage�N���X
================*/

Mage::Mage(ID2D1HwndRenderTarget* pRT, ID2D1Bitmap* img, D2D1_POINT_2F center, int row, int column) :
	Person(pRT, img, center, row, column)
{

	// ���j�[�N�ϐ��̏�����
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
	cooldownDuration = 20.0f;		// �ŏ��ɒZ�߂̃N�[���_�E����݂���

}


Mage::~Mage() {

	delete pB;
	pB = nullptr;

}



void Mage::Attack() {

	// �N�[���_�E�����I��������...
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

		cooldownDuration = 60.0f;		// �N�[���_�E����300�t���[��(5�b)�ɐݒ�

	}

	// �N�[���_�E�����c���Ă����...
	else {

		cooldownDuration--;		// �N�[���_�E�������炷

	}

}


void Mage::Update() {

	// �U���L���[�ɍU�����Ȃ�������...
	if (attack.empty()) {
		return;		// �������Ȃ�
	}

	// �U���L���[�ɍU������������...
	auto it = attack.begin();			// �C�e���[�^��������
	while (it != attack.end()) {		// �z��ɗv�f���������J��Ԃ�
		auto& currentAttack = *it;

		currentAttack.attack.left += 10.0f;   // �U���𓮂���
		currentAttack.attack.right += 10.0f;
		currentAttack.hitbox.left += 10.0f;   // �����蔻��𓮂���
		currentAttack.hitbox.right += 10.0f;

		if (currentAttack.displayTime <= 0.0f || currentAttack.attack.left >= pRT->GetSize().width) {

			it = attack.erase(it);  // currentAttack���폜���A�C�e���[�^���X�V

		}
		else {

			currentAttack.displayTime--;		// �`�掞�Ԃ����炷
			++it;  // �C�e���[�^�����ɐi�߂�

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
	�L�������j

	Hero:�o�����X�^�B��p�n�R�B
	Warrior:�U���^�B�h�䂪���Ⴍ�A�U�����x�����x�����A���Η�
	Knight:�h��^�B���U���͂͒�߂����A�����h��͂Ƒ���������߂��U���œS�ǂ̎�������
	Priest:�x���^�B�U����i�͂Ȃ����A���̖͂������T�|�[�g����B�̗͂̉񕜂�A�U����CD��������Ȃ�
	Mage:�������^�B����MP�ƍU���͂𐶂����A���������疂�@��łB�������A�̗͂Ɩh��͂���߁B

	�X�e�[�^�X�͂Ƃ肠�����@50�@����Ƃ���B

*/