#ifndef __GAME_SCENE_H__
#define __GAME_SCENE_H__

#include "cocos2d.h"
#include "Obstacle.h"

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

        void CreatePlayer();
        void AddPlayerToScene();
        void RemovePlayerFromScene();

        void AddObstacle();
        void RemoveObstacle(unsigned long Id);
        void RemoveAllObstacles();
        void GenerateObstacles();
        void ScrollObstacles();

        unsigned long tickCounter;
        float groundY;

        cocos2d::Sprite* bgScrollPrimary;
        cocos2d::Sprite* bgScrollSecondary;
        cocos2d::Sprite* bgStatic;

        cocos2d::Sprite* playerSprite;

        unsigned long obstacleCounter;
        unsigned long lastObstacleTime;
        typedef std::vector<Obstacle*> ObstacleVector;
        ObstacleVector obstacles;
};

#endif // __GAME_SCENE_H__
