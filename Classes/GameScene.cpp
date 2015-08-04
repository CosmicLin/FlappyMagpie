#include "GameScene.h"
#include "Configuration.h"
#include "Util.h"

USING_NS_CC;

Scene* GameScene::createScene()
{
    Scene* scene = Scene::create();
    GameScene* layer = GameScene::create();
    scene->addChild(layer);
    return scene;
}

bool GameScene::init()
{
    if (!LayerColor::initWithColor(Color4B(GAME_COLOUR)))
        return false;

    tickCounter = 0;
    groundY = 0;

    scheduleUpdate();
    CreateBackground();
    CreatePlayer();
    AddPlayerToScene();
    return true;
}

void GameScene::update(float dt)
{
    tick();
    ScrollBackground();
    GenerateObstacles();
    ScrollObstacles();
}

void GameScene::CreateBackground()
{
    bgScrollPrimary = Sprite::create("land.png");
    Util::ScaleSpriteToScreen(*bgScrollPrimary);
    bgScrollPrimary->setAnchorPoint(Vec2(0, 0));
    bgScrollPrimary->setPosition(Vec2(0, 0));
    groundY = bgScrollPrimary->getBoundingBox().size.height;

    bgScrollSecondary = Sprite::create("land.png");
    Util::ScaleSpriteToScreen(*bgScrollSecondary);
    bgScrollSecondary->setAnchorPoint(Vec2(0, 0));
    bgScrollSecondary->setPosition(Vec2(bgScrollPrimary->getBoundingBox().size.width, 0));

    bgStatic = Sprite::create("sky.png");
    Util::ScaleSpriteToScreen(*bgStatic);
    bgStatic->setAnchorPoint(Vec2(0, 0));
    bgStatic->setPosition(Vec2(0, groundY));

    addChild(bgScrollPrimary, BG_SCROLL_LAYER);
    addChild(bgScrollSecondary, BG_SCROLL_LAYER);
    addChild(bgStatic, BG_STATIC_LAYER);
}

void GameScene::ScrollBackground()
{
    bgScrollPrimary->setPosition(Vec2(bgScrollPrimary->getPosition().x - GAME_SCROLL_SPEED, bgScrollPrimary->getPosition().y));
    bgScrollSecondary->setPosition(Vec2(bgScrollSecondary->getPosition().x - GAME_SCROLL_SPEED, bgScrollSecondary->getPosition().y));

    if (bgScrollPrimary->getPosition().x < -bgScrollPrimary->getBoundingBox().size.width)
        bgScrollPrimary->setPosition(Vec2(bgScrollSecondary->getPosition().x + bgScrollSecondary->getBoundingBox().size.width, bgScrollPrimary->getPosition().y));

    if (bgScrollSecondary->getPosition().x < -bgScrollSecondary->getBoundingBox().size.width)
        bgScrollSecondary->setPosition(Vec2(bgScrollPrimary->getPosition().x + bgScrollPrimary->getBoundingBox().size.width, bgScrollSecondary->getPosition().y));
}

void GameScene::CreatePlayer()
{
    playerSprite = Sprite::create("bird.png");
    playerSprite->setName("Player");
    playerSprite->setScale(1.5f);
    playerSprite->setAnchorPoint(Vec2(0, 0));
    playerSprite->setPosition(Vec2(GAME_WIDTH / 4, (GAME_HEIGHT + bgScrollPrimary->getContentSize().height) / 2));
}

void GameScene::AddPlayerToScene()
{
    addChild(playerSprite, PLAYER_LAYER);
}

void GameScene::RemovePlayerFromScene()
{
    removeChild(playerSprite);
}

