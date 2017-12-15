//
//  Photo_Bridge.cpp
//  
//
//  Created by wh on 16/1/21.
//
//

#include "Photo_Bridge.h"
#include "HallDataMgr.h"
USING_NS_CC;

#if (CC_TARGET_PLATFORM == CC_PLATFORM_IOS)
#import "AppController.h"
#import "RootViewController.h"
#import <AVFoundation/AVFoundation.h>

@interface ChoiceDelegate : NSObject<UINavigationControllerDelegate,UIImagePickerControllerDelegate>
{
    
}
@property(assign)PhotoDelegate *m_delegate;

@property(assign)ChoiceType m_type;
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
    if(_m_type == Type_Head)
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
    if (_m_delegate)
    {
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
:m_type(Type_Head)
,m_completecallback(nullptr)
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
    Director::getInstance()->getEventDispatcher()->addEventListenerWithFixedPriority(EventListenerCustom::create(whNd_Android_PhotoImage, CC_CALLBACK_1(PhotoBridge::notifyAndroidPhotoImage, this)), 1);
}

void PhotoBridge::onExit()
{
    Director::getInstance()->getEventDispatcher()->removeCustomEventListeners(whNd_Android_PhotoImage);
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
            HallDataMgr::getInstance()->AddpopLayer("", "选择图片太小...", Type_Ensure);
            pimage->release();
            return;
        }
        if (m_completecallback) {
            m_completecallback(pimage);
        }
    }
    pimage->release();
}

void PhotoBridge::setChoiceType(int type)
{
    m_type = (ChoiceType)type;
}

void PhotoBridge::openPhoto()
{
#if (CC_TARGET_PLATFORM == CC_PLATFORM_IOS)
    ChoiceDelegate *delegate = [[ChoiceDelegate alloc] init];
    delegate.m_delegate = this;
    delegate.m_type = m_type;
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

void PhotoBridge::choiceComplete(cocos2d::Image *pimage)
{
    if (m_completecallback)
    {
        m_completecallback(pimage);
    }
}
