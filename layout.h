// layout.h
#import <UIKit/UIKit.h>
#import <sys/utsname.h>

// Variáveis globais que os hooks vão ler
bool g_aimbotEnabled = false;
bool g_espEnabled = false;
bool g_silentAimEnabled = false;
bool g_fovCircleEnabled = false;
bool g_streamMode = false; 
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
        self.currentTab = 1; // 1 = AIM, 2 = ESP, 3 = MISC
        self.autoresizingMask = UIViewAutoresizingFlexibleWidth | UIViewAutoresizingFlexibleHeight;
        [self setupUI];
    }
    return self;
}

// Mantém o painel centralizado quando gira a tela
- (void)layoutSubviews {
    [super layoutSubviews];
    self.panel.center = CGPointMake(self.bounds.size.width / 2, self.bounds.size.height / 2);
}

- (void)setupUI {
    CGFloat pW = 300.0;
    CGFloat pH = 280.0;

    // 1. PAINEL
    self.panel = [[UIView alloc] initWithFrame:CGRectMake(0, 0, pW, pH)];
    self.panel.center = self.center;
    self.panel.backgroundColor = [UIColor colorWithRed:0.05 green:0.02 blue:0.02 alpha:0.98];
    self.panel.layer.borderWidth = 1.5;
    self.panel.layer.borderColor = [UIColor colorWithRed:0.9 green:0.1 blue:0.1 alpha:1].CGColor;
    self.panel.clipsToBounds = YES;
    [self addSubview:self.panel];

    UIPanGestureRecognizer *pan = [[UIPanGestureRecognizer alloc] initWithTarget:self action:@selector(handlePan:)];
    [self.panel addGestureRecognizer:pan];

    // 2. CABEÇALHO
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

    // 3. ÁREA DE CONTEÚDO
    self.contentView = [[UIView alloc] initWithFrame:CGRectMake(0, 25, pW, pH - 60)];
    [self.panel addSubview:self.contentView];

    // 4. BARRA DE ABAS
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

    [self switchTab:self.tabButtons[0]]; // Inicia na AIM
}

- (void)switchTab:(UIButton *)sender {
    self.currentTab = (int)sender.tag;
    
    // Atualiza a cor dos botões (Ativo fica vermelho)
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

        if (i == 0) [self createSwitchAtY:y isOn:g_silentAimEnabled action:@selector(toggleSilentAim:)];
        else if (i == 1) [self createSwitchAtY:y isOn:g_aimbotEnabled action:@selector(toggleAimbot:)];
        else if (i == 2) [self createSwitchAtY:y isOn:g_fovCircleEnabled action:@selector(toggleFovCircle:)];
    }

    UILabel *targetLbl = [[UILabel alloc] initWithFrame:CGRectMake(20, 135, 200, 20)];
    targetLbl.text = @"Target: MIXED";
    targetLbl.textColor = [UIColor colorWithRed:0.9 green:0.1 blue:0.1 alpha:1];
    targetLbl.font = [UIFont systemFontOfSize:10];
    [self.contentView addSubview:targetLbl];

    UILabel *headshotLbl = [[UILabel alloc] initWithFrame:CGRectMake(20, 155, 200, 20)];
    headshotLbl.text = @"Headshot rate: 75%";
    headshotLbl.textColor = [UIColor colorWithRed:0.9 green:0.1 blue:0.1 alpha:1];
    headshotLbl.font = [UIFont systemFontOfSize:10];
    [self.contentView addSubview:headshotLbl];

    UILabel *sliderLbl = [[UILabel alloc] initWithFrame:CGRectMake(20, 185, 180, 20)];
    sliderLbl.text = @"FOV radius: 196";
    sliderLbl.textColor = [UIColor colorWithRed:0.9 green:0.1 blue:0.1 alpha:1];
    sliderLbl.font = [UIFont systemFontOfSize:11 weight:UIFontWeightMedium];
    [self.contentView addSubview:sliderLbl];

    UISlider *slider = [[UISlider alloc] initWithFrame:CGRectMake(20, 210, 260, 20)];
    slider.minimumValue = 0;
    slider.maximumValue = 360;
    slider.value = g_fovRadius;
    slider.minimumTrackTintColor = [UIColor colorWithRed:0.9 green:0.1 blue:0.5 alpha:1];
    slider.maximumTrackTintColor = [UIColor darkGrayColor];
    slider.thumbTintColor = UIColor.whiteColor;
    [slider addTarget:self action:@selector(fovChanged:) forControlEvents:UIControlEventValueChanged];
    [self.contentView addSubview:slider];
}

