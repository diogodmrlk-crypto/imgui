#import <UIKit/UIKit.h>
#import <Metal/Metal.h>
#import <MetalKit/MetalKit.h>
#import "imgui.h"
#import "backends/imgui_impl_metal.h"
#import "layout.h"

static UIWindow *PPOverlayWindow;
static BOOL g_imgui_initialized = NO;
static id<MTLDevice> g_device = nil;

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
        
        menu(); // Chama o layout
        
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

        if (!g_imgui_initialized) {
            ImGui::CreateContext();
            ImGuiIO& io = ImGui::GetIO();
            io.Fonts->AddFontDefault();
            io.IniFilename = NULL;
            g_imgui_initialized = YES;
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
        PPOverlayWindow.windowLevel = UIWindowLevelAlert + 1;
        PPOverlayWindow.backgroundColor = UIColor.clearColor;
        
        UIViewController *vc = [UIViewController new];
        vc.view.backgroundColor = UIColor.clearColor;
        
        PPImGuiView *imguiView = [[PPImGuiView alloc] initWithFrame:PPOverlayWindow.bounds device:g_device];
        imguiView.autoresizingMask = UIViewAutoresizingFlexibleWidth | UIViewAutoresizingFlexibleHeight;
        [vc.view addSubview:imguiView];
        
        PPOverlayWindow.rootViewController = vc;
        PPOverlayWindow.hidden = NO;
    });
}

%ctor {
    @autoreleasepool {
        dispatch_after(dispatch_time(DISPATCH_TIME_NOW, (int64_t)(3.0 * NSEC_PER_SEC)),
            dispatch_get_main_queue(), ^{ 
                PPShowPanel();
            });
    }
}
