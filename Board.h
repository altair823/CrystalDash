/*
* Board.h
* 
* ������ �����ϴ� ���� Ŭ�����̴�.
* 
* ������ �����Ѵ�. 
* 1. ���콺 �巡�׸� �Է¹޾� �� ���� ������ �ٲ�.
* 2. ���� ���� ���� ���ӵ� 3�� �̻��� ������ �����ϴ� ����.
* 3. ������ ���ŵ� �� ������ ä��� ���� �� �� �������� ����߸��� ����.
* 4. ���� ������ �巡�� �� �̵��������� ���� ������ �������� ���� ���, 
*    �̸� ���� ����ġ ��Ű�� ����.
* 5. ���� ������ �����ϴ� ���̰ų�, ����߸��� ���̰ų�,
     ��Ÿ �Է��� �����ؾ� �ϴ� ����� ������� �Է��� ���� ��.
* 6. ����ڰ� ���ݱ��� �� ���� ������ �����Ͽ����� ���� ������ �� �ֵ��� ��. 
* 7. ������ ������� ���� �������� ������ �������� �ٲٰ�, ������ �ʿ��� ������
*    �ʱ�ȭ ��. 
* 
* ���� ��ü�� ������ �ϳ��� �����ؾ� �Ѵ�. ���� �� ������ ���
* �ùٸ� ������ ������ �� ����. ���� �����ڰ� �ƴ� ���� ���丮 �Լ��� ����� ��. 
* 
* ���� ���� ��ü ��ü�� ���� �ٲٱ� ���� ���� ���� �ٲٴ� ������� �����Ͽ���. 
* 
* ������ �̵���Ű�� ����, �����ϴ� ����, ����߸��� ������ ������ �ð� ������ ���� ������ ���µ�, 
* �� �ð� ������ CommonDeclaration.h�� ���ǵǾ� �ִ�. 
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

	// ���� ���� ��ü��
	PiecePtr piece[MAX_SIZE][MAX_SIZE];

	// ���� ������ ���� ���ӵ� �������� ��� �迭
	std::vector<PiecePtr> serialPiece;

	// ���� ������ �巡�׷� ���� �ٲ� �� ����� Ÿ�̸�
	TimerPtr moveTimer;

	// �����ؼ� ���� ���� ��Ÿ�� �� �ش� �������� ó���ϸ鼭 ����� Ÿ�̸�
	TimerPtr clearSerialTimer;

	// �� ĭ�� �� ���� ������ ����߸��� ����� Ÿ�̸�
	TimerPtr dropTimer;

	// ���� ������ �巡�� �ߴ��� ǥ���ϴ� �÷���
	bool isPieceDragged = false;

	// ���� ������ ����ڰ� �������� ���ϰ� �ϴ� ��
	// ���� Ȱ��ȭ�Ǹ� Ÿ�̸ӵ� �����. 
	bool lock = false;
	
	// �巡�� ���� ������ ��ü ��� ���� ��ġ�� Ÿ�̸ӿ��� �Ѱ��ֱ� ���� ����
	MatrixPos originPos;
	MatrixPos targetPos;

	// �巡�� ������ Ÿ�̸ӿ� �Ѱ��ֱ� ���� ����
	MouseAction curAction;

	// ���ݱ��� ���ŵ� ���� ����
	int CountPieceCleared;

	// ���Ұ� �÷���
	bool isSoundMute = false;

public:

	// ���� ��ü�� �����ϴ� ���丮 �޼���
	static std::shared_ptr<Board> CreateBoard(ScenePtr);

	// �ϳ��� �� ������ ó���ϴ� �Լ�
	// �� ������ ���� ����� �� ���� �ϳ��� ����߸���. 
	void DropPiece(PiecePtr toDropPiece);

	//  �� ������ ������ �����ϰ� �����ϴ� �Լ�
	void CreateFirstRow();

	// ���� ���� ������ ���� �������� �����ϰ� �ٲٴ� �Լ�
	// �� �Լ��� �� �Ѱ��� ���� ���� �ٲ۴�. 
	// ���� ������ ����� ��. 
	void ClearSerialPiece();

	// �������� �� ���� �������� vector�� ��� �� vector�� ��ȯ�ϴ� �Լ�
	std::vector<PiecePtr> FindPieceToClear();

	// ���带 �ʱ�ȭ�ϴ� �Լ�
	void ResetBoard();

	// �巡�� ������ ����� ������ ������ �޾� �������� ���� ��ü�� �ϰ�
	// ��ǥ ������ ��ġ�� ��ȯ�ϴ� �Լ�. 
	// ���� �̵� �� ���� ������ �������� ���ϸ� �ٽ� ���� �� ������ �ٲ� ����ġ ���Ѿ� �Ѵ�. 
	MatrixPos DragMove(MouseAction action, MatrixPos origin);

	// ���� �������� ���콺 �ݹ� �Լ��� �����ϴ� �Լ�
	void PieceMove(MatrixPos curPos, MatrixPos targetPos);

	// ���� Ȱ��ȭ �Ǿ����� ���θ� ��ȯ�ϴ� �Լ�
	// Ÿ�̸ӿ� ���ȴ�. 
	bool getLock();

	// ���� ��ü�� ��״� �Լ�
	// Ÿ�̸Ӱ� ������ ���ȴ�. 
	void setLock();

	// ���ݱ��� ���ŵ� ���� ������ ������ ��ȯ�ϴ� �Լ�
	int getCountPieceCleared();

	// ���Ұ� ���θ� �����ϴ� �Լ�
	void setMute(bool);
};


typedef std::shared_ptr<Board> BoardPtr;