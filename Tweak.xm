#import <UIKit/UIKit.h>
#import "layout.h"

static UIWindow *PPOverlayWindow;

%ctor {
    @autoreleasepool {
        // Delay de 3 segundos pra engine do jogo carregar
        dispatch_after(dispatch_time(DISPATCH_TIME_NOW, (int64_t)(3.0 * NSEC_PER_SEC)), dispatch_get_main_queue(), ^{
            
            UIWindowScene *activeScene = nil;
            for (UIScene *scene in UIApplication.sharedApplication.connectedScenes) {
                if ([scene isKindOfClass:[UIWindowScene class]] && scene.activationState == UISceneActivationStateForegroundActive) {
                    activeScene = (UIWindowScene *)scene;
                    break;
                }
            }
            if (!activeScene) return;

            PPOverlayWindow = [[UIWindow alloc] initWithWindowScene:activeScene];
            PPOverlayWindow.frame = activeScene.coordinateSpace.bounds;
            PPOverlayWindow.windowLevel = UIWindowLevelAlert + 1;
            PPOverlayWindow.backgroundColor = UIColor.clearColor;
            
            UIViewController *vc = [UIViewController new];
            vc.view.backgroundColor = UIColor.clearColor;
            
            PurpouPanel *panel = [[PurpouPanel alloc] initWithFrame:PPOverlayWindow.bounds];
            [vc.view addSubview:panel];
            
            PPOverlayWindow.rootViewController = vc;
            PPOverlayWindow.hidden = NO;
        });
    }
}
