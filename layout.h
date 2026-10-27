// layout.h
#import <UIKit/UIKit.h>

// ==========================================
// VARIÁVEIS GLOBAIS DO CHEAT
// ==========================================
bool g_aimbotEnabled = false;
bool g_espEnabled = false;
bool g_silentAimEnabled = false;
bool g_fovCircleEnabled = false;
float g_fovRadius = 196.0f;

// ==========================================
// INTERFACE DO PAINEL
// ==========================================
@interface PurpouPanel : UIView
@property (nonatomic, strong) UIView *panel;
@property (nonatomic, strong) UIView *header;
@property (nonatomic, strong) UIView *contentView;
@property (nonatomic, strong) UIView *tabBar;
@property (nonatomic, assign) int currentTab;
@end

@implementation PurpouPanel

- (instancetype)initWithFrame:(CGRect)frame {
    self = [super initWithFrame:frame];
    if (self) {
        self.backgroundColor = UIColor.clearColor;
        self.currentTab = 1; // 1 = AIM, 2 = ESP
        [self setupUI];
    }
    return self;
}

- (void)setupUI {
    CGFloat pW = 300.0;
    CGFloat pH = 260.0;

    // 1. PAINEL (Fundo Preto, Borda Vermelha)
    self.panel = [[UIView alloc] initWithFrame:CGRectMake(0, 0, pW, pH)];
    self.panel.center = self.center;
    self.panel.backgroundColor = [UIColor colorWithRed:0.05 green:0.02 blue:0.02 alpha:0.98];
    self.panel.layer.borderWidth = 1.5;
    self.panel.layer.borderColor = [UIColor colorWithRed:0.9 green:0.1 blue:0.1 alpha:1].CGColor;
    self.panel.clipsToBounds = YES;
    [self addSubview:self.panel];

    UIPanGestureRecognizer *pan = [[UIPanGestureRecognizer alloc] initWithTarget:self action:@selector(handlePan:)];
    [self.panel addGestureRecognizer:pan];

    // 2. CABEÇALHO (PURPOU DEV)
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

    // 4. BARRA DE ABAS (Embaixo)
    self.tabBar = [[UIView alloc] initWithFrame:CGRectMake(0, pH - 35, pW, 35)];
    self.tabBar.backgroundColor = [UIColor colorWithRed:0.1 green:0.02 blue:0.02 alpha:1];
    [self.panel addSubview:self.tabBar];

    NSArray *tabs = @[@"AIM", @"ESP"]; // Ordem pra ficar igual à foto
    for (int i = 0; i < 2; i++) {
        UIButton *btn = [UIButton buttonWithType:UIButtonTypeSystem];
        btn.frame = CGRectMake((pW / 2) * i, 0, pW / 2, 35);
        [btn setTitle:tabs[i] forState:UIControlStateNormal];
        [btn setTitleColor:UIColor.whiteColor forState:UIControlStateNormal];
        btn.titleLabel.font = [UIFont boldSystemFontOfSize:11];
        btn.tag = i + 1;
        [btn addTarget:self action:@selector(switchTab:) forControlEvents:UIControlEventTouchUpInside];
        [self.tabBar addSubview:btn];
    }

    [self buildAimTab]; // Começa na aba AIM
}

- (void)buildAimTab {
    for (UIView *v in self.contentView.subviews) [v removeFromSuperview];
    
    // Textos e Toggles
    NSArray *options = @[@"SILENT AIM", @"AIM SYSTEM", @"FOV CIRCLE"];
    for (int i = 0; i < options.count; i++) {
        CGFloat y = 15 + (i * 40);
        UILabel *lbl = [[UILabel alloc] initWithFrame:CGRectMake(20, y, 180, 30)];
        lbl.text = options[i];
        lbl.textColor = [UIColor colorWithRed:0.9 green:0.1 blue:0.1 alpha:1];
        lbl.font = [UIFont systemFontOfSize:11 weight:UIFontWeightMedium];
        [self.contentView addSubview:lbl];

        UISwitch *toggle = [[UISwitch alloc] initWithFrame:CGRectMake(200, y, 51, 31)];
        toggle.transform = CGAffineTransformMakeScale(0.7, 0.7);
        toggle.onTintColor = [UIColor colorWithRed:0.9 green:0.1 blue:0.1 alpha:1];
        toggle.thumbTintColor = UIColor.whiteColor;
        
        if (i == 0) { toggle.on = g_silentAimEnabled; [toggle addTarget:self action:@selector(toggleSilentAim:) forControlEvents:UIControlEventValueChanged]; }
        else if (i == 1) { toggle.on = g_aimbotEnabled; [toggle addTarget:self action:@selector(toggleAimbot:) forControlEvents:UIControlEventValueChanged]; }
        else if (i == 2) { toggle.on = g_fovCircleEnabled; [toggle addTarget:self action:@selector(toggleFovCircle:) forControlEvents:UIControlEventValueChanged]; }
        
        [self.contentView addSubview:toggle];
    }

    // Textos Falsos (Target MIXED, Headshot rate)
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

    // Slider FOV
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
        CGFloat y = 10 + (i * 28); // Mais compacto pra caber tudo
        UILabel *lbl = [[UILabel alloc] initWithFrame:CGRectMake(20, y, 180, 20)];
        lbl.text = options[i];
        lbl.textColor = [UIColor colorWithRed:0.9 green:0.1 blue:0.1 alpha:1];
        lbl.font = [UIFont systemFontOfSize:10 weight:UIFontWeightMedium];
        [self.contentView addSubview:lbl];

        UISwitch *toggle = [[UISwitch alloc] initWithFrame:CGRectMake(200, y - 5, 51, 31)];
        toggle.transform = CGAffineTransformMakeScale(0.6, 0.6);
        toggle.onTintColor = [UIColor colorWithRed:0.9 green:0.1 blue:0.1 alpha:1];
        toggle.thumbTintColor = UIColor.whiteColor;
        toggle.on = g_espEnabled;
        [toggle addTarget:self action:@selector(toggleEsp:) forControlEvents:UIControlEventValueChanged];
        [self.contentView addSubview:toggle];
    }
}

- (void)switchTab:(UIButton *)sender {
    self.currentTab = (int)sender.tag;
    if (self.currentTab == 1) [self buildAimTab];
    else [self buildEspTab];
}

// AÇÕES (AQUI O CHEAT É ATIVADO)
- (void)toggleSilentAim:(UISwitch *)sender { g_silentAimEnabled = sender.isOn; }
- (void)toggleAimbot:(UISwitch *)sender { g_aimbotEnabled = sender.isOn; }
- (void)toggleFovCircle:(UISwitch *)sender { g_fovCircleEnabled = sender.isOn; }
- (void)toggleEsp:(UISwitch *)sender { g_espEnabled = sender.isOn; }
- (void)fovChanged:(UISlider *)sender { g_fovRadius = sender.value; }

- (void)handlePan:(UIPanGestureRecognizer *)gesture {
    CGPoint translation = [gesture translationInView:self];
    self.panel.center = CGPointMake(self.panel.center.x + translation.x, self.panel.center.y + translation.y);
    [gesture setTranslation:CGPointZero inView:self];
}

- (void)closePanel {
    self.hidden = YES;
}
@end
