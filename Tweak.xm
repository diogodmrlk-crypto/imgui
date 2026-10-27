#import <UIKit/UIKit.h>
#import "layout.h"

// ==========================================
// FUNÇÕES DE MEMÓRIA (PREPARADAS PROS OFFSETS)
// ==========================================
// As funções ReadPtr e ReadFloat estão aqui mas o iOS só deixa usar com permissão
// (Se o jogo fechar ao ativar o ESP, é o iOS bloqueando, não o código)

static UIWindow *PPOverlayWindow;

@interface PurpouWindow : UIWindow
@property (nonatomic, assign) UIView *panelView;
@end

@implementation PurpouWindow
- (UIView *)hitTest:(CGPoint)point withEvent:(UIEvent *)event {
    UIView *hitView = [super hitTest:point withEvent:event];
    if (self.panelView && CGRectContainsPoint(self.panelView.frame, point)) {
        return hitView; // Toca no painel
    }
    return nil; // Toca no jogo
}
@end

static void PPShowPanel(void) {
    dispatch_async(dispatch_get_main_queue(), ^{
        if (PPOverlayWindow) { PPOverlayWindow.hidden = NO; return; }

        UIWindowScene *activeScene = nil;
        for (UIScene *scene in UIApplication.sharedApplication.connectedScenes) {
            if ([scene isKindOfClass:[UIWindowScene class]] && scene.activationState == UISceneActivationStateForegroundActive) {
                activeScene = (UIWindowScene *)scene;
                break;
            }
        }
        if (!activeScene) return;

        PPOverlayWindow = [[PurpouWindow alloc] initWithWindowScene:activeScene];
        PPOverlayWindow.frame = activeScene.coordinateSpace.bounds;
        PPOverlayWindow.windowLevel = UIWindowLevelAlert + 1;
        PPOverlayWindow.backgroundColor = UIColor.clearColor;
        
        UIViewController *vc = [UIViewController new];
        vc.view.backgroundColor = UIColor.clearColor;
        
        PurpouPanel *panel = [[PurpouPanel alloc] initWithFrame:PPOverlayWindow.bounds];
        [vc.view addSubview:panel];
        
        PPOverlayWindow.rootViewController = vc;
        ((PurpouWindow *)PPOverlayWindow).panelView = panel.panel;
        PPOverlayWindow.hidden = NO;
    });
}

@interface TouchTrigger : NSObject
@end
@implementation TouchTrigger
+ (void)setup {
    UIWindow *mainWin = UIApplication.sharedApplication.windows.firstObject;
    if (!mainWin) return;
    
    UITapGestureRecognizer *tap = [[UITapGestureRecognizer alloc] initWithTarget:self action:@selector(triggered)];
    tap.numberOfTouchesRequired = 3;
    tap.numberOfTapsRequired = 4;
    [mainWin addGestureRecognizer:tap];
}
+ (void)triggered {
    if (!PPOverlayWindow) {
        PPShowPanel();
    } else {
        PPOverlayWindow.hidden = !PPOverlayWindow.hidden;
    }
}
@end

%ctor {
    @autoreleasepool {
        dispatch_after(dispatch_time(DISPATCH_TIME_NOW, (int64_t)(3.0 * NSEC_PER_SEC)), dispatch_get_main_queue(), ^{ 
            PPShowPanel();
        });

        dispatch_after(dispatch_time(DISPATCH_TIME_NOW, (int64_t)(4.0 * NSEC_PER_SEC)), dispatch_get_main_queue(), ^{
            [TouchTrigger setup];
        });
    }
}
