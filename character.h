#pragma once

#include <string>
#include <array>
#include <d2d1.h>
#pragma comment(lib, "d2d1.lib")
#include "field.h"
#include "image_manager.h"

#include "brush.h"		// ��ŏ����\������

/*
	character:�L�����N�^�[���񂽂�
	person:�l�̊��N���X
	hero:�E��
	warrior:
	knight:
	priest:
	mage:


*/

/*==== �N���X�̑O�錾 ====*/
class Person;
class Hero;
class Warrior;
class Knight;
class Priest;
class Mage;

// �z��Ǘ��p�ɗ񋓌^��錾
enum Jobs {

	HERO = 0,
	WARRIOR,
	KNIGHT,
	PRIEST,
	MAGE

};


// �L�����̃X�e�[�^�X
typedef struct CharacterStatus {

	int HP;		// �̗�
	int MP;		// MP
	int ATK;	// �U����
	int DEF;	// �h���
	int AGI;	// �q����

} Status;


typedef struct AttackInformation {

	std::string job;		// �U���҂̖�E

	D2D1_RECT_F attack;		// �U���̌�����
	D2D1_RECT_F hitbox;		// �U���̓����蔻��

	bool isAttacking;		// �U������
	float displayTime;		// �U���̕`�掞��				�����F(�ݒ肵�����b��) x fps(���̃v���O�����̏ꍇ�́A60fps)
	float cooldownTime;		// �N�[���_�E��					��F5�b�ɐݒ肵����...	5(�b) x 60(fps)	= 300	��	(float�^�ϐ�) = 300.0f 


} AttackInfo;



/*
	Character : �L�����N�^�[�̊Ǘ�������N���X
*/
class Character {
public:

	Character(ID2D1HwndRenderTarget*, ImageManager*, Field*, int);
	~Character();

	void Generate(float, float, int);		// �L�����N�^�[�����֐�

	void Update();
	void Draw();

	std::array<std::array<Person*, 3>, 5>& GetCharacters();
	std::array<ID2D1Bitmap*, 5>& GetCharacterImages();
	std::array<int, 5>& GetCost();
	bool GetCharacterFlag(int, int);

	void AddResource(int);
	int GetResource();



private:

	ID2D1HwndRenderTarget* pRT;
	ImageManager* img;
	Field* field;
	int resource;

	std::array<ID2D1Bitmap*, 5> images;
	std::array<int, 5> cost;
	std::array<std::array<Person*, 3>, 5> myChar;

	void TransferToBitmap();		// �摜���r�b�g�}�b�v�ɕϊ�
	void DetermineCharacterCost();	// �L�����N�^�[�̐ݒu�R�X�g�����߂�
	bool CanGenerate(int, int);

};



/*
	Person : �L�����N�^�[�̊��N���X
*/
class Person {
public:

	bool flag;				// �t���O�ϐ�
	std::vector<AttackInfo> attack;		// �U���̃L���[

	Person(ID2D1HwndRenderTarget*, ID2D1Bitmap*, D2D1_POINT_2F, int, int);
	virtual ~Person();

	virtual void Attack();
	virtual void Update();
	virtual void Draw();

	D2D1_RECT_F GetHitBox();
	std::vector<AttackInfo>& GetAttackQue();		// �U���̃L���[
	Status* GetStatus();


protected:

	std::string job;		// ���M�̃W���u�𕶎���Ƃ��Ď���
	ImageInfo image;		// �摜�f�[�^�̏��
	D2D1_RECT_F hitbox;		// �����蔻��
	D2D1_POINT_2F pos;		// ���L�����̍��W
	int row;
	int column;

	Status status;			// �̗͂Ȃǂ̃X�e�[�^�X

	ID2D1HwndRenderTarget* pRT;			// �����_�[�^�[�Q�b�g

};



/*
	Hero : Hero�Ɋւ���������N���X
*/

class Hero : public Person {
public:

	Hero(ID2D1HwndRenderTarget*, ID2D1Bitmap*, D2D1_POINT_2F, int, int);
	~Hero();

	void Attack() override;
	void Update() override;
	void Draw() override;

private:

	Brush* pB;
	float attackDuration;		// �U����������
	float cooldownDuration;		// �N�[���_�E������
	float attackHitBox;			// �U���͈̔�

};


/*
	Warrior : Warrior�Ɋւ���������N���X
*/

class Warrior : public Person {
public:

	Warrior(ID2D1HwndRenderTarget*, ID2D1Bitmap*, D2D1_POINT_2F, int, int);
	~Warrior();

	void Attack() override;
	void Update() override;
	void Draw() override;

private:

	Brush* pB;
	float attackDuration;		// �U����������
	float cooldownDuration;		// �N�[���_�E������
	float attackHitBox;			// �U���͈̔�

};



/*
	Knight : Knight�Ɋւ���������N���X
*/

class Knight : public Person {
public:

	Knight(ID2D1HwndRenderTarget*, ID2D1Bitmap*, D2D1_POINT_2F, int, int);
	~Knight();

	void Attack() override;
	void Update() override;
	void Draw() override;

private:

	Brush* pB;
	float attackDuration;		// �U����������
	float cooldownDuration;		// �N�[���_�E������
	float attackHitBox;			// �U���͈̔�

};




/*
	Priest : Priest�Ɋւ���������N���X
*/

class Priest : public Person {
public:

	Priest(ID2D1HwndRenderTarget*, ID2D1Bitmap*, D2D1_POINT_2F, int, int, Field*);
	~Priest();

	void Attack() override;
	void Update() override;
	void Draw() override;

private:

	Brush* pB;
	Field* f;
	float attackDuration;		// �U����������
	float cooldownDuration;		// �N�[���_�E������
	float attackHitBox;			// �U���͈̔�

};




/*
	Mage : Mage�Ɋւ���������N���X
*/

class Mage : public Person {
public:

	Mage(ID2D1HwndRenderTarget*, ID2D1Bitmap*, D2D1_POINT_2F, int, int);
	~Mage();

	void Attack() override;
	void Update() override;
	void Draw() override;

private:

	Brush* pB;
	float attackDuration;		// �U����������
	float cooldownDuration;		// �N�[���_�E������
	float attackHitBox;			// �U���͈̔�

};
