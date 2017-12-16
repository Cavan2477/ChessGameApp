//
//  Task.hpp
//  GameProject
//
//  Created by Tang Miao on 2/1/16.
//
//

#ifndef __TaskScene_h__
#define __TaskScene_h__

#include "cocos2d.h"
#include "ui/CocosGUI.h"

#include "../../Common/MacroDefine.h"
#include "../../Common/CMD_Stdafx.h"

USING_NS_CC;

using namespace std;

class TaskScene : public Layer
{
public:
    TaskScene();
	virtual ~TaskScene();
    
	virtual bool init();

    CREATE_FUNC(TaskScene);
public:
    void popTask();
    
    virtual void onEnterTransitionDidFinish();
    
    virtual void onExit();
    
    //清除任务数据
    void clearTaskList();
    void clearTaskStatus();
    
    void initTasklist();
    void updateTask(ST_TASK_STATUS *pstatus, ST_TASK_PARAM *pinfo);
    
    //请求加载任务列表
    void sendLoadTask();
    //请求领取任务
    void sendTakeTask(int taskID);
    //请求任务奖励
    void sendTaskReward(int taskID);
    
    //任务列表消息
    void TaskListResult(void* pData, WORD wSize);
    
    //任务状态消息
    void TaskInfoResult(void* pData, WORD wSize);
    
    //任务结果消息
    void TaskResult(void* pData, WORD wSize);
    
    void SignSuccessAnmition();
    
    void buttonEventWithClose(Ref* target,cocos2d::ui::Widget::TouchEventType type);
    
private:
    CC_SYNTHESIZE_RETAIN(Label *, _userScore, UserScore);
    CC_SYNTHESIZE_RETAIN(Label *, _userIngot, UserIngot);
    CC_SYNTHESIZE_RETAIN(Label *, _userBean, UserBean);
    
    //任务状态
    std::vector<ST_TASK_STATUS *>   m_TaskStatus;
    
    //任务列表
    std::map<int, ST_TASK_PARAM *> m_TaskList;
    
    cocos2d::ui::ListView  *m_scrollView;
};


#endif
