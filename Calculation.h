#pragma once
#include<iostream>

//多様性を持たせるテンプレート
template <class Type>
//計算の関数
//x剰の結果を出す
Type TheNumPower(const Type& num, int power)
{
	Type value = 1;

	for (int i = 0; i < power; i++)
	{
		value *= num;
	}

	return value;
}

template<typename T>

float GetRatio(T num, T num2)
{
	return num / (num + num2);
}

