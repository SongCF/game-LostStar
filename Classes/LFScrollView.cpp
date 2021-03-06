﻿////
////  SWScrollView.m
////  SWGameLib
////
////  Copyright (c) 2010-2012  cocos2d-x.org
////  Copyright (c) 2010 Sangwoo Im
////
////  Permission is hereby granted, free of charge, to any person obtaining a copy
////  of this software and associated documentation files (the "Software"), to deal
////  in the Software without restriction, including without limitation the rights
////  to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
////  copies of the Software, and to permit persons to whom the Software is
////  furnished to do so, subject to the following conditions:
////
////  The above copyright notice and this permission notice shall be included in
////  all copies or substantial portions of the Software.
////
////  THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
////  IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
////  FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN false EVENT SHALL THE
////  AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
////  LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
////  OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
////  THE SOFTWARE.
////  
////  Created by Sangwoo Im on 6/3/10.
////  Copyright 2010 Sangwoo Im. All rights reserved.
////	
////	回弹是否开启：
////	如果开启：
////	需要更改的：当平滑超过了container内容后，平滑的减速加速度就应该变很大，让其停止再往前滑动
////	然后，就开始恢复到极限位置，两种方式：加速再减速；初速度减速。用方式二合适。
////
////	不开启：
////	根本不用实现加减速
////
////	计划采取办法：直接有意图移动就直接移动了
////	
////
//
//#include "CCScrollView.h"
//#include "actions/CCActionInterval.h"
//#include "actions/CCActionTween.h"
//#include "actions/CCActionInstant.h"
//#include "support/CCPointExtension.h"
//#include "touch_dispatcher/CCTouchDispatcher.h"
//#include "effects/CCGrid.h"
//#include "CCDirector.h"
//#include "kazmath/GL/matrix.h"
//#include "touch_dispatcher/CCTouch.h"
//
//NS_CC_EXT_BEGIN
//
//#define SCROLL_DEACCEL_RATE  0.95f //减速的平率
//#define SCROLL_DEACCEL_DIST  1.0f  //一个数值，当每帧减速时移动的像素，小于这么多的时候，就直接设置到原处。
//#define BOUNCE_DURATION      0.15f //回弹的耗时
//#define INSET_RATIO          1.0f  //宽度的0.2移动   #define INSET_RATIO          0.2f
//
//bool bunceEnale = false;
//
//CCScrollView::CCScrollView()
//: m_fZoomScale(0.0f)
//, m_fMinZoomScale(0.0f)
//, m_fMaxZoomScale(0.0f)
//, m_pDelegate(NULL)
//, m_bDragging(false)
//, m_bBounceable(false)
//, m_eDirection(CCScrollViewDirectionBoth)
//, m_bClippingToBounds(false)
//, m_pContainer(NULL)
//, m_bTouchMoved(false)
//, m_fTouchLength(0.0f)
//, m_pTouches(NULL)
//, m_fMinScale(0.0f)
//, m_fMaxScale(0.0f)
//,	m_canDeaccelerate(false)//添加的，默认不开启减速
//{
//	m_MaxOffset = CCPointMake(0,0);
//}
//
//CCScrollView::~CCScrollView()
//{
//    m_pTouches->release();
//}
//
//CCScrollView* CCScrollView::viewWithViewSize(CCSize size, CCNode* container/* = NULL*/)
//{
//    return CCScrollView::create(size, container);
//}
//
//CCScrollView* CCScrollView::create(CCSize size, CCNode* container/* = NULL*/)
//{
//    CCScrollView* pRet = new CCScrollView();
//    if (pRet && pRet->initWithViewSize(size, container))
//    {
//        pRet->autorelease();
//    }
//    else
//    {
//        CC_SAFE_DELETE(pRet);
//    }
//    return pRet;
//}
//
//CCScrollView* CCScrollView::node()
//{
//    return CCScrollView::create();
//}
//
//CCScrollView* CCScrollView::create()
//{
//    CCScrollView* pRet = new CCScrollView();
//    if (pRet && pRet->init())
//    {
//        pRet->autorelease();
//    }
//    else
//    {
//        CC_SAFE_DELETE(pRet);
//    }
//    return pRet;
//}
//
//
//bool CCScrollView::initWithViewSize(CCSize size, CCNode *container/* = NULL*/)
//{
//    if (CCLayer::init())
//    {
//        m_pContainer = container;
//        
//        if (!this->m_pContainer)
//        {
//            m_pContainer = CCLayer::create();
//        }
//
//        this->setViewSize(size);
//
//        setTouchEnabled(true);
//        m_pTouches = new CCArray();
//        m_pDelegate = NULL;
//        m_bBounceable = true;
//        m_bClippingToBounds = true;
//        //m_pContainer->setContentSize(CCSizeZero);
//        m_eDirection  = CCScrollViewDirectionBoth;
//        m_pContainer->setPosition(ccp(0.0f, 0.0f));
//        m_fTouchLength = 0.0f;
//        
//        this->addChild(m_pContainer);
//        m_fMinScale = m_fMaxScale = 1.0f;
//        return true;
//    }
//    return false;
//}
//
//bool CCScrollView::init()
//{
//    return this->initWithViewSize(CCSizeMake(200, 200), NULL);
//}
//
//void CCScrollView::registerWithTouchDispatcher()
//{
//    CCDirector::sharedDirector()->getTouchDispatcher()->addTargetedDelegate(this, 0, false);
//}
//
//bool CCScrollView::isNodeVisible(CCNode* node)
//{
//    const CCPoint offset = this->getContentOffset();
//    const CCSize  size   = this->getViewSize();
//    const float   scale  = this->getZoomScale();
//    
//    CCRect viewRect;
//    
//    viewRect = CCRectMake(-offset.x/scale, -offset.y/scale, size.width/scale, size.height/scale); 
//    
//    return CCRect::CCRectIntersectsRect(viewRect, node->boundingBox());
//}
//
//void CCScrollView::pause(CCObject* sender)
//{
//    m_pContainer->pauseSchedulerAndActions();
//
//    CCObject* pObj = NULL;
//    CCArray* pChildren = m_pContainer->getChildren();
//
//    CCARRAY_FOREACH(pChildren, pObj)
//    {
//        CCNode* pChild = (CCNode*)pObj;
//        pChild->pauseSchedulerAndActions();
//    }
//}
//
//void CCScrollView::resume(CCObject* sender)
//{
//    CCObject* pObj = NULL;
//    CCArray* pChildren = m_pContainer->getChildren();
//
//    CCARRAY_FOREACH(pChildren, pObj)
//    {
//        CCNode* pChild = (CCNode*)pObj;
//        pChild->resumeSchedulerAndActions();
//    }
//
//    m_pContainer->resumeSchedulerAndActions();
//}
//
//void CCScrollView::setTouchEnabled(bool e)
//{
//    CCLayer::setTouchEnabled(e);
//    if (!e)
//    {
//        m_bDragging = false;
//        m_bTouchMoved = false;
//        m_pTouches->removeAllObjects();
//    }
//}
//void CCScrollView::setDeaccelerateEnable(bool e)
//{
//    m_canDeaccelerate = e;
//}
//
//void CCScrollView::setContentOffset(CCPoint offset, bool animated/* = false*/)
//{
//    if (animated)
//    { //animate scrolling
//        this->setContentOffsetInDuration(offset, BOUNCE_DURATION);
//    } 
//    else
//    { //set the container position directly
//        if (!m_bBounceable)
//        {
//            const CCPoint minOffset = this->minContainerOffset();
//            const CCPoint maxOffset = this->maxContainerOffset();
//            
//            offset.x = MAX(minOffset.x, MIN(maxOffset.x, offset.x));
//            offset.y = MAX(minOffset.y, MIN(maxOffset.y, offset.y));
//        }
//
//        m_pContainer->setPosition(offset);
//
//        if (m_pDelegate != NULL)
//        {
//            m_pDelegate->scrollViewDidScroll(this);   
//        }
//    }
//}
//
//void CCScrollView::setContentOffsetInDuration(CCPoint offset, float dt)
//{
//    CCFiniteTimeAction *scroll, *expire;
//    
//    scroll = CCMoveTo::create(dt, offset);
//    expire = CCCallFuncN::create(this, callfuncN_selector(CCScrollView::stoppedAnimatedScroll));
//    m_pContainer->runAction(CCSequence::create(scroll, expire, NULL));
//    this->schedule(schedule_selector(CCScrollView::performedAnimatedScroll));
//}
//
//CCPoint CCScrollView::getContentOffset()
//{
//    return m_pContainer->getPosition();
//}
//
//void CCScrollView::setZoomScale(float s)
//{
//    if (m_pContainer->getScale() != s)
//    {
//        CCPoint oldCenter, newCenter;
//        CCPoint center;
//        
//        if (m_fTouchLength == 0.0f) 
//        {
//            center = ccp(m_tViewSize.width*0.5f, m_tViewSize.height*0.5f);
//            center = this->convertToWorldSpace(center);
//        }
//        else
//        {
//            center = m_tTouchPoint;
//        }
//        
//        oldCenter = m_pContainer->convertToNodeSpace(center);
//        m_pContainer->setScale(MAX(m_fMinScale, MIN(m_fMaxScale, s)));
//        newCenter = m_pContainer->convertToWorldSpace(oldCenter);
//        
//        const CCPoint offset = ccpSub(center, newCenter);
//        if (m_pDelegate != NULL)
//        {
//            m_pDelegate->scrollViewDidZoom(this);
//        }
//        this->setContentOffset(ccpAdd(m_pContainer->getPosition(),offset));
//    }
//}
//
//CCFloat CCScrollView::getZoomScale()
//{
//    return m_pContainer->getScale();
//}
//
//void CCScrollView::setZoomScale(float s, bool animated)
//{
//    if (animated)
//    {
//        this->setZoomScaleInDuration(s, BOUNCE_DURATION);
//    }
//    else
//    {
//        this->setZoomScale(s);
//    }
//}
//
//void CCScrollView::setZoomScaleInDuration(float s, float dt)
//{
//    if (dt > 0)
//    {
//        if (m_pContainer->getScale() != s)
//        {
//            CCActionTween *scaleAction;
//            scaleAction = CCActionTween::create(dt, "zoomScale", m_pContainer->getScale(), s);
//            this->runAction(scaleAction);
//        }
//    }
//    else
//    {
//        this->setZoomScale(s);
//    }
//}
//
//void CCScrollView::setViewSize(CCSize size)
//{
//    m_tViewSize = size;
//
//    if (this->m_pContainer != NULL)
//    {
//        m_fMaxInset = this->maxContainerOffset();//这里最大的offset是0，因为不能再向右滑了
//        m_fMaxInset = ccp(m_fMaxInset.x + m_tViewSize.width * INSET_RATIO,
//        m_fMaxInset.y + m_tViewSize.height * INSET_RATIO);
//        m_fMinInset = this->minContainerOffset();//这里最小的ofsset就是整个container的大小
//        m_fMinInset = ccp(m_fMinInset.x - m_tViewSize.width * INSET_RATIO,
//        m_fMinInset.y - m_tViewSize.height * INSET_RATIO);
//    }
//
//    CCLayer::setContentSize(size);
//}
//
//CCNode * CCScrollView::getContainer()
//{
//    return this->m_pContainer;
//}
//
//void CCScrollView::setContainer(CCNode * pContainer)
//{
//    this->removeAllChildrenWithCleanup(true);
//
//    if (!pContainer) return;
//
//    this->m_pContainer = pContainer;
//
//    this->m_pContainer->ignoreAnchorPointForPosition(false);
//    this->m_pContainer->setAnchorPoint(ccp(0.0f, 0.0f));
//
//    this->addChild(this->m_pContainer);
//
//    this->setViewSize(this->m_tViewSize);
//}
//
//void CCScrollView::relocateContainer(bool animated)
//{
//    CCPoint oldPoint, min, max;
//    CCFloat newX, newY;
//    
//    min = this->minContainerOffset();
//    max = this->maxContainerOffset();
//    
//    oldPoint = m_pContainer->getPosition();
//
//    newX     = oldPoint.x;
//    newY     = oldPoint.y;
//    if (m_eDirection == CCScrollViewDirectionBoth || m_eDirection == CCScrollViewDirectionHorizontal)
//    {
//        newX     = MIN(newX, max.x);
//        newX     = MAX(newX, min.x);
//    }
//
//    if (m_eDirection == CCScrollViewDirectionBoth || m_eDirection == CCScrollViewDirectionVertical)
//    {
//        newY     = MIN(newY, max.y);
//        newY     = MAX(newY, min.y);
//    }
//
//    if (newY != oldPoint.y || newX != oldPoint.x)
//    {
//        this->setContentOffset(ccp(newX, newY), animated);
//    }
//}
//
//void CCScrollView::setMaxContainerOffset(CCPoint offset)
//{
//    m_MaxOffset = offset;
//}
//
//CCPoint CCScrollView::maxContainerOffset()
//{
//    return m_MaxOffset;
//    //return ccp(0.0f, 0.0f);
//}
//
//CCPoint CCScrollView::minContainerOffset()
//{
//    return ccp(m_tViewSize.width - m_pContainer->getContentSize().width*m_pContainer->getScaleX(), 
//               m_tViewSize.height - m_pContainer->getContentSize().height*m_pContainer->getScaleY());
//}
//
//
//void CCScrollView::deaccelerateScrolling(float dt)//一旦手拿开了就会触发这个函数，不断地每一帧都调用
//{
//    if (m_bDragging)//如果又开始拖动了，那么就取消减速
//    {
//        this->unschedule(schedule_selector(CCScrollView::deaccelerateScrolling));
//        return;
//    }
//    
//    CCFloat newX, newY;
//    CCPoint maxInset, minInset;
//
//	//新加的代码
//	float minOffset = minContainerOffset().x;
//	float maxOffset = maxContainerOffset().x;
//	if (bunceEnale)
//	{
//		minOffset = m_fMinInset.x;
//		maxOffset = m_fMaxInset.x; 
//	}
//	float containerPosX = m_pContainer->getPositionX();
//    //减速的速度在刚按下的时候是0，触摸移动的期间就会更改，改成当前移动的速度
//	if (  containerPosX > minOffset && containerPosX < maxOffset )
//	{
//		CCLog("%s%f%s%f%s%f","minOffset=",minOffset," maxOffset=",maxOffset," containerPosX=",containerPosX);
//		m_pContainer->setPosition(ccpAdd(m_pContainer->getPosition(), m_tScrollDistance));//m_tScrollDistance就是减速的速度
//	}else
//	{
//		this->unschedule(schedule_selector(CCScrollView::deaccelerateScrolling));
//		this->relocateContainer(true);
//		return;
//	}
//
//    //m_pContainer->setPosition(ccpAdd(m_pContainer->getPosition(), m_tScrollDistance));//m_tScrollDistance就是减速的速度
//    
//    if (m_bBounceable)
//    {
//        maxInset = m_fMaxInset; //被初始化为宽度的INSET_RATIO，这个值现在被改为是1.0f
//        minInset = m_fMinInset;	//被初始化为宽度的INSET_RATIO与容器的宽度之和，是负的。
//    }
//    else
//    {
//        maxInset = this->maxContainerOffset();
//        minInset = this->minContainerOffset();
//    }
//    
//    //check to see if offset lies within the inset bounds
//    newX     = MIN(m_pContainer->getPosition().x, maxInset.x);
//    newX     = MAX(newX, minInset.x);
//    newY     = MIN(m_pContainer->getPosition().y, maxInset.y);
//    newY     = MAX(newY, minInset.y);
//    
//    m_tScrollDistance     = ccpSub(m_tScrollDistance, ccp(newX - m_pContainer->getPosition().x, newY - m_pContainer->getPosition().y));
//    m_tScrollDistance     = ccpMult(m_tScrollDistance, SCROLL_DEACCEL_RATE);
//    this->setContentOffset(ccp(newX,newY));
//    
//    if ((fabsf(m_tScrollDistance.x) <= SCROLL_DEACCEL_DIST &&
//         fabsf(m_tScrollDistance.y) <= SCROLL_DEACCEL_DIST) ||
//        newX == maxInset.x || newX == minInset.x ||
//        newY == maxInset.y || newY == minInset.y)
//    {
//        this->unschedule(schedule_selector(CCScrollView::deaccelerateScrolling));
//        this->relocateContainer(true);
//    }
//}
//////
//////回弹，需要两个加速度，即从远处回弹时才开始的时候加速，然后减速回位
//////还需要一个加速减速比，也就是在length长的距离里面，多长用于加速，多长用于减速
//////如果acc_deacc_p为1，那么就是全加速，为0，就是全减速，当然肯定不能为0，不然就是一直往回跑了
////void relocateContainer(float accelerate,float deaccelerate,float acc_deacc_p)
////{
////	
////}
//
//
//void CCScrollView::stoppedAnimatedScroll(CCNode * node)
//{
//    this->unschedule(schedule_selector(CCScrollView::performedAnimatedScroll));
//}
//
//void CCScrollView::performedAnimatedScroll(float dt)
//{
//    if (m_bDragging)
//    {
//        this->unschedule(schedule_selector(CCScrollView::performedAnimatedScroll));
//        return;
//    }
//
//    if (m_pDelegate != NULL)
//    {
//        m_pDelegate->scrollViewDidScroll(this);
//    }
//}
//
//
//CCSize CCScrollView::getContentSize()
//{
//    return CCSizeMake(m_pContainer->getContentSize().width, m_pContainer->getContentSize().height); 
//}
//
//void CCScrollView::setContentSize(CCSize size)
//{
//    this->setViewSize(size);
//}
///**
// * make sure all children go to the container
// */
//void CCScrollView::addChild(CCNode * child, int zOrder, int tag)
//{
//    child->ignoreAnchorPointForPosition(false);
//    child->setAnchorPoint(ccp(0.0f, 0.0f));
//    if (m_pContainer != child) {
//        m_pContainer->addChild(child, zOrder, tag);
//    } else {
//        CCLayer::addChild(child, zOrder, tag);
//    }
//}
//
//void CCScrollView::addChild(CCNode * child, int zOrder)
//{
//    this->addChild(child, zOrder, child->getTag());
//}
//
//void CCScrollView::addChild(CCNode * child)
//{
//    this->addChild(child, child->getZOrder(), child->getTag());
//}
//
///**
// * clip this view so that outside of the visible bounds can be hidden.
// */
//void CCScrollView::beforeDraw()
//{
//    if (m_bClippingToBounds) 
//    {
//		// TODO: This scrollview should respect parents' positions
//		CCPoint screenPos = this->convertToWorldSpace(this->getParent()->getPosition());
//
//        glEnable(GL_SCISSOR_TEST);
//        float s = this->getScale();
//
//        CCDirector *director = CCDirector::sharedDirector();
//        s *= director->getContentScaleFactor();
//
//        glScissor((GLint)screenPos.x, (GLint)screenPos.y, (GLsizei)(m_tViewSize.width*s), (GLsizei)(m_tViewSize.height*s));
//		
//    }
//}
//
///**
// * retract what's done in beforeDraw so that there's no side effect to
// * other nodes.
// */
//void CCScrollView::afterDraw()
//{
//    if (m_bClippingToBounds)
//    {
//        glDisable(GL_SCISSOR_TEST);
//    }
//}
//
//void CCScrollView::visit()
//{
//	// quick return if not visible
//	if (!isVisible())
//    {
//		return;
//    }
//
//	kmGLPushMatrix();
//	
////	glPushMatrix();
//	
//    if (m_pGrid && m_pGrid->isActive())
//    {
//        m_pGrid->beforeDraw();
//        this->transformAncestors();
//    }
//
//	this->transform();
//    this->beforeDraw();
//
//	if(m_pChildren)
//    {
//		ccArray *arrayData = m_pChildren->data;
//		size_t i=0;
//		
//		// draw children zOrder < 0
//		for( ; i < arrayData->num; i++ )
//        {
//			CCNode *child =  (CCNode*)arrayData->arr[i];
//			if ( child->getZOrder() < 0 )
//            {
//				child->visit();
//			}
//            else
//            {
//				break;
//            }
//		}
//		
//		// this draw
//		this->draw();
//		
//		// draw children zOrder >= 0
//		for( ; i < arrayData->num; i++ )
//        {
//			CCNode* child = (CCNode*)arrayData->arr[i];
//			child->visit();
//		}
//        
//	}
//    else
//    {
//		this->draw();
//    }
//
//    this->afterDraw();
//	if ( m_pGrid && m_pGrid->isActive())
//    {
//		m_pGrid->afterDraw(this);
//    }
//
//	kmGLPopMatrix();
//	
////	glPopMatrix();
//}
//
//bool CCScrollView::ccTouchBegan(CCTouch* touch, CCEvent* event)
//{
//    if (!this->isVisible())
//    {
//        return false;
//    }
//    CCRect frame;
//    frame = CCRectMake(this->getPosition().x, this->getPosition().y, m_tViewSize.width, m_tViewSize.height);
//    
//    //dispatcher does not know about clipping. reject touches outside visible bounds.
//    if (m_pTouches->count() > 2 ||
//        m_bTouchMoved          ||
//        !CCRect::CCRectContainsPoint(frame, m_pContainer->convertToWorldSpace(m_pContainer->convertTouchToNodeSpace(touch))))
//    {
//        return false;
//    }
//
//    if (!m_pTouches->containsObject(touch))
//    {
//        m_pTouches->addObject(touch);
//    }
//
//    if (m_pTouches->count() == 1)
//    { // scrolling
//        m_tTouchPoint     = this->convertTouchToNodeSpace(touch);
//        m_bTouchMoved     = false;
//        m_bDragging     = true; //dragging started
//        m_tScrollDistance = ccp(0.0f, 0.0f);
//        m_fTouchLength    = 0.0f;
//    }
//    else if (m_pTouches->count() == 2)
//    {
//        m_tTouchPoint  = ccpMidpoint(this->convertTouchToNodeSpace((CCTouch*)m_pTouches->objectAtIndex(0)),
//                                   this->convertTouchToNodeSpace((CCTouch*)m_pTouches->objectAtIndex(1)));
//        m_fTouchLength = ccpDistance(m_pContainer->convertTouchToNodeSpace((CCTouch*)m_pTouches->objectAtIndex(0)),
//                                   m_pContainer->convertTouchToNodeSpace((CCTouch*)m_pTouches->objectAtIndex(1)));
//        m_bDragging  = false;
//    } 
//    return true;
//}
//
//void CCScrollView::ccTouchMoved(CCTouch* touch, CCEvent* event)
//{
//    if (!this->isVisible())
//    {
//        return;
//    }
//
//    if (m_pTouches->containsObject(touch))
//    {
//        if (m_pTouches->count() == 1 && m_bDragging)
//        { // scrolling
//            CCPoint moveDistance, newPoint, maxInset, minInset;
//            CCRect  frame;
//            CCFloat newX, newY;
//            
//            m_bTouchMoved  = true;
//            frame        = CCRectMake(this->getPosition().x, this->getPosition().y, m_tViewSize.width, m_tViewSize.height);
//            newPoint     = this->convertTouchToNodeSpace((CCTouch*)m_pTouches->objectAtIndex(0));
//            moveDistance = ccpSub(newPoint, m_tTouchPoint);
//            m_tTouchPoint  = newPoint;
//            
//            if (CCRect::CCRectContainsPoint(frame, this->convertToWorldSpace(newPoint)))
//            {
//                switch (m_eDirection)
//                {
//                    case CCScrollViewDirectionVertical:
//                        moveDistance = ccp(0.0f, moveDistance.y);
//                        break;
//                    case CCScrollViewDirectionHorizontal:
//                        moveDistance = ccp(moveDistance.x, 0.0f);
//                        break;
//                    default:
//                        break;
//                }
//
//				if (getContentOffset().x < -(m_pContainer->getContentSize().width - 1024) )
//				{
//					moveDistance.x /= 2.5;
//					//CCLog("CCScorllView cannot move to left");
//				}
//				if (getContentOffset().x > 0)
//				{
//					moveDistance.x /= 2.5;
//					//CCLog("CCScorllView cannot move to right");
//				}
//				
//				
//                m_pContainer->setPosition(ccpAdd(m_pContainer->getPosition(), moveDistance));
//                
//                maxInset = m_fMaxInset;
//                minInset = m_fMinInset;
//                
//                //check to see if offset lies within the inset bounds
//                newX     = MIN(m_pContainer->getPosition().x, maxInset.x);
//                newX     = MAX(newX, minInset.x);
//                newY     = MIN(m_pContainer->getPosition().y, maxInset.y);
//                newY     = MAX(newY, minInset.y);
//                
//                m_tScrollDistance     = ccpSub(moveDistance, ccp(newX - m_pContainer->getPosition().x, newY - m_pContainer->getPosition().y));
//                this->setContentOffset(ccp(newX, newY));
//            }
//        }
//        else if (m_pTouches->count() == 2 && !m_bDragging)
//        {
//            const CCFloat len = ccpDistance(m_pContainer->convertTouchToNodeSpace((CCTouch*)m_pTouches->objectAtIndex(0)),
//                                            m_pContainer->convertTouchToNodeSpace((CCTouch*)m_pTouches->objectAtIndex(1)));
//            this->setZoomScale(this->getZoomScale()*len/m_fTouchLength);
//        }
//    }
//}
//
//void CCScrollView::ccTouchEnded(CCTouch* touch, CCEvent* event)
//{
//    if (!this->isVisible())
//    {
//        return;
//    }
//    if (m_pTouches->containsObject(touch))
//    {
//        if (m_pTouches->count() == 1 && m_bTouchMoved && m_canDeaccelerate)
//        {
//            this->schedule(schedule_selector(CCScrollView::deaccelerateScrolling));
//        }
//        m_pTouches->removeObject(touch);
//    } 
//
//    if (m_pTouches->count() == 0)
//    {
//        m_bDragging = false;    
//        m_bTouchMoved = false;
//    }
//}
//
//void CCScrollView::ccTouchCancelled(CCTouch* touch, CCEvent* event)
//{
//    if (!this->isVisible())
//    {
//        return;
//    }
//    m_pTouches->removeObject(touch); 
//    if (m_pTouches->count() == 0)
//    {
//        m_bDragging = false;    
//        m_bTouchMoved = false;
//    }
//}
//
//NS_CC_EXT_END
