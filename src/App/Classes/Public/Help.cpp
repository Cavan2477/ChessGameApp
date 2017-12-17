//
//  Help.cpp
//  MyGame
//
//  Created by wh on 15/4/27.
//
//

#include "Help.h"
#include "../Utils/MD5/md5.h"
#include "../Utils/Gif/gif_lib.h"
#include "../DataMgr/HallDataMgr.h"
#include "cocostudio/DictionaryHelper.h"

#if (CC_TARGET_PLATFORM == CC_PLATFORM_IOS)
	#include <CommonCrypto/CommonDigest.h>
	#include <sys/types.h>
	#include <sys/socket.h> // Per msqr
	#include <sys/sysctl.h>
	#include <net/if.h>
	#include <net/if_dl.h>
	#include "iconv.h"
#elif (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID)//预编译 判断是否是android平台
	#include <jni.h>
	#include "platform/android/jni/JniHelper.h"
	#include <android/log.h>
#endif

USING_NS_CC;

using namespace cocostudio;

static    pthread_t               m_sThread;                  //线程句柄

int UTF8_To_UCS4( const BYTE* pbUTF8, DWORD& dwUCS4 )
{
    INT i, iLen;
    BYTE b;
    if( pbUTF8 == NULL )
    { // 参数错误
        return 0;
    }
    b = *pbUTF8++;
    if( b < 0x80 )
    {
        dwUCS4 = b;
        return 1;
    }
    if( b < 0xC0 || b > 0xFD )
    { // 非法UTF8
        return 0;
    }
    if( b < 0xE0 )
    {
        dwUCS4 = b & 0x1F;
        iLen = 2;
    }
    else if( b < 0xF0 )
    {
        dwUCS4 = b & 0x0F;
        iLen = 3;
    }
    else if( b < 0xF8 )
    {
        dwUCS4 = b & 7;
        iLen = 4;
    }
    else if( b < 0xFC )
    {
        dwUCS4 = b & 3;
        iLen = 5;
    }
    else
    {
        dwUCS4 = b & 1;
        iLen = 6;
    }
    for( i = 1; i < iLen; i++ )
    {
        b = *pbUTF8++;
        if( b < 0x80 || b > 0xBF )
        { // 非法UTF8
            break;
        }
        dwUCS4 = (dwUCS4 << 6) + (b & 0x3F);
    }
    if( i < iLen )
    { // 非法UTF8
        return 0;
    }
    else
    {
        return iLen;
    }
}

// 转换UCS4编码到UCS2编码
INT UCS4_To_UTF16( DWORD dwUCS4, WORD* pwUTF16 )
{
    if( dwUCS4 <= 0xFFFF )
    {
        if( pwUTF16 != NULL )
        {
            *pwUTF16 = static_cast<WORD>(dwUCS4);
        }
        return 1;
    }
    else if( dwUCS4 <= 0xEFFFF )
    {
        if( pwUTF16 != NULL )
        {
            pwUTF16[0] = static_cast<WORD>( 0xD800 + (dwUCS4 >> 10) - 0x40 ); // 高10位
            pwUTF16[1] = static_cast<WORD>( 0xDC00 + (dwUCS4 & 0x03FF) ); // 低10位
        }
        return 2;
    }
    else
    {
        return 0;
    }
}

// 转换UTF8字符串到UTF16字符串
void UTF8Str_To_UTF16Str( const std::string &str, WORD* pwszUTF16Str )
{
    std::u16string ptest;
    StringUtils::UTF8ToUTF16(str, ptest);
    memcpy(pwszUTF16Str, ptest.c_str(), ptest.length()*2);
}

