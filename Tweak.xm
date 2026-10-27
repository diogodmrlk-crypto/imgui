#import <UIKit/UIKit.h>
#import <mach/mach.h>
#import <mach-o/dyld.h>
#import <sys/sysctl.h>
#import "layout.h"

// ==========================================
// OFFSETS DO ROBLOX (OS QUE VOCÊ MANDOU)
// ==========================================
uintptr_t BaseAddress = 0;
uintptr_t FakeDataModel = 0;
uintptr_t VisualEngine = 0;

// Offsets
#define OFF_FakeDataModel 0x8dc2258
#define OFF_VisualEngine 0x83f0a08
#define OFF_RealDataModel 0x1f8
#define OFF_Workspace 0x158
#define OFF_CurrentCamera 0x4b8
#define OFF_ViewMatrix 0x1b0
#define OFF_LocalPlayer 0x130
#define OFF_PlayerName 0x138
#define OFF_ModelInstance 0x298
#define OFF_Humanoid 0x0
#define OFF_Health 0x190
#define OFF_HumanoidRootPart 0x470
#define OFF_Primitive 0x188
#define OFF_Position 0xec
#define OFF_ChildrenStart 0x78
#define OFF_ChildrenEnd 0x8

// ==========================================
// FUNÇÕES DE MEMÓRIA (MACH)
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
    kern_return_t kr = vm_read_overwrite(mach_task_self(), address, size, (vm_address_t)&value, &size);
    if (kr != KERN_SUCCESS) return 0;
    return value;
}

float ReadFloat(uintptr_t address) {
    if (address < 0x100000000) return 0.0f;
    float value = 0.0f;
    vm_size_t size = sizeof(value);
    vm_read_overwrite(mach_task_self(), address, size, (vm_address_t)&value, &size);
    return value;
}

// ==========================================
// MOTOR DE ESP (LÊ OS OFFSETS)
// ==========================================
void UpdateESP() {
    if (!g_espEnabled) return;
    
    // Pega o Base Address
    if (BaseAddress == 0) {
        BaseAddress = GetBaseAddress();
        if (BaseAddress == 0) return;
    }
    
    // Fake Data Model
    if (FakeDataModel == 0) {
        FakeDataModel = ReadPtr(BaseAddress + OFF_FakeDataModel);
        if (FakeDataModel == 0) return;
    }
    
    // Real Data Model
    uintptr_t RealDataModel = ReadPtr(FakeDataModel + OFF_RealDataModel);
    if (RealDataModel == 0) return;
    
    // Visual Engine
    if (VisualEngine == 0) {
        VisualEngine = ReadPtr(BaseAddress + OFF_VisualEngine);
        if (VisualEngine == 0) return;
    }
    
    // View Matrix
    uintptr_t viewMatrixAddr = VisualEngine + OFF_ViewMatrix;
    float viewMatrix[16];
    vm_size_t size = sizeof(viewMatrix);
    vm_read_overwrite(mach_task_self(), viewMatrixAddr, size, (vm_address_t)viewMatrix, &size);
    
    // Workspace e Câmera
    uintptr_t Workspace = ReadPtr(RealDataModel + OFF_Workspace);
    uintptr_t CurrentCamera = ReadPtr(Workspace + OFF_CurrentCamera);
    
    // Local Player
    uintptr_t LocalPlayer = ReadPtr(RealDataModel + OFF_LocalPlayer);
    if (LocalPlayer == 0) return;
    
    // Loop pelos players (Children do DataModel)
    uintptr_t childrenStart = ReadPtr(RealDataModel + OFF_ChildrenStart);
    uintptr_t childrenEnd = ReadPtr(RealDataModel + OFF_ChildrenEnd);
    
    for (uintptr_t child = childrenStart; child < childrenEnd; child += 0x10) {
        uintptr_t player = ReadPtr(child);
        if (player == LocalPlayer) continue;
        
        // Pega o ModelInstance (Character)
        uintptr_t modelInstance = ReadPtr(player + OFF_ModelInstance);
        if (modelInstance == 0) continue;
        
        // Pega o HumanoidRootPart
        uintptr_t humanoid = ReadPtr(modelInstance + OFF_Humanoid);
        uintptr_t hrp = ReadPtr(humanoid + OFF_HumanoidRootPart);
        if (hrp == 0) continue;
        
        // Pega a posição do inimigo
        uintptr_t primitive = ReadPtr(hrp + OFF_Primitive);
        float posX = ReadFloat(primitive + OFF_Position);
        float posY = ReadFloat(primitive + OFF_Position + 4);
        float posZ = ReadFloat(primitive + OFF_Position + 8);
        
        // Aqui tu faria o cálculo WorldToScreen e desenharia a caixa no inimigo
        // (Vou deixar o desenho pra UI, com um CGContext)
    }
}

// ==========================================
// JANELA DO PAINEL
// ==========================================
static UIWindow *PPOverlayWindow;

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
        
        // Inicia o loop do ESP
        CADisplayLink *link = [CADisplayLink displayLinkWithTarget:[NSClassFromString(@"PPESPEngine") class] selector:@selector(tick)];
        [link addToRunLoop:[NSRunLoop mainRunLoop] forMode:NSRunLoopCommonModes];
    });
}

%ctor {
    @autoreleasepool {
        dispatch_after(dispatch_time(DISPATCH_TIME_NOW, (int64_t)(3.0 * NSEC_PER_SEC)), dispatch_get_main_queue(), ^{ 
            PPShowPanel();
        });
    }
}
