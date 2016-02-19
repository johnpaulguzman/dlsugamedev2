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
	std::vector<cocos2d::Sprite*> blocks;
	int bh = 30;
	int bw = 100;
	CocosDenshion::SimpleAudioEngine* audio = CocosDenshion::SimpleAudioEngine::getInstance();
	float initdir = M_PI / 180.0 * 270;
	bool left = false;
	bool right = false;
	float balldir = initdir;
	float mpad = 5;
	float mball = 5;
	float ss = 80.0;
	float sb = 10.0;
	int cd = 5;
	int sw = 1200;
	int sh = 600;
    static cocos2d::Scene* createScene();
    virtual bool init();

    CREATE_FUNC(KeyboardScene);
	cocos2d::Sprite* createRandomBlock();
	bool doesCollide(cocos2d::Vec2, cocos2d::Vec2);
	bool doesCollideBlock(cocos2d::Vec2, cocos2d::Vec2);
	void update(float) override;

private:
    static std::map<cocos2d::EventKeyboard::KeyCode,
        std::chrono::high_resolution_clock::time_point> keys;
    cocos2d::Label* label;
};