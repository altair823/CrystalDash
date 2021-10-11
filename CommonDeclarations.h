/*
* CommonDeclaration.h
* 
* 프로그램 전반적으로 필요한 상수 등을 정의하는 헤더. 
* 
* 게임의 제한 시간, 애니메이션의 시간, 색깔 개수 등을 정의한다. 
* 이에 대한 수정이 필요할 경우, 이 헤더를 수정해야 한다. 
*/

#pragma once

using namespace bangtal;

// 게임 사운드를 재생하도록 하는 플래그
#define PLAY_GAME_SOUND

// 게임을 진행할 시간
#define GAME_TIME 120

// 카운터를 갱신할 시간 간격
#define REFRESH_TIME 0.1f

// 조각을 움직이고, 제거하고, 떨어뜨릴 때 사용할 시간 간격
#define MOVE_SPEED 0.2f
#define CLEAR_SPEED 0.05f
#define DROP_SPEED 0.03f

// 조각 하나가 없어질 때 발생하는 애니메이션의 시간 간격
#define CHANGE_SPEED 0.5f

// 스테이지마다 적용될 가중치
#define STAGE_WEIGHT 1.1f


// 퍼즐의 최대 사이즈
// 퍼즐의 가로 세로는 모두 이 값을 갖는다.
#define MAX_SIZE 10

// 조각의 한 변의 픽셀 사이즈
#define BLOCK_PX_SIZE 40

// 조각이 가질 수 있는 색
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

// 조각이 가지는 위치 클래스
// 행과 열 값을 전달하는 데 있어서 사용한다. 
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

	