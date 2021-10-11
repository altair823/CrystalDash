#include "Board.h"

Board::Board(ScenePtr BG) : background(BG), CountPieceCleared(0) {
	// 모든 퍼즐 조각들을 생성하고 
	// 각각에 대한 MouseAction 정의
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
		// 없애야 할 연속된 조각이 존재할 경우
		if (serialPiece.size() != 0) {
			ClearSerialPiece();
			clearSerialTimer->set(CLEAR_SPEED);
			clearSerialTimer->start();
			return true;
		}
		// 없앨 조각이 존재하지 않을 경우
		else {
			// 나머지 조각을 빈 곳으로 떨어뜨린다. 
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
				// 현재 조각이 보이지 않는 상태고, 바로 윗 조각이 보이는 상태일 경우
				if (piece[i][j]->getVisibleFlag() == false && piece[i - 1][j]->getVisibleFlag() != false) {
					pieceToDrop = piece[i][j];
					goto Exit;
				}
			}
		}
		pieceToDrop = nullptr;
	Exit:
		// 떨어뜨릴 수 있는 조각이 존재할 경우
		if (pieceToDrop != nullptr) {
			DropPiece(pieceToDrop);
			dropTimer->set(DROP_SPEED);
			dropTimer->start();
		}
		// 더 이상 떨어뜨릴 조각이 없는 경우
		else {
			serialPiece = FindPieceToClear();
			// 떨어뜨릴 조각도 없고 제거할 조각도 없는 경우 루프 탈출
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
		// 사용자가 조각의 위치를 바꿨을 때
		if (isPieceDragged == false) {
			targetPos = DragMove(curAction, originPos);
			isPieceDragged = true;
			serialPiece.clear();
			serialPiece = FindPieceToClear();
			moveTimer->set(MOVE_SPEED);
			moveTimer->start();
		}
		// 만약 연속된 세 개의 조각이 나타나지 않을 경우
		else if (serialPiece.size() == 0 && isPieceDragged == true) {
			// 다시 돌려놓음
			PieceMove(targetPos, originPos);
			isPieceDragged = false;
		}
		// 만약 연속된 세 개의 조각이 나타났을 경우
		else if (serialPiece.size() != 0 && isPieceDragged == true) {
			isPieceDragged = false;
			// 해당 조각들을 제거함. 
			clearSerialTimer->start();
		}
		return true;
		});

	// 게임이 시작할 때 세 개 이상의 연속된 퍼즐 조각을 없애고 
	// 위에서 나머지를 떨어뜨린다. 
	serialPiece.clear();
	serialPiece = FindPieceToClear();
	clearSerialTimer->start();
}

std::shared_ptr<Board> Board::CreateBoard(ScenePtr BG) {
	// 보드 객체는 두 개일 수 없는 싱글톤이다. 
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
	
	// 항상 떨어뜨리는 과정의 처음과 마지막에는 첫 줄을 생성하는 과정이 필요하다. 
	// 첫 줄은 항상 존재해야 한다. 
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
			// 같은 행에서 이어서 같은 색이 나올 경우
			int serialIndexPlus = 1;
			int serialSameCount = 1;
			while (j + serialIndexPlus < MAX_SIZE 
				&& piece[i][j]->getColor() == piece[i][j + serialIndexPlus]->getColor() 
				&& piece[i][j]->getColor() != BlockColor::NoColor) {
				serialSameCount++;
				// 만약 연속된 세 개의 조각이 같은 색일 경우
				if (serialSameCount == 3) {
					// 지금까지의 조각들을 저장한다. 
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
				// 만약 세 개 이후의 조각도 같은 색일 경우
				if (serialSameCount > 3) {
					if (std::find(serialPiece.begin(), serialPiece.end(), piece[i][j + serialIndexPlus]) == serialPiece.end()) {
						serialPiece.push_back(piece[i][j + serialIndexPlus]);
					}
				}
				serialIndexPlus++;
			}

			// 같은 열에서 이어서 같은 색이 나올 경우
			serialIndexPlus = 1;
			serialSameCount = 1;
			while (i + serialIndexPlus < MAX_SIZE 
				&& piece[i][j]->getColor() == piece[i + serialIndexPlus][j]->getColor() 
				&& piece[i][j]->getColor() != BlockColor::NoColor) {
				serialSameCount++;
				// 만약 연속된 세 개의 조각이 같은 색일 경우
				if (serialSameCount == 3) {
					// 지금까지의 조각들을 저장한다. 
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
				// 만약 세 개 이후의 조각도 같은 색일 경우
				if (serialSameCount > 3) {
					if (std::find(serialPiece.begin(), serialPiece.end(), piece[i + serialIndexPlus][j]) == serialPiece.end()) {
						serialPiece.push_back(piece[i + serialIndexPlus][j]);
					}
				}
				serialIndexPlus++;
			}

			// 만약 보드에 빈칸이 추가로 존재한다면 이는 없어져야한다. 
			if (piece[i][j]->getColor() == BlockColor::NoColor || piece[i][j]->getColor() == BlockColor::Sparckle) {
				serialPiece.push_back(piece[i][j]);
			}
		}
	}
	// 연속해서 같은 색이 나와 없어져야할 조각들 리스트를 반환한다. 
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
