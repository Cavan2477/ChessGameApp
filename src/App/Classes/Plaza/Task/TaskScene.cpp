//
//  Task.cpp
//  GameProject
//
//  Created by Tang Miao on 2/1/16.
//
//

#include "TaskScene.h"

#include "cocostudio/CocoStudio.h"

#include "../../DataMgr/HallDataMgr.h"
#include "../../DataMgr/NetworkMgr.h"
#include "../../DataMgr/GameConfigMgr.h"

using namespace cocos2d::ui;
TaskScene::TaskScene():
_userBean(nullptr),
_userIngot(nullptr),
_userScore(nullptr),
m_scrollView(nullptr)
{
}

TaskScene::~TaskScene()
{
    CC_SAFE_RELEASE_NULL(_userScore);
    CC_SAFE_RELEASE_NULL(_userBean);
    CC_SAFE_RELEASE_NULL(_userIngot);
}

bool TaskScene::init()
{
    if (!Layer::init())
        return false;
    
    Layout *_layout = Layout::create();
    _layout->setContentSize(Size(1136, 640));
    
    auto rootNode = CSLoader::createNode("task_res/Task.csb");
    _layout->addChild(rootNode);

    for ( auto node :rootNode->getChildren() )
    {
        log("%s",node->getName().c_str());
    }
    
    //用户金币
    Label *coin = Label::createWithSystemFont(getScorewithComma(HallDataMgr::getInstance()->m_UserScore, ","), FONT_TREBUCHET_MS_BOLD, 24);
    setUserScore(coin);
    _userScore->setTextColor(cocos2d::Color4B::YELLOW);
    Labellengthdeal(_userScore, 145);
    _userScore->setAnchorPoint(Vec2(.0, .5));
    _userScore->setPosition(Vec2(170, 595));
    _layout->addChild(_userScore);
    
    //用户元宝
    Label *ingot = Label::createWithSystemFont(getScorewithComma(HallDataMgr::getInstance()->m_Ingot, ","), FONT_TREBUCHET_MS_BOLD, 24);
    setUserIngot(ingot);
    _userIngot->setTextColor(cocos2d::Color4B::YELLOW);
    Labellengthdeal(_userIngot, 145);
    _userIngot->setAnchorPoint(Vec2(.0, .5));
    _userIngot->setPosition(Vec2(730, 595));
    _layout->addChild(_userIngot);
    
    //用户游戏豆
    Label *bean = Label::createWithSystemFont(__String::createWithFormat("%0.2f",HallDataMgr::getInstance()->m_Bean)->getCString(), FONT_TREBUCHET_MS_BOLD, 24);
    setUserBean(bean);
    _userBean->setTextColor(cocos2d::Color4B::YELLOW);
    Labellengthdeal(_userBean, 145);
    _userBean->setAnchorPoint(Vec2(.0, .5));
    _userBean->setPosition(Vec2(445, 595));
    _layout->addChild(_userBean);
    
    //关闭按钮
    Button *closeBtn = static_cast<Button *>(rootNode->getChildByName("button_return"));
    if (closeBtn != nullptr)
    {
        closeBtn->addTouchEventListener(CC_CALLBACK_2(TaskScene::buttonEventWithClose, this));
    }
    
    
    //滑动
    m_scrollView =  ListView::create();
    m_scrollView->setBounceEnabled(true);
    m_scrollView->setDirection(ui::ScrollView::Direction::VERTICAL);
    m_scrollView->setContentSize(Size(1048, 480));
    m_scrollView->setAnchorPoint(Vec2(.5, 0.0));
    m_scrollView->setPosition(Vec2(568, 15));
    m_scrollView->setItemsMargin(30.0f);
    _layout->addChild(m_scrollView);
    
    _layout->setScaleX(JudgeScale);
    this->addChild(_layout);
    
    return true;
}

void TaskScene::onEnterTransitionDidFinish()
{
    
    Layer::onEnterTransitionDidFinish();
    
    NetworkMgr::getInstance()->registeruserfunction(SUB_GP_TASK_LIST, CC_CALLBACK_2(TaskScene::TaskListResult, this));
    NetworkMgr::getInstance()->registeruserfunction(SUB_GP_TASK_INFO, CC_CALLBACK_2(TaskScene::TaskInfoResult, this));
    NetworkMgr::getInstance()->registeruserfunction(SUB_GP_TASK_RESULT, CC_CALLBACK_2(TaskScene::TaskResult, this));

}

