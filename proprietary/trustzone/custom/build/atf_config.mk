include $(TRUSTZONE_CUSTOM_BUILD_PATH)/common_config.mk

##
# global setting
##
ifeq ($(TARGET_BUILD_VARIANT),eng)
  ATF_INSTALL_MODE ?= Debug
  TARGET_BUILD_VARIANT_ENG :=1
else
  ATF_INSTALL_MODE ?= Debug
  TARGET_BUILD_VARIANT_ENG :=0
endif


### CUSTOMIZTION FILES ###
PART_DEFAULT_MEMADDR := 0xFFFFFFFF


### ATF SETTING ###
ifeq ($(ATF_INSTALL_MODE),Debug)
  ATF_INSTALL_MODE_LC := debug
  ATF_DEBUG_ENABLE := 1
else
  ATF_INSTALL_MODE_LC := release
  ATF_DEBUG_ENABLE := 0
endif
ifndef MTK_ATF_VERSION
  $(error MTK_ATF_VERSION is not defined)
endif
ATF_BUILD_PATH ?= vendor/mediatek/proprietary/trustzone/atf/$(MTK_ATF_VERSION)
ATF_ADDITIONAL_DEPENDENCIES := $(abspath $(TRUSTZONE_PROJECT_MAKEFILE) $(TRUSTZONE_CUSTOM_BUILD_PATH)/common_config.mk $(TRUSTZONE_CUSTOM_BUILD_PATH)/atf_config.mk)
ATF_RAW_IMAGE_NAME := $(TRUSTZONE_IMAGE_OUTPUT_PATH)/ATF_OBJ/$(ATF_INSTALL_MODE_LC)/bl31.bin
ATF_TEMP_PADDING_FILE := $(TRUSTZONE_IMAGE_OUTPUT_PATH)/bin/$(ARCH_MTK_PLATFORM)_atf_$(ATF_INSTALL_MODE_LC)_pad.txt
ATF_DRAM_TEMP_PADDING_FILE := $(TRUSTZONE_IMAGE_OUTPUT_PATH)/bin/$(ARCH_MTK_PLATFORM)_atf_dram_$(ATF_INSTALL_MODE_LC)_pad.txt
ATF_TEMP_CFG_FILE := $(TRUSTZONE_IMAGE_OUTPUT_PATH)/bin/img_hdr_atf.cfg
ATF_SIGNED_IMAGE_NAME := $(TRUSTZONE_IMAGE_OUTPUT_PATH)/bin/$(ARCH_MTK_PLATFORM)_atf_$(ATF_INSTALL_MODE_LC)_signed.img
ATF_PADDING_IMAGE_NAME := $(TRUSTZONE_IMAGE_OUTPUT_PATH)/bin/$(ARCH_MTK_PLATFORM)_atf_$(ATF_INSTALL_MODE_LC)_pad.img
ATF_COMP_IMAGE_NAME := $(TRUSTZONE_IMAGE_OUTPUT_PATH)/bin/$(ARCH_MTK_PLATFORM)_atf.img
ATF_DRAM_IMAGE_NAME := $(TRUSTZONE_IMAGE_OUTPUT_PATH)/bin/dram_atf.img
ATF_DRAM_PADDING_IMAGE_NAME := $(TRUSTZONE_IMAGE_OUTPUT_PATH)/bin/dram_atf_pad.img
ATF_DRAM_MKIMAGE_NAME := $(TRUSTZONE_IMAGE_OUTPUT_PATH)/bin/mkimg_dram_atf.img
ATF_SRAM_IMAGE_NAME := $(TRUSTZONE_IMAGE_OUTPUT_PATH)/bin/sram_atf.img
ATF_SRAM_MKIMAGE_NAME := $(TRUSTZONE_IMAGE_OUTPUT_PATH)/bin/mkimg_sram_atf.img
ATF_DRAM_TEMP_CFG_FILE := $(TRUSTZONE_IMAGE_OUTPUT_PATH)/bin/dram_img_hdr_atf.cfg
ATF_NOTICE_SOURCE := $(wildcard $(ATF_BUILD_PATH)/license.*)
ATF_NOTICE_TARGET := $(TOPDIR)$(PRODUCT_OUT)/obj/NOTICE_FILES/src/system/lib/atf.txt
ifeq ($(ATF_CROSS_COMPILE),)
#ATF_CROSS_COMPILE := $(abspath $(TARGET_TOOLS_PREFIX))
endif
ATF_GLOBAL_MAKE_OPTION := $(if $(ATF_CROSS_COMPILE),CROSS_COMPILE=$(ATF_CROSS_COMPILE)) BUILD_BASE=$(abspath $(TRUSTZONE_IMAGE_OUTPUT_PATH)/ATF_OBJ) DEBUG=$(ATF_DEBUG_ENABLE) PLAT=$(ARCH_MTK_PLATFORM) SECURE_OS=$(TRUSTZONE_IMPL) MACH_TYPE=$(MTK_MACH_TYPE)
TEE_GLOBAL_MAKE_OPTION += ARCH_MTK_PLATFORM=$(ARCH_MTK_PLATFORM)
ATF_GLOBAL_MAKE_OPTION += ATF_ADDITIONAL_DEPENDENCIES=$(ATF_ADDITIONAL_DEPENDENCIES)
ATF_GLOBAL_MAKE_OPTION += \
  SECURE_DEINT_SUPPORT=$(SECURE_DEINT_SUPPORT) \
  MTK_STACK_PROTECTOR=$(MTK_STACK_PROTECTOR) \
  MTK_ATF_RAM_DUMP=$(MTK_ATF_RAM_DUMP) \
  MTK_ATF_LOG_BUF_SLIM=$(MTK_ATF_LOG_BUF_SLIM) \
  DEBUG_SMC_ID_LOG=$(DEBUG_SMC_ID_LOG) \
  DRAM_EXTENSION_SUPPORT=$(DRAM_EXTENSION_SUPPORT) \
  ATF_BYPASS_DRAM=$(ATF_BYPASS_DRAM) \
  MTK_UFS_SUPPORT=$(MTK_UFS_SUPPORT) \
  MTK_EMMC_SUPPORT=$(MTK_EMMC_SUPPORT) \
  MTK_FIQ_CACHE_SUPPORT=$(MTK_FIQ_CACHE_SUPPORT) \
  MTK_INDIRECT_ACCESS_SUPPORT=$(MTK_INDIRECT_ACCESS_SUPPORT) \
  MTK_ICCS_SUPPORT=$(MTK_ICCS_SUPPORT) \
  MTK_ACAO_SUPPORT=$(MTK_ACAO_SUPPORT) \
  MTK_FPGA_EARLY_PORTING=$(MTK_FPGA_EARLY_PORTING) \
  TARGET_BUILD_VARIANT_ENG=$(TARGET_BUILD_VARIANT_ENG) \
  MTK_ATF_ON_DRAM=$(MTK_ATF_ON_DRAM) \
  MCUPM_FW_USE_PARTITION=$(MCUPM_FW_USE_PARTITION) \
  MTK_ASSERTION=$(MTK_ASSERTION) \
  MTK_DRCC=$(MTK_DRCC) \
  MTK_CM_MGR=$(MTK_CM_MGR) \
  MTK_ENABLE_GENIEZONE=$(MTK_ENABLE_GENIEZONE) \
  MTK_GIC_SAVE_REG_CACHE=$(MTK_GIC_SAVE_REG_CACHE) \
  MTK_SMC_ID_MGMT=$(MTK_SMC_ID_MGMT) \
  MTK_DEBUGSYS_LOCK=$(MTK_DEBUGSYS_LOCK) \
  MTK_ENABLE_MPU_HAL_SUPPORT=$(MTK_ENABLE_MPU_HAL_SUPPORT) \
  MTK_DEVMPU_SUPPORT=$(MTK_DEVMPU_SUPPORT)

