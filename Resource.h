/*
* Resource.h
* 
* 게임에서 사용되는 모든 이미지 파일들을 코드 내에서 쉽게 사용하기 위한 헤더.
*/

#pragma once

namespace TitleResource {
	constexpr auto BACKGROUND = "resource/Title.png";
	constexpr auto START_BUTTON = "resource/GameStart.png";
	constexpr auto HELP_BUTTON = "resource/Help.png";
	constexpr auto HELP_IMAGE = "resource/HelpImage.png";
}

namespace BoardResource {
	constexpr auto BACKGROUND = "resource/Background2.png";
	constexpr auto RESTART = "resource/RestartButton.png";
	constexpr auto MUTE_CHECKED = "resource/Checkbox_checked.png";
	constexpr auto MUTE_UNCHECKED = "resource/Checkbox_unchecked.png";
}

// Piece.cpp와의 호환에 주의할 것. 
namespace PieceResource {
	constexpr auto REDBLOCK = "resource/RedBlock2.png";
	constexpr auto GREENBLOCK = "resource/GreenBlock.png";
	constexpr auto BLUEBLOCK = "resource/BlueBlock.png";
	constexpr auto PULPLEBLOCK = "resource/PulpleBlock.png";
	constexpr auto YELLOWBLOCK = "resource/YellowBlock.png";
	constexpr auto BROWNBLOCK = "resource/BrownBlock.png";
	constexpr auto SPARCKLE = "resource/Sparkle.png";
	constexpr auto EMPTYBLOCK = "resource/EmptyBlock.png";
}

namespace CounterResource {
	constexpr auto STAGE_BACKGROUND = "resource/StageBackground.png";
	constexpr auto TIMER_BACKGROUND = "resource/TimerBackground.png";
	constexpr auto SCORE_BACKGROUND = "resource/ScoreBackground.png";
	constexpr auto ZERO = "resource/0.png";
	constexpr auto ONE = "resource/1.png";
	constexpr auto TWO = "resource/2.png";
	constexpr auto THREE = "resource/3.png";
	constexpr auto FOUR = "resource/4.png";
	constexpr auto FIVE = "resource/5.png";
	constexpr auto SIX = "resource/6.png";
	constexpr auto SEVEN = "resource/7.png";
	constexpr auto EIGHT = "resource/8.png";
	constexpr auto NINE = "resource/9.png";
}

namespace SoundResource {
	constexpr auto BACKGOUND_MUSIC = "resource/BackgroundMusic.mp3";
	constexpr auto PIECE_CLEAR = "resource/PieceClearSound.mp3";
	constexpr auto STAGE_CLEAR = "resource/StageClearSound.mp3";
}

namespace LabelResource {
	constexpr auto RESTART = "resource/RestartLabel.png";
	constexpr auto MUTE = "resource/MutetLabel.png";
}