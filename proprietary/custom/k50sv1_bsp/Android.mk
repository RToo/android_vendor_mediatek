LOCAL_PATH:= $(call my-dir)
ifeq ($(MTK_PROJECT), k50sv1_bsp)
include $(call all-makefiles-under,$(LOCAL_PATH))
endif