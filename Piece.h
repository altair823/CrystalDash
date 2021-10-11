/*
* Piece.h
* 
* 퍼즐 조각 클래스이다. 
* 
* 모든 퍼즐 조각은 팩토리 함수로 생성되며 그 위치는 생성 이후 변하지 않는다. 
* 단지 그 색깔을 변경할 수 있을 뿐이다. 
* 
* CommonDeclaration.h에서 정의된 BlockColor의 값들을 색으로 가지며, 
* EmptyColor는 투명하게 보이지 않음을 뜻한다. 
* 따라서 각 색깔에 대한 조각 이미지들과 크기가 같은 투명한 이미지가 필요하다. 
* 
* 조각의 색은 무작위로 바꿀 수 있으며 이는 처음 생성될 때도 마찬가지이다. 
*/

#pragma once

#include <bangtal>
#include <random>
#include <Windows.h>
#include "CommonDeclarations.h"
#include "Resource.h"

// 퍼즐 조각의 색깔


class _piece;

namespace Piece {
	// 퍼즐 조각 객체를 만드는 팩토리 매서드
	std::shared_ptr<_piece> create(ScenePtr background, MatrixPos rowcol);
	std::shared_ptr<_piece> create(ScenePtr background, int row, int column);
}

// 퍼즐 조각 클래스
class _piece {
	friend std::shared_ptr<_piece> Piece::create(ScenePtr, MatrixPos);
	friend std::shared_ptr<_piece> Piece::create(ScenePtr, int, int);
private:
	ObjectPtr pieceObject;
	_piece(const std::string& image, ScenePtr background, int c, MatrixPos rowcol);
	BlockColor color;

	// 퍼즐 조각의 위치
	MatrixPos position = {0, 0};

	// 퍼즐 조각이 현재 화면에 퍼즐 조각으로서 보이는지 여부를 표시하는 플래그
	bool isBlockVisible;

public:
	// 퍼즐 조각을 드래그 했을 때를 위한 콜백 함수. 
	// 이를 pieceObject에 전달한다. 
	void setDragCallback(std::function<bool(ObjectPtr, int, int, MouseAction)> callback);

	// 색깔에 대한 getter
	BlockColor getColor();

	// 조각을 임의의 색깔로 만드는 함수
	void setRandomColor();
	
	// 조각을 안보이게 만드는 함수
	void makeInvisible();

	// 퍼즐조각을 다른 이미지로 바꾸는 함수
	void changeBlockColor(BlockColor newColer);

	// 퍼즐 조각이 현재 보이는지 확인하는 함수
	bool getVisibleFlag();

	// 현재 위치를 반환하는 함수
	MatrixPos getPosition();
};

// 퍼즐 조각 객체는 shared_ptr로 관리한다. 
typedef std::shared_ptr<_piece> PiecePtr;


