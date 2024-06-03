#pragma once

/*

	ファイル名：random_number.h
	概要：ランダムなfloat型の値を生成・戻り値として渡す関数を定義

*/


/*
	インクルード
*/
#include <random>

int GenerateRandomInt(int, int);			// 整数の乱数生成関数
float GenerateRandomFloat(float, float);	// 実数の乱数生成関数