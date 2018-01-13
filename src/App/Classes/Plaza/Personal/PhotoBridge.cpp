/************************************************************************************
 * file: 		PhotoBridge.cpp
 * copyright:	Cavan.Liu 2017
 * Author: 		Cavan.Liu
 * Create: 		2017/12/12 23:03:40
 * Description: 
 * Version	Author		Time			Description
 * V1.0    	Cavan.Liu	2017/12/12			
 *
 ************************************************************************************/

#include "PhotoBridge.h"
#include "../../DataMgr/HallDataMgr.h"

USING_NS_CC;

#if (CC_TARGET_PLATFORM == CC_PLATFORM_IOS)
#import "AppController.h"
#import "RootViewController.h"
#import <AVFoundation/AVFoundation.h>

@interface ChoiceDelegate : NSObject<UINavigationControllerDelegate,UIImagePickerControllerDelegate>
{
    
}
@property(assign)PhotoDelegate *m_delegate;

@property(assign)EM_CHOOSE_TYPE m_type;
@end

@implementation ChoiceDelegate

+(UIImage*)reSizeImage:(UIImage*)image toSize:(CGSize)reSize
{
    UIGraphicsBeginImageContext(CGSizeMake(reSize.width, reSize.height));
    [image drawInRect:CGRectMake(0, 0, reSize.width, reSize.height)];
    UIImage *reSizeImage = UIGraphicsGetImageFromCurrentImageContext();
    UIGraphicsEndImageContext();
    return reSizeImage;
}

- (void)imagePickerController:(UIImagePickerController *)picker didFinishPickingImage:(UIImage *)image editingInfo:(NSDictionary *)editingInfo
{
    [picker dismissModalViewControllerAnimated:YES];
    NSData *imageData = NULL;
    if(_m_type == EM_CHOOSE_TYPE_HEAD)
    {
        UIImage *pimage = [ChoiceDelegate reSizeImage:image toSize:CGSizeMake(48.0, 48.0)];
        imageData = UIImagePNGRepresentation(pimage);
    }
    else
    {
        imageData = UIImageJPEGRepresentation(image, 0.01);
    }
    
    //void *data = (void *)[imageData bytes];
    cocos2d::Image * pcimage = new Image;
    pcimage->initWithImageData((const unsigned char *)[imageData bytes], [imageData length]);
    if (_m_delegate) {
        _m_delegate->choiceComplete(pcimage);
    }
    pcimage->release();
    [self release];
}

- (void)imagePickerControllerDidCancel:(UIImagePickerController *)picker
{
    [picker dismissViewControllerAnimated:YES completion:nil];
    [self release];
}

@end
#endif

PhotoBridge::PhotoBridge()
:m_emChooseType(EM_CHOOSE_TYPE_HEAD)
, m_completeCallback(nullptr)
{
#if (CC_TARGET_PLATFORM == CC_PLATFORM_IOS)
    pickerImage = [[UIImagePickerController alloc] init];
    if([UIImagePickerController isSourceTypeAvailable:UIImagePickerControllerSourceTypePhotoLibrary]) {
        ((UIImagePickerController *)pickerImage).sourceType = UIImagePickerControllerSourceTypePhotoLibrary;
        ((UIImagePickerController *)pickerImage).mediaTypes = [UIImagePickerController availableMediaTypesForSourceType:((UIImagePickerController *)pickerImage).sourceType];
    }
    ((UIImagePickerController *)pickerImage).delegate = nil;
    ((UIImagePickerController *)pickerImage).allowsEditing = YES;
#endif
}

PhotoBridge::~PhotoBridge()
{
#if (CC_TARGET_PLATFORM == CC_PLATFORM_IOS)
    [((UIImagePickerController *)pickerImage) release];
#endif
}

void PhotoBridge::onEnter()
{
    Node::onEnter();
    Director::getInstance()->getEventDispatcher()->addEventListenerWithFixedPriority(EventListenerCustom::create(STR_ND_ANDROID_PHOTO_IMAGE, CC_CALLBACK_1(PhotoBridge::notifyAndroidPhotoImage, this)), 1);
}

void PhotoBridge::onExit()
{
    Director::getInstance()->getEventDispatcher()->removeCustomEventListeners(STR_ND_ANDROID_PHOTO_IMAGE);
    Node::onExit();
}

void PhotoBridge::notifyAndroidPhotoImage(cocos2d::EventCustom *event)
{
    auto imagepath = static_cast<__String *>(event->getUserData());

    log("图片路径:%s ",imagepath->getCString());
    Image * pimage = new Image();

    if (pimage->initWithImageFile(imagepath->getCString()))
    {
        log("path:%s  length:%zd",imagepath->getCString(), pimage->getDataLen());

        if (pimage->getDataLen() < 9216) 
		{
			HallDataMgr::getInstance()->AddpopLayer("", "选择的图片太小", EM_MODE_TYPE_ENSURE);
            pimage->release();

            return;
        }

        if (m_completeCallback)
            m_completeCallback(pimage);
    }

    pimage->release();
}

void PhotoBridge::setChoiceType(int type)
{
    m_emChooseType = (EM_CHOOSE_TYPE)type;
}

void PhotoBridge::openPhoto()
{
#if (CC_TARGET_PLATFORM == CC_PLATFORM_IOS)
    ChoiceDelegate *delegate = [[ChoiceDelegate alloc] init];
    delegate.m_delegate = this;
    delegate.m_type = m_emChooseType;
    ((UIImagePickerController *)pickerImage).delegate = delegate;
    AppController * pApp = (AppController*)[[UIApplication sharedApplication] delegate];
    [pApp.viewController presentViewController:((UIImagePickerController *)pickerImage) animated:YES completion:nil];
#endif

#if (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID)
    JniMethodInfo minfo;
    jobject jobj;
    bool isHave = JniHelper::getStaticMethodInfo(minfo, "org/cocos2dx/cpp/AppActivity","openphoto","()Z");
    if (isHave)
    {
        minfo.env->CallStaticBooleanMethod(minfo.classID, minfo.methodID);
        minfo.env->DeleteLocalRef(minfo.classID);
    }
#endif
}

void PhotoBridge::choiceComplete(cocos2d::Image *pImage)
{
    if (m_completeCallback)
        m_completeCallback(pImage);
}