void TaskScene::onExit()
{
    NetworkMgr::getInstance()->unregisterloadfunction(SUB_GP_TASK_LIST);
    NetworkMgr::getInstance()->unregisterloadfunction(SUB_GP_TASK_INFO);
    NetworkMgr::getInstance()->unregisterloadfunction(SUB_GP_TASK_RESULT);
    
    clearTaskList();
    clearTaskStatus();
    
    Layer::onExit();
}

//清除任务数据
void TaskScene::clearTaskList()
{
    for (auto iter : m_TaskList)
    {
        if (iter.second != nullptr)
        {
            
            delete iter.second;
            iter.second = nullptr;
        }
    }
    
    m_TaskList.clear();

}
void TaskScene::clearTaskStatus()
{
    for (auto iter : m_TaskStatus)
    {
        if (iter != nullptr)
        {
            delete iter;
            iter = nullptr;
        }
        
    }
    
    m_TaskStatus.clear();
    m_TaskStatus.shrink_to_fit();
}

void TaskScene::initTasklist()
{
    
    m_scrollView->removeAllChildren();
    
    
    for (auto iter : m_TaskList)
    {
    
        Layout *listLayout = Layout::create();
        listLayout->setTag(iter.second->wTaskID + 1000);
        listLayout->setContentSize(Size(994, 166));
        if (listLayout->getContentSize().height > m_scrollView->getContentSize().height)
        {
            m_scrollView->setInnerContainerSize(cocos2d::Size(m_scrollView->getContentSize().width, listLayout->getContentSize().height));
        }
        else
        {
            m_scrollView->setInnerContainerSize(cocos2d::Size(m_scrollView->getContentSize().width, m_scrollView->getContentSize().height));
            
        }
        
        auto oneList = CSLoader::createNode("task_res/oneTask.csb");
        oneList->setContentSize(Size(994, 166));
        oneList->setTag(1000);
        oneList->setAnchorPoint(Vec2(.5, .5));
        oneList->setPosition(450,listLayout->getContentSize().height/2);
        listLayout->addChild(oneList);
        
        std::string des =  WHConverUnicodeToUtf8WithArray(iter.second->szTaskDesc);
        log("%s\n",des.c_str());
        
        std::string taskName = WHConverUnicodeToUtf8WithArray(iter.second->szTaskName);
        log("任务名称 %s \n",taskName.c_str());
        
        //按钮
        auto takeTask = static_cast<Button *>(oneList->getChildByName("task_take"));
        takeTask->setTag(iter.second->wTaskID);
        takeTask->addTouchEventListener([=](Ref *pSender, Widget::TouchEventType type){
           
            if (type == Widget::TouchEventType::ENDED)
            {
                auto take = static_cast<Button *>(pSender);
                this->sendTakeTask(take->getTag());
                take->removeFromParent();
            }
        });
        
        //任务类型
        int taskType = iter.second->wTaskType;
        std::string taskImage = "";
        switch (taskType)
        {
            case TASK_TYPE_FIRST_WIN:
                taskImage = "task_res/task_type_0.png";
                break;
            case TASK_TYPE_SUM_INNINGS:
                taskImage = "task_res/task_type_1.png";
                break;
            case TASK_TYPE_WIN_INNINGS:
                taskImage = "task_res/task_type_2.png";
                break;
            default:
                taskImage = "task_res/task_type_0.png";
                break;;
        }
        
        ImageView *taskIcon = static_cast<ImageView *>(oneList->getChildByName("task_icon"));
        taskIcon->loadTexture(taskImage);
        
        //任务描述
        auto describe = static_cast<Text *>(oneList->getChildByName("Text_taskDes"));
        describe->setString(des);
        m_scrollView->pushBackCustomItem(listLayout);
        
        //时间限制
        int hour = iter.second->dwTimeLimit/3600;
        int min = iter.second->dwTimeLimit%3600/60;
        int second = iter.second->dwTimeLimit%3600%60;
        
        auto hourstr = (hour<10)?__String::createWithFormat("0%d时",hour):__String::createWithFormat("%d时",hour);
        auto minstr = (min<10)?__String::createWithFormat(": 0%d分",min):__String::createWithFormat(": %d分",min);
        auto secstr = (second<10)?__String::createWithFormat(": 0%d秒",second):__String::createWithFormat(": %d秒",second);
        
        //时
        auto textHour = static_cast<Text *>(oneList->getChildByName("Text_hour"));
        textHour->setString(hourstr->getCString());
        //分
        auto textMin = static_cast<Text *>(oneList->getChildByName("Text_min"));
        textMin->setString(minstr->getCString());
        //秒
        auto textSec = static_cast<Text *>(oneList->getChildByName("Text_second"));
        textSec->setString(secstr->getCString());
        
        //奖励  会员用户
        auto vipCoin = static_cast<Text *>(oneList->getChildByName("Text_hy_coin"));
        vipCoin->setString(__String::createWithFormat("%lld",iter.second->lMemberAwardGold)->getCString());
        
        auto vipIngot = static_cast<Text *>(oneList->getChildByName("Text_hy_ingot"));
        vipIngot->setString(__String::createWithFormat("%lld",iter.second->lMemberAwardMedal)->getCString());
        
        //普通用户
        auto generCoin = static_cast<Text *>(oneList->getChildByName("Text_gener_coin"));
        generCoin->setString(__String::createWithFormat("%lld",iter.second->lStandardAwardGold)->getCString());
        
        auto generIngot = static_cast<Text *>(oneList->getChildByName("Text_gener_ingot"));
        generIngot->setString(__String::createWithFormat("%lld",iter.second->lStandardAwardMedal)->getCString());
        
        
        //进度条
        auto progressTip = static_cast<Sprite *>(oneList->getChildByName("task_progressTip_6"));
        progressTip->setVisible(false);
        
        auto empty = static_cast<ImageView *>(oneList->getChildByName("Image_progress_empty"));
        empty->setVisible(false);
        
        auto full = static_cast<ImageView *>(oneList->getChildByName("Image_progress_full"));
        full->setVisible(false);
        
        auto per = static_cast<Text *>(oneList->getChildByName("task_per"));
        per->setVisible(false);
        
    }
    
    
}
void TaskScene::updateTask(ST_TASK_STATUS *pstatus, ST_TASK_PARAM *pinfo)
{
    
    //目标任务
    auto _layout = static_cast<Layout *>(m_scrollView->getChildByTag(1000 + pinfo->wTaskID));
    auto oneList = static_cast<Node *>(_layout->getChildByTag(1000));
    
    //按钮
    auto takeTask = static_cast<Button *>(oneList->getChildByName("task_take"));
    takeTask->setVisible(false);
    
    switch (pstatus->cbTaskStatus)
    {
        case TASK_STATUS_UNFINISH:
        {
            
            ImageView *status = ImageView::create();
            status->loadTexture("task_res/task_playing.png");
            status->setAnchorPoint(Vec2(1.0, 0.5));
            status->setPosition(Vec2(oneList->getContentSize().width + 20 , oneList->getContentSize().height/2));
            oneList->addChild(status);
     
        }
            break;
        case TASK_STATUS_FAILED:
        {
            
            ImageView *status = ImageView::create();
            status->loadTexture("task_res/task_lose.png");
            status->setAnchorPoint(Vec2(1.0, 0.5));
            status->setPosition(Vec2(oneList->getContentSize().width + 20, oneList->getContentSize().height/2));
            oneList->addChild(status);
   
        }
            break;
        case TASK_STATUS_REWARD:
        {
            
            ImageView *status = ImageView::create();
            status->loadTexture("task_res/task_flish.png");
            status->setAnchorPoint(Vec2(1.0, 0.5));
            status->setPosition(Vec2(oneList->getContentSize().width + 20 , oneList->getContentSize().height/2));
            oneList->addChild(status);
        }
            break;
        case TASK_STATUS_SUCCESS:
        {
            auto pbutton = Button::create("task_res/task_waiting_take0.png", "task_res/task_waiting_take1.png");
            pbutton->setAnchorPoint(Vec2(1.0, 0.5));
            pbutton->setPosition(Vec2(oneList->getContentSize().width + 70 , oneList->getContentSize().height/2 + 30));
            pbutton->setTag(pinfo->wTaskID);
            pbutton->addTouchEventListener([=](Ref *pSender, Widget::TouchEventType type){
                if (type == Widget::TouchEventType::ENDED)
                {
                    auto paward = static_cast<Button *>(pSender);
                    this->sendTaskReward(paward->getTag());
                    paward->removeFromParent();
                }
            });
            
            oneList->addChild(pbutton);
        }
            break;
            
        default:
            break;
    }
    
    //进度条
    auto progressTip = static_cast<Sprite *>(oneList->getChildByName("task_progressTip_6"));
    progressTip->setVisible(true);
    
    auto empty = static_cast<ImageView *>(oneList->getChildByName("Image_progress_empty"));
    empty->setVisible(true);
    
    auto full = static_cast<ImageView *>(oneList->getChildByName("Image_progress_full"));
    full->setVisible(true);
    
    auto per = static_cast<Text *>(oneList->getChildByName("task_per"));
    per->setString(__String::createWithFormat("%d/%d",pstatus->wTaskProgress,pinfo->wTaskObject)->getCString());
    per->setVisible(true);
    
    float percent =  (pinfo->wTaskObject > 0) ? pstatus->wTaskProgress/(float)pinfo->wTaskObject : 0;
    percent = MIN(percent, 1.0);
    
   full->setScaleX(percent);
    
    

}