ifeq ($(HW_ASSISTED_COHERENCY),1)
  ATF_GLOBAL_MAKE_OPTION += HW_ASSISTED_COHERENCY=1
  ATF_GLOBAL_MAKE_OPTION += USE_COHERENT_MEM=0
endif

ifneq ($(TRUSTZONE_ROOT_DIR),)
  ATF_GLOBAL_MAKE_OPTION += ROOTDIR=$(TRUSTZONE_ROOT_DIR)
endif

ifneq ($(ATF_MEMBASE),)
  ATF_GLOBAL_MAKE_OPTION += ATF_MEMBASE=$(ATF_MEMBASE)
endif

ATF_MAKE_DEPENDENCIES := $(shell find $(ATF_BUILD_PATH) -type f -and -not -name ".*")
.KATI_RESTAT: $(ATF_RAW_IMAGE_NAME)
$(ATF_RAW_IMAGE_NAME): $(ATF_MAKE_DEPENDENCIES) $(ATF_ADDITIONAL_DEPENDENCIES)
	@echo ATF build: $@
	@echo  $(ATF_GLOBAL_MAKE_OPTION)
	$(hide) mkdir -p $(dir $@)
	$(PREBUILT_MAKE_PREFIX)$(MAKE) -C $(ATF_BUILD_PATH) $(ATF_GLOBAL_MAKE_OPTION) TRUSTZONE_PROJECT_MAKEFILE="$(TRUSTZONE_PROJECT_MAKEFILE_FULL_PATH)" $(PLATFORM_OPTION) all

