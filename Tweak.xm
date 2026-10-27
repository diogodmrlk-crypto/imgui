#import <UIKit/UIKit.h>
#import <Metal/Metal.h>
#import <MetalKit/MetalKit.h>
#import "imgui.h"
#import "backends/imgui_impl_metal.h"
#import "layout.h"

static UIWindow *PPOverlayWindow;
static id<MTLDevice> g_device = nil;

@interface PPImGuiView : MTKView
@end

@implementation PPImGuiView {
    id<MTLCommandQueue> _commandQueue;
}

- (instancetype)initWithFrame:(CGRect)frame device:(id<MTLDevice>)device {
    self = [super initWithFrame:frame device:device];
    if (self) {
        self.colorPixelFormat = MTLPixelFormatBGRA8Unorm;
        self.clearColor = MTLClearColorMake(0, 0, 0, 0);
        self.opaque = NO;
        self.paused = NO;
        
        _commandQueue = [device newCommandQueue];
        
        ImGui_ImplMetal_Init(device);
        
        CADisplayLink *link = [CADisplayLink displayLinkWithTarget:self selector:@selector(render)];
        [link addToRunLoop:[NSRunLoop mainRunLoop] forMode:NSRunLoopCommonModes];
    }
    return self;
}

- (void)render {
    @autoreleasepool {
        MTLRenderPassDescriptor *desc = self.currentRenderPassDescriptor;
        if (!desc) return;

        id<MTLCommandBuffer> cmd = [_commandQueue commandBuffer];
        id<MTLRenderCommandEncoder> enc = [cmd renderCommandEncoderWithDescriptor:desc];
        
        ImGui_ImplMetal_NewFrame(desc);
        ImGui::NewFrame();
        
        menu(); 
        
        ImGui::Render();
        ImGui_ImplMetal_RenderDrawData(ImGui::GetDrawData(), cmd, enc);
        
        [enc endEncoding];
        [cmd presentDrawable:self.currentDrawable];
        [cmd commit];
    }
}
@end

static void PPShowPanel(void) {
    dispatch_async(dispatch_get_main_queue(), ^{
        if (PPOverlayWindow) return;

        // 1. Cria o contexto do ImGui e do Metal ANTES de tudo
        if (!ImGui::GetCurrentContext()) {
            ImGui::CreateContext();
            ImGuiIO& io = ImGui::GetIO();
            io.Fonts->AddFontDefault(); // Fonte nativa do ImGui (sem crash)
            io.IniFilename = NULL;      // Não tenta salvar .ini no iOS
        }

        if (!g_device) {
            g_device = MTLCreateSystemDefaultDevice();
        }

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
        PPOverlayWindow.windowLevel = UIWindowLevelNormal + 1; // Abaixo do status bar e dos alertas
        PPOverlayWindow.backgroundColor = UIColor.clearColor;
        
        UIViewController *vc = [UIViewController new];
        vc.view.backgroundColor = UIColor.clearColor;
        
        PPImGuiView *imguiView = [[PPImGuiView alloc] initWithFrame:PPOverlayWindow.bounds device:g_device];
        imguiView.autoresizingMask = UIViewAutoresizingFlexibleWidth | UIViewAutoresizingFlexibleHeight;
        [vc.view addSubview:imguiView];
        
        PPOverlayWindow.rootViewController = vc;
        PPOverlayWindow.hidden = NO;
        // Sem makeKeyAndVisible. Só joga por cima.
    });
}

%ctor {
    @autoreleasepool {
        // Delay maior pra garantir que o jogo terminou de carregar a engine
        dispatch_after(dispatch_time(DISPATCH_TIME_NOW, (int64_t)(5.0 * NSEC_PER_SEC)),
            dispatch_get_main_queue(), ^{ PPShowPanel(); });
    }
}
