/*
* CommonDeclaration.h
* 
* ���α׷� ���������� �ʿ��� ��� ���� �����ϴ� ���. 
* 
* ������ ���� �ð�, �ִϸ��̼��� �ð�, ���� ���� ���� �����Ѵ�. 
* �̿� ���� ������ �ʿ��� ���, �� ����� �����ؾ� �Ѵ�. 
*/

#pragma once

using namespace bangtal;

// ���� ���带 ����ϵ��� �ϴ� �÷���
#define PLAY_GAME_SOUND

// ������ ������ �ð�
#define GAME_TIME 120

// ī���͸� ������ �ð� ����
#define REFRESH_TIME 0.1f

// ������ �����̰�, �����ϰ�, ����߸� �� ����� �ð� ����
#define MOVE_SPEED 0.2f
#define CLEAR_SPEED 0.05f
#define DROP_SPEED 0.03f

// ���� �ϳ��� ������ �� �߻��ϴ� �ִϸ��̼��� �ð� ����
#define CHANGE_SPEED 0.5f

// ������������ ����� ����ġ
#define STAGE_WEIGHT 1.1f


// ������ �ִ� ������
// ������ ���� ���δ� ��� �� ���� ���´�.
#define MAX_SIZE 10

// ������ �� ���� �ȼ� ������
#define BLOCK_PX_SIZE 40

// ������ ���� �� �ִ� ��
enum BlockColor {
	Red,
	Green,
	Blue,
	Pulple,
	Yellow,
	Brown,
	Sparckle,
	NoColor,
};

// ������ ������ ��ġ Ŭ����
// ��� �� ���� �����ϴ� �� �־ ����Ѵ�. 
struct MatrixPos
{
	int row;
	int column;
	MatrixPos(int r, int c) : row(r), column(c){}
	MatrixPos() : row(0), column(0) {};
	MatrixPos& operator=(const MatrixPos& a) {
		this->row = a.row;
		this->column = a.column;
		return *this;
	}
};

	