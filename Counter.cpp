#include "Counter.h"

Counter::Counter(ScenePtr BG, std::string counterBG = "", int x = 0, int y = 0) : background(BG) {

	if (counterBG != "") {
		// 세 자리 숫자들을 위한 배경. 
		// 게임의 배경(ScenePtr)과 다름. 
		counterBackground = Object::create(counterBG, background, x, y);
	}

	units = Object::create(CounterResource::ZERO, background, x+ 160, y + 10);
	units->setScale(0.25f);
	tens = Object::create(CounterResource::ZERO, background, x + 90, y + 10);
	tens->setScale(0.25f);
	hundreds = Object::create(CounterResource::ZERO, background, x + 20, y + 10);
	hundreds->setScale(0.25f);
}

void Counter::changeNum(int num) {
	currentCount = num;
	// 숫자는 반드시 세자리 수 이하여야 한다. 
	if (num > 999) {
		std::cout << "Wrong timer value!";
	}

	if (num / 100 >= 1) {
		setHundreds(num / 100);
	}
	else {
		setHundreds(0);
	}
	if (num / 10 >= 1) {
		setTens(((int)num % 100) / 10);
	}
	else {
		setTens(0);
	}
	if (num >= 0) {
		setUnits(((int)num % 10) / 1);
	}
	else {
		setUnits(0);
	}
}

void Counter::setHundreds(int n) {
	switch (n) {
	case 1:
		hundreds->setImage(CounterResource::ONE);
		break;
	case 2:
		hundreds->setImage(CounterResource::TWO);
		break;
	case 3:
		hundreds->setImage(CounterResource::THREE);
		break;
	case 4:
		hundreds->setImage(CounterResource::FOUR);
		break;
	case 5:
		hundreds->setImage(CounterResource::FIVE);
		break;
	case 6:
		hundreds->setImage(CounterResource::SIX);
		break;
	case 7:
		hundreds->setImage(CounterResource::SEVEN);
		break;
	case 8:
		hundreds->setImage(CounterResource::EIGHT);
		break;
	case 9:
		hundreds->setImage(CounterResource::NINE);
		break;
	case 0:
		hundreds->setImage(CounterResource::ZERO);
		break;
	default:
		break;
	}
}

void Counter::setTens(int n) {
	switch (n) {
	case 1:
		tens->setImage(CounterResource::ONE);
		break;
	case 2:
		tens->setImage(CounterResource::TWO);
		break;
	case 3:
		tens->setImage(CounterResource::THREE);
		break;
	case 4:
		tens->setImage(CounterResource::FOUR);
		break;
	case 5:
		tens->setImage(CounterResource::FIVE);
		break;
	case 6:
		tens->setImage(CounterResource::SIX);
		break;
	case 7:
		tens->setImage(CounterResource::SEVEN);
		break;
	case 8:
		tens->setImage(CounterResource::EIGHT);
		break;
	case 9:
		tens->setImage(CounterResource::NINE);
		break;
	case 0:
		tens->setImage(CounterResource::ZERO);
		break;
	default:
		break;
	}
}

void Counter::setUnits(int n) {
	switch (n) {
	case 1:
		units->setImage(CounterResource::ONE);
		break;
	case 2:
		units->setImage(CounterResource::TWO);
		break;
	case 3:
		units->setImage(CounterResource::THREE);
		break;
	case 4:
		units->setImage(CounterResource::FOUR);
		break;
	case 5:
		units->setImage(CounterResource::FIVE);
		break;
	case 6:
		units->setImage(CounterResource::SIX);
		break;
	case 7:
		units->setImage(CounterResource::SEVEN);
		break;
	case 8:
		units->setImage(CounterResource::EIGHT);
		break;
	case 9:
		units->setImage(CounterResource::NINE);
		break;
	case 0:
		units->setImage(CounterResource::ZERO);
		break;
	default:
		break;
	}
}

bool Counter::isCounterOver() {
	if (currentCount == 0) {
		return true;
	}
	else {
		return false;
	}
}

int Counter::getScore() {
	return currentCount;
}


