TARGET = iphone:clang:latest:14.0
ARCHS = arm64 arm64e
INSTALL_TARGET_PROCESSES = SpringBoard

include $(THEOS)/makefiles/common.mk

TWEAK_NAME = PurpouPanel
PurpouPanel_FILES = Tweak.xm
PurpouPanel_FRAMEWORKS = UIKit Foundation QuartzCore
# Adicionado -Wno-deprecated-declarations para ignorar o aviso do keyWindow
PurpouPanel_CFLAGS = -Wno-deprecated-declarations

include $(THEOS_MAKE_PATH)/tweak.mk
