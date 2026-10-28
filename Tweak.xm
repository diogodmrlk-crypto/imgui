#import <UIKit/UIKit.h>
#import <mach/mach.h>
#import <mach-o/dyld.h>
#import "layout.h"

static UIWindow *PPOverlayWindow;

// Motor de Execução (Esqueleto)
uintptr_t BaseAddress = 0;
uintptr_t LuaState = 0;

uintptr_t GetBaseAddress() {
    uintptr_t base = 0;
    uint32_t count = _dyld_image_count();
    for (uint32_t i = 0; i < count; i++) {
        const char *name = _dyld_get_image_name(i);
        if (strstr(name, "Roblox")) {
            base = (uintptr_t)_dyld_get_image_header(i);
            break;
        }
    }
    return base;
}

void ExecuteLuauScript(const char* script) {
    // OFFSETS DA VERSÃO 2.720.1164 (DO REPOSITÓRIO QUE TU MANDOU)
    #define LUA_RESUME_OFFSET 0x03f674dc
    #define LUA_STATE_OFFSET  0x03f6fac4
    
    // Pega o endereço base
    uintptr_t base = GetBaseAddress();
    if (base == 0) return;
    
    // Chama a função lua_resume
    // (Isso é o esqueleto, pra funcionar tu precisa passar o lua_state correto)
    uintptr_t lua_resume_addr = base + LUA_RESUME_OFFSET;
    NSLog(@"[EXECUTOR] Endereço lua_resume: %lx", lua_resume_addr);
    NSLog(@"[EXECUTOR] Script: %s", script);
}

// Janela
@interface PurpouWindow : UIWindow
@property (nonatomic, assign) UIView *panelView;
@end
@implementation PurpouWindow
- (UIView *)hitTest:(CGPoint)point withEvent:(UIEvent *)event {
    UIView *hitView = [super hitTest:point withEvent:event];
    if (self.panelView && CGRectContainsPoint(self.panelView.frame, point)) {
        return hitView;
    }
    return nil;
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

%ctor {
    @autoreleasepool {
        dispatch_after(dispatch_time(DISPATCH_TIME_NOW, (int64_t)(3.0 * NSEC_PER_SEC)), dispatch_get_main_queue(), ^{ 
            PPShowPanel();
        });
    }
}
