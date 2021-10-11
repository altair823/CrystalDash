/*
* Counter.h
* 
* ���� ȭ�鿡 �� �ڸ� ���� ���ڸ� ǥ���ϴ� Ŭ�����̴�. 
* 0���� 9���� �ϰ��� �������� ���� �̹����� �ʿ��ϸ�
* �� ���ڵ��� ȿ�������� �ΰ��� �� �ֵ��� �� �ڸ� ���ڸ�
* ���δ� ����� ���� ������ �� �ִ�. 
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

	// ���� �̹����� ���
	ObjectPtr counterBackground;

	// ���� ��
	int currentCount;

	// ���� �ڸ� ���� �̹���
	ObjectPtr hundreds;

	// ���� �ڸ� ���� �̹���
	ObjectPtr units;

	// ���� �ڸ� ���� �̹���
	ObjectPtr tens;

public:
	Counter(ScenePtr BG, std::string counterBG, int x, int y);

	// �� �ڸ� ������ �Է¹޾� �װ����� ���� �̹����� �����ϴ� �Լ�
	void changeNum(int num);

	// �� �ڸ��� ���ڸ� �ٲٴ� �Լ�
	void setHundreds(int n); // 100�� �ڸ�
	void setTens(int n); // 10�� �ڸ�
	void setUnits(int n); // 1�� �ڸ�

	// ���� 0�� �Ǿ����� Ȯ���ϴ� �Լ�
	bool isCounterOver();

	// ���� ���� ��ȯ�ϴ� �Լ�
	int getScore();
};

