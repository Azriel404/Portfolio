/*

	�t�@�C�����Frandom_number.cpp
	�T�v�Frandom_number.h�̃\�[�X�t�@�C��

*/


/*
	�C���N���[�h
*/
#include "random_number.h"


/*==== �����������𐶐� ====*/
std::random_device rd;		// �V�[�h�l�̐���
std::mt19937 gen(rd());		// �V�[�h�l�����Ƃɗ����������𐶐�


/*==== �����̗��������֐� ====*/
int GenerateRandomInt(int min, int max) {

	std::uniform_int_distribution<int> dis(min, max);		// ��������l�͈̔͂�ݒ�
	return dis(gen);			// �l�𐶐��B�߂�l�Ƃ��ĕԂ� 

}


/*==== �����̗��������֐� ====*/
float GenerateRandomFloat(float min, float max) {

	std::uniform_real_distribution<float> dis(min, max);		// ��������l�͈̔͂�ݒ�
	return dis(gen);			// �l�𐶐��B�߂�l�Ƃ��ĕԂ� 

}