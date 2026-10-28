#import <UIKit/UIKit.h>
#import <mach/mach.h>
#import <mach-o/dyld.h>
#import "layout.h"

static UIWindow *PPOverlayWindow;
ExecutorWindow *execWindow;

// ==========================================
// OFFSETS (Do teu Offsets.hpp)
// ==========================================
#define OFF_FakeDataModel 0x8dc2258
#define OFF_RealDataModel 0x1f8
#define OFF_ScriptContext 0x440
#define OFF_LuaState      0x28

// Offset da função (do repositório iOS)
#define OFF_LUA_RESUME 0x03f674dc

// ==========================================
// FUNÇÕES DE MEMÓRIA
// ==========================================
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

uintptr_t ReadPtr(uintptr_t address) {
    if (address < 0x100000000) return 0;
    uintptr_t value = 0;
    vm_size_t size = sizeof(value);
    vm_read_overwrite(mach_task_self(), address, size, (vm_address_t)&value, &size);
    return value;
}

// ==========================================
// MOTOR DO EXECUTOR
// ==========================================
void ExecuteLuauScript(const char* script) {
    uintptr_t base = GetBaseAddress();
    if (base == 0) return;

    // 1. Pega o FakeDataModel
    uintptr_t fakeDataModel = ReadPtr(base + OFF_FakeDataModel);
    if (fakeDataModel == 0) return;

    // 2. Pega o RealDataModel
    uintptr_t realDataModel = ReadPtr(fakeDataModel + OFF_RealDataModel);
    if (realDataModel == 0) return;

    // 3. Pega o ScriptContext
    uintptr_t scriptContext = ReadPtr(realDataModel + OFF_ScriptContext);
    if (scriptContext == 0) return;

    // 4. Pega o LuaState
    uintptr_t luaState = ReadPtr(scriptContext + OFF_LuaState);
    if (luaState == 0) return;

    // 5. Chama o lua_resume
    uintptr_t lua_resume_addr = base + OFF_LUA_RESUME;
    NSLog(@"[EXECUTOR] lua_resume: %lx", lua_resume_addr);
    NSLog(@"[EXECUTOR] luaState: %lx", luaState);
    NSLog(@"[EXECUTOR] Script: %s", script);
    
    // Se tudo estiver certo, o script é enviado pra VM.
}

// ==========================================
// JANELAS
// ==========================================
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

        execWindow = [[ExecutorWindow alloc] initWithFrame:activeScene.coordinateSpace.bounds];
        execWindow.hidden = YES;
        execWindow.rootViewController = [UIViewController new];
    });
}

%ctor {
    @autoreleasepool {
        dispatch_after(dispatch_time(DISPATCH_TIME_NOW, (int64_t)(3.0 * NSEC_PER_SEC)), dispatch_get_main_queue(), ^{ 
            PPShowPanel();
        });
    }
}
