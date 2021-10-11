#include "Board.h"

Board::Board(ScenePtr BG) : background(BG), CountPieceCleared(0) {
	// ��� ���� �������� �����ϰ� 
	// ������ ���� MouseAction ����
	for (int i = 0; i < MAX_SIZE; i++) {
		for (int j = 0; j < MAX_SIZE; j++) {
			piece[i][j] = Piece::create(background, i, j);
			piece[i][j]->setDragCallback([=](auto object, int x, int y, auto action)->bool {
				if (action != MouseAction::MOUSE_CLICK && isPieceDragged == false && lock == false) {
					originPos = { i, j };
					curAction = action;
					moveTimer->start();
				}
				return true;
				});
		}
	}

	clearSerialTimer = Timer::create(CLEAR_SPEED);
	clearSerialTimer->setOnTimerCallback([&](auto)->bool {
		lock = true;
		// ���־� �� ���ӵ� ������ ������ ���
		if (serialPiece.size() != 0) {
			ClearSerialPiece();
			clearSerialTimer->set(CLEAR_SPEED);
			clearSerialTimer->start();
			return true;
		}
		// ���� ������ �������� ���� ���
		else {
			// ������ ������ �� ������ ����߸���. 
			dropTimer->set(0.5f);
			dropTimer->start();
			serialPiece.clear();
			return true;
		}
	});

	dropTimer = Timer::create(DROP_SPEED);
	dropTimer->setOnTimerCallback([&](auto)->bool {
		PiecePtr pieceToDrop = nullptr;
		CreateFirstRow();
		for (int i = MAX_SIZE - 1; i > 0; i--) {
			for (int j = 0; j < MAX_SIZE; j++) {
				// ���� ������ ������ �ʴ� ���°�, �ٷ� �� ������ ���̴� ������ ���
				if (piece[i][j]->getVisibleFlag() == false && piece[i - 1][j]->getVisibleFlag() != false) {
					pieceToDrop = piece[i][j];
					goto Exit;
				}
			}
		}
		pieceToDrop = nullptr;
	Exit:
		// ����߸� �� �ִ� ������ ������ ���
		if (pieceToDrop != nullptr) {
			DropPiece(pieceToDrop);
			dropTimer->set(DROP_SPEED);
			dropTimer->start();
		}
		// �� �̻� ����߸� ������ ���� ���
		else {
			serialPiece = FindPieceToClear();
			// ����߸� ������ ���� ������ ������ ���� ��� ���� Ż��
			if (serialPiece.size() == 0) {
				lock = false;
				return true;
			}
			clearSerialTimer->set(0.1f);
			clearSerialTimer->start();
			return true;
		}
		return true;
	});

	moveTimer = Timer::create(MOVE_SPEED);
	moveTimer->setOnTimerCallback([&](auto)->bool {
		// ����ڰ� ������ ��ġ�� �ٲ��� ��
		if (isPieceDragged == false) {
			targetPos = DragMove(curAction, originPos);
			isPieceDragged = true;
			serialPiece.clear();
			serialPiece = FindPieceToClear();
			moveTimer->set(MOVE_SPEED);
			moveTimer->start();
		}
		// ���� ���ӵ� �� ���� ������ ��Ÿ���� ���� ���
		else if (serialPiece.size() == 0 && isPieceDragged == true) {
			// �ٽ� ��������
			PieceMove(targetPos, originPos);
			isPieceDragged = false;
		}
		// ���� ���ӵ� �� ���� ������ ��Ÿ���� ���
		else if (serialPiece.size() != 0 && isPieceDragged == true) {
			isPieceDragged = false;
			// �ش� �������� ������. 
			clearSerialTimer->start();
		}
		return true;
		});

	// ������ ������ �� �� �� �̻��� ���ӵ� ���� ������ ���ְ� 
	// ������ �������� ����߸���. 
	serialPiece.clear();
	serialPiece = FindPieceToClear();
	clearSerialTimer->start();
}

std::shared_ptr<Board> Board::CreateBoard(ScenePtr BG) {
	// ���� ��ü�� �� ���� �� ���� �̱����̴�. 
	boardInstanceCount++;
	if (boardInstanceCount > 1) {
		std::cout << "there is two or more Board instances! \nabort!" << std::endl;
		exit(1);
	}
	std::shared_ptr<Board> board(new Board(BG));
	return board;
}

void Board::DropPiece(PiecePtr toDropPiece) {
	auto upperPiece = piece[toDropPiece->getPosition().row - 1][toDropPiece->getPosition().column];
	toDropPiece->changeBlockColor(upperPiece->getColor());
	upperPiece->changeBlockColor(BlockColor::NoColor);
	
	// �׻� ����߸��� ������ ó���� ���������� ù ���� �����ϴ� ������ �ʿ��ϴ�. 
	// ù ���� �׻� �����ؾ� �Ѵ�. 
	CreateFirstRow();
}

void Board::CreateFirstRow() {
	for (int i = 0; i < MAX_SIZE; i++) {
		if (piece[0][i]->getVisibleFlag() == false) {
			piece[0][i]->setRandomColor();
		}
	}
}

void Board::ClearSerialPiece() {
	PiecePtr cur;
	cur = serialPiece.back();
	cur->makeInvisible();
	serialPiece.pop_back();
	CountPieceCleared++;
#ifdef PLAY_GAME_SOUND
	if (!isSoundMute) {
		SoundPtr clearSound = Sound::create(SoundResource::PIECE_CLEAR);
		clearSound->play();
	}
#endif // PLAY_GAME_SOUND
}

