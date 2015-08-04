#ifndef __GAME_SCENE_H__
#define __GAME_SCENE_H__

#include "cocos2d.h"

class GameScene : public cocos2d::LayerColor
{
    public:
        CREATE_FUNC(GameScene);

        static cocos2d::Scene* createScene();

        virtual bool init();
};

#endif // __GAME_SCENE_H__
