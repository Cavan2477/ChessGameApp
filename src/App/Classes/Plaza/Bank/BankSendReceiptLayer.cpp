/************************************************************************************
 * file: 		BankSendReceiptLayer.cpp
 * copyright:	Cavan.Liu 2017
 * Author: 		Cavan.Liu
 * Create: 		2017/12/12 23:18:31
 * Description: 
 * Version	Author		Time			Description
 * V1.0    	Cavan.Liu	2017/12/12			
 *
 ************************************************************************************/

#include "BankSendReceiptLayer.h"

USING_NS_CC;

using namespace ui;

BankSendReceiptLayer::BankSendReceiptLayer()
{
    
}

BankSendReceiptLayer::~BankSendReceiptLayer()
{
    
}

bool BankSendReceiptLayer::init()
{
    bool bRes = false;

    do
    {
        CC_BREAK_IF(!Layer::init());
        
        m_root = CSLoader::createNode("bank_res/SendReceiptLayer.csb");
        CC_ASSERT(nullptr != m_root);
        
        auto ll = Layout::create();
        ll->setContentSize(Size(1136, 640));
        ll->addChild(m_root);
        ll->setScaleX(JUDGE_SCALE);
        this->addChild(ll);
        
        //确定按钮
        auto btn = static_cast<Button*>(m_root->getChildByName("sure_btn"));
        CC_ASSERT(btn != nullptr);
        btn->addTouchEventListener([=](cocos2d::Ref *pSender, cocos2d::ui::Widget::TouchEventType type)
                                   {
                                       if (Widget::TouchEventType::ENDED == type)
                                       {
                                           this->removeFromParentAndCleanup(true);
                                       }
                                   });
        //编号
        m_textCode = static_cast<Text*>(m_root->getChildByName("receipt_code"));
        CC_ASSERT(m_textCode != nullptr);
        
        //赠送人昵称
        m_textSendUserName = static_cast<Text*>(m_root->getChildByName("send_username"));
        CC_ASSERT(m_textSendUserName != nullptr);
        
        //赠送人id
        m_textSendUserId = static_cast<Text*>(m_root->getChildByName("send_userid"));
        CC_ASSERT(m_textSendUserId != nullptr);
        
        //接收人昵称
        m_textRecUserName = static_cast<Text*>(m_root->getChildByName("rec_username"));
        CC_ASSERT(m_textRecUserName != nullptr);
        
        //接收人id
        m_textRecUserId = static_cast<Text*>(m_root->getChildByName("rec_userid"));
        CC_ASSERT(m_textRecUserId != nullptr);
        
        //赠送游戏币
        m_textSendCount = static_cast<Text*>(m_root->getChildByName("send_count"));
        CC_ASSERT(m_textSendCount != nullptr);
        
        //数额大写
        m_textDaXie = static_cast<Text*>(m_root->getChildByName("count_daxie"));
        CC_ASSERT(m_textDaXie != nullptr);
        
        //接收日期
        m_textSendDate = static_cast<Text*>(m_root->getChildByName("rec_date"));
        CC_ASSERT(m_textSendDate != nullptr);
        
        //赠送结果
        m_spSendRes = static_cast<Sprite*>(m_root->getChildByName("sp_sendres"));
        CC_ASSERT(m_spSendRes != nullptr);
        
        bRes = true;
    } while (false);
    
    return bRes;
}

void BankSendReceiptLayer::refreshReceipt(const _stReceipt &stReceipt)
{
    char buf[128] = "";
    
    //编号
    sprintf(buf, "编号:%d", stReceipt.dwRecordIndex);
    m_textCode->setString(buf);
    
    //昵称
    m_textSendUserName->setString(stReceipt.strSendUserName);
    
    //id
    sprintf(buf, "%d", stReceipt.dwSendUserId);
    m_textSendUserId->setString(buf);
    
    //昵称
    m_textRecUserName->setString(stReceipt.strRecUserName);
    
    //id
    sprintf(buf, "%d",stReceipt.dwRecUserId);
    m_textRecUserId->setString(buf);
    
    //赠送数额
    sprintf(buf, "%lld", stReceipt.llSendCount);
    m_textSendCount->setString(buf);
    
    //大写
    std::string numStr = numberTransiform(buf);
    m_textDaXie->setString(numStr);
    
    //日期
    time_t t = time(nullptr);
    tm *tm = localtime(&t);
    sprintf(buf, "%02d-%02d-%02d %02d:%02d", ((int) tm->tm_year + 1900),
            (int) tm->tm_mon + 1, (int) tm->tm_mday, (int) tm->tm_hour,
            (int) tm->tm_min);
    m_textSendDate->setString(buf);
}

std::string BankSendReceiptLayer::numberTransiform(const std::string &strCount)
{
    std::string str = "";
    
    std::string big_num[] = {"零","壹","贰","叄","肆","伍","陆","柒","捌","玖"};
    std::string unit[] = {"元","拾","佰","仟","万",
                          //拾万位到千万位
                          "拾","佰","仟",
                          //亿万位到万亿位
                          "亿","拾","佰","仟","万"};
    
    auto len = strCount.length();

    for (int i=0; i<len; ++i)
    {
        str = str + big_num[strCount[i] - 48] + unit[len - i - 1];
    }

    return cleanZero(str);
}

std::string BankSendReceiptLayer::replaceAll(std::string &src, const std::string &regex, const std::string &replacement)
{
    size_t len = regex.length();
    while (src.find(regex) < src.length())
    {
        src.replace(src.find(regex), len, replacement);
    }
    return src;
}

std::string BankSendReceiptLayer::cleanZero(std::string &str)
{
    // 如果传入的是空串则继续返回空串
    if ("" == str)
        return "";

    // 字符串中存在多个'零'在一起的时候只读出一个'零'，并省略多余的单位
    
    std::string regex1[] = {"零仟", "零佰", "零拾"};
    std::string regex2[] = {"零亿", "零万", "零元"};
    std::string regex3[] = {"亿", "万", "元"};
    std::string regex4[] = {"零角", "零分"};
    
    // 第一轮转换把 "零仟", 零佰","零拾"等字符串替换成一个"零"
    for(int i = 0; i < 3; i ++)
        str = replaceAll(str, regex1[i], "零");

    // 第二轮转换考虑 "零亿","零万","零元"等情况
    // "亿","万","元"这些单位有些情况是不能省的，需要保留下来
    for(int i = 0; i < 3; i ++)
    {
        // 当第一轮转换过后有可能有很多个零叠在一起
        // 要把很多个重复的零变成一个零
        str = replaceAll(str, "零零零", "零");
        str = replaceAll(str, "零零", "零");
        str = replaceAll(str, regex2[i], regex3[i]);
    }

    // 第三轮转换把"零角","零分"字符串省略
    for(int i = 0; i < 2; i ++)
    {
        str = replaceAll(str, regex4[i], "");
    }

    // 当"万"到"亿"之间全部是"零"的时候，忽略"亿万"单位，只保留一个"亿"
    str = replaceAll(str, "亿万", "亿");
    
    //去掉单位
    str = replaceAll(str, "元", "");

    return str;
}