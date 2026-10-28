// layout.h
#import <UIKit/UIKit.h>
#import <sys/utsname.h>

bool g_aimbotEnabled = false;
bool g_espEnabled = false;
bool g_executorEnabled = false; // Toggle do Executor
float g_fovRadius = 196.0f;

@interface PurpouPanel : UIView
@property (nonatomic, strong) UIView *panel;
@property (nonatomic, strong) UIView *header;
@property (nonatomic, strong) UIView *contentView;
@property (nonatomic, strong) UIView *tabBar;
@property (nonatomic, assign) int currentTab;
@property (nonatomic, strong) NSArray *tabButtons;
@end

@implementation PurpouPanel

- (instancetype)initWithFrame:(CGRect)frame {
    self = [super initWithFrame:frame];
    if (self) {
        self.backgroundColor = UIColor.clearColor;
        self.currentTab = 1;
        self.autoresizingMask = UIViewAutoresizingFlexibleWidth | UIViewAutoresizingFlexibleHeight;
        [self setupUI];
    }
    return self;
}

- (void)layoutSubviews {
    [super layoutSubviews];
    self.panel.center = CGPointMake(self.bounds.size.width / 2, self.bounds.size.height / 2);
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

    UIPanGestureRecognizer *pan = [[UIPanGestureRecognizer alloc] initWithTarget:self action:@selector(handlePan:)];
    [self.panel addGestureRecognizer:pan];

    self.header = [[UIView alloc] initWithFrame:CGRectMake(0, 0, pW, 25)];
    self.header.backgroundColor = [UIColor colorWithRed:0.1 green:0.02 blue:0.02 alpha:1];
    [self.panel addSubview:self.header];

    UILabel *title = [[UILabel alloc] initWithFrame:CGRectMake(0, 0, pW - 30, 25)];
    title.text = @"PURPOU DEV";
    title.textColor = [UIColor colorWithRed:0.9 green:0.1 blue:0.1 alpha:1];
    title.font = [UIFont boldSystemFontOfSize:11];
    title.textAlignment = NSTextAlignmentCenter;
    [self.header addSubview:title];

    UIButton *closeBtn = [UIButton buttonWithType:UIButtonTypeSystem];
    closeBtn.frame = CGRectMake(pW - 30, 0, 30, 25);
    [closeBtn setTitle:@"X" forState:UIControlStateNormal];
    [closeBtn setTitleColor:UIColor.whiteColor forState:UIControlStateNormal];
    [closeBtn addTarget:self action:@selector(closePanel) forControlEvents:UIControlEventTouchUpInside];
    [self.header addSubview:closeBtn];

    self.contentView = [[UIView alloc] initWithFrame:CGRectMake(0, 25, pW, pH - 60)];
    [self.panel addSubview:self.contentView];

    self.tabBar = [[UIView alloc] initWithFrame:CGRectMake(0, pH - 35, pW, 35)];
    self.tabBar.backgroundColor = [UIColor colorWithRed:0.1 green:0.02 blue:0.02 alpha:1];
    [self.panel addSubview:self.tabBar];

    NSArray *tabs = @[@"AIM", @"ESP", @"MISC"];
    NSMutableArray *btns = [NSMutableArray array];
    for (int i = 0; i < 3; i++) {
        UIButton *btn = [UIButton buttonWithType:UIButtonTypeSystem];
        btn.frame = CGRectMake((pW / 3) * i, 0, pW / 3, 35);
        [btn setTitle:tabs[i] forState:UIControlStateNormal];
        [btn setTitleColor:UIColor.whiteColor forState:UIControlStateNormal];
        btn.titleLabel.font = [UIFont boldSystemFontOfSize:11];
        btn.tag = i + 1;
        [btn addTarget:self action:@selector(switchTab:) forControlEvents:UIControlEventTouchUpInside];
        [self.tabBar addSubview:btn];
        [btns addObject:btn];
    }
    self.tabButtons = btns;

    [self switchTab:self.tabButtons[0]];
}

- (void)switchTab:(UIButton *)sender {
    self.currentTab = (int)sender.tag;
    for (UIButton *btn in self.tabButtons) {
        btn.backgroundColor = (btn.tag == self.currentTab) ? [UIColor colorWithRed:0.9 green:0.1 blue:0.1 alpha:1] : UIColor.clearColor;
    }

    if (self.currentTab == 1) [self buildAimTab];
    else if (self.currentTab == 2) [self buildEspTab];
    else [self buildMiscTab];
}

