# Copyright Statement:
#
# This software/firmware and related documentation ("MediaTek Software") are
# protected under relevant copyright laws. The information contained herein
# is confidential and proprietary to MediaTek Inc. and/or its licensors.
# Without the prior written permission of MediaTek inc. and/or its licensors,
# any reproduction, modification, use or disclosure of MediaTek Software,
# and information contained herein, in whole or in part, shall be strictly prohibited.

# MediaTek Inc. (C) 2010. All rights reserved.
#
# BY OPENING THIS FILE, RECEIVER HEREBY UNEQUIVOCALLY ACKNOWLEDGES AND AGREES
# THAT THE SOFTWARE/FIRMWARE AND ITS DOCUMENTATIONS ("MEDIATEK SOFTWARE")
# RECEIVED FROM MEDIATEK AND/OR ITS REPRESENTATIVES ARE PROVIDED TO RECEIVER ON
# AN "AS-IS" BASIS ONLY. MEDIATEK EXPRESSLY DISCLAIMS ANY AND ALL WARRANTIES,
# EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE IMPLIED WARRANTIES OF
# MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE OR NONINFRINGEMENT.
# NEITHER DOES MEDIATEK PROVIDE ANY WARRANTY WHATSOEVER WITH RESPECT TO THE
# SOFTWARE OF ANY THIRD PARTY WHICH MAY BE USED BY, INCORPORATED IN, OR
# SUPPLIED WITH THE MEDIATEK SOFTWARE, AND RECEIVER AGREES TO LOOK ONLY TO SUCH
# THIRD PARTY FOR ANY WARRANTY CLAIM RELATING THERETO. RECEIVER EXPRESSLY ACKNOWLEDGES
# THAT IT IS RECEIVER'S SOLE RESPONSIBILITY TO OBTAIN FROM ANY THIRD PARTY ALL PROPER LICENSES
# CONTAINED IN MEDIATEK SOFTWARE. MEDIATEK SHALL ALSO NOT BE RESPONSIBLE FOR ANY MEDIATEK
# SOFTWARE RELEASES MADE TO RECEIVER'S SPECIFICATION OR TO CONFORM TO A PARTICULAR
# STANDARD OR OPEN FORUM. RECEIVER'S SOLE AND EXCLUSIVE REMEDY AND MEDIATEK'S ENTIRE AND
# CUMULATIVE LIABILITY WITH RESPECT TO THE MEDIATEK SOFTWARE RELEASED HEREUNDER WILL BE,
# AT MEDIATEK'S OPTION, TO REVISE OR REPLACE THE MEDIATEK SOFTWARE AT ISSUE,
# OR REFUND ANY SOFTWARE LICENSE FEES OR SERVICE CHARGE PAID BY RECEIVER TO
# MEDIATEK FOR SUCH MEDIATEK SOFTWARE AT ISSUE.
#
# The following software/firmware and/or related documentation ("MediaTek Software")
# have been modified by MediaTek Inc. All revisions are subject to any receiver's
# applicable license agreements with MediaTek Inc.

# Use project first
ifeq ($(wildcard $(MTK_PATH_CUSTOM)/hal/camera_exif),)

################################################################################
#
################################################################################

LOCAL_PATH := $(call my-dir)

################################################################################
#
################################################################################
include $(CLEAR_VARS)

#-----------------------------------------------------------
LOCAL_SRC_FILES += camera_custom_exif.cpp

#-----------------------------------------------------------
LOCAL_C_INCLUDES += $(TOP)/$(MTK_PATH_COMMON)/hal/inc
LOCAL_C_INCLUDES += $(TOP)/$(MTK_PATH_CUSTOM_PLATFORM)/hal/inc/debug_exif/cam
LOCAL_C_INCLUDES += $(TOP)/$(MTK_PATH_CUSTOM_PLATFORM)/hal/inc/debug_exif/cam/$(MTK_CAM_SW_VERSION)
LOCAL_C_INCLUDES += $(TOP)/$(MTK_PATH_CUSTOM_PLATFORM)/hal/inc/debug_exif/$(MTK_CAM_SW_VERSION)/cam

#------------- Exif Check Sum ----------------------------------------------
# Note:
# Run this script instead mk file:
# \custom\common\hal\inc\custom\debug_exif\gen_exif_tag_chksum_folder_offline.sh
#---------------------------------------------------------------------------
#MTK_CAM_EXIF_CHECKSUM_UPDATE := no
#---------------------------------------------------------------------------
#CHKSUMFOLDER          := $(MTK_PATH_SOURCE)/hardware/mtkcam/aaa/source/common/utils/gen_exif_tag_chksum_folder
#CHKSUMEXE             := $(MTK_PATH_SOURCE)/hardware/mtkcam/aaa/source/common/utils/AdlerCheck
#MTK_CAM_EXIF_TAG_PATH := $(TOP)/$(MTK_PATH_COMMON)/hal/inc/custom/debug_exif/cam
#OUT_PATH              := $(TOP)/$(MTK_PATH_COMMON)/hal/inc/custom/debug_exif
#
#$(OUT_PATH)/cam_exif_tag_chksum.h: \
# $(CHKSUMFOLDER) \
# $(CHKSUMEXE) \
# $(MTK_CAM_EXIF_TAG_PATH)
#	echo "generate $@ from $^"
#	rm -rf $@
#	$(CHKSUMFOLDER) $(MTK_CAM_EXIF_TAG_PATH) $(CHKSUMEXE) || \
#	{ >2 echo "generate $@ fail"; exit -1; }
#	echo "$(shell $(CHKSUMFOLDER) $(MTK_CAM_EXIF_TAG_PATH) $(CHKSUMEXE))" >> $@
#
#ifeq ($(MTK_CAM_EXIF_CHECKSUM_UPDATE),yes)
#	LOCAL_GENERATED_SOURCES += $(OUT_PATH)/cam_exif_tag_chksum.h
#endif
#-----------------------------------------------------------


LOCAL_HEADER_LIBRARIES += liblog_headers
#-----------------------------------------------------------
LOCAL_CFLAGS +=

#-----------------------------------------------------------
LOCAL_MODULE := libcameracustom.camera_exif
LOCAL_MODULE_OWNER := mtk
LOCAL_PROPRIETARY_MODULE := true

#-----------------------------------------------------------
include $(MTK_STATIC_LIBRARY)

################################################################################
#
################################################################################
#include $(CLEAR_VARS)
#include $(call all-makefiles-under,$(LOCAL_PATH))

endif