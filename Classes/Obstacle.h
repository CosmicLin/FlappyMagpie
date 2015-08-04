#ifndef __OBSTACLE_H__
#define __OBSTACLE_H__

#include "cocos2d.h"

class Obstacle
{
    public:
        Obstacle(unsigned long _Id, float _centerY,
            cocos2d::Sprite* _pipeUpper, cocos2d::Sprite* _pipeUpperBody,
            cocos2d::Sprite* _pipeLower, cocos2d::Sprite* _pipeLowerBody);

        unsigned long Id;
        float centerY;
        cocos2d::Sprite* pipeUpper;
        cocos2d::Sprite* pipeUpperBody;
        cocos2d::Sprite* pipeLower;
        cocos2d::Sprite* pipeLowerBody;
};

#endif // __OBSTACLE_H__