int UTF8Str_To_UTF16Str_BYTE( const BYTE* pbszUTF8Str, WORD* pwszUTF16Str )
{
    INT iNum, iLen;
    DWORD dwUCS4;
    if( pbszUTF8Str == NULL )
    { // 参数错误
        return 0;
    }
    iNum = 0; // 统计有效字符个数
    while( *pbszUTF8Str )
    { // UTF8编码转换为UCS4编码
        iLen = UTF8_To_UCS4( pbszUTF8Str, dwUCS4 );
        if( iLen == 0 )
        { // 非法的UTF8编码
            return 0;
        }
        pbszUTF8Str += iLen;
        // UCS4编码转换为UTF16编码
        iLen = UCS4_To_UTF16( dwUCS4, pwszUTF16Str );
        if( iLen == 0 )
        {
            return 0;
        }
        if( pwszUTF16Str != NULL )
        {
            pwszUTF16Str += iLen;
        }
        iNum += iLen;
    }
    if( pwszUTF16Str != NULL )
    {
        *pwszUTF16Str = 0; // 写入字符串结束标记
    }
    
    return iNum;
}

//单字符:unicode -> utf8 返回转换长度
int WHConverUnicodeToUtf8(char* destUtf8, WORD srcUnicode)
{
    int i=0;
    if( 0x80 > srcUnicode )
    {
        destUtf8[i++]=(char)srcUnicode;
    }
    else if(0x80 <= srcUnicode && srcUnicode <= 0x07FF)
    {
        destUtf8[i++] = (char)((srcUnicode>>6)|0xC0);
        destUtf8[i++] = (char)((srcUnicode&0x3F)|0x80);
    }
    else
    {
        destUtf8[i++] = (char)(0xE0|(srcUnicode>>12));
        destUtf8[i++] = (char)(0x80|((srcUnicode>>6)&0x3F));
        destUtf8[i++] = (char)(0x80|(srcUnicode&0x3F));
    }
    return i;
    
}

//数组:unicode -> utf8
const std::string WHConverUnicodeToUtf8WithArray(WORD srcUnicode[])
{
    WORD wIndex=0;

    for (int i=0; ; i++)
	{
        if(srcUnicode[i])
            wIndex += 1;
        else
            break;
    }

    std::u16string ptest((const char16_t *)srcUnicode, wIndex);
    std::string str;

    StringUtils::UTF16ToUTF8(ptest, str);
    
    return str;
}

LONGLONG getsystemtomillisecond()
{
    struct timeval tv;
    memset(&tv, 0, sizeof(tv));
    gettimeofday(&tv, NULL);
    
    LONGLONG time = tv.tv_sec*1000 + tv.tv_usec/1000;
    return time;
}

const std::string getCurrentTime()
{
    struct timeval timeval;
    memset(&timeval, 0, sizeof(timeval));
    gettimeofday(&timeval, NULL);
    
    struct tm* ptime;
    ptime = localtime(&timeval.tv_sec);
    std::string timestr = cocos2d::__String::createWithFormat("%02d/%02d", ptime->tm_hour, ptime->tm_min)->getCString();
    return timestr;
}

//获取时间
const std::string getTimeStr(SCORE timesec)
{
    time_t _time = timesec;
    struct tm* ptime;
    ptime = localtime(&_time);
    std::string timestr =cocos2d::__String::createWithFormat("%d/%02d/%02d", ptime->tm_year+1900, ptime->tm_mon+1, ptime->tm_mday)->getCString();
    return timestr;
}

