/*
* Piece.h
* 
* ���� ���� Ŭ�����̴�. 
* 
* ��� ���� ������ ���丮 �Լ��� �����Ǹ� �� ��ġ�� ���� ���� ������ �ʴ´�. 
* ���� �� ������ ������ �� ���� ���̴�. 
* 
* CommonDeclaration.h���� ���ǵ� BlockColor�� ������ ������ ������, 
* EmptyColor�� �����ϰ� ������ ������ ���Ѵ�. 
* ���� �� ���� ���� ���� �̹������ ũ�Ⱑ ���� ������ �̹����� �ʿ��ϴ�. 
* 
* ������ ���� �������� �ٲ� �� ������ �̴� ó�� ������ ���� ���������̴�. 
*/

#pragma once

#include <bangtal>
#include <random>
#include <Windows.h>
#include "CommonDeclarations.h"
#include "Resource.h"

// ���� ������ ����


class _piece;

namespace Piece {
	// ���� ���� ��ü�� ����� ���丮 �ż���
	std::shared_ptr<_piece> create(ScenePtr background, MatrixPos rowcol);
	std::shared_ptr<_piece> create(ScenePtr background, int row, int column);
}

// ���� ���� Ŭ����
class _piece {
	friend std::shared_ptr<_piece> Piece::create(ScenePtr, MatrixPos);
	friend std::shared_ptr<_piece> Piece::create(ScenePtr, int, int);
private:
	ObjectPtr pieceObject;
	_piece(const std::string& image, ScenePtr background, int c, MatrixPos rowcol);
	BlockColor color;

	// ���� ������ ��ġ
	MatrixPos position = {0, 0};

	// ���� ������ ���� ȭ�鿡 ���� �������μ� ���̴��� ���θ� ǥ���ϴ� �÷���
	bool isBlockVisible;

public:
	// ���� ������ �巡�� ���� ���� ���� �ݹ� �Լ�. 
	// �̸� pieceObject�� �����Ѵ�. 
	void setDragCallback(std::function<bool(ObjectPtr, int, int, MouseAction)> callback);

	// ���� ���� getter
	BlockColor getColor();

	// ������ ������ ����� ����� �Լ�
	void setRandomColor();
	
	// ������ �Ⱥ��̰� ����� �Լ�
	void makeInvisible();

	// ���������� �ٸ� �̹����� �ٲٴ� �Լ�
	void changeBlockColor(BlockColor newColer);

	// ���� ������ ���� ���̴��� Ȯ���ϴ� �Լ�
	bool getVisibleFlag();

	// ���� ��ġ�� ��ȯ�ϴ� �Լ�
	MatrixPos getPosition();
};

// ���� ���� ��ü�� shared_ptr�� �����Ѵ�. 
typedef std::shared_ptr<_piece> PiecePtr;


