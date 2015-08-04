#include "Obstacle.h"

USING_NS_CC;

Obstacle::Obstacle(unsigned long _Id, float _centerY,
    cocos2d::Sprite* _pipeUpper, cocos2d::Sprite* _pipeUpperBody,
    cocos2d::Sprite* _pipeLower, cocos2d::Sprite* _pipeLowerBody)
{
    Id = _Id;
    centerY = _centerY;
    pipeUpper = _pipeUpper;
    pipeUpperBody = _pipeUpperBody;
    pipeLower = _pipeLower;
    pipeLowerBody = _pipeLowerBody;
}