//MARK::SEND DATA
//请求加载任务列表
void TaskScene::sendLoadTask()
{
    NetworkMgr::getInstance()->doConnect(LOGON_ADDRESS_YM, LOGON_PORT, EM_DATA_TYPE_LOAD);
    CMD_GP_TaskLoadInfo taskloadinfo;
    memset(&taskloadinfo, 0, sizeof(taskloadinfo));
    taskloadinfo.dwUserID = HallDataMgr::getInstance()->m_dwUserID;
    UTF8Str_To_UTF16Str(HallDataMgr::getInstance()->m_pPassword.c_str(), taskloadinfo.szPassword);
    NetworkMgr::getInstance()->sendData(MDM_GP_USER_SERVICE, SUB_GP_TASK_LOAD, &taskloadinfo, sizeof(taskloadinfo),NetworkMgr::getInstance()->getSocketOnce());
    
}
//请求领取任务
void TaskScene::sendTakeTask(int taskID)
{
    
    CMD_GP_TaskTake TaskTake ;
    memset(&TaskTake, 0, sizeof(CMD_GP_TaskTake));
    TaskTake.wTaskID = taskID;
    TaskTake.dwUserID = HallDataMgr::getInstance()->m_dwUserID;
    UTF8Str_To_UTF16Str(HallDataMgr::getInstance()->m_pPassword.c_str(), TaskTake.szPassword);
    UTF8Str_To_UTF16Str(HallDataMgr::getInstance()->m_Machine.c_str(), TaskTake.szMachineID);
    
    NetworkMgr::getInstance()->doConnect(LOGON_ADDRESS_YM, LOGON_PORT, EM_DATA_TYPE_LOAD);
    NetworkMgr::getInstance()->sendData(MDM_GP_USER_SERVICE, SUB_GP_TASK_TAKE, &TaskTake, sizeof(TaskTake), NetworkMgr::getInstance()->getSocketOnce());
    
}
//请求任务奖励
void TaskScene::sendTaskReward(int taskID)
{
    //领取奖励
    CMD_GR_C_TakeReward TaskReward;
    memset(&TaskReward, 0, sizeof(CMD_GR_C_TakeReward));
    
    TaskReward.wTaskID = taskID;
    TaskReward.dwUserID = HallDataMgr::getInstance()->m_dwUserID;
    UTF8Str_To_UTF16Str(HallDataMgr::getInstance()->m_pPassword.c_str(), TaskReward.szPassword);
    UTF8Str_To_UTF16Str(HallDataMgr::getInstance()->m_Machine.c_str(), TaskReward.szMachineID);
    
    NetworkMgr::getInstance()->doConnect(LOGON_ADDRESS_YM, LOGON_PORT, EM_DATA_TYPE_LOAD);
    NetworkMgr::getInstance()->sendData(MDM_GP_USER_SERVICE, SUB_GP_TASK_REWARD, &TaskReward, sizeof(TaskReward), NetworkMgr::getInstance()->getSocketOnce());
}