void GameScene::AddObstacle()
{
    float centerY = Util::frand(groundY + OBSTACLE_PADDING, GAME_HEIGHT - OBSTACLE_PADDING);

    Sprite* pipeUpper = Sprite::create("pipe-down.png");
    pipeUpper->setScale(1.5f);
    float upperY = centerY + (OBSTACLE_GAP / 2.f);
    pipeUpper->setAnchorPoint(Vec2(0, 0));
    pipeUpper->setPosition(Vec2(GAME_WIDTH, upperY));
    Sprite* pipeUpperBody = Sprite::create("pipe.png");
    pipeUpperBody->setScale(1.5f, GAME_HEIGHT - (upperY + pipeUpper->getBoundingBox().size.height));
    pipeUpperBody->setAnchorPoint(Vec2(0, 0));
    pipeUpperBody->setPosition(Vec2(pipeUpper->getPositionX(), upperY + pipeUpper->getBoundingBox().size.height));

    addChild(pipeUpper, OBSTACLE_LAYER);
    addChild(pipeUpperBody, OBSTACLE_LAYER);

    Sprite* pipeLower = Sprite::create("pipe-up.png");
    pipeLower->setScale(1.5f);
    float lowerY = (centerY - (OBSTACLE_GAP / 2.f)) - pipeLower->getBoundingBox().size.height;
    pipeLower->setAnchorPoint(Vec2(0, 0));
    pipeLower->setPosition(Vec2(GAME_WIDTH, lowerY));
    Sprite* pipeLowerBody = Sprite::create("pipe.png");
    pipeLowerBody->setScale(1.5f, lowerY - groundY);
    pipeLowerBody->setAnchorPoint(Vec2(0, 0));
    pipeLowerBody->setPosition(Vec2(pipeLower->getPositionX(), lowerY - pipeLowerBody->getBoundingBox().size.height));

    addChild(pipeLower, OBSTACLE_LAYER);
    addChild(pipeLowerBody, OBSTACLE_LAYER);

    obstacles.push_back(new Obstacle(++obstacleCounter, centerY, pipeUpper, pipeUpperBody, pipeLower, pipeLowerBody));
}

void GameScene::RemoveObstacle(unsigned long Id)
{
    for (ObstacleVector::iterator itr = obstacles.begin(); itr != obstacles.end(); ++itr)
    {
        Obstacle* o = (*itr);
        if (o->Id == Id)
        {
            removeChild(o->pipeLower);
            removeChild(o->pipeLowerBody);
            removeChild(o->pipeUpper);
            removeChild(o->pipeUpperBody);
            obstacles.erase(itr);
            break;
        }
    }
}

void GameScene::RemoveAllObstacles()
{
    for (ObstacleVector::iterator itr = obstacles.begin(); itr != obstacles.end(); ++itr)
    {
        Obstacle* o = (*itr);
        removeChild(o->pipeLower);
        removeChild(o->pipeLowerBody);
        removeChild(o->pipeUpper);
        removeChild(o->pipeUpperBody);
    }

    obstacles.clear();
}

void GameScene::GenerateObstacles()
{
    unsigned long msTime = Util::GetMSTime(tickCounter);
    if ((msTime - lastObstacleTime) >= OBSTACLE_FREQUENCY_TIME)
    {
        AddObstacle();
        lastObstacleTime = msTime;
    }
}

void GameScene::ScrollObstacles()
{
    unsigned long removeId = 0;
    for (ObstacleVector::iterator itr = obstacles.begin(); itr != obstacles.end(); ++itr)
    {
        Obstacle* o = (*itr);

        float pipeUpperNewX = o->pipeUpper->getPositionX() - GAME_SCROLL_SPEED;
        o->pipeUpper->setPositionX(pipeUpperNewX);

        float pipeUpperWidth = o->pipeUpper->getBoundingBox().size.width;
        if ((pipeUpperNewX) < -pipeUpperWidth)
            removeId = o->Id;

        float pipeUpperBodyNewX = o->pipeUpperBody->getPositionX() - GAME_SCROLL_SPEED;
        o->pipeUpperBody->setPositionX(pipeUpperBodyNewX);

        float pipeLowerNewX = o->pipeLower->getPositionX() - GAME_SCROLL_SPEED;
        o->pipeLower->setPositionX(pipeLowerNewX);

        float pipeLowerBodyNewX = o->pipeLowerBody->getPositionX() - GAME_SCROLL_SPEED;
        o->pipeLowerBody->setPositionX(pipeLowerBodyNewX);
    }

    if (removeId)
        RemoveObstacle(removeId);
}
