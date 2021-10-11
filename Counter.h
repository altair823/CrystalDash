/*
* Counter.h
* 
* 게임 화면에 세 자리 이하 숫자를 표현하는 클래스이다. 
* 0부터 9까지 일관된 디자인의 숫자 이미지가 필요하며
* 그 숫자들이 효과적으로 부각될 수 있도록 세 자리 숫자를
* 감싸는 배경을 따로 지정할 수 있다. 
*/

#pragma once
#include <bangtal>
#include <string>
#include <iostream>
#include "CommonDeclarations.h"
#include "Resource.h"

class Counter {
private:
	ScenePtr background;

	// 숫자 이미지의 배경
	ObjectPtr counterBackground;

	// 현재 값
	int currentCount;

	// 백의 자리 숫자 이미지
	ObjectPtr hundreds;

	// 일의 자리 숫자 이미지
	ObjectPtr units;

	// 십의 자리 숫자 이미지
	ObjectPtr tens;

public:
	Counter(ScenePtr BG, std::string counterBG, int x, int y);

	// 세 자리 정수를 입력받아 그것으로 숫자 이미지를 갱신하는 함수
	void changeNum(int num);

	// 각 자리의 숫자를 바꾸는 함수
	void setHundreds(int n); // 100의 자리
	void setTens(int n); // 10의 자리
	void setUnits(int n); // 1의 자리

	// 값이 0이 되었는지 확인하는 함수
	bool isCounterOver();

	// 현재 값을 반환하는 함수
	int getScore();
};

