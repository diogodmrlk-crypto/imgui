// layout.h
#import <UIKit/UIKit.h>

@interface ExecutorWindow : UIWindow
@property (nonatomic, strong) UIView *bgView;
@property (nonatomic, strong) UITextView *scriptInput;
@property (nonatomic, strong) UIButton *executeBtn;
@end

@implementation ExecutorWindow

- (instancetype)initWithFrame:(CGRect)frame {
    self = [super initWithFrame:frame];
    if (self) {
        self.windowLevel = UIWindowLevelAlert + 2;
        self.backgroundColor = UIColor.clearColor;
        
        self.bgView = [[UIView alloc] initWithFrame:self.bounds];
        self.bgView.backgroundColor = [UIColor blackColor];
        [self addSubview:self.bgView];
        
        UILabel *title = [[UILabel alloc] initWithFrame:CGRectMake(0, 40, self.bounds.size.width, 40)];
        title.text = @"CONSOLE EXECUTOR";
        title.textColor = [UIColor whiteColor];
        title.textAlignment = NSTextAlignmentCenter;
        title.font = [UIFont boldSystemFontOfSize:16];
        [self.bgView addSubview:title];
        
        UIButton *closeBtn = [UIButton buttonWithType:UIButtonTypeSystem];
        closeBtn.frame = CGRectMake(self.bounds.size.width - 50, 40, 40, 40);
        [closeBtn setTitle:@"X" forState:UIControlStateNormal];
        [closeBtn setTitleColor:[UIColor redColor] forState:UIControlStateNormal];
        [closeBtn addTarget:self action:@selector(closeExecutor) forControlEvents:UIControlEventTouchUpInside];
        [self.bgView addSubview:closeBtn];
        
        self.scriptInput = [[UITextView alloc] initWithFrame:CGRectMake(15, 90, self.bounds.size.width - 30, 300)];
        self.scriptInput.backgroundColor = [UIColor colorWithWhite:0.1 alpha:1];
        self.scriptInput.textColor = [UIColor greenColor];
        self.scriptInput.font = [UIFont fontWithName:@"Courier" size:12];
        self.scriptInput.layer.cornerRadius = 8;
        self.scriptInput.text = @"-- Cole seu script aqui\nprint('Hello Roblox!')";
        [self.bgView addSubview:self.scriptInput];
        
        self.executeBtn = [UIButton buttonWithType:UIButtonTypeSystem];
        self.executeBtn.frame = CGRectMake(15, 400, self.bounds.size.width - 30, 50);
        [self.executeBtn setTitle:@"EXECUTAR SCRIPT" forState:UIControlStateNormal];
        [self.executeBtn setTitleColor:[UIColor whiteColor] forState:UIControlStateNormal];
        self.executeBtn.backgroundColor = [UIColor redColor];
        self.executeBtn.layer.cornerRadius = 10;
        self.executeBtn.titleLabel.font = [UIFont boldSystemFontOfSize:14];
        [self.executeBtn addTarget:self action:@selector(runScript) forControlEvents:UIControlEventTouchUpInside];
        [self.bgView addSubview:self.executeBtn];
    }
    return self;
}

- (void)runScript {
    NSString *script = self.scriptInput.text;
    NSLog(@"[EXECUTOR] Script: %@", script);
    
    extern void ExecuteLuauScript(const char* script);
    ExecuteLuauScript([script UTF8String]);
    
    [self closeExecutor];
}

- (void)closeExecutor {
    self.hidden = YES;
}
@end

@interface PurpouPanel : UIView
@property (nonatomic, strong) UIView *panel;
@property (nonatomic, strong) UIView *contentView;
@end

@implementation PurpouPanel

- (instancetype)initWithFrame:(CGRect)frame {
    self = [super initWithFrame:frame];
    if (self) {
        self.backgroundColor = UIColor.clearColor;
        [self setupUI];
    }
    return self;
}

- (void)setupUI {
    CGFloat pW = 300.0;
    CGFloat pH = 280.0;

    self.panel = [[UIView alloc] initWithFrame:CGRectMake(0, 0, pW, pH)];
    self.panel.center = self.center;
    self.panel.backgroundColor = [UIColor colorWithRed:0.05 green:0.02 blue:0.02 alpha:0.98];
    self.panel.layer.borderWidth = 1.5;
    self.panel.layer.borderColor = [UIColor colorWithRed:0.9 green:0.1 blue:0.1 alpha:1].CGColor;
    self.panel.clipsToBounds = YES;
    [self addSubview:self.panel];

    self.contentView = [[UIView alloc] initWithFrame:CGRectMake(0, 25, pW, pH - 60)];
    [self.panel addSubview:self.contentView];

    UIButton *execBtn = [UIButton buttonWithType:UIButtonTypeSystem];
    execBtn.frame = CGRectMake(20, 20, 260, 40);
    [execBtn setTitle:@"ABRIR EXECUTOR" forState:UIControlStateNormal];
    [execBtn setTitleColor:[UIColor whiteColor] forState:UIControlStateNormal];
    execBtn.backgroundColor = [UIColor colorWithRed:0.9 green:0.1 blue:0.1 alpha:1];
    execBtn.layer.cornerRadius = 8;
    [execBtn addTarget:self action:@selector(openExecutor) forControlEvents:UIControlEventTouchUpInside];
    [self.contentView addSubview:execBtn];
}

- (void)openExecutor {
    extern ExecutorWindow *execWindow;
    if (execWindow) {
        execWindow.hidden = NO;
        [execWindow makeKeyAndVisible];
    }
}
@end
