#include "GameScene.h"
#include "Configuration.h"
#include "Util.h"

USING_NS_CC;

Scene* GameScene::createScene()
{
    Scene* scene = Scene::createWithPhysics();
    scene->getPhysicsWorld()->setSpeed(GAME_PHYSICS_SPEED);
    scene->getPhysicsWorld()->setGravity(Vec2(GAME_PHYSICS_GRAVITY));
    //scene->getPhysicsWorld()->setDebugDrawMask(PhysicsWorld::DEBUGDRAW_ALL);

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
    gameStart = true;
    gameRunning = false;
    gameOver = false;
    scrollBackground = false;
    genObstacles = false;
    InitListeners();

    CreateBackground();

    DisplayStartScreen();
    return true;
}

void GameScene::update(float dt)
{
    if (gameRunning)
    {
        tick();

        ScrollBackground();

        GenerateObstacles();
        ScrollObstacles();

        CheckCollision();
        CheckPlayerJump();

        IncrementScore();
    }

    CheckPosition();
}

void GameScene::InitListeners()
{
    EventListenerTouchOneByOne* tListener = EventListenerTouchOneByOne::create();
    tListener->setSwallowTouches(true);
    tListener->onTouchBegan = CC_CALLBACK_2(GameScene::onTouchBegan, this);
    Director::getInstance()->getEventDispatcher()->addEventListenerWithSceneGraphPriority(tListener, this);
}

bool GameScene::onTouchBegan(cocos2d::Touch* touch, cocos2d::Event* event)
{
    Vec2 p = touch->getLocation();

    if (gameStart)
        if (startButton->getBoundingBox().containsPoint(p))
        {
            StarGame();
            return true;
        }

    if (gameRunning)
    {
        PlayerJump();
        return true;
    }

    if (gameOver)
        if (playAgainButton->getBoundingBox().containsPoint(p))
        {
            StarGame();
            return true;
        }

    return false;
}

void GameScene::DisplayStartScreen()
{
    gameTitleImage = Sprite::create("title.png");
    gameTitleImage->setAnchorPoint(Vec2(0, 0));
    gameTitleImage->setPosition(Vec2((GAME_WIDTH / 2) - (gameTitleImage->getBoundingBox().size.width / 2), ((GAME_HEIGHT - groundY) / 5) * 4));
    addChild(gameTitleImage, TITLE_LAYER);

    startButton = Sprite::create("start.png");
    startButton->setAnchorPoint(Vec2(0, 0));
    startButton->setPosition(Vec2((GAME_WIDTH / 2) - (startButton->getBoundingBox().size.width / 2), ((GAME_HEIGHT - groundY) / 5) * 3));
    addChild(startButton, TITLE_LAYER);
}

void GameScene::DisplayGameOverScreen()
{
    gameOverLabel = Label::createWithTTF("Game Over", "font/flappyfont.ttf", FONT_SIZE_GAME_OVER);
    gameOverLabel->enableOutline(Color4B(0, 0, 0, 255), 3);
    gameOverLabel->setAnchorPoint(Vec2(0, 0));
    gameOverLabel->setPosition(Vec2((GAME_WIDTH / 2) - (gameOverLabel->getBoundingBox().size.width / 2), ((GAME_HEIGHT - groundY) / 5) * 4));
    addChild(gameOverLabel, TITLE_LAYER);

    playAgainButton = Sprite::create("playagain.png");
    playAgainButton->setAnchorPoint(Vec2(0, 0));
    playAgainButton->setPosition(Vec2((GAME_WIDTH / 2) - (playAgainButton->getBoundingBox().size.width / 2), ((GAME_HEIGHT - groundY) / 5) * 3));
    addChild(playAgainButton, TITLE_LAYER);
}