- (UISwitch *)createSwitchAtY:(CGFloat)y isOn:(BOOL)isOn action:(SEL)action {
    UISwitch *toggle = [[UISwitch alloc] initWithFrame:CGRectMake(220, y, 51, 31)];
    toggle.transform = CGAffineTransformMakeScale(0.7, 0.7);
    toggle.onTintColor = [UIColor colorWithRed:0.9 green:0.1 blue:0.1 alpha:1];
    toggle.thumbTintColor = UIColor.whiteColor;
    toggle.on = isOn;
    [toggle addTarget:self action:action forControlEvents:UIControlEventValueChanged];
    [self.contentView addSubview:toggle];
    return toggle;
}

- (void)buildAimTab {
    for (UIView *v in self.contentView.subviews) [v removeFromSuperview];
    NSArray *options = @[@"SILENT AIM", @"AIM SYSTEM", @"FOV CIRCLE"];

    for (int i = 0; i < options.count; i++) {
        CGFloat y = 15 + (i * 40);
        UILabel *lbl = [[UILabel alloc] initWithFrame:CGRectMake(20, y, 180, 30)];
        lbl.text = options[i];
        lbl.textColor = [UIColor colorWithRed:0.9 green:0.1 blue:0.1 alpha:1];
        lbl.font = [UIFont systemFontOfSize:11 weight:UIFontWeightMedium];
        [self.contentView addSubview:lbl];
        [self createSwitchAtY:y isOn:g_aimbotEnabled action:@selector(toggleAimbot:)];
    }
}

- (void)buildEspTab {
    for (UIView *v in self.contentView.subviews) [v removeFromSuperview];
    NSArray *options = @[@"ESP MASTER", @"PLAYER BOX", @"TOP TRACER"];
    for (int i = 0; i < options.count; i++) {
        CGFloat y = 10 + (i * 30);
        UILabel *lbl = [[UILabel alloc] initWithFrame:CGRectMake(20, y, 180, 20)];
        lbl.text = options[i];
        lbl.textColor = [UIColor colorWithRed:0.9 green:0.1 blue:0.1 alpha:1];
        lbl.font = [UIFont systemFontOfSize:10 weight:UIFontWeightMedium];
        [self.contentView addSubview:lbl];
        [self createSwitchAtY:y-5 isOn:g_espEnabled action:@selector(toggleEsp:)];
    }
}

- (void)buildMiscTab {
    for (UIView *v in self.contentView.subviews) [v removeFromSuperview];
    
    UILabel *execLbl = [[UILabel alloc] initWithFrame:CGRectMake(20, 20, 180, 30)];
    execLbl.text = @"EXECUTOR";
    execLbl.textColor = [UIColor colorWithRed:0.9 green:0.1 blue:0.1 alpha:1];
    execLbl.font = [UIFont boldSystemFontOfSize:11];
    [self.contentView addSubview:execLbl];
    
    [self createSwitchAtY:20 isOn:g_executorEnabled action:@selector(toggleExecutor:)];
}

- (void)toggleAimbot:(UISwitch *)sender { g_aimbotEnabled = sender.isOn; }
- (void)toggleEsp:(UISwitch *)sender { g_espEnabled = sender.isOn; }
- (void)toggleExecutor:(UISwitch *)sender { 
    g_executorEnabled = sender.isOn; 
    
    if (g_executorEnabled) {
        // Se ativou, mostra a tela do executor
        UIAlertController *alert = [UIAlertController alertControllerWithTitle:@"Executor" message:@"Digite seu script:" preferredStyle:UIAlertControllerStyleAlert];
        [alert addTextFieldWithConfigurationHandler:^(UITextField *textField) {
            textField.placeholder = @"loadstring...";
        }];
        
        UIAlertAction *exec = [UIAlertAction actionWithTitle:@"Executar" style:UIAlertActionStyleDefault handler:^(UIAlertAction *action) {
            NSString *script = alert.textFields.firstObject.text;
            NSLog(@"[EXECUTOR] Script: %@", script);
            // A chamada pra injetar no jogo viria aqui
        }];
        
        [alert addAction:exec];
        [alert addAction:[UIAlertAction actionWithTitle:@"Cancelar" style:UIAlertActionStyleCancel handler:nil]];
        [[UIApplication sharedApplication].keyWindow.rootViewController presentViewController:alert animated:YES completion:nil];
    }
}

- (void)handlePan:(UIPanGestureRecognizer *)gesture {
    CGPoint translation = [gesture translationInView:self];
    self.panel.center = CGPointMake(self.panel.center.x + translation.x, self.panel.center.y + translation.y);
    [gesture setTranslation:CGPointZero inView:self];
}

- (void)closePanel {
    self.hidden = YES;
}
@end
