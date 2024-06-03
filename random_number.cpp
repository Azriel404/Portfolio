/*

	ファイル名：random_number.cpp
	概要：random_number.hのソースファイル

*/


/*
	インクルード
*/
#include "random_number.h"


/*==== 乱数生成期を生成 ====*/
std::random_device rd;		// シード値の生成
std::mt19937 gen(rd());		// シード値をもとに乱数生成期を生成


/*==== 整数の乱数生成関数 ====*/
int GenerateRandomInt(int min, int max) {

	std::uniform_int_distribution<int> dis(min, max);		// 生成する値の範囲を設定
	return dis(gen);			// 値を生成。戻り値として返す 

}


/*==== 実数の乱数生成関数 ====*/
float GenerateRandomFloat(float min, float max) {

	std::uniform_real_distribution<float> dis(min, max);		// 生成する値の範囲を設定
	return dis(gen);			// 値を生成。戻り値として返す 

}