std::vector<PiecePtr> Board::FindPieceToClear() {
	std::vector<PiecePtr> serialPiece;

	for (int i = 0; i < MAX_SIZE; i++) {
		for (int j = 0; j < MAX_SIZE; j++) {
			// ���� �࿡�� �̾ ���� ���� ���� ���
			int serialIndexPlus = 1;
			int serialSameCount = 1;
			while (j + serialIndexPlus < MAX_SIZE 
				&& piece[i][j]->getColor() == piece[i][j + serialIndexPlus]->getColor() 
				&& piece[i][j]->getColor() != BlockColor::NoColor) {
				serialSameCount++;
				// ���� ���ӵ� �� ���� ������ ���� ���� ���
				if (serialSameCount == 3) {
					// ���ݱ����� �������� �����Ѵ�. 
					if (std::find(serialPiece.begin(), serialPiece.end(), piece[i][j]) == serialPiece.end()) {
						serialPiece.push_back(piece[i][j]);
					}
					if (std::find(serialPiece.begin(), serialPiece.end(), piece[i][j + 1]) == serialPiece.end()) {
						serialPiece.push_back(piece[i][j + 1]);
					}
					if (std::find(serialPiece.begin(), serialPiece.end(), piece[i][j + 2]) == serialPiece.end()) {
						serialPiece.push_back(piece[i][j + 2]);
					}
				}
				// ���� �� �� ������ ������ ���� ���� ���
				if (serialSameCount > 3) {
					if (std::find(serialPiece.begin(), serialPiece.end(), piece[i][j + serialIndexPlus]) == serialPiece.end()) {
						serialPiece.push_back(piece[i][j + serialIndexPlus]);
					}
				}
				serialIndexPlus++;
			}

			// ���� ������ �̾ ���� ���� ���� ���
			serialIndexPlus = 1;
			serialSameCount = 1;
			while (i + serialIndexPlus < MAX_SIZE 
				&& piece[i][j]->getColor() == piece[i + serialIndexPlus][j]->getColor() 
				&& piece[i][j]->getColor() != BlockColor::NoColor) {
				serialSameCount++;
				// ���� ���ӵ� �� ���� ������ ���� ���� ���
				if (serialSameCount == 3) {
					// ���ݱ����� �������� �����Ѵ�. 
					if (std::find(serialPiece.begin(), serialPiece.end(), piece[i][j]) == serialPiece.end()) {
						serialPiece.push_back(piece[i][j]);
					}
					if (std::find(serialPiece.begin(), serialPiece.end(), piece[i + 1][j]) == serialPiece.end()) {
						serialPiece.push_back(piece[i + 1][j]);
					}
					if (std::find(serialPiece.begin(), serialPiece.end(), piece[i + 2][j]) == serialPiece.end()) {
						serialPiece.push_back(piece[i + 2][j]);
					}
				}
				// ���� �� �� ������ ������ ���� ���� ���
				if (serialSameCount > 3) {
					if (std::find(serialPiece.begin(), serialPiece.end(), piece[i + serialIndexPlus][j]) == serialPiece.end()) {
						serialPiece.push_back(piece[i + serialIndexPlus][j]);
					}
				}
				serialIndexPlus++;
			}

			// ���� ���忡 ��ĭ�� �߰��� �����Ѵٸ� �̴� ���������Ѵ�. 
			if (piece[i][j]->getColor() == BlockColor::NoColor || piece[i][j]->getColor() == BlockColor::Sparckle) {
				serialPiece.push_back(piece[i][j]);
			}
		}
	}
	// �����ؼ� ���� ���� ���� ���������� ������ ����Ʈ�� ��ȯ�Ѵ�. 
	return serialPiece;
}

void Board::ResetBoard() {
	for (auto& row : piece) {
		for (auto& column : row) {
			column->setRandomColor();
		}
	}
	originPos = { 0, 0 };
	targetPos = { 0, 0 };
	isPieceDragged = false;
	lock = false;
	serialPiece.clear();
	serialPiece = FindPieceToClear();
	CountPieceCleared = 0;
	clearSerialTimer->start();
}

MatrixPos Board::DragMove(MouseAction action, MatrixPos origin) {
	int i = origin.row;
	int j = origin.column;
	MatrixPos targetPos = { 0, 0 };
	if (action == MouseAction::MOUSE_DRAG_UP && i > 0) {
		PieceMove({ i, j }, { i - 1, j });
		targetPos = { i - 1, j };
	}
	else if (action == MouseAction::MOUSE_DRAG_DOWN && i < MAX_SIZE - 1) {
		PieceMove({ i, j }, { i + 1, j });
		targetPos = { i + 1, j };
	}
	else if (action == MouseAction::MOUSE_DRAG_LEFT && j > 0) {
		PieceMove({ i, j }, { i, j - 1 });
		targetPos = { i , j - 1 };
	}
	else if (action == MouseAction::MOUSE_DRAG_RIGHT && j < MAX_SIZE - 1) {
		PieceMove({ i, j }, { i, j + 1 });
		targetPos = { i , j + 1 };
	}
	return targetPos;
}

void Board::PieceMove(MatrixPos curPos, MatrixPos targetPos) {
	BlockColor tempColor = piece[curPos.row][curPos.column]->getColor();
	piece[curPos.row][curPos.column]->changeBlockColor(piece[targetPos.row][targetPos.column]->getColor());
	piece[targetPos.row][targetPos.column]->changeBlockColor(tempColor);
}

bool Board::getLock() {
	return lock;
}

void Board::setLock() {
	lock = true;
}

int Board::getCountPieceCleared() {
	return CountPieceCleared;
}

void Board::setMute(bool m){
	isSoundMute = m;
}