//MARK::NOTIFY
//任务列表消息
void TaskScene::TaskListResult(void* pData, WORD wSize)
{
    
    this->clearTaskList();
    
    HallDataMgr::getInstance()->AddpopLayer("", "", Type_Delete);
    
    WORD wParemeterSize=0;
    BYTE *pDatabuffer=(BYTE*)pData;
    WORD wTaskCount=*pDatabuffer;
    pDatabuffer+=sizeof(wTaskCount);
    
    for (int i=0;i<wTaskCount;i++)
    {
        wParemeterSize=*(WORD*)pDatabuffer;
        pDatabuffer+=sizeof(wParemeterSize);
        
        auto Parameter = new ST_TASK_PARAM();
        memset(Parameter, 0, sizeof(ST_TASK_PARAM));
        if(pDatabuffer==NULL||wParemeterSize==0)
        {
            delete Parameter;
            Parameter = nullptr;
            continue;
        }
        
        memcpy(Parameter, pDatabuffer, wParemeterSize);
        //指针叠加
        pDatabuffer+=wParemeterSize;
        if ((DWORD)EM_GAME_DEFALUT != HallDataMgr::getInstance()->m_dwKindID)
        {
            //任务过滤
            if (Parameter->wKindID != HallDataMgr::getInstance()->m_dwKindID)
            {
                delete Parameter;
                Parameter = nullptr;
                continue;
            }
        }

        if (GameConfigMgr::getInstance()->isGameActive(Parameter->wKindID)) {
            m_TaskList.insert(std::make_pair(Parameter->wTaskID, Parameter));
        }
    }
    
    this->initTasklist();
    
    
}

