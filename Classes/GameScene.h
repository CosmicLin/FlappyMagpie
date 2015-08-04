#ifndef __GAME_SCENE_H__
#define __GAME_SCENE_H__

#include "cocos2d.h"

class GameScene : public cocos2d::LayerColor
{
    public:
        CREATE_FUNC(GameScene);

        static cocos2d::Scene* createScene();

        virtual bool init();
        void update(float dt);

        void tick() { ++tickCounter; }

        void CreateBackground();
        void ScrollBackground();

        unsigned long tickCounter;
        float groundY;

        cocos2d::Sprite* bgScrollPrimary;
        cocos2d::Sprite* bgScrollSecondary;
        cocos2d::Sprite* bgStatic;
};

#endif // __GAME_SCENE_H__
