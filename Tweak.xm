#import <UIKit/UIKit.h>
#import <mach/mach.h>
#import <mach-o/dyld.h>
#import "layout.h"

// ==========================================
// OFFSETS DO ROBLOX (OS QUE VOCÊ MANDOU)
// ==========================================
uintptr_t BaseAddress = 0;
uintptr_t FakeDataModel = 0;
uintptr_t VisualEngine = 0;

#define OFF_FakeDataModel 0x8dc2258
#define OFF_VisualEngine 0x83f0a08
#define OFF_RealDataModel 0x1f8
#define OFF_Workspace 0x158
#define OFF_CurrentCamera 0x4b8
#define OFF_ViewMatrix 0x1b0
#define OFF_ChildrenStart 0x78
#define OFF_ChildrenEnd 0x8
#define OFF_LocalPlayer 0x130
#define OFF_ModelInstance 0x298
#define OFF_Humanoid 0x0
#define OFF_Health 0x190
#define OFF_HumanoidRootPart 0x470
#define OFF_Primitive 0x188
#define OFF_Position 0xec

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
// ESP OVERLAY (DESENHA AS CAIXAS)
// ==========================================
@interface ESPOverlay : UIView
@end
@implementation ESPOverlay
- (void)drawRect:(CGRect)rect {
    if (!g_espEnabled) return;
    
    // Pega o Base Address
    if (BaseAddress == 0) {
        BaseAddress = GetBaseAddress();
        if (BaseAddress == 0) return;
    }
    
    // Pega o Fake Data Model
    if (FakeDataModel == 0) {
        FakeDataModel = ReadPtr(BaseAddress + OFF_FakeDataModel);
        if (FakeDataModel == 0) return;
    }
    
    // Pega o Real Data Model
    uintptr_t RealDataModel = ReadPtr(FakeDataModel + OFF_RealDataModel);
    if (RealDataModel == 0) return;
    
    // Pega o Visual Engine
    if (VisualEngine == 0) {
        VisualEngine = ReadPtr(BaseAddress + OFF_VisualEngine);
        if (VisualEngine == 0) return;
    }
    
    // View Matrix (Matriz da câmera)
    uintptr_t viewMatrixAddr = VisualEngine + OFF_ViewMatrix;
    float viewMatrix[16];
    vm_size_t size = sizeof(viewMatrix);
    vm_read_overwrite(mach_task_self(), viewMatrixAddr, size, (vm_address_t)viewMatrix, &size);
    
    // Pega a Workspace e a Câmera (pra saber a tela)
    uintptr_t Workspace = ReadPtr(RealDataModel + OFF_Workspace);
    uintptr_t CurrentCamera = ReadPtr(Workspace + OFF_CurrentCamera);
    
    // Local Player
    uintptr_t LocalPlayer = ReadPtr(RealDataModel + OFF_LocalPlayer);
    if (LocalPlayer == 0) return;
    
    // Pega os players
    uintptr_t childrenStart = ReadPtr(RealDataModel + OFF_ChildrenStart);
    uintptr_t childrenEnd = ReadPtr(RealDataModel + OFF_ChildrenEnd);
    
    CGContextRef ctx = UIGraphicsGetCurrentContext();
    CGContextSetStrokeColorWithColor(ctx, [UIColor redColor].CGColor);
    CGContextSetLineWidth(ctx, 2.0f);
    
    for (uintptr_t child = childrenStart; child < childrenEnd; child += 0x10) {
        uintptr_t player = ReadPtr(child);
        if (player == LocalPlayer) continue;
        
        uintptr_t modelInstance = ReadPtr(player + OFF_ModelInstance);
        if (modelInstance == 0) continue;
        
        uintptr_t humanoid = ReadPtr(modelInstance + OFF_Humanoid);
        if (humanoid == 0) continue;
        
        float health = ReadFloat(humanoid + OFF_Health);
        if (health <= 0) continue;
        
        uintptr_t hrp = ReadPtr(humanoid + OFF_HumanoidRootPart);
        if (hrp == 0) continue;
        
        uintptr_t primitive = ReadPtr(hrp + OFF_Primitive);
        if (primitive == 0) continue;
        
        float posX = ReadFloat(primitive + OFF_Position);
        float posY = ReadFloat(primitive + OFF_Position + 4);
        float posZ = ReadFloat(primitive + OFF_Position + 8);
        
        // Aqui a gente faz o cálculo WorldToScreen
        // (A matemática de projeção da câmera)
        CGFloat screenWidth = [UIScreen mainScreen].bounds.size.width;
        CGFloat screenHeight = [UIScreen mainScreen].bounds.size.height;
        
        float w = viewMatrix[3] * posX + viewMatrix[7] * posY + viewMatrix[11] * posZ + viewMatrix[15];
        if (w < 0.01f) continue; // Atrás da câmera
        
        float x = viewMatrix[0] * posX + viewMatrix[4] * posY + viewMatrix[8] * posZ + viewMatrix[12];
        float y = viewMatrix[1] * posX + viewMatrix[5] * posY + viewMatrix[9] * posZ + viewMatrix[13];
        
        float screenX = (screenWidth / 2) * (1 + x / w);
        float screenY = (screenHeight / 2) * (1 - y / w);
        
        // Desenha a caixa em volta do inimigo
        if (screenX > 0 && screenX < screenWidth && screenY > 0 && screenY < screenHeight) {
            CGRect rect = CGRectMake(screenX - 20, screenY - 40, 40, 80);
            CGContextStrokeRect(ctx, rect);
        }
    }
}
@end

// ==========================================
// JANELA E RENDERIZAÇÃO
// ==========================================
static UIWindow *PPOverlayWindow;
static ESPOverlay *espOverlay;

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
        
        // Adiciona o overlay do ESP (por baixo do painel)
        espOverlay = [[ESPOverlay alloc] initWithFrame:PPOverlayWindow.bounds];
        espOverlay.backgroundColor = UIColor.clearColor;
        espOverlay.userInteractionEnabled = NO;
        [vc.view addSubview:espOverlay];
        
        // Adiciona o painel
        PurpouPanel *panel = [[PurpouPanel alloc] initWithFrame:PPOverlayWindow.bounds];
        [vc.view addSubview:panel];
        
        PPOverlayWindow.rootViewController = vc;
        ((PurpouWindow *)PPOverlayWindow).panelView = panel.panel;
        PPOverlayWindow.hidden = NO;
        
        // Loop de atualização do ESP (chama o drawRect 60x por segundo)
        CADisplayLink *link = [CADisplayLink displayLinkWithTarget:espOverlay selector:@selector(setNeedsDisplay)];
        [link addToRunLoop:[NSRunLoop mainRunLoop] forMode:NSRunLoopCommonModes];
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
