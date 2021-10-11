/*
* Board.h
* 
* 게임을 진행하는 보드 클래스이다.
* 
* 다음을 구현한다. 
* 1. 마우스 드래그를 입력받아 두 퍼즐 조각을 바꿈.
* 2. 같은 색을 가진 연속된 3개 이상의 조각을 제거하는 동작.
* 3. 조각이 제거된 빈 공간을 채우기 위해 그 윗 조각들을 떨어뜨리는 동작.
* 4. 만약 조각을 드래그 해 이동시켰지만 제거 조건을 만족하지 못할 경우, 
*    이를 도로 원위치 시키는 동작.
* 5. 퍼즐 조각을 제거하는 중이거나, 떨어뜨리는 중이거나,
     기타 입력을 차단해야 하는 경우라면 사용자의 입력을 차단 함.
* 6. 사용자가 지금까지 몇 개의 조각을 제거하였는지 쉽게 참조할 수 있도록 함. 
* 7. 게임의 재시작을 위해 조각들의 색깔을 무작위로 바꾸고, 나머지 필요한 값들을
*    초기화 함. 
* 
* 보드 객체는 언제나 하나만 존재해야 한다. 여러 개 존재할 경우
* 올바른 동작을 보장할 수 없다. 따라서 생성자가 아닌 내장 팩토리 함수를 사용할 것. 
* 
* 퍼즐 조각 객체 자체를 서로 바꾸기 보다 둘의 색을 바꾸는 방식으로 구현하였다. 
* 
* 조각을 이동시키는 동작, 제거하는 동작, 떨어뜨리는 동작은 일정한 시간 간격을 갖고 루프를 도는데, 
* 이 시간 간격은 CommonDeclaration.h에 정의되어 있다. 
*/

#pragma once

#include <iostream>
#include "Piece.h"
#include <string>
#include <bitset>
#include <algorithm>

static int boardInstanceCount = 0;
class Board {
	friend std::shared_ptr<Board> std::make_shared<Board>();
private:
	ScenePtr background;
	Board(ScenePtr BG);

	// 퍼즐 조각 객체들
	PiecePtr piece[MAX_SIZE][MAX_SIZE];

	// 같은 색깔을 가진 연속된 조각들을 담는 배열
	std::vector<PiecePtr> serialPiece;

	// 퍼즐 조각을 드래그로 서로 바꿀 때 사용할 타이머
	TimerPtr moveTimer;

	// 연속해서 같은 색이 나타날 때 해당 조각들을 처리하면서 사용할 타이머
	TimerPtr clearSerialTimer;

	// 빈 칸에 그 위의 조각을 떨어뜨리며 사용할 타이머
	TimerPtr dropTimer;

	// 퍼즐 조각을 드래그 했는지 표시하는 플래그
	bool isPieceDragged = false;

	// 퍼즐 조각을 사용자가 움직이지 못하게 하는 락
	// 락이 활성화되면 타이머도 멈춘다. 
	bool lock = false;
	
	// 드래그 시작 조각과 교체 대상 조각 위치를 타이머에게 넘겨주기 위한 변수
	MatrixPos originPos;
	MatrixPos targetPos;

	// 드래그 방향을 타이머에 넘겨주기 위한 변수
	MouseAction curAction;

	// 지금까지 제거된 조각 개수
	int CountPieceCleared;

	// 음소거 플래그
	bool isSoundMute = false;

public:

	// 보드 객체를 생성하는 팩토리 메서드
	static std::shared_ptr<Board> CreateBoard(ScenePtr);

	// 하나의 빈 조각을 처리하는 함수
	// 빈 공간의 가장 가까운 위 조각 하나를 떨어뜨린다. 
	void DropPiece(PiecePtr toDropPiece);

	//  맨 윗줄의 조각을 랜덤하게 생성하는 함수
	void CreateFirstRow();

	// 색이 같은 인접한 퍼즐 조각들을 투명하게 바꾸는 함수
	// 이 함수는 단 한개의 조각 씩만 바꾼다. 
	// 따라서 루프를 사용할 것. 
	void ClearSerialPiece();

	// 없어져야 할 퍼즐 조각들을 vector에 담아 그 vector를 반환하는 함수
	std::vector<PiecePtr> FindPieceToClear();

	// 보드를 초기화하는 함수
	void ResetBoard();

	// 드래그 동작의 방향과 기준점 조각을 받아 일차적인 조각 교체를 하고
	// 목표 조각의 위치를 반환하는 함수. 
	// 만약 이동 후 제거 조건을 만족하지 못하면 다시 도로 두 조각을 바꿔 원위치 시켜야 한다. 
	MatrixPos DragMove(MouseAction action, MatrixPos origin);

	// 퍼즐 조각들의 마우스 콜백 함수를 설정하는 함수
	void PieceMove(MatrixPos curPos, MatrixPos targetPos);

	// 락이 활성화 되었는지 여부를 반환하는 함수
	// 타이머에 사용된다. 
	bool getLock();

	// 보드 전체를 잠그는 함수
	// 타이머가 끝나면 사용된다. 
	void setLock();

	// 지금까지 제거된 퍼즐 조각의 개수를 반환하는 함수
	int getCountPieceCleared();

	// 음소거 여부를 설정하는 함수
	void setMute(bool);
};


typedef std::shared_ptr<Board> BoardPtr;