void GameScene::StarGame()
{
    RemoveAllObstacles();
    removeChild(scoreLabel);
    removeChild(gameOverLabel);
    removeChild(playAgainButton);
    removeChild(playerSprite);
    removeChild(gameTitleImage);
    removeChild(startButton);
    tickCounter = 0;
    score = 0;
    scrollBackground = true;
    obstacleCounter = 0;
    genObstacles = true;
    gameStart = false;
    gameRunning = true;
    gameOver = false;
    AddScoreLabel();
    CreatePlayer();
    AddPlayerToScene();

    scheduleUpdate();
}

void GameScene::EndGame()
{
    scrollBackground = false;
    genObstacles = false;
    gameRunning = false;
    gameOver = true;
    DisplayGameOverScreen();
}

void GameScene::CheckCollision()
{
    bool GameOver = false;
    float playerX = playerSprite->getPositionX() + playerSprite->getBoundingBox().size.width / 2;
    float playerTopY = playerSprite->getPositionY() + playerSprite->getBoundingBox().size.width / 2;
    float playerBottomY = playerSprite->getPositionY() - playerSprite->getBoundingBox().size.width / 2;

    if (playerTopY >= GAME_HEIGHT)
    {
        playerBody->setGravityEnable(true);
        playerBody->setVelocity(Vec2(0, 20.f));
        playerJumping = false;
    }

    if (playerBottomY <= groundY)
        GameOver = true;

    if (genObstacles)
    {
        for (ObstacleVector::iterator itr = obstacles.begin(); itr != obstacles.end(); ++itr)
        {
            Obstacle* o = (*itr);

            float pipeUpperLeftSide = o->pipeUpper->getPositionX();
            float pipeUpperRightSide = o->pipeUpper->getPositionX() + o->pipeUpper->getBoundingBox().size.width;
            float pipeUpperBottomSide = o->pipeUpper->getPositionY();
            float pipeUpperTopSide = o->pipeUpper->getPositionY() + o->pipeUpper->getBoundingBox().size.height;
            if ((playerX >= pipeUpperLeftSide && playerX <= pipeUpperRightSide) &&
                (playerTopY >= pipeUpperBottomSide && playerBottomY <= pipeUpperTopSide))
                GameOver = true;

            float pipeUpperBodyLeftSide = o->pipeUpperBody->getPositionX();
            float pipeUpperBodyRightSide = o->pipeUpperBody->getPositionX() + o->pipeUpperBody->getBoundingBox().size.width;
            float pipeUpperBodyBottomSide = o->pipeUpperBody->getPositionY();
            float pipeUpperBodyTopSide = o->pipeUpperBody->getPositionY() + o->pipeUpperBody->getBoundingBox().size.height;
            if ((playerX >= pipeUpperBodyLeftSide && playerX <= pipeUpperBodyRightSide) &&
                (playerTopY >= pipeUpperBodyBottomSide && playerBottomY <= pipeUpperBodyTopSide))
                GameOver = true;

            float pipeLowerLeftSide = o->pipeLower->getPositionX();
            float pipeLowerRightSide = o->pipeLower->getPositionX() + o->pipeLower->getBoundingBox().size.width;
            float pipeLowerBottomSide = o->pipeLower->getPositionY();
            float pipeLowerTopSide = o->pipeLower->getPositionY() + o->pipeLower->getBoundingBox().size.height;
            if ((playerX >= pipeLowerLeftSide && playerX <= pipeLowerRightSide) &&
                (playerTopY >= pipeLowerBottomSide && playerBottomY <= pipeLowerTopSide))
                GameOver = true;

            float pipeLowerBodyLeftSide = o->pipeLowerBody->getPositionX();
            float pipeLowerBodyRightSide = o->pipeLowerBody->getPositionX() + o->pipeLowerBody->getBoundingBox().size.width;
            float pipeLowerBodyBottomSide = o->pipeLowerBody->getPositionY();
            float pipeLowerBodyTopSide = o->pipeLowerBody->getPositionY() + o->pipeLowerBody->getBoundingBox().size.height;
            if ((playerX >= pipeLowerBodyLeftSide && playerX <= pipeLowerBodyRightSide) &&
                (playerTopY >= pipeLowerBodyBottomSide && playerBottomY <= pipeLowerBodyTopSide))
                GameOver = true;
        }
    }

    if (GameOver)
        EndGame();
}

