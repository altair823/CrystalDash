#include "Piece.h"

// 블럭 파일 경로 리스트
std::vector<std::string> BlockList = {
		PieceResource::REDBLOCK,
		PieceResource::GREENBLOCK,
		PieceResource::BLUEBLOCK,
		PieceResource::PULPLEBLOCK,
		PieceResource::YELLOWBLOCK,
		PieceResource::BROWNBLOCK,
		PieceResource::SPARCKLE,
		PieceResource::EMPTYBLOCK
};
_piece::_piece(const std::string& image, ScenePtr background, int c, MatrixPos rowcol) {
	pieceObject = Object::create(image, background, rowcol.column * BLOCK_PX_SIZE + 100, (MAX_SIZE - rowcol.row - 1) * BLOCK_PX_SIZE + 100);
	position.row = rowcol.row;
	position.column = rowcol.column;
	color = static_cast<BlockColor>(c);
	isBlockVisible = true;
}

void _piece::setDragCallback(std::function<bool(ObjectPtr, int, int, MouseAction)> callback){
	pieceObject->setOnMouseCallback(callback);
}

BlockColor _piece::getColor(){
	return color;
}
void _piece::makeInvisible() {
	TimerPtr pieceChangerTimer = Timer::create(CHANGE_SPEED);
	pieceChangerTimer->setOnTimerCallback([&](auto)->bool {
		changeBlockColor(BlockColor::NoColor);
		return true;
	});
	changeBlockColor(BlockColor::Sparckle);
	pieceChangerTimer->start();
}
void _piece::setRandomColor() {
	std::random_device rd;
	std::mt19937 gen(rd());
	std::uniform_int_distribution<int> dis(0, BlockList.size() - 3);
	int t = (dis(gen) % BlockList.size());

	changeBlockColor(static_cast<BlockColor>(t));
}

void _piece::changeBlockColor(BlockColor newColer) {
	color = newColer;
	pieceObject->setImage(BlockList[newColer]);
	if (newColer == BlockColor::NoColor) {
		isBlockVisible = false;
	}
	else {
		isBlockVisible = true;
	}
}

bool _piece::getVisibleFlag() {
	return isBlockVisible;
}

MatrixPos _piece::getPosition(){
	return position;
}

std::shared_ptr<_piece> Piece::create(ScenePtr background, MatrixPos rowcol)
{
	std::random_device rd;
	std::mt19937 gen(rd());
	std::uniform_int_distribution<int> dis(0, BlockList.size() - 3);
	int t = (dis(gen) % BlockList.size());

	// 임의의 블럭 이미지 선택
	std::shared_ptr<_piece> result(new _piece(BlockList[t], background, t, rowcol));
	return result;
}
std::shared_ptr<_piece> Piece::create(ScenePtr background, int row, int column)
{
	MatrixPos temp = { row, column };
	std::random_device rd;
	std::mt19937 gen(rd());
	std::uniform_int_distribution<int> dis(0, BlockList.size() - 3);
	int t = (dis(gen) % BlockList.size());

	// 임의의 블럭 이미지 선택
	std::shared_ptr<_piece> result(new _piece(BlockList[t], background, t, temp));
	return result;
}