void getCurrentVersion(WORD kindID)
{
    
    struct timeval tv;
    memset(&tv, 0, sizeof(tv));
    gettimeofday(&tv, NULL);
    
    auto time =tv.tv_sec ;
    
    auto prequest = new cocos2d::network::HttpRequest;
    prequest->setUrl(VersionCheckURL);
    prequest->setRequestType(cocos2d::network::HttpRequest::Type::POST);
    prequest->setResponseCallback([=](cocos2d::network::HttpClient *sender, cocos2d::network::HttpResponse *response){
        
        if (response == NULL)
        {
            HallDataMgr::getInstance()->AddpopLayer("", "", EM_MODE_TYPE_REMOVE);
            HallDataMgr::getInstance()->_versionCheckcallback();
            return;
        }
        if (response->isSucceed() == false)
        {
            HallDataMgr::getInstance()->AddpopLayer("", "", EM_MODE_TYPE_REMOVE);
            HallDataMgr::getInstance()->_versionCheckcallback();
            return;
        }
        std::vector<char> *buffer = response->getResponseData();
        int nsize = (int)buffer->size();
        if (nsize == 0)
        {
            HallDataMgr::getInstance()->AddpopLayer("", "", EM_MODE_TYPE_REMOVE);
            HallDataMgr::getInstance()->_versionCheckcallback();
            return;
        }
        std::string backdata;
        backdata.append(buffer->begin(), buffer->end());
        
        log("%s \n",backdata.c_str());
        
        rapidjson::Document doc;
        doc.Parse<0>(backdata.c_str());
        
        const rapidjson::Value &val = DICTOOL->getSubDictionary_json(doc,"Data");
        
        
#if (CC_TARGET_PLATFORM == CC_PLATFORM_IOS)
        
        std::string _version = DICTOOL->getStringValue_json(val, "Version");
        
       //获取当前版本号
        NSDictionary *infoDic = [[NSBundle mainBundle] infoDictionary];
        NSString *curVersion = [infoDic objectForKey:@"CFBundleVersion"];
        
        const char *version = [curVersion UTF8String];
        
        //校验版本
        if (strcmp(_version.c_str(), version) != 0)
        {
            
            std::string updateStr = DICTOOL->getStringValue_json(val, "DownloadUrl");

            NSString *updateUrl = [NSString stringWithUTF8String:updateStr.c_str()];
            [updateUrl retain];
            auto player = static_cast<ModeLayer *>(HallDataMgr::getInstance()->AddpopLayer("提示", "发现新版本,是否前往更新", Type_Ensure_Cancel));
            player->setEnsureCallback([=](){
                
                if([[UIApplication sharedApplication] canOpenURL:[NSURL URLWithString:updateUrl]])
                {
                    [[UIApplication sharedApplication] openURL:[NSURL URLWithString:updateUrl]];
                    [updateUrl release];
                }
                
            });
            
            player->setCancelCallback([=](){
            
                HallDataMgr::getInstance()->_versionCheckcallback();
                [updateUrl release];
            });
        }else
        {
            
            HallDataMgr::getInstance()->AddpopLayer("", "", EM_MODE_TYPE_REMOVE);
            HallDataMgr::getInstance()->_versionCheckcallback();
        }
#endif
        
        
#if (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID)
        HallDataMgr::getInstance()->AddpopLayer("", "", EM_MODE_TYPE_REMOVE);
        HallDataMgr::getInstance()->_versionCheckcallback();
#endif
        
    });
    kindID = 200;
    std::string postData = "";
    postData += __String::createWithFormat("ID=%d",kindID)->getCString();
    postData += "&";
#if (CC_TARGET_PLATFORM == CC_PLATFORM_IOS)
    
    postData += "TypeID=1";
    postData += "&";
    postData += "PlatfromID=1";
#endif
    
#if (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID)
    postData += "TypeID=2";
    postData += "&";
    postData += "PlatfromID=2";
#endif
    postData += "&";
    postData += __String::createWithFormat("ServerTime=%ld",time)->getCString();
    
    prequest->setRequestData(postData.c_str(), postData.size());
    cocos2d::network::HttpClient::getInstance()->send(prequest);
    prequest->release();
    
}

const std::string MD5Encrypt(const std::string &str)
{
    char md5p[33];
    md5_passwd((char *)str.c_str(), md5p);
    std::string test;
    test.append(md5p);

    for (int index=0; index<test.size(); ++index) 
	{
        if (test[index] >='a' && test[index] <= 'z') 
            test[index] -= 32;
    }

    return test;
}

const std::string getScoreString(LONGLONG score)
{
    std::string scorestr;
    scorestr = cocos2d::__String::createWithFormat("%lld", score)->getCString();
    if (score < 10000) {
        return scorestr;
    }
    int length = (int) scorestr.length();
    if (score < 100000000) {
        scorestr = scorestr.substr(0,length-2);
        scorestr.insert(length-4, ".");
        scorestr = scorestr.substr(0,5);
        if (scorestr.substr(4,1) == ".") {
            scorestr = scorestr.substr(0,4);
        }
		scorestr.append("万");
		return scorestr;
    }
    scorestr = scorestr.substr(0,length-6);
    scorestr.insert(length-8, ".");
    scorestr.append("亿");
    return scorestr;
}

