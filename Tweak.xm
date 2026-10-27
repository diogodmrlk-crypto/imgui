#import <UIKit/UIKit.h>
#import <Metal/Metal.h>
#import <MetalKit/MetalKit.h>
#import "imgui.h"
#import "backends/imgui_impl_metal.h"
#import "layout.h"

static UIWindow *PPOverlayWindow;

@interface PPImGuiView : MTKView
@end

@implementation PPImGuiView {
    id<MTLCommandQueue> _commandQueue;
}

- (instancetype)initWithFrame:(CGRect)frame {
    self = [super initWithFrame:frame];
    if (self) {
        id<MTLDevice> device = MTLCreateSystemDefaultDevice();
        self.device = device;
        self.colorPixelFormat = MTLPixelFormatBGRA8Unorm;
        self.clearColor = MTLClearColorMake(0, 0, 0, 0); // Fundo transparente
        self.opaque = NO;
        self.paused = NO;
        
        _commandQueue = [device newCommandQueue];
        
        // Inicializa o ImGui
        ImGui::CreateContext();
        ImGuiIO& io = ImGui::GetIO();
        
        // CRUCIAL: Carrega a fonte padrão do ImGui pro iOS (resolve o crash)
        io.Fonts->AddFontDefault();
        
        ImGui_ImplMetal_Init(device);
        
        // Loop de renderização a 60fps
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
        
        // CHAMA A TUA FUNÇÃO DO LAYOUT.H
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
        
        // Nível de alerta para ficar em cima do jogo, mas abaixo da status bar
        PPOverlayWindow.windowLevel = UIWindowLevelAlert + 1;
        PPOverlayWindow.backgroundColor = UIColor.clearColor;
        
        UIViewController *vc = [UIViewController new];
        vc.view.backgroundColor = UIColor.clearColor;
        
        PPImGuiView *imguiView = [[PPImGuiView alloc] initWithFrame:PPOverlayWindow.bounds];
        imguiView.autoresizingMask = UIViewAutoresizingFlexibleWidth | UIViewAutoresizingFlexibleHeight;
        [vc.view addSubview:imguiView];
        
        PPOverlayWindow.rootViewController = vc;
        PPOverlayWindow.hidden = NO;
        
        // NÃO USA makeKeyAndVisible (isso causava o travamento no jogo)
        // Apenas deixa a janela visível por cima
    });
}

%ctor {
    @autoreleasepool {
        dispatch_after(dispatch_time(DISPATCH_TIME_NOW, (int64_t)(2.0 * NSEC_PER_SEC)),
            dispatch_get_main_queue(), ^{ PPShowPanel(); });
    }
}
