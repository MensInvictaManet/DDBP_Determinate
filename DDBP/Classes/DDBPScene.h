#ifndef __DDBP_SCENE_H__
#define __DDBP_SCENE_H__

#include "cocos2d.h"

#include "ui/UIButton.h"
#include "2d/CCLabel.h"

class DDBP : public cocos2d::Layer
{
public:
    static cocos2d::Scene* createScene();

    virtual bool init();
    
    // a selector callback
    void menuCloseCallback(cocos2d::Ref* pSender);
    
    // implement the "static create()" method manually
    CREATE_FUNC(DDBP);

private:
	cocos2d::Label*			m_CardAction[5];
	cocos2d::ui::Button*	m_CardButton[5];

	int						m_CardIndex[5];

	void setCard(int buttonIndex, int cardIndex);
	void iterateCard(int cardIndex, int amount = 1);
	void determineBestMove();
};

#endif // __DDBP_SCENE_H__