ifeq ($(DRAM_EXTENSION_SUPPORT), yes)
$(ATF_SRAM_IMAGE_NAME): $(ATF_RAW_IMAGE_NAME) $(ATF_ADDITIONAL_DEPENDENCIES)
	@echo ATF build: $@
	$(hide) mkdir -p $(dir $@)
	@echo Split $(ATF_RAW_IMAGE_NAME) to $@
	dd if=$(ATF_RAW_IMAGE_NAME) of=$@ bs=$(ATF_SRAM_IMG_SIZE) count=1

else
$(ATF_SRAM_IMAGE_NAME): $(ATF_RAW_IMAGE_NAME) $(ATF_ADDITIONAL_DEPENDENCIES)
	@echo ATF build: $@
	$(hide) mkdir -p $(dir $@)
	cp -f $(ATF_RAW_IMAGE_NAME) $@

endif

$(ATF_TEMP_PADDING_FILE): ALIGNMENT=512
$(ATF_TEMP_PADDING_FILE): MKIMAGE_HDR_SIZE=512
$(ATF_TEMP_PADDING_FILE): RSA_SIGN_HDR_SIZE=576
$(ATF_TEMP_PADDING_FILE): $(ATF_SRAM_IMAGE_NAME) $(ATF_ADDITIONAL_DEPENDENCIES)
	@echo ATF build: $@
	$(hide) mkdir -p $(dir $@)
	$(hide) rm -f $@
	FILE_SIZE=$$(($$(wc -c < "$(ATF_SRAM_IMAGE_NAME)")+$(MKIMAGE_HDR_SIZE)+$(RSA_SIGN_HDR_SIZE)));\
	REMAINDER=$$(($${FILE_SIZE} % $(ALIGNMENT)));\
	if [ $${REMAINDER} -ne 0 ]; then dd if=/dev/zero of=$@ bs=$$(($(ALIGNMENT)-$${REMAINDER})) count=1; else touch $@; fi

$(ATF_TEMP_CFG_FILE): $(ATF_ADDITIONAL_DEPENDENCIES)
	@echo ATF build: $@
	$(hide) mkdir -p $(dir $@)
	$(hide) rm -f $@
	@echo "LOAD_MODE = 0" > $@
	@echo "NAME = atf" >> $@
	@echo "LOAD_ADDR =" $(PART_DEFAULT_MEMADDR) >> $@

$(ATF_DRAM_TEMP_CFG_FILE): $(ATF_ADDITIONAL_DEPENDENCIES)
	@echo ATF build: $@
	$(hide) mkdir -p $(dir $@)
	$(hide) rm -f $@
	@echo "LOAD_MODE = -1" > $@
	@echo "NAME = atf_dram" >> $@
	@echo "LOAD_ADDR =" $(ATF_DRAM_START_ADDR) >> $@

$(ATF_PADDING_IMAGE_NAME): $(ATF_SRAM_IMAGE_NAME) $(ATF_TEMP_PADDING_FILE) $(ATF_ADDITIONAL_DEPENDENCIES)
	@echo ATF build: $@
	$(hide) mkdir -p $(dir $@)
	$(hide) cat $(ATF_SRAM_IMAGE_NAME) $(ATF_TEMP_PADDING_FILE) > $@

$(ATF_SIGNED_IMAGE_NAME): ALIGNMENT=512
$(ATF_SIGNED_IMAGE_NAME): $(ATF_PADDING_IMAGE_NAME) $(TRUSTZONE_SIGN_TOOL) $(TRUSTZONE_IMG_PROTECT_CFG) $(ATF_ADDITIONAL_DEPENDENCIES)
	@echo ATF build: $@
	$(hide) mkdir -p $(dir $@)
	$(hide) $(TRUSTZONE_SIGN_TOOL) $(TRUSTZONE_IMG_PROTECT_CFG) $(ATF_PADDING_IMAGE_NAME) $@ $(PART_DEFAULT_MEMADDR)
	$(hide) FILE_SIZE=$$(wc -c < "$@");REMAINDER=$$(($${FILE_SIZE} % $(ALIGNMENT)));\
	if [ $${REMAINDER} -ne 0 ]; then echo "[ERROR] File $@ size $${FILE_SIZE} is not $(ALIGNMENT) bytes aligned";exit 1; fi