void GameScene::CheckPosition()
{
    if (Node* plNode = getChildByName("Player"))
        if (plNode->getPositionY() - plNode->getBoundingBox().size.width / 2 <= groundY)
            unscheduleUpdate();
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
    if (scrollBackground)
    {
        bgScrollPrimary->setPosition(Vec2(bgScrollPrimary->getPosition().x - GAME_SCROLL_SPEED, bgScrollPrimary->getPosition().y));
        bgScrollSecondary->setPosition(Vec2(bgScrollSecondary->getPosition().x - GAME_SCROLL_SPEED, bgScrollSecondary->getPosition().y));

        if (bgScrollPrimary->getPosition().x < -bgScrollPrimary->getBoundingBox().size.width)
            bgScrollPrimary->setPosition(Vec2(bgScrollSecondary->getPosition().x + bgScrollSecondary->getBoundingBox().size.width, bgScrollPrimary->getPosition().y));

        if (bgScrollSecondary->getPosition().x < -bgScrollSecondary->getBoundingBox().size.width)
            bgScrollSecondary->setPosition(Vec2(bgScrollPrimary->getPosition().x + bgScrollPrimary->getBoundingBox().size.width, bgScrollSecondary->getPosition().y));
    }
}

void GameScene::CreatePlayer()
{
    playerSprite = Sprite::create("bird.png");
    playerSprite->setName("Player");
    playerSprite->setScale(1.5f);
    playerSprite->setAnchorPoint(Vec2(0, 0));
    playerSprite->setPosition(Vec2(GAME_WIDTH / 4, (GAME_HEIGHT + bgScrollPrimary->getContentSize().height) / 2));

    playerBody = PhysicsBody::createBox(playerSprite->getBoundingBox().size); // orginal flappy bird difficulty :^)
    playerSprite->setPhysicsBody(playerBody);

    playerJumping = false;
    JumpY = 0;
}

void GameScene::AddPlayerToScene()
{
    addChild(playerSprite, PLAYER_LAYER);
}

void GameScene::RemovePlayerFromScene()
{
    removeChild(playerSprite);
}

void GameScene::PlayerJump()
{
    if (gameRunning)
    {
        playerJumping = true;
        JumpY = playerSprite->getPositionY();
        playerBody->setGravityEnable(false);
        playerBody->setVelocity(Vec2(PLAYER_JUMP_VELOCITY));
    }
}

void GameScene::CheckPlayerJump()
{
    if (playerJumping)
    {
        if (playerSprite->getPositionY() - JumpY > PLAYER_JUMP_HEIGHT)
        {
            playerBody->setGravityEnable(true);
            playerJumping = false;
        }
    }
}

void GameScene::AddObstacle()
{
    if (genObstacles)
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
    if (genObstacles)
    {
        unsigned long msTime = Util::GetMSTime(tickCounter);
        if ((msTime - lastObstacleTime) >= OBSTACLE_FREQUENCY_TIME)
        {
            AddObstacle();
            lastObstacleTime = msTime;
        }
    }
}

void GameScene::ScrollObstacles()
{
    if (genObstacles)
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
}

void GameScene::AddScoreLabel()
{
    scoreLabel = Label::createWithTTF("0", "font/flappyfont.ttf", FONT_SIZE_SCORE);
    scoreLabel->enableOutline(Color4B(0, 0, 0, 255), 3);
    scoreLabel->setAnchorPoint(Vec2(0, 0));
    scoreLabel->setPosition(Vec2((GAME_WIDTH - FONT_SIZE_SCORE) / 2, (GAME_HEIGHT / 5) * 4));
    addChild(scoreLabel, SCORE_LAYER);
}

void GameScene::IncrementScore()
{
    if ((Util::GetMSTime(tickCounter) / 1000) > score)
    {
        ++score;
        scoreLabel->setString(std::to_string(score));
    }
}
