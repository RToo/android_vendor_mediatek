ifeq ($(TRUSTONIC_TEE_SUPPORT),yes)

##define mtk-install-drtl
##include $$(CLEAR_VARS)
##LOCAL_MODULE := $$(shell basename $(1))
##LOCAL_MODULE_CLASS := ETC
##LOCAL_MODULE_PATH := $(2)
##LOCAL_SRC_FILES := $(1)
##include $$(BUILD_PREBUILT)
##MTK_TRUSTONIC_INSTALLED_MODULES += $$(LOCAL_INSTALLED_MODULE)
##endef
#
##LOCAL_PATH := $(TOP)
##drbins := $(shell find $(PRODUCT_OUT)/trustzone -type f -name "*.drbin" -print)
##tlbins := $(shell find $(PRODUCT_OUT)/trustzone -type f -name "*.tlbin" -print)
#
##$(foreach item,$(drbins),$(eval ALL_DEFAULT_INSTALLED_MODULES += $(shell basename $(item))))
##$(foreach item,$(drbins),$(eval $(call mtk-install-drtl,$(item),$(TARGET_OUT_APPS)/mcRegistry)))
##$(foreach item,$(tlbins),$(eval ALL_DEFAULT_INSTALLED_MODULES += $(shell basename $(item))))
##$(foreach item,$(tlbins),$(eval $(call mtk-install-drtl,$(item),$(TARGET_OUT_APPS)/mcRegistry)))
#
##LOCAL_PATH := $(TOP)
##-include $(LOCAL_PATH)/vendor/mediatek/proprietary/protect-bsp/trustzone/trustonic/trustlets/key_install/TlcKeyInstall/Android.mk
##
#LOCAL_PATH := $(TOP)
#
#-include $(LOCAL_PATH)/vendor/mediatek/proprietary/trustzone/trustonic/secure/trustlets/video/platform/mt6763/MtkH264Vdec/MtkH264SecVdecTLC/Locals/Code/Android.mk
##
#LOCAL_PATH := $(TOP)
#
#-include $(LOCAL_PATH)/vendor/mediatek/proprietary/trustzone/trustonic/secure/trustlets/video/platform/mt6763/MtkH265Vdec/MtkH265SecVdecTLC/Locals/Code/Android.mk
#
#LOCAL_PATH := $(TOP)
#
#-include $(LOCAL_PATH)/vendor/mediatek/proprietary/trustzone/trustonic/secure/trustlets/video/platform/mt6763/MtkVP9Vdec/MtkVP9SecVdecTLC/Locals/Code/Android.mk
#
#LOCAL_PATH := $(TOP)
#
#-include $(LOCAL_PATH)/vendor/mediatek/proprietary/trustzone/trustonic/secure/trustlets/video/platform/mt6763/MtkH264Venc/MtkH264SecVencTLC/Locals/Code/Android.mk
##
##ifneq ($(wildcard  $(TOP)/vendor/trustonic/platform/$(shell echo $(MTK_PLATFORM) | tr A-Z a-z)/export/sectrace/libsectrace.so),)
##  ALL_DEFAULT_INSTALLED_MODULES += libsectrace
##  include $(CLEAR_VARS)
##  LOCAL_MODULE := libsectrace
##  LOCAL_MODULE_SUFFIX := .so
##  LOCAL_MODULE_CLASS := SHARED_LIBRARIES
##  LOCAL_MODULE_PATH := $(TARGET_OUT)/lib
##  LOCAL_SRC_FILES := $(TOP)/vendor/trustonic/platform/$(shell echo $(MTK_PLATFORM) | tr A-Z a-z)/export/sectrace/libsectrace.so
##  include $(BUILD_PREBUILT)
##endif
##
################################################################
##Widevine DRM
#LOCAL_PATH := $(TOP)
#-include $(LOCAL_PATH)/vendor/mediatek/proprietary/trustzone/trustonic/secure/trustlets/widevine/common/modular_drm/TlcWidevineModularDrm/Locals/Code/Android.mk
#
##LOCAL_PATH := $(TOP)
##-include $(LOCAL_PATH)/vendor/mediatek/proprietary/trustzone/trustonic/secure/trustlets/widevine/common/classic_drm/TlcWidevineClassicDrm/Locals/Code/Android.mk
################################################################
##LOCAL_PATH := $(TOP)
##-include $(LOCAL_PATH)/vendor/trustonic/platform/common/trustlets/rpmb/Tlcrpmb/Locals/Code/Android.mk
#
ifeq ($(strip $(MTK_FINGERPRINT_SUPPORT)),yes)
ifeq ($(MTK_FINGERPRINT_SELECT), $(filter $(MTK_FINGERPRINT_SELECT), GF316M GF318M GF3118M GF518M GF5118M GF516M GF816M GF3208 GF3206 GF3266 GF3288 GF5206 GF5216 GF5208))
LOCAL_PATH := $(TOP)
-include $(LOCAL_PATH)/vendor/mediatek/proprietary/trustzone/trustonic/source/trustlets/fingerprint/platform/$(ARCH_MTK_PLATFORM)/goodix_fp/gf_ca/Android.mk
endif
endif
##
##LOCAL_PATH := $(TOP)

endif