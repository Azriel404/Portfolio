#pragma once

#include <string>
#include <array>
#include <d2d1.h>
#pragma comment(lib, "d2d1.lib")
#include "field.h"
#include "image_manager.h"

#include "brush.h"		// 後で消す可能性あり

/*
	character:キャラクターぜんたい
	person:人の基底クラス
	hero:勇者
	warrior:
	knight:
	priest:
	mage:


*/

/*==== クラスの前宣言 ====*/
class Person;
class Hero;
class Warrior;
class Knight;
class Priest;
class Mage;

// 配列管理用に列挙型を宣言
enum Jobs {

	HERO = 0,
	WARRIOR,
	KNIGHT,
	PRIEST,
	MAGE

};


// キャラのステータス
typedef struct CharacterStatus {

	int HP;		// 体力
	int MP;		// MP
	int ATK;	// 攻撃力
	int DEF;	// 防御力
	int AGI;	// 敏捷性

} Status;


typedef struct AttackInformation {

	std::string job;		// 攻撃者の役職

	D2D1_RECT_F attack;		// 攻撃の見た目
	D2D1_RECT_F hitbox;		// 攻撃の当たり判定

	bool isAttacking;		// 攻撃中か
	float displayTime;		// 攻撃の描画時間				公式：(設定したい秒数) x fps(このプログラムの場合は、60fps)
	float cooldownTime;		// クールダウン					例：5秒に設定したい...	5(秒) x 60(fps)	= 300	→	(float型変数) = 300.0f 


} AttackInfo;



/*
	Character : キャラクターの管理をするクラス
*/
class Character {
public:

	Character(ID2D1HwndRenderTarget*, ImageManager*, Field*, int);
	~Character();

	void Generate(float, float, int);		// キャラクター生成関数

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

	void TransferToBitmap();		// 画像をビットマップに変換
	void DetermineCharacterCost();	// キャラクターの設置コストを決める
	bool CanGenerate(int, int);

};



/*
	Person : キャラクターの基底クラス
*/
class Person {
public:

	bool flag;				// フラグ変数
	std::vector<AttackInfo> attack;		// 攻撃のキュー

	Person(ID2D1HwndRenderTarget*, ID2D1Bitmap*, D2D1_POINT_2F, int, int);
	virtual ~Person();

	virtual void Attack();
	virtual void Update();
	virtual void Draw();

	D2D1_RECT_F GetHitBox();
	std::vector<AttackInfo>& GetAttackQue();		// 攻撃のキュー
	Status* GetStatus();


protected:

	std::string job;		// 自信のジョブを文字列として持つ
	ImageInfo image;		// 画像データの情報
	D2D1_RECT_F hitbox;		// 当たり判定
	D2D1_POINT_2F pos;		// 自キャラの座標
	int row;
	int column;

	Status status;			// 体力などのステータス

	ID2D1HwndRenderTarget* pRT;			// レンダーターゲット

};



/*
	Hero : Heroに関する情報を持つクラス
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
	float attackDuration;		// 攻撃持続時間
	float cooldownDuration;		// クールダウン時間
	float attackHitBox;			// 攻撃の範囲

};


/*
	Warrior : Warriorに関する情報を持つクラス
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
	float attackDuration;		// 攻撃持続時間
	float cooldownDuration;		// クールダウン時間
	float attackHitBox;			// 攻撃の範囲

};



/*
	Knight : Knightに関する情報を持つクラス
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
	float attackDuration;		// 攻撃持続時間
	float cooldownDuration;		// クールダウン時間
	float attackHitBox;			// 攻撃の範囲

};




/*
	Priest : Priestに関する情報を持つクラス
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
	float attackDuration;		// 攻撃持続時間
	float cooldownDuration;		// クールダウン時間
	float attackHitBox;			// 攻撃の範囲

};




/*
	Mage : Mageに関する情報を持つクラス
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
	float attackDuration;		// 攻撃持続時間
	float cooldownDuration;		// クールダウン時間
	float attackHitBox;			// 攻撃の範囲

};
