OUTPUT_NAME := msee_fwk_drv
DRIVER_UUID := efd7b0adb51747c282bf014ea7935b29
DRIVER_VENDOR_ID := 
DRIVER_NUMBER := 
DRIVER_ID := $$(($(DRIVER_VENDOR_ID)<<16|$(DRIVER_NUMBER)))
TBASE_API_LEVEL := 7
BUILD_DRIVER_LIBRARY_ONLY := YES
DRIVER_DIR ?= Locals/Code
DRLIB_DIR := $(DRIVER_DIR)/api
DRSDK_DIR_SRC ?= $(DRSDK_DIR)
TLSDK_DIR_SRC ?= $(TLSDK_DIR)
include $(DRSDK_DIR)/driver_release.mk