const std::string getScorewithComma(LONGLONG score ,const std::string & plusstr)
{
    int addpoint[5] = {3,7,11,15,19};
    std::string scorestr;
    scorestr = cocos2d::__String::createWithFormat("%lld", score)->getCString();
    int length = (int)scorestr.length();
    for (int index=0; index<5; ++index) {
        if (length > addpoint[index]) {
            scorestr.insert(length-addpoint[index], plusstr);
            length = (int)scorestr.length();
        }
        else
        {
            break;
        }
    }
    return scorestr;
}

const std::string getScoreWithLength(LONGLONG score, int length)
{
    std::string scorestr;
    scorestr = cocos2d::__String::createWithFormat("%lld", score)->getCString();
    int scorelength = (int)scorestr.length();
    if (scorelength < length) {
        int pluslength = length-scorelength;
        for (int index=0; index<pluslength; ++index) {
            scorestr.insert(0, "0");
        }
    }
    return scorestr;
}

void Labellengthdeal(cocos2d::Label *plabel , float length)
{
    if (plabel->getContentSize().width > length) {
        plabel->setScale(length/plabel->getContentSize().width);
    }
}
const std::string File(EM_GAME kind,const std::string &res)
{
    
    std::string file = "";
    FileUtils *_utils = FileUtils::getInstance();
    switch (kind)
    {
        /*case LandLord:
            file = __String::createWithFormat("LDResources/%s",res.c_str())->getCString();
            file = _utils->getWritablePath() + file;
            break;*/
        default:
            break;
    }
    
    if (!_utils->isFileExist(file))
    {
        DebugLog("文件不存在");
        assert(false);
    }
    
    return file;
}

//mac address
const std::string WHGetMacAddress()
{
    //安卓设备 mac
#ifdef WIN32
    {
        return NULL;
    }
    //ios设备 mac
#endif
#if (CC_TARGET_PLATFORM == CC_PLATFORM_IOS)
    NSString *macaddress = [[[UIDevice currentDevice] identifierForVendor] UUIDString];
    std::string test = [macaddress UTF8String];
    test = MD5Encrypt(test);
    return test;
#endif
    
    
#if (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID)
    JniMethodInfo t;
    std::string ret("");
    if (JniHelper::getStaticMethodInfo(t, "org/cocos2dx/cpp/AppActivity", "getMachineID", "()Ljava/lang/String;")) {
        jstring str = (jstring)t.env->CallStaticObjectMethod(t.classID, t.methodID);
        t.env->DeleteLocalRef(t.classID);
        ret = JniHelper::jstring2string(str);
        t.env->DeleteLocalRef(str);
    }
    ret = MD5Encrypt(ret);
    return ret;
#endif
    
}

void Labellengthaddpoint(cocos2d::Label *plabel, float lenght)
{
    if (plabel->getContentSize().width > lenght) {
        plabel->setDimensions(lenght-plabel->getSystemFontSize(), plabel->getSystemFontSize()+5);
        auto plabel1 = Label::createWithSystemFont("..", plabel->getSystemFontName(), plabel->getSystemFontSize());
        plabel1->setColor(plabel->getColor());
        plabel1->setAnchorPoint(Vec2(0.f, 0.5f));
        if (plabel->getTextAlignment() == TextHAlignment::CENTER && plabel->getAnchorPoint().x == 0.5f) {
            plabel1->setPosition(Vec2(plabel->getContentSize().width/2+plabel->getPositionX(), plabel->getPositionY()));
        }
        else
        {
            plabel1->setPosition(Vec2(plabel->getContentSize().width+plabel->getPositionX(), plabel->getPositionY()));
        }
        
        plabel->getParent()->addChild(plabel1);
        plabel1->setTag(1000);
    }
    else
    {
        plabel->getParent()->removeChildByTag(1000);
        plabel->setDimensions(0, 0);
    }
}

