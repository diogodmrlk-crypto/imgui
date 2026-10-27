#import <UIKit/UIKit.h>
#import <Metal/Metal.h>
#import <MetalKit/MetalKit.h>
#import "imgui.h"
#import "backends/imgui_impl_metal.h"
#import "layout.h"

static UIWindow *PPOverlayWindow = nil;
static id<MTLDevice> g_device = nil;
static BOOL g_imgui_initialized = NO;

@interface PPImGuiView : MTKView
@end

@implementation PPImGuiView {
    id<MTLCommandQueue> _commandQueue;
    CADisplayLink *_displayLink;
}

- (instancetype)initWithFrame:(CGRect)frame device:(id<MTLDevice>)device {
    self = [super initWithFrame:frame device:device];
    if (self) {
        self.colorPixelFormat = MTLPixelFormatBGRA8Unorm;
        self.clearColor = MTLClearColorMake(0, 0, 0, 0);
        self.opaque = NO;
        self.paused = NO;
        self.enableSetNeedsDisplay = NO;
        
        _commandQueue = [device newCommandQueue];
        ImGui_ImplMetal_Init(device);
        
        _displayLink = [CADisplayLink displayLinkWithTarget:self selector:@selector(render)];
        [_displayLink addToRunLoop:[NSRunLoop mainRunLoop] forMode:NSRunLoopCommonModes];
    }
    return self;
}

- (void)dealloc {
    [_displayLink invalidate];
}

- (void)render {
    @autoreleasepool {
        MTLRenderPassDescriptor *desc = self.currentRenderPassDescriptor;
        if (!desc) return;

        id<MTLCommandBuffer> cmd = [_commandQueue commandBuffer];
        id<MTLRenderCommandEncoder> enc = [cmd renderCommandEncoderWithDescriptor:desc];
        
        ImGui_ImplMetal_NewFrame(desc);
        ImGui::NewFrame();
        
        // CHAMA O TEU MENU (design laranja, sidebar, etc.)
        menu(); 
        
        ImGui::Render();
        ImGui_ImplMetal_RenderDrawData(ImGui::GetDrawData(), cmd, enc);
        
        [enc endEncoding];
        [cmd presentDrawable:self.currentDrawable];
        [cmd commit];
    }
}
@end

// Função que sobe o painel (chamada quando a janela do jogo tá ativa)
static void PPShowPanel(void) {
    if (PPOverlayWindow) return;

    // 1. Pega a cena ativa
    UIWindowScene *activeScene = nil;
    for (UIScene *scene in UIApplication.sharedApplication.connectedScenes) {
        if ([scene isKindOfClass:[UIWindowScene class]] && scene.activationState == UISceneActivationStateForegroundActive) {
            activeScene = (UIWindowScene *)scene;
            break;
        }
    }
    if (!activeScene) return;

    // 2. Cria o contexto do ImGui UMA vez
    if (!g_imgui_initialized) {
        ImGui::CreateContext();
        ImGuiIO& io = ImGui::GetIO();
        io.Fonts->AddFontDefault(); // Fonte padrão
        io.IniFilename = NULL;
        g_imgui_initialized = YES;
    }

    // 3. Cria o Metal Device
    if (!g_device) {
        g_device = MTLCreateSystemDefaultDevice();
    }

    // 4. Cria a janela
    PPOverlayWindow = [[UIWindow alloc] initWithWindowScene:activeScene];
    PPOverlayWindow.frame = activeScene.coordinateSpace.bounds;
    PPOverlayWindow.windowLevel = UIWindowLevelNormal + 1;
    PPOverlayWindow.backgroundColor = UIColor.clearColor;
    
    UIViewController *vc = [UIViewController new];
    vc.view.backgroundColor = UIColor.clearColor;
    
    PPImGuiView *imguiView = [[PPImGuiView alloc] initWithFrame:PPOverlayWindow.bounds device:g_device];
    imguiView.autoresizingMask = UIViewAutoresizingFlexibleWidth | UIViewAutoresizingFlexibleHeight;
    [vc.view addSubview:imguiView];
    
    PPOverlayWindow.rootViewController = vc;
    PPOverlayWindow.hidden = NO;
    
    NSLog(@"[PURPOU] Painel criado e visível!");
}

// Observa quando o jogo está prestes a ficar ativo e chama o painel
static void setupObservers(void) {
    [[NSNotificationCenter defaultCenter] addObserverForName:UIApplicationDidBecomeActiveNotification
        object:nil queue:[NSOperationQueue mainQueue]
        usingBlock:^(__unused NSNotification *notification) {
            // Espera 2 segundos DEPOIS que o jogo ficou ativo
            dispatch_after(dispatch_time(DISPATCH_TIME_NOW, (int64_t)(2.0 * NSEC_PER_SEC)),
                dispatch_get_main_queue(), ^{ 
                    PPShowPanel();
                });
        }];
}

%ctor {
    @autoreleasepool {
        setupObservers();
    }
}