//任务状态消息
void TaskScene::TaskInfoResult(void* pData, WORD wSize)
{
    
    auto ptaskinfo = static_cast<CMD_GP_TaskInfo *>(pData);
    for (int index=0; index<ptaskinfo->wTaskCount; ++index)
    {
        auto pinfo = new ST_TASK_STATUS();
        memset(pinfo, 0, sizeof(ST_TASK_STATUS));
        memcpy(pinfo, &ptaskinfo->TaskStatus[index], sizeof(ST_TASK_STATUS));
        auto ptask = m_TaskList.at(pinfo->wTaskID);
        if (ptask)
        {
            this->updateTask(pinfo, ptask);
        }
        CC_SAFE_DELETE(pinfo);
    }
    
     NetworkMgr::getInstance()->Disconnect(EM_DATA_TYPE_LOAD);
}

//任务结果消息
void TaskScene::TaskResult(void* pData, WORD wSize)
{

    NetworkMgr::getInstance()->Disconnect(EM_DATA_TYPE_LOAD);
    
    auto TaskResult = static_cast<CMD_GP_TaskResult *>(pData);
    std::string tipstr = WHConverUnicodeToUtf8WithArray(TaskResult->szNotifyContent);
    HallDataMgr::getInstance()->AddpopLayer("系统提示", tipstr, Type_Ensure);
    if (TaskResult->bSuccessed)
    {
        this->sendLoadTask();
        
        if (TaskResult->lCurrScore > 0)
        {
            HallDataMgr::getInstance()->m_UserScore = TaskResult->lCurrScore;
            HallDataMgr::getInstance()->m_Ingot = TaskResult->lCurrIngot;
            
            auto puser = HallDataMgr::getInstance()->m_UserList.at(HallDataMgr::getInstance()->m_dwUserID);
            if (puser)
            {
                puser->m_date.lScore = TaskResult->lCurrScore;
                puser->m_date.lIngot = TaskResult->lCurrIngot;
            }
            
            //金币更新
            EventCustom event(whEvent_User_Data_Change);
            auto value = __Integer::create(User_Change_Score);
            event.setUserData(value);
            Director::getInstance()->getEventDispatcher()->dispatchEvent(&event);
        }
    }
}
//MARK::BUTTON EVENT

void TaskScene::buttonEventWithClose(cocos2d::Ref *target, cocos2d::ui::Widget::TouchEventType type)
{
    
    if (type == cocos2d::ui::Widget::TouchEventType::ENDED)
    {
        CallFunc *func = CallFunc::create([this]{this->removeFromParent();});
        Sequence *quence = Sequence::createWithTwoActions(MoveTo::create(0.4f, Vec2(1136, 0)), func);
        
        this->runAction(quence);
    }
}

//MARK::ANIMATE
void TaskScene::popTask()
{
    
    CallFunc *func = CallFunc::create([this]{
        HallDataMgr::getInstance()->AddpopLayer("", "正在加载任务,请稍后...", Type_Wait_Text);
        this->sendLoadTask();
        
    });
    
    auto run = MoveTo::create(0.2f, Vec2(.0,.0));
    
    this->runAction(Sequence::createWithTwoActions(run, func));
}

void TaskScene::SignSuccessAnmition()
{
    
    
    
    
    
}
