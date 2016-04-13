#include "DDBPScene.h"

#include "Poker.h"

USING_NS_CC;

const char* cardFilenames[52] = 
{
	"AS", "2S", "3S", "4S", "5S", "6S", "7S", "8S", "9S", "TS", "JS", "QS", "KS",
	"AC", "2C", "3C", "4C", "5C", "6C", "7C", "8C", "9C", "TC", "JC", "QC", "KD",
	"AD", "2D", "3D", "4D", "5D", "6D", "7D", "8D", "9D", "TD", "JD", "QD", "KD",
	"AH", "2H", "3H", "4H", "5H", "6H", "7H", "8H", "9H", "TH",	"JH", "QH", "KH"
};

Scene* DDBP::createScene()
{
    // 'scene' is an autorelease object
    auto scene = Scene::create();
    
    // 'layer' is an autorelease object
    auto layer = DDBP::create();

    // add layer as a child to scene
    scene->addChild(layer);

    // return the scene
    return scene;
}

// on "init" you need to initialize your instance
bool DDBP::init()
{
    //////////////////////////////
    // 1. super init first
    if ( !Layer::init() )
    {
        return false;
    }
    
    Size visibleSize = Director::getInstance()->getVisibleSize();
    Vec2 origin = Director::getInstance()->getVisibleOrigin();

    /////////////////////////////
    // 2. add a menu item with "X" image, which is clicked to quit the program
    //    you may modify it.

    // add a "close" icon to exit the progress. it's an autorelease object
    auto closeItem = MenuItemImage::create(
                                           "CloseNormal.png",
                                           "CloseSelected.png",
                                           CC_CALLBACK_1(DDBP::menuCloseCallback, this));
    
	closeItem->setPosition(Vec2(origin.x + visibleSize.width - closeItem->getContentSize().width/2 ,
                                origin.y + closeItem->getContentSize().height/2));

    // create menu, it's an autorelease object
    auto menu = Menu::create(closeItem, NULL);
    menu->setPosition(Vec2::ZERO);
    this->addChild(menu, 1);

	//  Create the five cards and the suit changing button under them
	for (int i = 0; i < 5; ++i)
	{
		m_CardAction[i] = cocos2d::Label::create();
		m_CardAction[i]->setPosition(Vec2(visibleSize.width / 2 - 256 + (i * 128), visibleSize.height / 2 + 100));
		m_CardAction[i]->setBMFontSize(20.0f);
		this->addChild(m_CardAction[i], 1);

		m_CardButton[i] = cocos2d::ui::Button::create();
		m_CardButton[i]->setPosition(Vec2(visibleSize.width / 2 - 256 + (i * 128), visibleSize.height / 2));
		m_CardButton[i]->addClickEventListener([=](Ref*) { iterateCard(i); });
		setCard(i, i);
		this->addChild(m_CardButton[i], 1);

		auto suitChange = cocos2d::ui::Button::create("SuitButton.png");
		suitChange->setPosition(Vec2(visibleSize.width / 2 - 256 + (i * 128), visibleSize.height / 2 - 80));
		suitChange->addClickEventListener([=](Ref*) { iterateCard(i, 13); });
		this->addChild(suitChange, 1);
	}

	auto determine = cocos2d::ui::Button::create("DetermineButton.png");
	determine->setPosition(Vec2(visibleSize.width / 2, visibleSize.height / 2 - 130));
	determine->addClickEventListener([=](Ref*) { determineBestMove(); });
	this->addChild(determine, 1);

	//  Set the cards to their starting types
	setCard(0, 0);
	setCard(1, 1);
	setCard(2, 2);
	setCard(3, 3);
	setCard(4, 4);

	//  Set up the functions needed to test for best moves
	InitializeStatistics();

    return true;
}


void DDBP::menuCloseCallback(Ref* pSender)
{
    Director::getInstance()->end();

#if (CC_TARGET_PLATFORM == CC_PLATFORM_IOS)
    exit(0);
#endif
}

void DDBP::setCard(int buttonIndex, int cardIndex)
{
	if (buttonIndex < 0 || buttonIndex >= 5) return;
	if (cardIndex >= 52) cardIndex -= 52;

	char filename[64];
	sprintf(filename, "Cards/%s.png", cardFilenames[cardIndex]);

	m_CardIndex[buttonIndex] = cardIndex;
	m_CardButton[buttonIndex]->loadTextureNormal(filename);
}

void DDBP::iterateCard(int buttonIndex, int amount)
{
	if (buttonIndex < 0 || buttonIndex >= 5) return;

	int newCardIndex = m_CardIndex[buttonIndex];
	while (true)
	{
		newCardIndex += amount;
		if (newCardIndex >= 52) newCardIndex -= 52;
		bool alreadyUsed = false;
		for (int i = 0; i < 5; ++i)
		{
			if (i == buttonIndex) continue;
			if (m_CardIndex[i] == newCardIndex)
			{
				alreadyUsed = true;
				break;
			}
		}
		if (alreadyUsed == false) break;
	}

	setCard(buttonIndex, newCardIndex);
}


void DDBP::determineBestMove()
{
	River river(m_CardIndex[0], m_CardIndex[1], m_CardIndex[2], m_CardIndex[3], m_CardIndex[4]);
	DetermineBestMove(river);

	std::string bestMoveString = DescribeMove(river.BestMove);
	for (int i = 0; i < 5; ++i)
	{
		std::string singleChar = "";
		singleChar += bestMoveString[i];
		m_CardAction[i]->setString(singleChar);
	}
}