void readAnimation(const char *FramForm, const char *SaveName, int framnum ,float time , int beginindex)
{
    cocos2d::Vector<SpriteFrame *> animarray;
    for (int index = beginindex ; index < framnum ; ++index)    {
        auto str = __String::createWithFormat(FramForm,index)->getCString();
        auto pframe = SpriteFrameCache::getInstance()->getSpriteFrameByName(str);
        animarray.pushBack(pframe);
    }
    auto panim = Animation::createWithSpriteFrames(animarray, time);
    AnimationCache::getInstance()->addAnimation(panim, SaveName);
}

int strLength(const std::string &str)
{
    if (typeid(str) == typeid(std::string) && str.length() > 0) {
        int len = (int)str.length();
        std::vector<std::string> dump;
        int i = 0;
        while(i < len) {
            if (~(str.at(i) >> 8) == 0) {
                dump.push_back(str.substr(i, 3));
                i = i + 3;
            } else {
                dump.push_back(str.substr(i, 1));
                i = i + 1;
            }
        }
        return (int)dump.size();
    } else {
        printf("str is not string\n");
        return 0;
    }
}

//单字符:utf8 -> unicode 返回转换长度
int WHConverUtf8ToUnicode(WORD *destUnicode, c_char *srcUtf8)
{
    int i=0;
    if( (0x80&srcUtf8[0]) == 0 )         //一个字节的UTF-8
    {
        destUnicode[0] = srcUtf8[i++];
    }
    if( (0xE0&srcUtf8[0]) == 0xC0 )      //两字节的UTF-8
    {
        int a = ( 0x1F & srcUtf8[i++] ) << 6;
        int b = ( 0x3F & srcUtf8[i++] );
        destUnicode[0] = a + b;
    }
    if( (0xF0&srcUtf8[0]) == 0xE0 )      //三字节的UTF-8
    {
        int a = ( 0x1F & srcUtf8[i++] ) << 12;
        int b = ( 0x3F & srcUtf8[i++] ) << 6;
        int c = ( 0x3F & srcUtf8[i++] );
        
        destUnicode[0] = a + b + c;
    }
    
    return i;
}

//GIF转换回调函数
int memReadFuncGif(GifFileType* GifFile, GifByteType* buf, int count)
{
    char* ptr = (char*)(GifFile->UserData);
    memcpy(buf, ptr, count);
    GifFile->UserData = ptr + count;
    return count;
}

//GIF图片数据转换为RGBA8888
unsigned char* DecodeMemoryGIF(unsigned char* fData, long fSize, int& width, int& height, int index)
{
    GifFileType *GifFile;
    
    if ((GifFile = DGifOpen(fData, memReadFuncGif)) == NULL)
    {
        return 0;
    }
    
    if (DGifSlurp(GifFile) == 0)
    {
        return 0;
    }
    
    unsigned char *image_data = 0, *src = 0, *dst = 0;
    ColorMapObject *ColorMap;
    SavedImage *saveImg;
    GifRowType *ScreenBuffer;
    GifColorType *ColorMapEntry;
    int loc = 0;
    
    if (index >= GifFile->ImageCount)
    {
        return 0;
    }
    //    index = index % GifFile->ImageCount;
    
    ColorMap = (GifFile->Image.ColorMap ? GifFile->Image.ColorMap : GifFile->SColorMap);
    saveImg = &(GifFile->SavedImages[index]);
    ScreenBuffer = &(saveImg->RasterBits);
    
    if ((image_data = (unsigned char *) malloc(GifFile->SWidth * 4 * GifFile->SHeight)) == NULL)
    {
        DGifCloseFile(GifFile);
        return 0;
    }
    src = saveImg->RasterBits;
    dst = image_data;
    
    for (int i = 0; i < GifFile->SHeight; i++)
    {
        loc = GifFile->SWidth * i;
        for (int j = 0; j < GifFile->SWidth; j++)
        {
            ColorMapEntry = &(ColorMap->Colors[*(src + loc + j)]);
            *dst++ = ColorMapEntry->Red;
            *dst++ = ColorMapEntry->Green;
            *dst++ = ColorMapEntry->Blue;
            *dst++ = 255;
        }
    }
    
    width = GifFile->SWidth;
    height = GifFile->SHeight;
    
    DGifCloseFile(GifFile);
    return image_data;
}

