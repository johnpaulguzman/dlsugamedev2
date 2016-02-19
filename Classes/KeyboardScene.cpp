#include "KeyboardScene.h"
#include <vector>
USING_NS_CC;

cocos2d::Sprite* KeyboardScene::createRandomBlock() {
	auto random = rand() % 6;
	switch (random) {
	case 0:
		return cocos2d::Sprite::create("r.png");
	case 1:
		return cocos2d::Sprite::create("o.png");
	case 2:
		return cocos2d::Sprite::create("y.png");
	case 3:
		return cocos2d::Sprite::create("g.png");
	case 4:
		return cocos2d::Sprite::create("b.png");
	case 5:
		return cocos2d::Sprite::create("v.png");
	}
}

bool KeyboardScene::doesCollide(cocos2d::Vec2 position1, cocos2d::Vec2 position2) {
	return (pow(position1.x - position2.x, 2) + pow(position1.y - position2.y, 2) <= pow((ss + sb) / 2, 2));
}

bool KeyboardScene::doesCollideBlock(cocos2d::Vec2 blockpos, cocos2d::Vec2 ballpos) {
	return ((std::abs(blockpos.x - ballpos.x) < (bw + sb)/2) && (std::abs(blockpos.y - ballpos.y ) < (bh + sb)/2));
}

Scene* KeyboardScene::createScene(){
	auto scene = Scene::create();
	auto layer = KeyboardScene::create();
	scene->addChild(layer);
	return scene;
}

bool KeyboardScene::init(){
	if (!Layer::init())
		return false;
	audio->preloadBackgroundMusic("fight.mp3");
	audio->preloadEffect("die.mp3");
	audio->preloadEffect("eat.mp3");
	//audio->playBackgroundMusic("fight.mp3");
	bg->setPosition(sw/2, sh/2);
	bg->setAnchorPoint(Vec2(0.5, 0.5));
	this->addChild(bg);
	sprite->setAnchorPoint(Vec2(0.5, 0.0));
	sprite->setPosition(sw/2, 0);
	this->addChild(sprite);
	ball->setAnchorPoint(Vec2(0.5, 0.5));
	ball->setPosition(sw / 2, sh/2);
	//ball->setPosition(sw/2, sh-ss*2);
	this->addChild(ball);
	int wasd = 12;
	int hasd = 4;
	for (int i = 0; i < wasd; i++) {
		for (int j = 0; j < hasd; j++) {
			if (i %2 == 0) {
				auto temp = createRandomBlock();
				temp->setAnchorPoint(Vec2(0.5, 0.5));
				temp->setPosition(i*bw + bw / 2, j*bh + bh * 14);
				this->addChild(temp);
				blocks.push_back(temp);
			}
		}
	}

	auto eventListener = EventListenerKeyboard::create();
	eventListener->onKeyPressed = [&](EventKeyboard::KeyCode keyCode, Event* event) {
		switch (keyCode) {
		case EventKeyboard::KeyCode::KEY_LEFT_ARROW:
			left = true;
			break;
		case EventKeyboard::KeyCode::KEY_RIGHT_ARROW:
			right = true;
			break;
		case EventKeyboard::KeyCode::KEY_SPACE:
			ball->setPosition(sw / 2, sh / 2);
			balldir = initdir;
			break;
		}
	};

	eventListener->onKeyReleased = [&](EventKeyboard::KeyCode keyCode, Event* event) {
		switch (keyCode) {
		case EventKeyboard::KeyCode::KEY_LEFT_ARROW:
			left = false;
			break;
		case EventKeyboard::KeyCode::KEY_RIGHT_ARROW:
			right = false;
			break;
		}
	};

	this->_eventDispatcher->addEventListenerWithSceneGraphPriority(eventListener, sprite);
	this->scheduleUpdate();
	return true;
}

void KeyboardScene::update(float delta) {
	CCLOG("");
	auto retardationConstant = 1.0;//1 dapat pero retarded ako
	bool blockdie = false;
	int boo = -1;
	for (int b = 0; b < blocks.size(); b++) {
		if (doesCollideBlock(blocks.at(b)->getPosition(), ball->getPosition())) {
			blockdie = true;
			boo = b;
			break;
		}
	}
	if (cd>0 && (ball->getPosition().y<sb / 2 || ball->getPosition().y>sh-sb/2)) {
		cd = -mball;
		balldir = -balldir;
	}
	else if (cd>0 && (ball->getPosition().x<sb / 2 || ball->getPosition().x>sw-sb/2)) {
		cd = -mball;
		balldir = -balldir + M_PI;
	}
	else if (blockdie) {
		auto balls = cocos2d::Sprite::create("ball.png");
		balls->setAnchorPoint(Vec2(0.5, 0.0));
		balls->setPosition(ball->getPosition());
		this->addChild(balls);
		auto le = std::abs(blocks.at(boo)->getPosition().x-bw/2 - ball->getPosition().x);
		auto ri = std::abs(blocks.at(boo)->getPosition().x+bw/2 - ball->getPosition().x);
		auto to = std::abs(blocks.at(boo)->getPosition().y+bh/2 - ball->getPosition().y);
		auto bo = std::abs(blocks.at(boo)->getPosition().y-bh/2 - ball->getPosition().y);
		auto min = std::min(le, std::min(ri, std::min(to, bo)));
		if (min == le || min == ri) {
			cd = -mball;
			balldir = -balldir + M_PI;
			CCLOG("PARR");
		}
		else if (min == to || min == bo) {
			cd = -mball;
			balldir = -balldir;
			CCLOG("ORT");
		}
	}
	//check paddle collide and bounce off using - complement - arctan(derivative sqrt(1-x^2)) where x=(padcenter-ballcenter)/padsize (assuming ball is point particle)
	else if (cd>0 && doesCollide(ball->getPosition(), sprite->getPosition())) {
		cd = -mball;
		balldir = -balldir - atan(-(retardationConstant*(sprite->getPosition().x- ball->getPosition().x)/ss) / sqrt(1-pow(retardationConstant*(sprite->getPosition().x- ball->getPosition().x)/ss, 2)));
	}
	//move paddle and ball
	if (left && !right)
		sprite->setPosition(sprite->getPosition().x - mpad, sprite->getPosition().y);
	else if (right && !left)
		sprite->setPosition(sprite->getPosition().x + mpad, sprite->getPosition().y);
	ball->setPosition(ball->getPosition().x+mball*cos(balldir), ball->getPosition().y+mball*sin(balldir));
	cd++;
}