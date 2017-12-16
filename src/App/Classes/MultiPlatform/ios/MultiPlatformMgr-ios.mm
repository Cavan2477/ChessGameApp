//
//  MultiPlatformMgr-ios.cpp
//  NiuNiu
//
//  Created by zhong on 1/4/16.
//
//
#include "MultiPlatformMgr-ios.h"
#import "AppController.h"
#import "RootViewController.h"
#import <AVFoundation/AVFoundation.h>

//iOS中获取网卡mac信息
#include <sys/socket.h> // Per msqr
#include <sys/sysctl.h>
#include <net/if.h>
#include <net/if_dl.h>
@interface FaceChoiceDelegate : NSObject<UINavigationControllerDelegate,UIImagePickerControllerDelegate>
{
    
}
@end

@implementation FaceChoiceDelegate

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
    UIImage *pimage = [FaceChoiceDelegate reSizeImage:image toSize:CGSizeMake(48.0, 48.0)];
    NSData *imageData = UIImagePNGRepresentation(pimage);
    
    cocos2d::Image * pcimage = new cocos2d::Image();
    if (pcimage->initWithImageData((const unsigned char *)[imageData bytes], [imageData length]))
    {
        if (MultiPlatformMgr::getInstance()->getPlatformDelegate())
        {
            MultiPlatformMgr::getInstance()->getPlatformDelegate()->didPickImgFinish(pcimage);
        }
    }    
    pcimage->release();
    [self release];
}

- (void)imagePickerControllerDidCancel:(UIImagePickerController *)picker
{
    [picker dismissModalViewControllerAnimated:YES];
    [self release];
}

@end

MultiPlatformMgr_ios::MultiPlatformMgr_ios()
{
    
}

MultiPlatformMgr_ios::~MultiPlatformMgr_ios()
{
    
}

MultiPlatformMgr* MultiPlatformMgr::getInstance()
{
    if (nullptr == s_multiInstance)
    {
        s_multiInstance = new MultiPlatformMgr_ios();
    }
    return s_multiInstance;
}

void MultiPlatformMgr_ios::test()
{
    cocos2d::log("ios test");
}

void MultiPlatformMgr_ios::triggerPickImg()
{
    UIImagePickerController *pickerImage = [[UIImagePickerController alloc] init];
    if([UIImagePickerController isSourceTypeAvailable:UIImagePickerControllerSourceTypePhotoLibrary]) {
        pickerImage.sourceType = UIImagePickerControllerSourceTypePhotoLibrary;
        pickerImage.mediaTypes = [UIImagePickerController availableMediaTypesForSourceType:pickerImage.sourceType];
        
    }
    FaceChoiceDelegate *delegate = [[FaceChoiceDelegate alloc] init];
    pickerImage.delegate = delegate;
    pickerImage.allowsEditing = YES;
    AppController * pApp = (AppController*)[[UIApplication sharedApplication] delegate];
    //[pApp.viewController presentModalViewController:pickerImage animated:YES];
    [pApp.viewController presentViewController:((UIImagePickerController *)pickerImage) animated:YES completion:nil];
}

void MultiPlatformMgr_ios::switchVibratorState(bool bOpen)
{
    m_bOpenVibrator = bOpen;
    if (m_bOpenVibrator)
    {
        AudioServicesPlaySystemSound(kSystemSoundID_Vibrate);
    }
}

void MultiPlatformMgr_ios::vibrator()
{
    if (!m_bOpenVibrator)
    {
        return;
    }
    AudioServicesPlaySystemSound(kSystemSoundID_Vibrate);
}

void MultiPlatformMgr_ios::updateVersion()
{
    NSString *app_version = [[[NSBundle mainBundle] infoDictionary] objectForKey:@"CFBundleShortVersionString"];
    if (nil != app_version)
    {
        m_strVersion = [app_version UTF8String];
    }
    else
    {
        m_strVersion = "";
    }
}

void MultiPlatformMgr_ios::updateMachineID()
{
    int                 mib[6];
    size_t              len;
    char                *buf;
    unsigned char       *ptr;
    struct if_msghdr    *ifm;
    struct sockaddr_dl  *sdl;
    
    mib[0] = CTL_NET;
    mib[1] = AF_ROUTE;
    mib[2] = 0;
    mib[3] = AF_LINK;
    mib[4] = NET_RT_IFLIST;
    
    if ((mib[5] = if_nametoindex("en0")) == 0)
    {
        printf("Error: if_nametoindex error/n");
        return ;
    }
    
    if (sysctl(mib, 6, NULL, &len, NULL, 0) < 0)
    {
        printf("Error: sysctl, take 1/n");
        return ;
    }
    
    if ((buf=(char*)malloc(len)) == NULL)
    {
        printf("Could not allocate memory. error!/n");
        return ;
    }
    
    if (sysctl(mib, 6, buf, &len, NULL, 0) < 0)
    {
        printf("Error: sysctl, take 2");
        free(buf);
        return;
    }
    
    ifm = (struct if_msghdr *)buf;
    sdl = (struct sockaddr_dl *)(ifm + 1);
    ptr = (unsigned char *)LLADDR(sdl);
    
    NSString *outstring = [NSString stringWithFormat:@"%02X:%02X:%02X:%02X:%02X:%02X",*ptr, *(ptr+1), *(ptr+2), *(ptr+3), *(ptr+4), *(ptr+5)];
    free(buf);
    
    NSString *bundleIdentifier = [[NSBundle mainBundle] bundleIdentifier];
    NSString *stringToHash = [NSString stringWithFormat:@"%@%@",outstring,bundleIdentifier];
    m_strMachineID = [stringToHash UTF8String];
}