Texture2D *CreatewithImageData(unsigned char *pdata, DWORD wsize)
{
    char buffer[5];
    memset(buffer, 0, 5);
    memcpy(buffer, pdata, 3);
    std::string imageHead(buffer);
    Texture2D *texture = new Texture2D;
    Image *image = new Image();
    if (imageHead == "<!D" || imageHead == "\r\n<") {
        return NULL;
    }
    if (imageHead == "GIF") {
        int headwidth,headheight;
        unsigned char * RGBAdata = DecodeMemoryGIF(pdata, wsize,headwidth,headheight,0);
        texture->initWithData(RGBAdata, wsize, Texture2D::PixelFormat::RGBA8888, headwidth, headheight, cocos2d::Size(headwidth, headheight));
    }
    else
    {
        image->initWithImageData(pdata, wsize);
        texture->initWithImage(image);
    }
    image->release();
    texture->autorelease();
    return texture;
}

//线程接收函数
void *threadLoadAreaConfig(void* param)
{
 
    auto data = FileUtils::getInstance()->getDataFromFile("shop_res/area.json");
    const char * pData = (char *)data.getBytes();
    rapidjson::Document doc;
    doc.Parse<0>(pData);
    
    //省份读取
    std::string key("0");
    std::map<int,std::string> m_list;
    int num = DICTOOL->getArrayCount_json(doc, key.c_str());
    for (int id=0; id<num; ++id)
    {
        std::string province = DICTOOL->getStringValueFromArray_json(doc, key.c_str(), id);
        m_list.insert(std::make_pair(id, province));
        
        //city读取
        std::string citykey = __String::createWithFormat("0-%d",id)->getCString();
        int citynum = DICTOOL->getArrayCount_json(doc, citykey.c_str());
        std::map<int,std::string> m_cityList;
        for (int cityid=0; cityid<citynum; ++cityid)
        {
            std::string city = DICTOOL->getStringValueFromArray_json(doc, citykey.c_str(), cityid);
            m_cityList.insert(std::make_pair(cityid, city));
            
            //区读取
            std::string areakey = citykey+ __String::createWithFormat("-%d",cityid)->getCString();
            int areanum = DICTOOL->getArrayCount_json(doc, areakey.c_str());
            
            std::map<int,std::string> m_areaList;
            for (int areaid=0; areaid<areanum; ++areaid)
            {
                if (id == (num-1))
                {
                    break;
                }
                std::string area = DICTOOL->getStringValueFromArray_json(doc, areakey.c_str(), areaid);
                m_areaList.insert(std::make_pair(areaid, area));
                
            }
            HallDataMgr::getInstance()->m_adderssList.insert(std::make_pair(areakey, m_areaList));
        }
        HallDataMgr::getInstance()->m_adderssList.insert(std::make_pair(citykey, m_cityList));
    }
    HallDataMgr::getInstance()->m_adderssList.insert(std::make_pair(key, m_list));

    pthread_kill(m_sThread,0);
    
    
    return 0;
}
void getAreaConfig()
{
    
    //创建线程
    pthread_attr_t attr;
    
    pthread_attr_init(&attr);
    
    pthread_attr_setdetachstate(&attr, PTHREAD_CREATE_DETACHED);
    
    int code = pthread_create(&m_sThread, 0, threadLoadAreaConfig, nullptr);
    
    if(code!=0)
    {
        CCLOG("线程创建失败");
        
        return;
    }
    
    //4/4 join
    pthread_detach(m_sThread);

    
}
