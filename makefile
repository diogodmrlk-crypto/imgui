TARGET = iphone:clang:latest:14.0
ARCHS = arm64 arm64e
INSTALL_TARGET_PROCESSES = SpringBoard

include $(THEOS)/makefiles/common.mk

TWEAK_NAME = PurpouPanel

# Como os arquivos estão na raiz, tira o "imgui/" da frente
PurpouPanel_FILES = Tweak.xm imgui.cpp imgui_draw.cpp imgui_tables.cpp imgui_widgets.cpp backends/imgui_impl_metal.mm

PurpouPanel_FRAMEWORKS = UIKit Foundation QuartzCore Metal MetalKit
PurpouPanel_CFLAGS = -I. -Ibackends -DIMGUI_DEFINE_MATH_OPERATORS -DIMGUI_IMPL_METAL

include $(THEOS_MAKE_PATH)/tweak.mk
