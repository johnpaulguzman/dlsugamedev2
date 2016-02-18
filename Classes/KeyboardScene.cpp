#include "KeyboardScene.h"
#include <vector>
USING_NS_CC;

cocos2d::Vec2 KeyboardScene::movedir(int dir, cocos2d::Vec2 position) {
	switch (dir) {
	case 1:
		position.x -= ss;
		break;
	case 2:
		position.x += ss;
		break;
	case 3:
		position.y += ss;
		break;
	case 4:
		position.y -= ss;
		break;
	}
	return position;
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
	sprite->setPosition(sw/2, ss*2);
	this->addChild(sprite);
	ball->setAnchorPoint(Vec2(0.5, 0.5));
	ball->setPosition(sw / 2, sh/2);
	//ball->setPosition(sw/2, sh-ss*2);
	this->addChild(ball);
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
			balldir = M_PI * 3.0 / 2.0;
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
	auto position = sprite->getPosition();
	auto ballpos = ball->getPosition();
	//check wall collide

	//check paddle collide and bounce off using - complement - arctan(derivative sqrt(1-x^2)) where x=(padcenter-ballcenter)/padsize (assuming ball is point particle)
	//add while within ball moveout
	if (pow(position.x-ballpos.x,2)+pow(position.y-ballpos.y,2) <= pow((ss+sb)/2,2))
		balldir = balldir - M_PI - atan(-((position.x - ballpos.x)/ss) / sqrt(1-pow((position.x-ballpos.x)/ss, 2)));
	//move paddle and ball
	if (left && !right)
		sprite->setPosition(position.x - mpad, position.y);
	else if (right && !left)
		sprite->setPosition(position.x + mpad, position.y);
	ball->setPosition(ballpos.x+mball*cos(balldir), ballpos.y+mball*sin(balldir));

}