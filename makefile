TARGET = iphone:clang:latest:14.0
ARCHS = arm64 arm64e
INSTALL_TARGET_PROCESSES = SpringBoard

include $(THEOS)/makefiles/common.mk

TWEAK_NAME = PurpouPanel

PurpouPanel_FILES = Tweak.xm imgui.cpp imgui_draw.cpp imgui_tables.cpp imgui_widgets.cpp backends/imgui_impl_metal.mm

PurpouPanel_FRAMEWORKS = UIKit Foundation QuartzCore Metal MetalKit

# Adicionado -Wno-error e -fno-objc-arc (mas vou forçar o ARC no arquivo do metal na próxima linha)
PurpouPanel_CFLAGS = -I. -Ibackends -DIMGUI_DEFINE_MATH_OPERATORS -DIMGUI_IMPL_METAL -Wno-error -fno-objc-arc

# AQUI ESTÁ O PULO DO GATO: Forçar ARC só no arquivo do Metal
backends/imgui_impl_metal.mm_CFLAGS = -fobjc-arc

include $(THEOS_MAKE_PATH)/tweak.mk
