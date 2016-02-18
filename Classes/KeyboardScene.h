#pragma once

#include "cocos2d.h"
#include <map>
#include <vector>
#include "SimpleAudioEngine.h"
#include <cmath>

class KeyboardScene : public cocos2d::Layer
{

public:
	cocos2d::Sprite* sprite = cocos2d::Sprite::create("bumper.png");
	cocos2d::Sprite* bg = cocos2d::Sprite::create("walls.png");
	cocos2d::Sprite* ball = cocos2d::Sprite::create("ball.png");
	CocosDenshion::SimpleAudioEngine* audio = CocosDenshion::SimpleAudioEngine::getInstance();
	bool left = false;
	bool right = false;
	float balldir = M_PI*3.0/2.0;
	int mpad = 3;
	int mball = 2;
	float ss = 80.0;
	float sb = 20;
	int sw = 1200;
	int sh = 600;
    static cocos2d::Scene* createScene();
    virtual bool init();

    CREATE_FUNC(KeyboardScene);
	cocos2d::Vec2 movedir(int, cocos2d::Vec2);
	void update(float) override;

private:
    static std::map<cocos2d::EventKeyboard::KeyCode,
        std::chrono::high_resolution_clock::time_point> keys;
    cocos2d::Label* label;
};