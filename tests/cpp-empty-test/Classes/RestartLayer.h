#ifndef __RESTART_LAYER_H__
#define __RESTART_LAYER_H__

#include "cocos2d.h"

class RestartLayer : public cocos2d::LayerColor
{
public:
    virtual void onEnter();

    void onRetry(Ref* sender);

    bool onTouchBegan(cocos2d::Touch* touch, cocos2d::Event  *event);
    void onTouchEnded(cocos2d::Touch* touch, cocos2d::Event  *event);

    void setTipLabel(const std::string & tipMsg);
    CREATE_FUNC(RestartLayer);
    
private:
    RestartLayer();

    cocos2d::MenuItemLabel* _itemRestart;
    cocos2d::Label* _tipLabel;
    std::string _tipMsg;
};

#endif // __RESTART_LAYER_H__