$(ATF_SRAM_MKIMAGE_NAME): $(ATF_SIGNED_IMAGE_NAME) $(MTK_MKIMAGE_TOOL) $(ATF_TEMP_CFG_FILE) $(ATF_ADDITIONAL_DEPENDENCIES)
	@echo ATF build: $@
	$(hide) mkdir -p $(dir $@)
	$(hide) $(MTK_MKIMAGE_TOOL) $(ATF_SIGNED_IMAGE_NAME) $(ATF_TEMP_CFG_FILE) > $@

ifeq ($(DRAM_EXTENSION_SUPPORT), yes)
$(ATF_DRAM_IMAGE_NAME): $(ATF_RAW_IMAGE_NAME) $(ATF_ADDITIONAL_DEPENDENCIES)
	@echo ATF build: $@
	$(hide) mkdir -p $(dir $@)
	@echo Split $(ATF_RAW_IMAGE_NAME) to $@
	dd if=$(ATF_RAW_IMAGE_NAME) of=$@ skip=$(ATF_SRAM_IMG_SIZE) bs=1

$(ATF_DRAM_TEMP_PADDING_FILE): ALIGNMENT=512
$(ATF_DRAM_TEMP_PADDING_FILE): $(ATF_DRAM_IMAGE_NAME) $(ATF_ADDITIONAL_DEPENDENCIES)
	@echo ATF build: $@
	$(hide) mkdir -p $(dir $@)
	$(hide) rm -f $@
	FILE_SIZE=$$(($$(wc -c < "$(ATF_DRAM_IMAGE_NAME)")));\
	REMAINDER=$$(($${FILE_SIZE} % $(ALIGNMENT)));\
	if [ $${REMAINDER} -ne 0 ]; then dd if=/dev/zero of=$@ bs=$$(($(ALIGNMENT)-$${REMAINDER})) count=1; else touch $@; fi

$(ATF_DRAM_PADDING_IMAGE_NAME): $(ATF_DRAM_IMAGE_NAME) $(ATF_DRAM_TEMP_PADDING_FILE) $(ATF_ADDITIONAL_DEPENDENCIES)
	@echo ATF build: $@
	$(hide) mkdir -p $(dir $@)
	$(hide) cat $(ATF_DRAM_IMAGE_NAME) $(ATF_DRAM_TEMP_PADDING_FILE) > $@

$(ATF_DRAM_MKIMAGE_NAME): $(ATF_DRAM_PADDING_IMAGE_NAME) $(MTK_MKIMAGE_TOOL) $(ATF_DRAM_TEMP_CFG_FILE) $(ATF_DRAM_TEMP_CFG_FILE)
	@echo ATF build: $@
	$(hide) mkdir -p $(dir $@)
	$(MTK_MKIMAGE_TOOL) $(ATF_DRAM_PADDING_IMAGE_NAME) $(ATF_DRAM_TEMP_CFG_FILE) > $@

endif


$(ATF_COMP_IMAGE_NAME): ALIGNMENT=512
ifeq ($(DRAM_EXTENSION_SUPPORT), yes)
$(ATF_COMP_IMAGE_NAME): $(ATF_SRAM_MKIMAGE_NAME) $(ATF_DRAM_MKIMAGE_NAME)
else
$(ATF_COMP_IMAGE_NAME): $(ATF_SRAM_MKIMAGE_NAME)
endif
$(ATF_COMP_IMAGE_NAME): $(ATF_ADDITIONAL_DEPENDENCIES) $(ATF_NOTICE_TARGET)
	@echo ATF build: $@
	$(hide) mkdir -p $(dir $@)
ifeq ($(DRAM_EXTENSION_SUPPORT), yes)
	cat $(ATF_SRAM_MKIMAGE_NAME) $(ATF_DRAM_MKIMAGE_NAME) > $@
else
	cat $(ATF_SRAM_MKIMAGE_NAME) > $@
endif
	$(hide) FILE_SIZE=$$(wc -c < "$@");REMAINDER=$$(($${FILE_SIZE} % $(ALIGNMENT)));\
	if [ $${REMAINDER} -ne 0 ]; then echo "[ERROR] File $@ size $${FILE_SIZE} is not $(ALIGNMENT) bytes aligned";exit 1; fi

$(ATF_NOTICE_TARGET): $(ATF_NOTICE_SOURCE)
	@echo Notice file: $< => $@
	@mkdir -p $(dir $@)
	@if [ $< ] ; then cp $< $@ ; fi