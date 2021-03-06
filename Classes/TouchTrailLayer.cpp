﻿/*
 * cocos2d+ext for iPhone
 *
 * Copyright (c) 2011 - Ngo Duc Hiep
 * Copyright (c) 2012 - YangLe
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"), to deal
 * in the Software without restriction, including without limitation the rights
 * to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 * copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in
 * all copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
 * THE SOFTWARE.
 *
 */

#include "TouchTrailLayer.h"
#include "CCBlade.h"
#include "ResManager.h"
#include "BaseScense.h"

#define kFileStreak "streak.png"

USING_NS_CC;

TouchTrailLayer::TouchTrailLayer()
{
    setTouchEnabled(true);
}

TouchTrailLayer* TouchTrailLayer::create()
{
    TouchTrailLayer *pRet = new TouchTrailLayer();
    pRet->autorelease();
    return pRet;
}

void TouchTrailLayer::ccTouchesBegan(CCSet *pTouches, CCEvent *pEvent)
{
	if (!BaseScense::getCurrentScene()->getCollisionEnable())
	{
		return;
	}

    for (CCSetIterator it = pTouches->begin(); it != pTouches->end(); it++) {
        CCTouch *touch = (CCTouch *)*it;
		CCBlade *blade = CCBlade::create(ResManager::getManager()->getSharedFilePath(g_gameinPath+kFileStreak).c_str(), 5, 40);
        _map[touch] = blade;
		addChild(blade);
        
        //blade->setColor(ccc3(255,0,0));
        blade->setOpacity(200);
		
		//修改滑动屏幕时不出现光刀的bug
		//由120->改回60
		//setDrainInterval(1.0/120)--->setDrainInterval(1.0/60);
		blade->setDrainInterval(1.0f/60.0f);
        
        CCPoint point = touch->getLocationInView();
        point = CCDirector::sharedDirector()->convertToGL(point);
//        CCPoint point = convertTouchToNodeSpace(touch);
		blade->push(point);
	}
}

void TouchTrailLayer::ccTouchesMoved(CCSet *pTouches, CCEvent *pEvent)
{
    for (CCSetIterator it = pTouches->begin(); it != pTouches->end(); it++) {
        CCTouch *touch = (CCTouch *)*it;
        if (_map.find(touch) == _map.end()) continue;
        
        CCBlade *blade = _map[touch];
//        CCPoint point = convertTouchToNodeSpace(touch);
        CCPoint point = touch->getLocationInView();
        point = CCDirector::sharedDirector()->convertToGL(point);
		blade->push(point);
    }
}

void TouchTrailLayer::ccTouchesEnded(CCSet *pTouches, CCEvent *pEvent)
{
    for (CCSetIterator it = pTouches->begin(); it != pTouches->end(); it++) {
        CCTouch *touch = (CCTouch *)*it;
        if (_map.find(touch) == _map.end()) continue;
        
        CCBlade *blade = _map[touch];
        blade->autoCleanup();
        _map.erase(touch);
    }
}