TARGET = iphone:clang:latest:14.0
ARCHS = arm64 arm64e
INSTALL_TARGET_PROCESSES = SpringBoard

include $(THEOS)/makefiles/common.mk

TWEAK_NAME = PurpouPanel

# Caminhos exatos dos arquivos que estão no teu repositório
PurpouPanel_FILES = Tweak.xm imgui/imgui.cpp imgui/imgui_draw.cpp imgui/imgui_tables.cpp imgui/imgui_widgets.cpp imgui/backends/imgui_impl_metal.mm

# Frameworks e flags pro Metal e ImGui funcionarem no iOS
PurpouPanel_FRAMEWORKS = UIKit Foundation QuartzCore Metal MetalKit
PurpouPanel_CFLAGS = -Iimgui -Iimgui/backends -I. -DIMGUI_DEFINE_MATH_OPERATORS -DIMGUI_IMPL_METAL

include $(THEOS_MAKE_PATH)/tweak.mk
