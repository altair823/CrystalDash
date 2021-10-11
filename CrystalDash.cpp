/*
* CrystalDash.cpp
* 
* main 함수가 존재하는 파일이다. 
* 
* 게임에 필요한 모든 요소들을 종합하여 적절히 배치하고 
* 게임을 작동시키는 역할을 한다. 
* 
* 이 파일에서 정의된 그 어떤 요소도 다른 파일에서 참조해서는 안된다. 
*/

#include <bangtal>
#include "Board.h"
#include "Counter.h"
#include "Resource.h"

int getCurrentTime() {
	return int(time(NULL));
}

void GoNextStage(BoardPtr board, Counter& totalTimer, TimerPtr refreshTimer, int* totalTime, Counter& StageCounter, int* stageNum) {
	board->ResetBoard();
	totalTimer.changeNum(GAME_TIME);
	*totalTime = GAME_TIME;
	refreshTimer->start();
	(*stageNum)++;
	StageCounter.changeNum(*stageNum + 1);
}

void Restart(BoardPtr board, Counter& totalTimer, TimerPtr refreshTimer, int* totalTime, Counter& StageCounter, int* stageNum, Counter& ScoreCounter, int* totalScore) {
	board->ResetBoard();
	totalTimer.changeNum(GAME_TIME);
	*totalTime = GAME_TIME;
	refreshTimer->start();
	*stageNum = 0;
	StageCounter.changeNum(*stageNum + 1);
	*totalScore = 0;
	ScoreCounter.changeNum(*totalScore);
}

int main() {
	setGameOption(GameOption::GAME_OPTION_INVENTORY_BUTTON, false);
	setGameOption(GameOption::GAME_OPTION_MESSAGE_BOX_BUTTON, false);
	setGameOption(GameOption::GAME_OPTION_ROOM_TITLE, false);

	ScenePtr background = Scene::create("배경", BoardResource::BACKGROUND);

	// 배경음악
	auto BGM = Sound::create(SoundResource::BACKGOUND_MUSIC);
	BGM->play(true);

	// 제한시간과 점수 카운터를 갱신하는 타이머
	auto refreshTimer = Timer::create(REFRESH_TIME);

	// 보드 객체
	BoardPtr board = nullptr;

	// 타이틀 화면
	auto title = Scene::create("타이틀", TitleResource::BACKGROUND);
	auto startButton = Object::create(TitleResource::START_BUTTON, title, 110, 150);
	startButton->setOnMouseCallback([&](auto object, int x, int y, auto action)->bool {
		background->enter();
		board = Board::CreateBoard(background);
		refreshTimer->start();
		return true;
		});

	// 도움말 버튼
	auto helpButton = Object::create(TitleResource::HELP_BUTTON, title, 360, 150);
	helpButton->setOnMouseCallback([&](auto object, int x, int y, auto action)->bool {
		auto helpImage = Scene::create("도움말", TitleResource::HELP_IMAGE);
		auto helpStartButton = Object::create(TitleResource::START_BUTTON, helpImage, 220, 50);
		helpStartButton->setOnMouseCallback([&](auto object, int x, int y, auto action)->bool {
			background->enter();
			board = Board::CreateBoard(background);
			refreshTimer->start();
			return true;
			});
		helpImage->enter();
		return true;
		});

	// 현재 스테이지 
	int stageNum = 0;

	// 스테이지를 표시할 카운터
	Counter StageCounter(background, CounterResource::STAGE_BACKGROUND, 600, 300);
	StageCounter.changeNum(stageNum + 1);

	// 제한시간을 표시할 카운터
	Counter totalTimer(background, CounterResource::TIMER_BACKGROUND, 600, 170);
	totalTimer.changeNum(GAME_TIME);

	// 점수를 표시할 카운터
	Counter ScoreCounter(background, CounterResource::SCORE_BACKGROUND, 600, 40);

	// 전체 게임 시간
	int totalTime = GAME_TIME;

	// 각 스테이지 별 목표 점수
	int stageScore[10] = { 100, 120, 140, 160, 180, 200, 220, 240, 260, 280 };
	int totalScore = 0;

	// 재시작 버튼
	auto restartLabel = Object::create(LabelResource::RESTART, background, 600, 530);
	auto restartButton = Object::create(BoardResource::RESTART, background, 740, 530);
	restartButton->setScale(0.03f);
	restartButton->setOnMouseCallback([&](auto object, int x, int y, auto action)->bool {
		Restart(board, totalTimer, refreshTimer, &totalTime, StageCounter, &stageNum, ScoreCounter, &totalScore);
		return true;
	});

	// 음소거 버튼
	// 음소거 버튼을 누를 때마다 버튼의 체크가 바뀐다. 
	auto muteLabel = Object::create(LabelResource::MUTE, background, 600, 450);
	auto muteButton = Object::create(BoardResource::MUTE_UNCHECKED, background, 740, 450);
	muteButton->setScale(0.25f);
	bool isSoundMute = false;
	muteButton->setOnMouseCallback([&](auto, int, int, auto)->bool {
		if (!isSoundMute) {
			isSoundMute = true;
			muteButton->setImage(BoardResource::MUTE_CHECKED);
			BGM->stop();
			board->setMute(true);
		}
		else {
			isSoundMute = false;
			muteButton->setImage(BoardResource::MUTE_UNCHECKED);
			BGM->play(true);
			board->setMute(false);
		}
		return true;
		});

	int beforeSec = getCurrentTime();
	refreshTimer->setOnTimerCallback([&](auto)->bool {
		int curSec = getCurrentTime();
		// 1초가 바뀔 때, 그리고 플레이어가 보드를 조작하는 것이 가능할 경우에는 타이머가 작동한다. 
		if (curSec - beforeSec > 0 && board->getLock() == false) {
			beforeSec = curSec;
			totalTime -= 1;
			totalTimer.changeNum(totalTime);
		}
		// 타이머가 끝나면 게임을 종료하고 보드를 잠근다. 
		if (totalTimer.isCounterOver()) {
			totalScore += ScoreCounter.getScore();
			showMessage("TIME OVER!\n당신의 총 점수는 "+std::to_string(totalScore)+"입니다.");
			board->setLock();
			return true;
		}
		// 플레이어가 목표 점수에 도달하면 다음 스테이지로 바뀐다. 
		if (ScoreCounter.getScore() >= stageScore[stageNum]) {
			totalScore += ScoreCounter.getScore();
#ifdef PLAY_GAME_SOUND
			if (!isSoundMute) {
				SoundPtr stageClearSound = Sound::create(SoundResource::STAGE_CLEAR);
				stageClearSound->play();
			}
#endif // PLAY_GAME_SOUND

			// 마지막 스테이지에 도달하면 게임을 종료한다. 
			if (stageNum == 9) {
				showMessage("스테이지 10 클리어!\n모든 스테이지를 클리어하셨습니다.\n총 점수: " + std::to_string(totalScore));
				return true;
			}
			showMessage("스테이지" + std::to_string(stageNum + 1) + " 클리어!");
			GoNextStage(board, totalTimer, refreshTimer, &totalTime, StageCounter, &stageNum);
		}
		ScoreCounter.changeNum(board->getCountPieceCleared());
		refreshTimer->set(REFRESH_TIME);
		refreshTimer->start();
		return true;
		});

	startGame(title);
}