- (void)buildEspTab {
    for (UIView *v in self.contentView.subviews) [v removeFromSuperview];
    NSArray *options = @[@"ESP MASTER", @"PLAYER BOX", @"TOP TRACER", @"HEALTH BAR", @"PLAYER NAME", @"DISTANCE", @"SKELETON"];

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
    
    struct utsname systemInfo;
    uname(&systemInfo);
    NSString *deviceModel = [NSString stringWithCString:systemInfo.machine encoding:NSUTF8StringEncoding];
    NSString *iosVersion = [[UIDevice currentDevice] systemVersion];
    
    UILabel *deviceLbl = [[UILabel alloc] initWithFrame:CGRectMake(20, 20, 260, 30)];
    deviceLbl.text = [NSString stringWithFormat:@"Device: %@ iOS %@", deviceModel, iosVersion];
    deviceLbl.textColor = [UIColor colorWithRed:0.9 green:0.1 blue:0.1 alpha:1];
    deviceLbl.font = [UIFont boldSystemFontOfSize:11];
    [self.contentView addSubview:deviceLbl];

    UILabel *streamLbl = [[UILabel alloc] initWithFrame:CGRectMake(20, 70, 180, 30)];
    streamLbl.text = @"MODE STREAM (GRAVAÇÃO)";
    streamLbl.textColor = [UIColor colorWithRed:0.9 green:0.1 blue:0.1 alpha:1];
    streamLbl.font = [UIFont systemFontOfSize:11 weight:UIFontWeightMedium];
    [self.contentView addSubview:streamLbl];

    [self createSwitchAtY:70 isOn:g_streamMode action:@selector(toggleStreamMode:)];
    
    UILabel *infoLbl = [[UILabel alloc] initWithFrame:CGRectMake(20, 120, 260, 60)];
    infoLbl.text = @"Modo Stream esconde o painel de gravações de tela, mantendo as funções ativas.";
    infoLbl.textColor = [UIColor grayColor];
    infoLbl.font = [UIFont systemFontOfSize:10];
    infoLbl.numberOfLines = 0;
    [self.contentView addSubview:infoLbl];
}

// AÇÕES
- (void)toggleSilentAim:(UISwitch *)sender { g_silentAimEnabled = sender.isOn; }
- (void)toggleAimbot:(UISwitch *)sender { g_aimbotEnabled = sender.isOn; }
- (void)toggleFovCircle:(UISwitch *)sender { g_fovCircleEnabled = sender.isOn; }
- (void)toggleEsp:(UISwitch *)sender { g_espEnabled = sender.isOn; }
- (void)fovChanged:(UISlider *)sender { g_fovRadius = sender.value; }

- (void)toggleStreamMode:(UISwitch *)sender {
    g_streamMode = sender.isOn;
    UIWindow *win = self.window;
    if (g_streamMode) {
        [win setValue:@YES forKey:@"_secure"]; // Hack pra sumir da gravação
    } else {
        [win setValue:@NO forKey:@"_secure"];
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

// ==========================================
// JANELA CUSTOMIZADA (Toques Passam pro Jogo)
// ==========================================
@interface PurpouWindow : UIWindow
@property (nonatomic, weak) UIView *panelView;
@end

@implementation PurpouWindow
- (UIView *)hitTest:(CGPoint)point withEvent:(UIEvent *)event {
    UIView *hitView = [super hitTest:point withEvent:event];
    if (self.panelView && CGRectContainsPoint(self.panelView.frame, point)) {
        return hitView; // Se tocar no painel, repassa o toque pra ele
    }
    return nil; // Se tocar fora, joga o toque pro jogo
}

// GATILHO DE 3 DEDOS (4 Toques)
+ (void)load {
    dispatch_after(dispatch_time(DISPATCH_TIME_NOW, (int64_t)(2.0 * NSEC_PER_SEC)), dispatch_get_main_queue(), ^{
        UITapGestureRecognizer *tap = [[UITapGestureRecognizer alloc] initWithTarget:self action:@selector(handleTripleTap)];
        tap.numberOfTouchesRequired = 3;
        tap.numberOfTapsRequired = 4;
        [[UIApplication sharedApplication].keyWindow addGestureRecognizer:tap];
    });
}

+ (void)handleTripleTap {
    UIWindow *win = nil;
    for (UIWindow *w in [UIApplication sharedApplication].windows) {
        if ([w isKindOfClass:[PurpouWindow class]]) { win = w; break; }
    }
    if (win) win.hidden = !win.hidden;
}
@end
