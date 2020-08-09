/* Copyright Statement:
 *
 * This software/firmware and related documentation ("MediaTek Software") are
 * protected under relevant copyright laws. The information contained herein
 * is confidential and proprietary to MediaTek Inc. and/or its licensors.
 * Without the prior written permission of MediaTek inc. and/or its licensors,
 * any reproduction, modification, use or disclosure of MediaTek Software,
 * and information contained herein, in whole or in part, shall be strictly prohibited.
 */
/* MediaTek Inc. (C) 2010. All rights reserved.
 *
 * BY OPENING THIS FILE, RECEIVER HEREBY UNEQUIVOCALLY ACKNOWLEDGES AND AGREES
 * THAT THE SOFTWARE/FIRMWARE AND ITS DOCUMENTATIONS ("MEDIATEK SOFTWARE")
 * RECEIVED FROM MEDIATEK AND/OR ITS REPRESENTATIVES ARE PROVIDED TO RECEIVER ON
 * AN "AS-IS" BASIS ONLY. MEDIATEK EXPRESSLY DISCLAIMS ANY AND ALL WARRANTIES,
 * EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE IMPLIED WARRANTIES OF
 * MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE OR NONINFRINGEMENT.
 * NEITHER DOES MEDIATEK PROVIDE ANY WARRANTY WHATSOEVER WITH RESPECT TO THE
 * SOFTWARE OF ANY THIRD PARTY WHICH MAY BE USED BY, INCORPORATED IN, OR
 * SUPPLIED WITH THE MEDIATEK SOFTWARE, AND RECEIVER AGREES TO LOOK ONLY TO SUCH
 * THIRD PARTY FOR ANY WARRANTY CLAIM RELATING THERETO. RECEIVER EXPRESSLY ACKNOWLEDGES
 * THAT IT IS RECEIVER'S SOLE RESPONSIBILITY TO OBTAIN FROM ANY THIRD PARTY ALL PROPER LICENSES
 * CONTAINED IN MEDIATEK SOFTWARE. MEDIATEK SHALL ALSO NOT BE RESPONSIBLE FOR ANY MEDIATEK
 * SOFTWARE RELEASES MADE TO RECEIVER'S SPECIFICATION OR TO CONFORM TO A PARTICULAR
 * STANDARD OR OPEN FORUM. RECEIVER'S SOLE AND EXCLUSIVE REMEDY AND MEDIATEK'S ENTIRE AND
 * CUMULATIVE LIABILITY WITH RESPECT TO THE MEDIATEK SOFTWARE RELEASED HEREUNDER WILL BE,
 * AT MEDIATEK'S OPTION, TO REVISE OR REPLACE THE MEDIATEK SOFTWARE AT ISSUE,
 * OR REFUND ANY SOFTWARE LICENSE FEES OR SERVICE CHARGE PAID BY RECEIVER TO
 * MEDIATEK FOR SUCH MEDIATEK SOFTWARE AT ISSUE.
 *
 * The following software/firmware and/or related documentation ("MediaTek Software")
 * have been modified by MediaTek Inc. All revisions are subject to any receiver's
 * applicable license agreements with MediaTek Inc.
 */

/********************************************************************************************
 *     LEGAL DISCLAIMER
 *
 *     (Header of MediaTek Software/Firmware Release or Documentation)
 *
 *     BY OPENING OR USING THIS FILE, BUYER HEREBY UNEQUIVOCALLY ACKNOWLEDGES AND AGREES
 *     THAT THE SOFTWARE/FIRMWARE AND ITS DOCUMENTATIONS ("MEDIATEK SOFTWARE") RECEIVED
 *     FROM MEDIATEK AND/OR ITS REPRESENTATIVES ARE PROVIDED TO BUYER ON AN "AS-IS" BASIS
 *     ONLY. MEDIATEK EXPRESSLY DISCLAIMS ANY AND ALL WARRANTIES, EXPRESS OR IMPLIED,
 *     INCLUDING BUT NOT LIMITED TO THE IMPLIED WARRANTIES OF MERCHANTABILITY, FITNESS FOR
 *     A PARTICULAR PURPOSE OR NONINFRINGEMENT. NEITHER DOES MEDIATEK PROVIDE ANY WARRANTY
 *     WHATSOEVER WITH RESPECT TO THE SOFTWARE OF ANY THIRD PARTY WHICH MAY BE USED BY,
 *     INCORPORATED IN, OR SUPPLIED WITH THE MEDIATEK SOFTWARE, AND BUYER AGREES TO LOOK
 *     ONLY TO SUCH THIRD PARTY FOR ANY WARRANTY CLAIM RELATING THERETO. MEDIATEK SHALL ALSO
 *     NOT BE RESPONSIBLE FOR ANY MEDIATEK SOFTWARE RELEASES MADE TO BUYER'S SPECIFICATION
 *     OR TO CONFORM TO A PARTICULAR STANDARD OR OPEN FORUM.
 *
 *     BUYER'S SOLE AND EXCLUSIVE REMEDY AND MEDIATEK'S ENTIRE AND CUMULATIVE LIABILITY WITH
 *     RESPECT TO THE MEDIATEK SOFTWARE RELEASED HEREUNDER WILL BE, AT MEDIATEK'S OPTION,
TO REVISE OR REPLACE THE MEDIATEK SOFTWARE AT ISSUE, OR REFUND ANY SOFTWARE LICENSE
 *     FEES OR SERVICE CHARGE PAID BY BUYER TO MEDIATEK FOR SUCH MEDIATEK SOFTWARE AT ISSUE.
 *
 *     THE TRANSACTION CONTEMPLATED HEREUNDER SHALL BE CONSTRUED IN ACCORDANCE WITH THE LAWS
 *     OF THE STATE OF CALIFORNIA, USA, EXCLUDING ITS CONFLICT OF LAWS PRINCIPLES.
 ************************************************************************************************/

#define LOG_TAG "isp_tuning_custom"

#ifndef ENABLE_MY_LOG
    #define ENABLE_MY_LOG       (0)
#endif

#include <aaa_types.h>
#include <aaa_log.h>
#include <camera_custom_nvram.h>
#include <isp_tuning_cam_info.h>
#include <isp_tuning_idx.h>
#include <isp_tuning_custom.h>
#include <isp_tuning_custom_swnr.h>
#include <isp_tuning_custom_instance.h>
#include <stdlib.h>                     // For atoi()
#include <stdio.h>
#include <cutils/properties.h>	// For property_get().
#include "camera_custom_3dnr.h"
#include <MTKGma.h>
//#include <MTKLce.h>

#define CEIL(a)   ( (a-(int)a) == 0 ? (int)a : (int)(a+1))

#define FLOOR(a)  ( (int)a)

using namespace NSIspTuning;


// ais
/*
 *  disable sl2a & sl2b. enable slb2 for ais stage3/4, disable for other cases
 *  make sure shading_tuning_custom.cpp & isp_tuning_custom.cpp use the same value
 */
#define TUNING_FOR_AIS  0
#define AIS_INTERPOLATION   0

static float AIS_NORMAL_CFA_RATIO[4] = {0.10f, 1.00f, 0.50f, 0.50f}; //0=ais, 1=normal, for stage1-4 respectively
static float AIS_NORMAL_YNR_RATIO[4] = {0.10f, 1.00f, 0.50f, 0.50f}; //0=ais, 1=normal, for stage1-4 respectively
static float AIS_NORMAL_CNR_RATIO[4] = {0.20f, 1.00f, 0.20f, 0.20f}; //0=ais, 1=normal, for stage1-4 respectively
static float AIS_NORMAL_EE_RATIO[4]  = {0.10f, 1.00f, 0.50f, 0.50f}; //0=ais, 1=normal, for stage1-4 respectively

#define IS_AIS          (rCamInfo.rAEInfo.u4OrgRealISOValue != rCamInfo.u4ISOValue)

#define ISO_TOP_SUPPORT_NUM 4
#define ISO_TOP_INTERVAL_NUM 20


#ifndef ISO_TOP_SUPPORT_MODE_main
#define ISO_TOP_SUPPORT_MODE_main 0  //0:3200,  1:6400,  2:12800
#endif

#ifndef ISO_TOP_SUPPORT_MODE_main2
#define ISO_TOP_SUPPORT_MODE_main2 0  //0:3200,  1:6400,  2:12800
#endif

#ifndef ISO_TOP_SUPPORT_MODE_sub
#define ISO_TOP_SUPPORT_MODE_sub 0  //0:3200,  1:6400,  2:12800
#endif

#define ANR_TBL_SUPPORT_ISO_NUM 6

#define FINE_TUNE_SUPPORT_NUM 2

static MUINT32 ANR_TBL_SUPPORT_ISO [(ISO_TOP_SUPPORT_NUM-1)][ANR_TBL_SUPPORT_ISO_NUM] =
{
    //3200
    { 100, 200, 400, 800, 1600, 3200},

    //6400
    { 100, 400, 800, 1600, 3200, 6400},

    //12800
    { 100, 400, 1600, 3200, 6400, 12800}
};

static ISP_FEATURE_TOP_CTL ispTopCtl[EIspProfile_NUM] =
{
    //0:disable, 1:enable

    //EIspProfile_Preview 00
    ( F_SL2FG_EN(1)  |F_DBS_EN(1)    |F_OBC_EN(1)  |F_BPC_EN(1)  |F_NR1_EN(1)
     |F_RMM_EN(0)    |F_RMG_EN(0)    |F_SL2H_EN(1) |F_RNR_EN(1)  |F_RPGN_EN(1)
     |F_SL2_EN(1)    |F_UDM_EN(1)    |F_CCM_EN(1)  |F_GGM_EN(1)  |F_G2C_EN(1)
     |F_SL2B_EN(1)   |F_ANR_EN(1)    |F_SL2C_EN(1) |F_ANR2_EN(1) |F_CCR_EN(1)
     |F_PCA_EN(1)    |F_SL2I_EN(1)   |F_HFG_EN(1)  |F_SL2D_EN(1) |F_EE_EN(1)
     |F_LCE_EN(1) ),

    //EIspProfile_Video 01
    ( F_SL2FG_EN(1)  |F_DBS_EN(1)    |F_OBC_EN(1)  |F_BPC_EN(1)  |F_NR1_EN(1)
     |F_RMM_EN(0)    |F_RMG_EN(0)    |F_SL2H_EN(1) |F_RNR_EN(1)  |F_RPGN_EN(1)
     |F_SL2_EN(1)    |F_UDM_EN(1)    |F_CCM_EN(1)  |F_GGM_EN(1)  |F_G2C_EN(1)
     |F_SL2B_EN(1)   |F_ANR_EN(1)    |F_SL2C_EN(1) |F_ANR2_EN(1) |F_CCR_EN(1)
     |F_PCA_EN(1)    |F_SL2I_EN(1)   |F_HFG_EN(1)  |F_SL2D_EN(1) |F_EE_EN(1)
     |F_LCE_EN(1) ),

    //EIspProfile_Capture 02
    ( F_SL2FG_EN(1)  |F_DBS_EN(1)    |F_OBC_EN(1)  |F_BPC_EN(1)  |F_NR1_EN(1)
     |F_RMM_EN(0)    |F_RMG_EN(0)    |F_SL2H_EN(1) |F_RNR_EN(1)  |F_RPGN_EN(1)
     |F_SL2_EN(1)    |F_UDM_EN(1)    |F_CCM_EN(1)  |F_GGM_EN(1)  |F_G2C_EN(1)
     |F_SL2B_EN(1)   |F_ANR_EN(1)    |F_SL2C_EN(1) |F_ANR2_EN(1) |F_CCR_EN(1)
     |F_PCA_EN(1)    |F_SL2I_EN(1)   |F_HFG_EN(1)  |F_SL2D_EN(1) |F_EE_EN(1)
     |F_LCE_EN(1) ),

    //EIspProfile_iHDR_Preview 03
    ( F_SL2FG_EN(1)  |F_DBS_EN(1)    |F_OBC_EN(1)  |F_BPC_EN(1)  |F_NR1_EN(1)
     |F_RMM_EN(1)    |F_RMG_EN(1)    |F_SL2H_EN(1) |F_RNR_EN(1)  |F_RPGN_EN(1)
     |F_SL2_EN(1)    |F_UDM_EN(1)    |F_CCM_EN(1)  |F_GGM_EN(1)  |F_G2C_EN(1)
     |F_SL2B_EN(1)   |F_ANR_EN(1)    |F_SL2C_EN(1) |F_ANR2_EN(1) |F_CCR_EN(1)
     |F_PCA_EN(1)    |F_SL2I_EN(1)   |F_HFG_EN(1)  |F_SL2D_EN(1) |F_EE_EN(1)
     |F_LCE_EN(1) ),

    //EIspProfile_zHDR_Preview 04
    ( F_SL2FG_EN(1)  |F_DBS_EN(1)    |F_OBC_EN(1)  |F_BPC_EN(1)  |F_NR1_EN(1)
     |F_RMM_EN(1)    |F_RMG_EN(1)    |F_SL2H_EN(1) |F_RNR_EN(1)  |F_RPGN_EN(1)
     |F_SL2_EN(1)    |F_UDM_EN(1)    |F_CCM_EN(1)  |F_GGM_EN(1)  |F_G2C_EN(1)
     |F_SL2B_EN(1)   |F_ANR_EN(1)    |F_SL2C_EN(1) |F_ANR2_EN(1) |F_CCR_EN(1)
     |F_PCA_EN(1)    |F_SL2I_EN(1)   |F_HFG_EN(1)  |F_SL2D_EN(1) |F_EE_EN(1)
     |F_LCE_EN(1) ),

    //EIspProfile_mHDR_Preview 05
    ( F_SL2FG_EN(1)  |F_DBS_EN(1)    |F_OBC_EN(1)  |F_BPC_EN(1)  |F_NR1_EN(1)
     |F_RMM_EN(0)    |F_RMG_EN(0)    |F_SL2H_EN(1) |F_RNR_EN(1)  |F_RPGN_EN(1)
     |F_SL2_EN(1)    |F_UDM_EN(1)    |F_CCM_EN(1)  |F_GGM_EN(1)  |F_G2C_EN(1)
     |F_SL2B_EN(1)   |F_ANR_EN(1)    |F_SL2C_EN(1) |F_ANR2_EN(1) |F_CCR_EN(1)
     |F_PCA_EN(1)    |F_SL2I_EN(1)   |F_HFG_EN(1)  |F_SL2D_EN(1) |F_EE_EN(1)
     |F_LCE_EN(1) ),

    //EIspProfile_iHDR_Video 06
    ( F_SL2FG_EN(1)  |F_DBS_EN(1)    |F_OBC_EN(1)  |F_BPC_EN(1)  |F_NR1_EN(1)
     |F_RMM_EN(1)    |F_RMG_EN(1)    |F_SL2H_EN(1) |F_RNR_EN(1)  |F_RPGN_EN(1)
     |F_SL2_EN(1)    |F_UDM_EN(1)    |F_CCM_EN(1)  |F_GGM_EN(1)  |F_G2C_EN(1)
     |F_SL2B_EN(1)   |F_ANR_EN(1)    |F_SL2C_EN(1) |F_ANR2_EN(1) |F_CCR_EN(1)
     |F_PCA_EN(1)    |F_SL2I_EN(1)   |F_HFG_EN(1)  |F_SL2D_EN(1) |F_EE_EN(1)
     |F_LCE_EN(1) ),

    //EIspProfile_zHDR_Video 07
    ( F_SL2FG_EN(1)  |F_DBS_EN(1)    |F_OBC_EN(1)  |F_BPC_EN(1)  |F_NR1_EN(1)
     |F_RMM_EN(1)    |F_RMG_EN(1)    |F_SL2H_EN(1) |F_RNR_EN(1)  |F_RPGN_EN(1)
     |F_SL2_EN(1)    |F_UDM_EN(1)    |F_CCM_EN(1)  |F_GGM_EN(1)  |F_G2C_EN(1)
     |F_SL2B_EN(1)   |F_ANR_EN(1)    |F_SL2C_EN(1) |F_ANR2_EN(1) |F_CCR_EN(1)
     |F_PCA_EN(1)    |F_SL2I_EN(1)   |F_HFG_EN(1)  |F_SL2D_EN(1) |F_EE_EN(1)
     |F_LCE_EN(1) ),

    //EIspProfile_mHDR_Video 08
    ( F_SL2FG_EN(1)  |F_DBS_EN(1)    |F_OBC_EN(1)  |F_BPC_EN(1)  |F_NR1_EN(1)
     |F_RMM_EN(0)    |F_RMG_EN(0)    |F_SL2H_EN(1) |F_RNR_EN(1)  |F_RPGN_EN(1)
     |F_SL2_EN(1)    |F_UDM_EN(1)    |F_CCM_EN(1)  |F_GGM_EN(1)  |F_G2C_EN(1)
     |F_SL2B_EN(1)   |F_ANR_EN(1)    |F_SL2C_EN(1) |F_ANR2_EN(1) |F_CCR_EN(1)
     |F_PCA_EN(1)    |F_SL2I_EN(1)   |F_HFG_EN(1)  |F_SL2D_EN(1) |F_EE_EN(1)
     |F_LCE_EN(1) ),

    //EIspProfile_iHDR_Preview_VSS 09
    ( F_SL2FG_EN(1)  |F_DBS_EN(1)    |F_OBC_EN(1)  |F_BPC_EN(1)  |F_NR1_EN(1)
     |F_RMM_EN(1)    |F_RMG_EN(1)    |F_SL2H_EN(1) |F_RNR_EN(1)  |F_RPGN_EN(1)
     |F_SL2_EN(1)    |F_UDM_EN(1)    |F_CCM_EN(1)  |F_GGM_EN(1)  |F_G2C_EN(1)
     |F_SL2B_EN(1)   |F_ANR_EN(1)    |F_SL2C_EN(1) |F_ANR2_EN(1) |F_CCR_EN(1)
     |F_PCA_EN(1)    |F_SL2I_EN(1)   |F_HFG_EN(1)  |F_SL2D_EN(1) |F_EE_EN(1)
     |F_LCE_EN(1) ),

    //EIspProfile_zHDR_Preview_VSS 10
    ( F_SL2FG_EN(1)  |F_DBS_EN(1)    |F_OBC_EN(1)  |F_BPC_EN(1)  |F_NR1_EN(1)
     |F_RMM_EN(1)    |F_RMG_EN(1)    |F_SL2H_EN(1) |F_RNR_EN(1)  |F_RPGN_EN(1)
     |F_SL2_EN(1)    |F_UDM_EN(1)    |F_CCM_EN(1)  |F_GGM_EN(1)  |F_G2C_EN(1)
     |F_SL2B_EN(1)   |F_ANR_EN(1)    |F_SL2C_EN(1) |F_ANR2_EN(1) |F_CCR_EN(1)
     |F_PCA_EN(1)    |F_SL2I_EN(1)   |F_HFG_EN(1)  |F_SL2D_EN(1) |F_EE_EN(1)
     |F_LCE_EN(1) ),

    //EIspProfile_mHDR_Preview_VSS 11
    ( F_SL2FG_EN(1)  |F_DBS_EN(1)    |F_OBC_EN(1)  |F_BPC_EN(1)  |F_NR1_EN(1)
     |F_RMM_EN(0)    |F_RMG_EN(0)    |F_SL2H_EN(1) |F_RNR_EN(1)  |F_RPGN_EN(1)
     |F_SL2_EN(1)    |F_UDM_EN(1)    |F_CCM_EN(1)  |F_GGM_EN(1)  |F_G2C_EN(1)
     |F_SL2B_EN(1)   |F_ANR_EN(1)    |F_SL2C_EN(1) |F_ANR2_EN(1) |F_CCR_EN(1)
     |F_PCA_EN(1)    |F_SL2I_EN(1)   |F_HFG_EN(1)  |F_SL2D_EN(1) |F_EE_EN(1)
     |F_LCE_EN(1) ),

    //EIspProfile_iHDR_Video_VSS 12
    ( F_SL2FG_EN(1)  |F_DBS_EN(1)    |F_OBC_EN(1)  |F_BPC_EN(1)  |F_NR1_EN(1)
     |F_RMM_EN(1)    |F_RMG_EN(1)    |F_SL2H_EN(1) |F_RNR_EN(1)  |F_RPGN_EN(1)
     |F_SL2_EN(1)    |F_UDM_EN(1)    |F_CCM_EN(1)  |F_GGM_EN(1)  |F_G2C_EN(1)
     |F_SL2B_EN(1)   |F_ANR_EN(1)    |F_SL2C_EN(1) |F_ANR2_EN(1) |F_CCR_EN(1)
     |F_PCA_EN(1)    |F_SL2I_EN(1)   |F_HFG_EN(1)  |F_SL2D_EN(1) |F_EE_EN(1)
     |F_LCE_EN(1) ),

     //EIspProfile_zHDR_Video_VSS 13
    ( F_SL2FG_EN(1)  |F_DBS_EN(1)    |F_OBC_EN(1)  |F_BPC_EN(1)  |F_NR1_EN(1)
     |F_RMM_EN(1)    |F_RMG_EN(1)    |F_SL2H_EN(1) |F_RNR_EN(1)  |F_RPGN_EN(1)
     |F_SL2_EN(1)    |F_UDM_EN(1)    |F_CCM_EN(1)  |F_GGM_EN(1)  |F_G2C_EN(1)
     |F_SL2B_EN(1)   |F_ANR_EN(1)    |F_SL2C_EN(1) |F_ANR2_EN(1) |F_CCR_EN(1)
     |F_PCA_EN(1)    |F_SL2I_EN(1)   |F_HFG_EN(1)  |F_SL2D_EN(1) |F_EE_EN(1)
     |F_LCE_EN(1) ),
    //EIspProfile_mHDR_Video_VSS 14
    ( F_SL2FG_EN(1)  |F_DBS_EN(1)    |F_OBC_EN(1)  |F_BPC_EN(1)  |F_NR1_EN(1)
     |F_RMM_EN(0)    |F_RMG_EN(0)    |F_SL2H_EN(1) |F_RNR_EN(1)  |F_RPGN_EN(1)
     |F_SL2_EN(1)    |F_UDM_EN(1)    |F_CCM_EN(1)  |F_GGM_EN(1)  |F_G2C_EN(1)
     |F_SL2B_EN(1)   |F_ANR_EN(1)    |F_SL2C_EN(1) |F_ANR2_EN(1) |F_CCR_EN(1)
     |F_PCA_EN(1)    |F_SL2I_EN(1)   |F_HFG_EN(1)  |F_SL2D_EN(1) |F_EE_EN(1)
     |F_LCE_EN(1) ),

    //EIspProfile_zHDR_Capture 15
    ( F_SL2FG_EN(1)  |F_DBS_EN(1)    |F_OBC_EN(1)  |F_BPC_EN(1)  |F_NR1_EN(1)
     |F_RMM_EN(1)    |F_RMG_EN(1)    |F_SL2H_EN(1) |F_RNR_EN(1)  |F_RPGN_EN(1)
     |F_SL2_EN(1)    |F_UDM_EN(1)    |F_CCM_EN(1)  |F_GGM_EN(1)  |F_G2C_EN(1)
     |F_SL2B_EN(1)   |F_ANR_EN(1)    |F_SL2C_EN(1) |F_ANR2_EN(1) |F_CCR_EN(1)
     |F_PCA_EN(1)    |F_SL2I_EN(1)   |F_HFG_EN(1)  |F_SL2D_EN(1) |F_EE_EN(1)
     |F_LCE_EN(1) ),

    //EIspProfile_mHDR_Capture 16
    ( F_SL2FG_EN(1)  |F_DBS_EN(1)    |F_OBC_EN(1)  |F_BPC_EN(1)  |F_NR1_EN(1)
     |F_RMM_EN(0)    |F_RMG_EN(0)    |F_SL2H_EN(1) |F_RNR_EN(1)  |F_RPGN_EN(1)
     |F_SL2_EN(1)    |F_UDM_EN(1)    |F_CCM_EN(1)  |F_GGM_EN(1)  |F_G2C_EN(1)
     |F_SL2B_EN(1)   |F_ANR_EN(1)    |F_SL2C_EN(1) |F_ANR2_EN(1) |F_CCR_EN(1)
     |F_PCA_EN(1)    |F_SL2I_EN(1)   |F_HFG_EN(1)  |F_SL2D_EN(1) |F_EE_EN(1)
     |F_LCE_EN(1) ),

    //EIspProfile_Auto_iHDR_Preview 17
    ( F_SL2FG_EN(1)  |F_DBS_EN(1)    |F_OBC_EN(1)  |F_BPC_EN(1)  |F_NR1_EN(1)
     |F_RMM_EN(1)    |F_RMG_EN(1)    |F_SL2H_EN(1) |F_RNR_EN(1)  |F_RPGN_EN(1)
     |F_SL2_EN(1)    |F_UDM_EN(1)    |F_CCM_EN(1)  |F_GGM_EN(1)  |F_G2C_EN(1)
     |F_SL2B_EN(1)   |F_ANR_EN(1)    |F_SL2C_EN(1) |F_ANR2_EN(1) |F_CCR_EN(1)
     |F_PCA_EN(1)    |F_SL2I_EN(1)   |F_HFG_EN(1)  |F_SL2D_EN(1) |F_EE_EN(1)
     |F_LCE_EN(1) ),

    //EIspProfile_Auto_zHDR_Preview 18
    ( F_SL2FG_EN(1)  |F_DBS_EN(1)    |F_OBC_EN(1)  |F_BPC_EN(1)  |F_NR1_EN(1)
     |F_RMM_EN(1)    |F_RMG_EN(1)    |F_SL2H_EN(1) |F_RNR_EN(1)  |F_RPGN_EN(1)
     |F_SL2_EN(1)    |F_UDM_EN(1)    |F_CCM_EN(1)  |F_GGM_EN(1)  |F_G2C_EN(1)
     |F_SL2B_EN(1)   |F_ANR_EN(1)    |F_SL2C_EN(1) |F_ANR2_EN(1) |F_CCR_EN(1)
     |F_PCA_EN(1)    |F_SL2I_EN(1)   |F_HFG_EN(1)  |F_SL2D_EN(1) |F_EE_EN(1)
     |F_LCE_EN(1) ),

    //EIspProfile_Auto_mHDR_Preview 19
    ( F_SL2FG_EN(1)  |F_DBS_EN(1)    |F_OBC_EN(1)  |F_BPC_EN(1)  |F_NR1_EN(1)
     |F_RMM_EN(0)    |F_RMG_EN(0)    |F_SL2H_EN(1) |F_RNR_EN(1)  |F_RPGN_EN(1)
     |F_SL2_EN(1)    |F_UDM_EN(1)    |F_CCM_EN(1)  |F_GGM_EN(1)  |F_G2C_EN(1)
     |F_SL2B_EN(1)   |F_ANR_EN(1)    |F_SL2C_EN(1) |F_ANR2_EN(1) |F_CCR_EN(1)
     |F_PCA_EN(1)    |F_SL2I_EN(1)   |F_HFG_EN(1)  |F_SL2D_EN(1) |F_EE_EN(1)
     |F_LCE_EN(1) ),

    //EIspProfile_Auto_iHDR_Video 20
    ( F_SL2FG_EN(1)  |F_DBS_EN(1)    |F_OBC_EN(1)  |F_BPC_EN(1)  |F_NR1_EN(1)
     |F_RMM_EN(1)    |F_RMG_EN(1)    |F_SL2H_EN(1) |F_RNR_EN(1)  |F_RPGN_EN(1)
     |F_SL2_EN(1)    |F_UDM_EN(1)    |F_CCM_EN(1)  |F_GGM_EN(1)  |F_G2C_EN(1)
     |F_SL2B_EN(1)   |F_ANR_EN(1)    |F_SL2C_EN(1) |F_ANR2_EN(1) |F_CCR_EN(1)
     |F_PCA_EN(1)    |F_SL2I_EN(1)   |F_HFG_EN(1)  |F_SL2D_EN(1) |F_EE_EN(1)
     |F_LCE_EN(1) ),

    //EIspProfile_Auto_zHDR_Video 21
    ( F_SL2FG_EN(1)  |F_DBS_EN(1)    |F_OBC_EN(1)  |F_BPC_EN(1)  |F_NR1_EN(1)
     |F_RMM_EN(1)    |F_RMG_EN(1)    |F_SL2H_EN(1) |F_RNR_EN(1)  |F_RPGN_EN(1)
     |F_SL2_EN(1)    |F_UDM_EN(1)    |F_CCM_EN(1)  |F_GGM_EN(1)  |F_G2C_EN(1)
     |F_SL2B_EN(1)   |F_ANR_EN(1)    |F_SL2C_EN(1) |F_ANR2_EN(1) |F_CCR_EN(1)
     |F_PCA_EN(1)    |F_SL2I_EN(1)   |F_HFG_EN(1)  |F_SL2D_EN(1) |F_EE_EN(1)
     |F_LCE_EN(1) ),

    //EIspProfile_Auto_mHDR_Video 22
    ( F_SL2FG_EN(1)  |F_DBS_EN(1)    |F_OBC_EN(1)  |F_BPC_EN(1)  |F_NR1_EN(1)
     |F_RMM_EN(0)    |F_RMG_EN(0)    |F_SL2H_EN(1) |F_RNR_EN(1)  |F_RPGN_EN(1)
     |F_SL2_EN(1)    |F_UDM_EN(1)    |F_CCM_EN(1)  |F_GGM_EN(1)  |F_G2C_EN(1)
     |F_SL2B_EN(1)   |F_ANR_EN(1)    |F_SL2C_EN(1) |F_ANR2_EN(1) |F_CCR_EN(1)
     |F_PCA_EN(1)    |F_SL2I_EN(1)   |F_HFG_EN(1)  |F_SL2D_EN(1) |F_EE_EN(1)
     |F_LCE_EN(1) ),

    //EIspProfile_Auto_iHDR_Preview_VSS 23
    ( F_SL2FG_EN(1)  |F_DBS_EN(1)    |F_OBC_EN(1)  |F_BPC_EN(1)  |F_NR1_EN(1)
     |F_RMM_EN(1)    |F_RMG_EN(1)    |F_SL2H_EN(1) |F_RNR_EN(1)  |F_RPGN_EN(1)
     |F_SL2_EN(1)    |F_UDM_EN(1)    |F_CCM_EN(1)  |F_GGM_EN(1)  |F_G2C_EN(1)
     |F_SL2B_EN(1)   |F_ANR_EN(1)    |F_SL2C_EN(1) |F_ANR2_EN(1) |F_CCR_EN(1)
     |F_PCA_EN(1)    |F_SL2I_EN(1)   |F_HFG_EN(1)  |F_SL2D_EN(1) |F_EE_EN(1)
     |F_LCE_EN(1) ),

    //EIspProfile_Auto_zHDR_Preview_VSS 24
    ( F_SL2FG_EN(1)  |F_DBS_EN(1)    |F_OBC_EN(1)  |F_BPC_EN(1)  |F_NR1_EN(1)
     |F_RMM_EN(1)    |F_RMG_EN(1)    |F_SL2H_EN(1) |F_RNR_EN(1)  |F_RPGN_EN(1)
     |F_SL2_EN(1)    |F_UDM_EN(1)    |F_CCM_EN(1)  |F_GGM_EN(1)  |F_G2C_EN(1)
     |F_SL2B_EN(1)   |F_ANR_EN(1)    |F_SL2C_EN(1) |F_ANR2_EN(1) |F_CCR_EN(1)
     |F_PCA_EN(1)    |F_SL2I_EN(1)   |F_HFG_EN(1)  |F_SL2D_EN(1) |F_EE_EN(1)
     |F_LCE_EN(1) ),

    //EIspProfile_Auto_mHDR_Preview_VSS 25
    ( F_SL2FG_EN(1)  |F_DBS_EN(1)    |F_OBC_EN(1)  |F_BPC_EN(1)  |F_NR1_EN(1)
     |F_RMM_EN(0)    |F_RMG_EN(0)    |F_SL2H_EN(1) |F_RNR_EN(1)  |F_RPGN_EN(1)
     |F_SL2_EN(1)    |F_UDM_EN(1)    |F_CCM_EN(1)  |F_GGM_EN(1)  |F_G2C_EN(1)
     |F_SL2B_EN(1)   |F_ANR_EN(1)    |F_SL2C_EN(1) |F_ANR2_EN(1) |F_CCR_EN(1)
     |F_PCA_EN(1)    |F_SL2I_EN(1)   |F_HFG_EN(1)  |F_SL2D_EN(1) |F_EE_EN(1)
     |F_LCE_EN(1) ),

    //EIspProfile_Auto_iHDR_Video_VSS 26
    ( F_SL2FG_EN(1)  |F_DBS_EN(1)    |F_OBC_EN(1)  |F_BPC_EN(1)  |F_NR1_EN(1)
     |F_RMM_EN(1)    |F_RMG_EN(1)    |F_SL2H_EN(1) |F_RNR_EN(1)  |F_RPGN_EN(1)
     |F_SL2_EN(1)    |F_UDM_EN(1)    |F_CCM_EN(1)  |F_GGM_EN(1)  |F_G2C_EN(1)
     |F_SL2B_EN(1)   |F_ANR_EN(1)    |F_SL2C_EN(1) |F_ANR2_EN(1) |F_CCR_EN(1)
     |F_PCA_EN(1)    |F_SL2I_EN(1)   |F_HFG_EN(1)  |F_SL2D_EN(1) |F_EE_EN(1)
     |F_LCE_EN(1) ),

    //EIspProfile_Auto_zHDR_Video_VSS 27
    ( F_SL2FG_EN(1)  |F_DBS_EN(1)    |F_OBC_EN(1)  |F_BPC_EN(1)  |F_NR1_EN(1)
     |F_RMM_EN(1)    |F_RMG_EN(1)    |F_SL2H_EN(1) |F_RNR_EN(1)  |F_RPGN_EN(1)
     |F_SL2_EN(1)    |F_UDM_EN(1)    |F_CCM_EN(1)  |F_GGM_EN(1)  |F_G2C_EN(1)
     |F_SL2B_EN(1)   |F_ANR_EN(1)    |F_SL2C_EN(1) |F_ANR2_EN(1) |F_CCR_EN(1)
     |F_PCA_EN(1)    |F_SL2I_EN(1)   |F_HFG_EN(1)  |F_SL2D_EN(1) |F_EE_EN(1)
     |F_LCE_EN(1) ),

    //EIspProfile_Auto_mHDR_Video_VSS 28
    ( F_SL2FG_EN(1)  |F_DBS_EN(1)    |F_OBC_EN(1)  |F_BPC_EN(1)  |F_NR1_EN(1)
     |F_RMM_EN(0)    |F_RMG_EN(0)    |F_SL2H_EN(1) |F_RNR_EN(1)  |F_RPGN_EN(1)
     |F_SL2_EN(1)    |F_UDM_EN(1)    |F_CCM_EN(1)  |F_GGM_EN(1)  |F_G2C_EN(1)
     |F_SL2B_EN(1)   |F_ANR_EN(1)    |F_SL2C_EN(1) |F_ANR2_EN(1) |F_CCR_EN(1)
     |F_PCA_EN(1)    |F_SL2I_EN(1)   |F_HFG_EN(1)  |F_SL2D_EN(1) |F_EE_EN(1)
     |F_LCE_EN(1) ),

    //EIspProfile_Auto_zHDR_Capture 29
    ( F_SL2FG_EN(1)  |F_DBS_EN(1)    |F_OBC_EN(1)  |F_BPC_EN(1)  |F_NR1_EN(1)
     |F_RMM_EN(1)    |F_RMG_EN(1)    |F_SL2H_EN(1) |F_RNR_EN(1)  |F_RPGN_EN(1)
     |F_SL2_EN(1)    |F_UDM_EN(1)    |F_CCM_EN(1)  |F_GGM_EN(1)  |F_G2C_EN(1)
     |F_SL2B_EN(1)   |F_ANR_EN(1)    |F_SL2C_EN(1) |F_ANR2_EN(1) |F_CCR_EN(1)
     |F_PCA_EN(1)    |F_SL2I_EN(1)   |F_HFG_EN(1)  |F_SL2D_EN(1) |F_EE_EN(1)
     |F_LCE_EN(1) ),

    //EIspProfile_Auto_mHDR_Capture 30
    ( F_SL2FG_EN(1)  |F_DBS_EN(1)    |F_OBC_EN(1)  |F_BPC_EN(1)  |F_NR1_EN(1)
     |F_RMM_EN(0)    |F_RMG_EN(0)    |F_SL2H_EN(1) |F_RNR_EN(1)  |F_RPGN_EN(1)
     |F_SL2_EN(1)    |F_UDM_EN(1)    |F_CCM_EN(1)  |F_GGM_EN(1)  |F_G2C_EN(1)
     |F_SL2B_EN(1)   |F_ANR_EN(1)    |F_SL2C_EN(1) |F_ANR2_EN(1) |F_CCR_EN(1)
     |F_PCA_EN(1)    |F_SL2I_EN(1)   |F_HFG_EN(1)  |F_SL2D_EN(1) |F_EE_EN(1)
     |F_LCE_EN(1) ),

    //EIspProfile_MFNR_Before_Blend 31
    ( F_SL2FG_EN(1)  |F_DBS_EN(1)    |F_OBC_EN(1)  |F_BPC_EN(1)  |F_NR1_EN(1)
     |F_RMM_EN(0)    |F_RMG_EN(0)    |F_SL2H_EN(1) |F_RNR_EN(1)  |F_RPGN_EN(1)
     |F_SL2_EN(1)    |F_UDM_EN(1)    |F_CCM_EN(1)  |F_GGM_EN(1)  |F_G2C_EN(1)
     |F_SL2B_EN(1)   |F_ANR_EN(1)    |F_SL2C_EN(1) |F_ANR2_EN(1) |F_CCR_EN(0)
     |F_PCA_EN(1)    |F_SL2I_EN(0)   |F_HFG_EN(0)  |F_SL2D_EN(0) |F_EE_EN(0)
     |F_LCE_EN(1) ),

    //EIspProfile_MFNR_Single 32
    ( F_SL2FG_EN(0)  |F_DBS_EN(0)    |F_OBC_EN(0)  |F_BPC_EN(0)  |F_NR1_EN(0)
     |F_RMM_EN(0)    |F_RMG_EN(0)    |F_SL2H_EN(0) |F_RNR_EN(0)  |F_RPGN_EN(0)
     |F_SL2_EN(1)    |F_UDM_EN(0)    |F_CCM_EN(0)  |F_GGM_EN(0)  |F_G2C_EN(0)
     |F_SL2B_EN(1)   |F_ANR_EN(1)    |F_SL2C_EN(1) |F_ANR2_EN(1) |F_CCR_EN(1)
     |F_PCA_EN(0)    |F_SL2I_EN(1)   |F_HFG_EN(1)  |F_SL2D_EN(1) |F_EE_EN(1)
     |F_LCE_EN(0) ),

    //EIspProfile_MFNR_MFB 33
    ( F_SL2FG_EN(0)  |F_DBS_EN(0)    |F_OBC_EN(0)  |F_BPC_EN(0)  |F_NR1_EN(0)
     |F_RMM_EN(0)    |F_RMG_EN(0)    |F_SL2H_EN(0) |F_RNR_EN(0)  |F_RPGN_EN(0)
     |F_SL2_EN(0)    |F_UDM_EN(0)    |F_CCM_EN(0)  |F_GGM_EN(0)  |F_G2C_EN(0)
     |F_SL2B_EN(0)   |F_ANR_EN(0)    |F_SL2C_EN(0) |F_ANR2_EN(0) |F_CCR_EN(0)
     |F_PCA_EN(0)    |F_SL2I_EN(0)   |F_HFG_EN(0)  |F_SL2D_EN(0) |F_EE_EN(0)
     |F_LCE_EN(0) ),

    //EIspProfile_MFNR_After_Blend 34
    ( F_SL2FG_EN(0)  |F_DBS_EN(0)    |F_OBC_EN(0)  |F_BPC_EN(0)  |F_NR1_EN(0)
     |F_RMM_EN(0)    |F_RMG_EN(0)    |F_SL2H_EN(0) |F_RNR_EN(0)  |F_RPGN_EN(0)
     |F_SL2_EN(1)    |F_UDM_EN(0)    |F_CCM_EN(0)  |F_GGM_EN(0)  |F_G2C_EN(0)
     |F_SL2B_EN(1)   |F_ANR_EN(1)    |F_SL2C_EN(1) |F_ANR2_EN(1) |F_CCR_EN(1)
     |F_PCA_EN(0)    |F_SL2I_EN(1)   |F_HFG_EN(1)  |F_SL2D_EN(1) |F_EE_EN(1)
     |F_LCE_EN(0) ),

    //EIspProfile_zHDR_Capture_MFNR_Before_Blend 35
    ( F_SL2FG_EN(1)  |F_DBS_EN(1)    |F_OBC_EN(1)  |F_BPC_EN(1)  |F_NR1_EN(1)
     |F_RMM_EN(0)    |F_RMG_EN(0)    |F_SL2H_EN(1) |F_RNR_EN(1)  |F_RPGN_EN(1)
     |F_SL2_EN(1)    |F_UDM_EN(1)    |F_CCM_EN(1)  |F_GGM_EN(1)  |F_G2C_EN(1)
     |F_SL2B_EN(1)   |F_ANR_EN(1)    |F_SL2C_EN(1) |F_ANR2_EN(1) |F_CCR_EN(0)
     |F_PCA_EN(1)    |F_SL2I_EN(0)   |F_HFG_EN(0)  |F_SL2D_EN(0) |F_EE_EN(0)
     |F_LCE_EN(1) ),

    //EIspProfile_zHDR_Capture_MFNR_Single 36
    ( F_SL2FG_EN(0)  |F_DBS_EN(0)    |F_OBC_EN(0)  |F_BPC_EN(0)  |F_NR1_EN(0)
     |F_RMM_EN(0)    |F_RMG_EN(0)    |F_SL2H_EN(0) |F_RNR_EN(0)  |F_RPGN_EN(0)
     |F_SL2_EN(1)    |F_UDM_EN(0)    |F_CCM_EN(0)  |F_GGM_EN(0)  |F_G2C_EN(0)
     |F_SL2B_EN(1)   |F_ANR_EN(1)    |F_SL2C_EN(1) |F_ANR2_EN(1) |F_CCR_EN(1)
     |F_PCA_EN(0)    |F_SL2I_EN(1)   |F_HFG_EN(1)  |F_SL2D_EN(1) |F_EE_EN(1)
     |F_LCE_EN(0) ),

    //EIspProfile_zHDR_Capture_MFNR_MFB 37
    ( F_SL2FG_EN(0)  |F_DBS_EN(0)    |F_OBC_EN(0)  |F_BPC_EN(0)  |F_NR1_EN(0)
     |F_RMM_EN(0)    |F_RMG_EN(0)    |F_SL2H_EN(0) |F_RNR_EN(0)  |F_RPGN_EN(0)
     |F_SL2_EN(0)    |F_UDM_EN(0)    |F_CCM_EN(0)  |F_GGM_EN(0)  |F_G2C_EN(0)
     |F_SL2B_EN(0)   |F_ANR_EN(0)    |F_SL2C_EN(0) |F_ANR2_EN(0) |F_CCR_EN(0)
     |F_PCA_EN(0)    |F_SL2I_EN(0)   |F_HFG_EN(0)  |F_SL2D_EN(0) |F_EE_EN(0)
     |F_LCE_EN(0) ),

    //EIspProfile_zHDR_Capture_MFNR_After_Blend 38
    ( F_SL2FG_EN(0)  |F_DBS_EN(0)    |F_OBC_EN(0)  |F_BPC_EN(0)  |F_NR1_EN(0)
     |F_RMM_EN(0)    |F_RMG_EN(0)    |F_SL2H_EN(0) |F_RNR_EN(0)  |F_RPGN_EN(0)
     |F_SL2_EN(1)    |F_UDM_EN(0)    |F_CCM_EN(0)  |F_GGM_EN(0)  |F_G2C_EN(0)
     |F_SL2B_EN(1)   |F_ANR_EN(1)    |F_SL2C_EN(1) |F_ANR2_EN(1) |F_CCR_EN(1)
     |F_PCA_EN(0)    |F_SL2I_EN(1)   |F_HFG_EN(1)  |F_SL2D_EN(1) |F_EE_EN(1)
     |F_LCE_EN(0) ),

    //EIspProfile_EIS_Preview 39
    ( F_SL2FG_EN(1)  |F_DBS_EN(1)    |F_OBC_EN(1)  |F_BPC_EN(1)  |F_NR1_EN(1)
     |F_RMM_EN(0)    |F_RMG_EN(0)    |F_SL2H_EN(1) |F_RNR_EN(1)  |F_RPGN_EN(1)
     |F_SL2_EN(1)    |F_UDM_EN(1)    |F_CCM_EN(1)  |F_GGM_EN(1)  |F_G2C_EN(1)
     |F_SL2B_EN(1)   |F_ANR_EN(1)    |F_SL2C_EN(1) |F_ANR2_EN(1) |F_CCR_EN(1)
     |F_PCA_EN(1)    |F_SL2I_EN(1)   |F_HFG_EN(1)  |F_SL2D_EN(1) |F_EE_EN(1)
     |F_LCE_EN(1) ),

    //EIspProfile_EIS_Video 40
    ( F_SL2FG_EN(1)  |F_DBS_EN(1)    |F_OBC_EN(1)  |F_BPC_EN(1)  |F_NR1_EN(1)
     |F_RMM_EN(0)    |F_RMG_EN(0)    |F_SL2H_EN(1) |F_RNR_EN(1)  |F_RPGN_EN(1)
     |F_SL2_EN(1)    |F_UDM_EN(1)    |F_CCM_EN(1)  |F_GGM_EN(1)  |F_G2C_EN(1)
     |F_SL2B_EN(1)   |F_ANR_EN(1)    |F_SL2C_EN(1) |F_ANR2_EN(1) |F_CCR_EN(1)
     |F_PCA_EN(1)    |F_SL2I_EN(1)   |F_HFG_EN(1)  |F_SL2D_EN(1) |F_EE_EN(1)
     |F_LCE_EN(1) ),

    //EIspProfile_EIS_iHDR_Preview 41
    ( F_SL2FG_EN(1)  |F_DBS_EN(1)    |F_OBC_EN(1)  |F_BPC_EN(1)  |F_NR1_EN(1)
     |F_RMM_EN(1)    |F_RMG_EN(1)    |F_SL2H_EN(1) |F_RNR_EN(1)  |F_RPGN_EN(1)
     |F_SL2_EN(1)    |F_UDM_EN(1)    |F_CCM_EN(1)  |F_GGM_EN(1)  |F_G2C_EN(1)
     |F_SL2B_EN(1)   |F_ANR_EN(1)    |F_SL2C_EN(1) |F_ANR2_EN(1) |F_CCR_EN(1)
     |F_PCA_EN(1)    |F_SL2I_EN(1)   |F_HFG_EN(1)  |F_SL2D_EN(1) |F_EE_EN(1)
     |F_LCE_EN(1) ),

    //EIspProfile_EIS_zHDR_Preview 42
    ( F_SL2FG_EN(1)  |F_DBS_EN(1)    |F_OBC_EN(1)  |F_BPC_EN(1)  |F_NR1_EN(1)
     |F_RMM_EN(1)    |F_RMG_EN(1)    |F_SL2H_EN(1) |F_RNR_EN(1)  |F_RPGN_EN(1)
     |F_SL2_EN(1)    |F_UDM_EN(1)    |F_CCM_EN(1)  |F_GGM_EN(1)  |F_G2C_EN(1)
     |F_SL2B_EN(1)   |F_ANR_EN(1)    |F_SL2C_EN(1) |F_ANR2_EN(1) |F_CCR_EN(1)
     |F_PCA_EN(1)    |F_SL2I_EN(1)   |F_HFG_EN(1)  |F_SL2D_EN(1) |F_EE_EN(1)
     |F_LCE_EN(1) ),

    //EIspProfile_EIS_mHDR_Preview 43
    ( F_SL2FG_EN(1)  |F_DBS_EN(1)    |F_OBC_EN(1)  |F_BPC_EN(1)  |F_NR1_EN(1)
     |F_RMM_EN(0)    |F_RMG_EN(0)    |F_SL2H_EN(1) |F_RNR_EN(1)  |F_RPGN_EN(1)
     |F_SL2_EN(1)    |F_UDM_EN(1)    |F_CCM_EN(1)  |F_GGM_EN(1)  |F_G2C_EN(1)
     |F_SL2B_EN(1)   |F_ANR_EN(1)    |F_SL2C_EN(1) |F_ANR2_EN(1) |F_CCR_EN(1)
     |F_PCA_EN(1)    |F_SL2I_EN(1)   |F_HFG_EN(1)  |F_SL2D_EN(1) |F_EE_EN(1)
     |F_LCE_EN(1) ),

    //EIspProfile_EIS_iHDR_Video 44
    ( F_SL2FG_EN(1)  |F_DBS_EN(1)    |F_OBC_EN(1)  |F_BPC_EN(1)  |F_NR1_EN(1)
     |F_RMM_EN(1)    |F_RMG_EN(1)    |F_SL2H_EN(1) |F_RNR_EN(1)  |F_RPGN_EN(1)
     |F_SL2_EN(1)    |F_UDM_EN(1)    |F_CCM_EN(1)  |F_GGM_EN(1)  |F_G2C_EN(1)
     |F_SL2B_EN(1)   |F_ANR_EN(1)    |F_SL2C_EN(1) |F_ANR2_EN(1) |F_CCR_EN(1)
     |F_PCA_EN(1)    |F_SL2I_EN(1)   |F_HFG_EN(1)  |F_SL2D_EN(1) |F_EE_EN(1)
     |F_LCE_EN(1) ),

    //EIspProfile_EIS_zHDR_Video 45
    ( F_SL2FG_EN(1)  |F_DBS_EN(1)    |F_OBC_EN(1)  |F_BPC_EN(1)  |F_NR1_EN(1)
     |F_RMM_EN(1)    |F_RMG_EN(1)    |F_SL2H_EN(1) |F_RNR_EN(1)  |F_RPGN_EN(1)
     |F_SL2_EN(1)    |F_UDM_EN(1)    |F_CCM_EN(1)  |F_GGM_EN(1)  |F_G2C_EN(1)
     |F_SL2B_EN(1)   |F_ANR_EN(1)    |F_SL2C_EN(1) |F_ANR2_EN(1) |F_CCR_EN(1)
     |F_PCA_EN(1)    |F_SL2I_EN(1)   |F_HFG_EN(1)  |F_SL2D_EN(1) |F_EE_EN(1)
     |F_LCE_EN(1) ),

    //EIspProfile_EIS_mHDR_Video 46
    ( F_SL2FG_EN(1)  |F_DBS_EN(1)    |F_OBC_EN(1)  |F_BPC_EN(1)  |F_NR1_EN(1)
     |F_RMM_EN(0)    |F_RMG_EN(0)    |F_SL2H_EN(1) |F_RNR_EN(1)  |F_RPGN_EN(1)
     |F_SL2_EN(1)    |F_UDM_EN(1)    |F_CCM_EN(1)  |F_GGM_EN(1)  |F_G2C_EN(1)
     |F_SL2B_EN(1)   |F_ANR_EN(1)    |F_SL2C_EN(1) |F_ANR2_EN(1) |F_CCR_EN(1)
     |F_PCA_EN(1)    |F_SL2I_EN(1)   |F_HFG_EN(1)  |F_SL2D_EN(1) |F_EE_EN(1)
     |F_LCE_EN(1) ),

    //EIspProfile_EIS_Auto_iHDR_Preview 47
    ( F_SL2FG_EN(1)  |F_DBS_EN(1)    |F_OBC_EN(1)  |F_BPC_EN(1)  |F_NR1_EN(1)
     |F_RMM_EN(1)    |F_RMG_EN(1)    |F_SL2H_EN(1) |F_RNR_EN(1)  |F_RPGN_EN(1)
     |F_SL2_EN(1)    |F_UDM_EN(1)    |F_CCM_EN(1)  |F_GGM_EN(1)  |F_G2C_EN(1)
     |F_SL2B_EN(1)   |F_ANR_EN(1)    |F_SL2C_EN(1) |F_ANR2_EN(1) |F_CCR_EN(1)
     |F_PCA_EN(1)    |F_SL2I_EN(1)   |F_HFG_EN(1)  |F_SL2D_EN(1) |F_EE_EN(1)
     |F_LCE_EN(1) ),

    //EIspProfile_EIS_Auto_zHDR_Preview 48
    ( F_SL2FG_EN(1)  |F_DBS_EN(1)    |F_OBC_EN(1)  |F_BPC_EN(1)  |F_NR1_EN(1)
     |F_RMM_EN(1)    |F_RMG_EN(1)    |F_SL2H_EN(1) |F_RNR_EN(1)  |F_RPGN_EN(1)
     |F_SL2_EN(1)    |F_UDM_EN(1)    |F_CCM_EN(1)  |F_GGM_EN(1)  |F_G2C_EN(1)
     |F_SL2B_EN(1)   |F_ANR_EN(1)    |F_SL2C_EN(1) |F_ANR2_EN(1) |F_CCR_EN(1)
     |F_PCA_EN(1)    |F_SL2I_EN(1)   |F_HFG_EN(1)  |F_SL2D_EN(1) |F_EE_EN(1)
     |F_LCE_EN(1) ),

    //EIspProfile_EIS_Auto_mHDR_Preview 49
    ( F_SL2FG_EN(1)  |F_DBS_EN(1)    |F_OBC_EN(1)  |F_BPC_EN(1)  |F_NR1_EN(1)
     |F_RMM_EN(0)    |F_RMG_EN(0)    |F_SL2H_EN(1) |F_RNR_EN(1)  |F_RPGN_EN(1)
     |F_SL2_EN(1)    |F_UDM_EN(1)    |F_CCM_EN(1)  |F_GGM_EN(1)  |F_G2C_EN(1)
     |F_SL2B_EN(1)   |F_ANR_EN(1)    |F_SL2C_EN(1) |F_ANR2_EN(1) |F_CCR_EN(1)
     |F_PCA_EN(1)    |F_SL2I_EN(1)   |F_HFG_EN(1)  |F_SL2D_EN(1) |F_EE_EN(1)
     |F_LCE_EN(1) ),

    //EIspProfile_EIS_Auto_iHDR_Video 50
    ( F_SL2FG_EN(1)  |F_DBS_EN(1)    |F_OBC_EN(1)  |F_BPC_EN(1)  |F_NR1_EN(1)
     |F_RMM_EN(1)    |F_RMG_EN(1)    |F_SL2H_EN(1) |F_RNR_EN(1)  |F_RPGN_EN(1)
     |F_SL2_EN(1)    |F_UDM_EN(1)    |F_CCM_EN(1)  |F_GGM_EN(1)  |F_G2C_EN(1)
     |F_SL2B_EN(1)   |F_ANR_EN(1)    |F_SL2C_EN(1) |F_ANR2_EN(1) |F_CCR_EN(1)
     |F_PCA_EN(1)    |F_SL2I_EN(1)   |F_HFG_EN(1)  |F_SL2D_EN(1) |F_EE_EN(1)
     |F_LCE_EN(1) ),

    //EIspProfile_EIS_Auto_zHDR_Video 51
    ( F_SL2FG_EN(1)  |F_DBS_EN(1)    |F_OBC_EN(1)  |F_BPC_EN(1)  |F_NR1_EN(1)
     |F_RMM_EN(1)    |F_RMG_EN(1)    |F_SL2H_EN(1) |F_RNR_EN(1)  |F_RPGN_EN(1)
     |F_SL2_EN(1)    |F_UDM_EN(1)    |F_CCM_EN(1)  |F_GGM_EN(1)  |F_G2C_EN(1)
     |F_SL2B_EN(1)   |F_ANR_EN(1)    |F_SL2C_EN(1) |F_ANR2_EN(1) |F_CCR_EN(1)
     |F_PCA_EN(1)    |F_SL2I_EN(1)   |F_HFG_EN(1)  |F_SL2D_EN(1) |F_EE_EN(1)
     |F_LCE_EN(1) ),

    //EIspProfile_EIS_Auto_mHDR_Video 52
    ( F_SL2FG_EN(1)  |F_DBS_EN(1)    |F_OBC_EN(1)  |F_BPC_EN(1)  |F_NR1_EN(1)
     |F_RMM_EN(0)    |F_RMG_EN(0)    |F_SL2H_EN(1) |F_RNR_EN(1)  |F_RPGN_EN(1)
     |F_SL2_EN(1)    |F_UDM_EN(1)    |F_CCM_EN(1)  |F_GGM_EN(1)  |F_G2C_EN(1)
     |F_SL2B_EN(1)   |F_ANR_EN(1)    |F_SL2C_EN(1) |F_ANR2_EN(1) |F_CCR_EN(1)
     |F_PCA_EN(1)    |F_SL2I_EN(1)   |F_HFG_EN(1)  |F_SL2D_EN(1) |F_EE_EN(1)
     |F_LCE_EN(1) ),

    //EIspProfile_Capture_MultiPass_HWNR 53
    ( F_SL2FG_EN(0)  |F_DBS_EN(0)    |F_OBC_EN(0)  |F_BPC_EN(0)  |F_NR1_EN(0)
     |F_RMM_EN(0)    |F_RMG_EN(0)    |F_SL2H_EN(0) |F_RNR_EN(0)  |F_RPGN_EN(0)
     |F_SL2_EN(0)    |F_UDM_EN(0)    |F_CCM_EN(0)  |F_GGM_EN(0)  |F_G2C_EN(0)
     |F_SL2B_EN(1)   |F_ANR_EN(1)    |F_SL2C_EN(1) |F_ANR2_EN(1) |F_CCR_EN(1)
     |F_PCA_EN(0)    |F_SL2I_EN(0)   |F_HFG_EN(0)  |F_SL2D_EN(0) |F_EE_EN(0)
     |F_LCE_EN(1) ),

    //EIspProfile_YUV_Reprocess 54
    ( F_SL2FG_EN(0)  |F_DBS_EN(0)    |F_OBC_EN(0)  |F_BPC_EN(0)  |F_NR1_EN(0)
     |F_RMM_EN(0)    |F_RMG_EN(0)    |F_SL2H_EN(0) |F_RNR_EN(0)  |F_RPGN_EN(0)
     |F_SL2_EN(0)    |F_UDM_EN(0)    |F_CCM_EN(0)  |F_GGM_EN(0)  |F_G2C_EN(0)
     |F_SL2B_EN(0)   |F_ANR_EN(1)    |F_SL2C_EN(0) |F_ANR2_EN(1) |F_CCR_EN(1)
     |F_PCA_EN(0)    |F_SL2I_EN(0)   |F_HFG_EN(0)  |F_SL2D_EN(0) |F_EE_EN(1)
     |F_LCE_EN(0) ),

    //EIspProfile_Flash_Capture 55
    ( F_SL2FG_EN(1)  |F_DBS_EN(1)    |F_OBC_EN(1)  |F_BPC_EN(1)  |F_NR1_EN(1)
     |F_RMM_EN(0)    |F_RMG_EN(0)    |F_SL2H_EN(1) |F_RNR_EN(1)  |F_RPGN_EN(1)
     |F_SL2_EN(1)    |F_UDM_EN(1)    |F_CCM_EN(1)  |F_GGM_EN(1)  |F_G2C_EN(1)
     |F_SL2B_EN(1)   |F_ANR_EN(1)    |F_SL2C_EN(1) |F_ANR2_EN(1) |F_CCR_EN(1)
     |F_PCA_EN(1)    |F_SL2I_EN(1)   |F_HFG_EN(1)  |F_SL2D_EN(1) |F_EE_EN(1)
     |F_LCE_EN(1) )
};

static ISP_NVRAM_ANR_T customANR_main[FINE_TUNE_SUPPORT_NUM][NVRAM_ISP_REGS_ISO_SUPPORT_NUM] =
{
    //set 1
    {
        {set:{//ANR_custom: profile = capture, sensor mode = capture, iso = ISO_0
            0x01000113, 0x04410002, 0x89090000, 0x0010000A, 0x00A06428, 0x0C101010, 0x18180000, 0x00A06428, 0x10101010, 0x00000000,
            0x00000000, 0x00000000, 0x00C08040, 0x12101010, 0x00000000, 0x06040302, 0x001E100A, 0x08060402, 0x00080004, 0x10305405,
            0x00308003, 0x0A300300, 0x1613100D, 0x00000000, 0xB9193C78, 0x00060000, 0x00000000, 0x10001008, 0x00010010, 0x00000000,
            0x00200020, 0x1E051E0E, 0x2805280E, 0x4005400E, 0x00040000, 0x00000000, 0x00000000, 0x2D002D00, 0x01300000, 0x00002020,
            0xFF00FF00, 0x09070503, 0x00004668, 0x0204404A
        }},
        {set:{//ANR_custom: profile = capture, sensor mode = capture, iso = ISO_1
            0x01000113, 0x04410002, 0x89090000, 0x0010000A, 0x00A06428, 0x0C101010, 0x18170000, 0x00A06428, 0x10101010, 0x00000000,
            0x00000000, 0x00000000, 0x00C08040, 0x12101010, 0x00040000, 0x06040503, 0x001E100A, 0x08060402, 0x00080004, 0x10305405,
            0x00308003, 0x0A300300, 0x1613100D, 0x00000000, 0xB9193C78, 0x00060000, 0x00000000, 0x10001008, 0x00010010, 0x00000000,
            0x00200020, 0x1905190E, 0x2305230E, 0x3505350E, 0x00040000, 0x00000000, 0x00000000, 0x29002900, 0x01300000, 0x0000201C,
            0xFF00FF00, 0x0A080503, 0x00004668, 0x0204404A
        }},
        {set:{//ANR_custom: profile = capture, sensor mode = capture, iso = ISO_2
            0x01000113, 0x04410002, 0x89090000, 0x0010000A, 0x00A06428, 0x0C101010, 0x18180000, 0x00A06428, 0x10101010, 0x00000000,
            0x00000000, 0x00000000, 0x00C08040, 0x12101010, 0x00000000, 0x06040804, 0x001E100A, 0x08060402, 0x00080004, 0x10305405,
            0x00308003, 0x0A300300, 0x1613100D, 0x00000000, 0xB9193C78, 0x00060000, 0x00000000, 0x10001008, 0x00010010, 0x00000000,
            0x00200020, 0x0F050F0E, 0x1905190E, 0x1E051E0E, 0x00040000, 0x00000000, 0x00000000, 0x20002000, 0x01300000, 0x00002014,
            0xFF00FF00, 0x0C090603, 0x00004668, 0x0204404A
        }},
        {set:{//ANR_custom: profile = capture, sensor mode = capture, iso = ISO_3
            0x01000113, 0x04410002, 0x89090000, 0x0010000A, 0x00A06428, 0x0C101010, 0x18180000, 0x00A06428, 0x10101010, 0x00000000,
            0x00000000, 0x00000000, 0x00C08040, 0x1C161210, 0x000C0804, 0x0A080603, 0x001E100A, 0x0C090603, 0x00080008, 0x10305405,
            0x00308003, 0x0A300300, 0x1613100D, 0x00000000, 0xB9193C78, 0x00060000, 0x00000000, 0x10001008, 0x00010010, 0x00000000,
            0x00200020, 0x1405140E, 0x1E051E0E, 0x1E051E0E, 0x00040000, 0x00000000, 0x00000000, 0x40004000, 0x01300000, 0x00002014,
            0xFF00FF00, 0x0C090603, 0x00004668, 0x0204404A
        }},
        {set:{//ANR_custom: profile = capture, sensor mode = capture, iso = ISO_4
            0x01000113, 0x02410102, 0x89090000, 0x0010000A, 0x00A06428, 0x0C101010, 0x18180000, 0x00A06428, 0x10101010, 0x00000000,
            0x00000000, 0x00000000, 0x00C08040, 0x1C161010, 0x000C0C00, 0x08060402, 0x001E100A, 0x140F0A05, 0x00080004, 0x10305405,
            0x00308003, 0x0A300300, 0x1613100D, 0x00000000, 0xB9193C78, 0x00060000, 0x00000000, 0x10000008, 0x00010010, 0x00000000,
            0x00200020, 0x14051414, 0x14051414, 0x14051414, 0x00040010, 0x12041210, 0x00000010, 0x40004000, 0x01300000, 0x00002020,
            0xFF00FF00, 0x08060402, 0x00004668, 0x0204404A
        }},
        {set:{//ANR_custom: profile = capture, sensor mode = capture, iso = ISO_5
            0x01000113, 0x02410102, 0x89090000, 0x0010000A, 0x00A06428, 0x0C101010, 0x18180000, 0x00A06428, 0x10101010, 0x00000000,
            0x00000000, 0x00000000, 0x00C08040, 0x1C161010, 0x000C0C00, 0x08060402, 0x001E100A, 0x18120C06, 0x00080004, 0x10305405,
            0x00308003, 0x0A300300, 0x1613100D, 0x00000000, 0xB9193C78, 0x00060000, 0x00000000, 0x10000008, 0x00010010, 0x00000000,
            0x00200020, 0x14051414, 0x14051414, 0x14051414, 0x00040010, 0x12041210, 0x00000010, 0x40004000, 0x01300000, 0x00002020,
            0xFF00FF00, 0x08060402, 0x00004668, 0x0204404A
        }},
        {set:{//ANR_custom: profile = capture, sensor mode = capture, iso = ISO_6
            0x01000113, 0x02410102, 0x89090000, 0x0010000A, 0x00A06428, 0x0C101010, 0x18170000, 0x00A06428, 0x10101010, 0x00000000,
            0x00000000, 0x00000000, 0x00C08040, 0x1C161010, 0x000C0C00, 0x08060402, 0x001E100A, 0x18120C06, 0x00080004, 0x10305405,
            0x00308003, 0x0A300300, 0x1613100D, 0x00000000, 0xB9193C78, 0x00060000, 0x00000000, 0x10000008, 0x00010010, 0x00000000,
            0x00200020, 0x14051414, 0x14051414, 0x14051414, 0x00040010, 0x12041210, 0x00000010, 0x40004000, 0x01300000, 0x00002020,
            0xFF00FF00, 0x08060402, 0x00004668, 0x0204404A
        }},
        {set:{//ANR_custom: profile = capture, sensor mode = capture, iso = ISO_7
            0x01000113, 0x02410102, 0x89090000, 0x0010000A, 0x00A06428, 0x0C101010, 0x18170000, 0x00A06428, 0x10101010, 0x00000000,
            0x00000000, 0x00000000, 0x00C08040, 0x1C161010, 0x000C0C00, 0x08060402, 0x001E100A, 0x18120C06, 0x00080004, 0x10305405,
            0x00308003, 0x0A300300, 0x1613100D, 0x00000000, 0xB9193C78, 0x00060000, 0x00000000, 0x10000008, 0x00010010, 0x00000000,
            0x00200020, 0x14051414, 0x14051414, 0x14051414, 0x00040010, 0x12041210, 0x00000010, 0x40004000, 0x01300000, 0x00002020,
            0xFF00FF00, 0x08060402, 0x00004668, 0x0204404A
        }},
        {set:{//ANR_custom: profile = capture, sensor mode = capture, iso = ISO_8
            0x01000113, 0x02410102, 0x89090000, 0x0010000A, 0x00A06428, 0x0C101010, 0x18170000, 0x00A06428, 0x10101010, 0x00000000,
            0x00000000, 0x00000000, 0x00C08040, 0x1C161010, 0x000C0C00, 0x08060402, 0x001E100A, 0x18120C06, 0x00080004, 0x10305405,
            0x00308003, 0x0A300300, 0x1613100D, 0x00000000, 0xB9193C78, 0x00060000, 0x00000000, 0x10000008, 0x00010010, 0x00000000,
            0x00200020, 0x14051414, 0x14051414, 0x14051414, 0x00040010, 0x12041210, 0x00000010, 0x40004000, 0x01300000, 0x00002020,
            0xFF00FF00, 0x08060402, 0x00004668, 0x0204404A
        }},
        {set:{//ANR_custom: profile = capture, sensor mode = capture, iso = ISO_9
            0x01100133, 0x02410102, 0x89090808, 0x0010000A, 0x00E0A000, 0x0E101010, 0x1F1C0000, 0x00A06428, 0x10101010, 0x00000000,
            0x00000000, 0x00000000, 0x00C08040, 0x1C161010, 0x000C0C00, 0x140F0A05, 0x001E100A, 0x18120C06, 0x00080006, 0x3C305302,
            0x0C50C0C5, 0x1430C506, 0x32302C24, 0x00000000, 0xB9193C78, 0x00060000, 0x00000000, 0x10000008, 0x00010010, 0x00000000,
            0x00200020, 0x0A050A1E, 0x1E051E1E, 0x0A050A1E, 0x00040000, 0x12040000, 0x00000010, 0x40004000, 0x01300808, 0x00002020,
            0xFF00FF00, 0x08060402, 0x00004668, 0x0204404A
        }},
        {set:{//ANR_custom: profile = capture, sensor mode = capture, iso = ISO_10
            0x01100133, 0x02410102, 0x89090808, 0x0010000A, 0x00E0A000, 0x0E101010, 0x1F1C0000, 0x00A06428, 0x10101010, 0x00000000,
            0x00000000, 0x00000000, 0x00C08040, 0x1C161010, 0x000C0C00, 0x140F0A05, 0x001E100A, 0x18120C06, 0x00080006, 0x41305302,
            0x0C50C0C5, 0x1430C506, 0x32302C24, 0x00000000, 0xB9193C78, 0x00060000, 0x00000000, 0x10000008, 0x00010010, 0x00000000,
            0x00200020, 0x0A050A1E, 0x1E051E1E, 0x0A050A1E, 0x00040000, 0x12040000, 0x00000010, 0x40004000, 0x01300808, 0x00002020,
            0xFF00FF00, 0x08060402, 0x00004668, 0x0204404A
        }},
        {set:{//ANR_custom: profile = capture, sensor mode = capture, iso = ISO_11
            0x01100133, 0x02410102, 0x89090808, 0x0010000A, 0x00E0A000, 0x0E101010, 0x1F1C0000, 0x00A06428, 0x10101010, 0x00000000,
            0x00000000, 0x00000000, 0x00C08040, 0x1C161010, 0x000C0C00, 0x140F0A05, 0x001E100A, 0x18120C06, 0x00080006, 0x46305302,
            0x0C50C0C5, 0x1430C506, 0x32302C24, 0x00000000, 0xB9193C78, 0x00060000, 0x00000000, 0x10000008, 0x00010010, 0x00000000,
            0x00200020, 0x0A050A1E, 0x1E051E1E, 0x0A050A1E, 0x00040000, 0x12040000, 0x00000010, 0x40004000, 0x01300808, 0x00002020,
            0xFF00FF00, 0x08060402, 0x00004668, 0x0204404A
        }},
        {set:{//ANR_custom: profile = capture, sensor mode = capture, iso = ISO_12
            0x01100133, 0x02410102, 0x89090808, 0x0010000A, 0x00E0A000, 0x0E101010, 0x1F1C0000, 0x00A06428, 0x10101010, 0x00000000,
            0x00000000, 0x00000000, 0x00C08040, 0x1C161010, 0x000C0C00, 0x140F0A05, 0x001E100A, 0x18120C06, 0x00080006, 0x46305302,
            0x0C50C0C5, 0x1430C506, 0x32302C24, 0x00000000, 0xB9193C78, 0x00060000, 0x00000000, 0x10000008, 0x00010010, 0x00000000,
            0x00200020, 0x0A050A1E, 0x1E051E1E, 0x0A050A1E, 0x00040000, 0x12040000, 0x00000010, 0x40004000, 0x01300808, 0x00002020,
            0xFF00FF00, 0x08060402, 0x00004668, 0x0204404A
        }},
        {set:{//ANR_custom: profile = capture, sensor mode = capture, iso = ISO_13
            0x01100133, 0x02410102, 0x89090808, 0x0010000A, 0x00E0A000, 0x0E101010, 0x1F1C0000, 0x00A06428, 0x10101010, 0x00000000,
            0x00000000, 0x00000000, 0x00C08040, 0x1C161010, 0x000C0C00, 0x140F0A05, 0x001E100A, 0x18120C06, 0x00080006, 0x46305302,
            0x0C50C0C5, 0x1430C506, 0x32302C24, 0x00000000, 0xB9193C78, 0x00060000, 0x00000000, 0x10000008, 0x00010010, 0x00000000,
            0x00200020, 0x0A050A1E, 0x1E051E1E, 0x0A050A1E, 0x00040000, 0x12040000, 0x00000010, 0x40004000, 0x01300808, 0x00002020,
            0xFF00FF00, 0x08060402, 0x00004668, 0x0204404A
        }},
        {set:{//ANR_custom: profile = capture, sensor mode = capture, iso = ISO_14
            0x01100133, 0x02410102, 0x89090808, 0x0010000A, 0x00E0A000, 0x0E101010, 0x1F1C0000, 0x00A06428, 0x10101010, 0x00000000,
            0x00000000, 0x00000000, 0x00C08040, 0x1C161010, 0x000C0C00, 0x140F0A05, 0x001E100A, 0x18120C06, 0x00080006, 0x46305302,
            0x0C50C0C5, 0x1430C506, 0x32302C24, 0x00000000, 0xB9193C78, 0x00060000, 0x00000000, 0x10000008, 0x00010010, 0x00000000,
            0x00200020, 0x0A050A1E, 0x1E051E1E, 0x0A050A1E, 0x00040000, 0x12040000, 0x00000010, 0x40004000, 0x01300808, 0x00002020,
            0xFF00FF00, 0x08060402, 0x00004668, 0x0204404A
        }},
        {set:{//ANR_custom: profile = capture, sensor mode = capture, iso = ISO_15
            0x01100133, 0x02410102, 0x89090808, 0x0010000A, 0x00E0A000, 0x0E101010, 0x1F1C0000, 0x00A06428, 0x10101010, 0x00000000,
            0x00000000, 0x00000000, 0x00C08040, 0x1C161010, 0x000C0C00, 0x140F0A05, 0x001E100A, 0x18120C06, 0x00080006, 0x46305302,
            0x0C50C0C5, 0x1430C506, 0x32302C24, 0x00000000, 0xB9193C78, 0x00060000, 0x00000000, 0x10000008, 0x00010010, 0x00000000,
            0x00200020, 0x0A050A1E, 0x1E051E1E, 0x0A050A1E, 0x00040000, 0x12040000, 0x00000010, 0x40004000, 0x01300808, 0x00002020,
            0xFF00FF00, 0x08060402, 0x00004668, 0x0204404A
        }},
        {set:{//ANR_custom: profile = capture, sensor mode = capture, iso = ISO_16
            0x01100133, 0x02410102, 0x89090808, 0x0010000A, 0x00E0A000, 0x0E101010, 0x1F1C0000, 0x00A06428, 0x10101010, 0x00000000,
            0x00000000, 0x00000000, 0x00C08040, 0x1C161010, 0x000C0C00, 0x140F0A05, 0x001E100A, 0x18120C06, 0x00080006, 0x46305302,
            0x0C50C0C5, 0x1430C506, 0x32302C24, 0x00000000, 0xB9193C78, 0x00060000, 0x00000000, 0x10000008, 0x00010010, 0x00000000,
            0x00200020, 0x0A050A1E, 0x1E051E1E, 0x0A050A1E, 0x00040000, 0x12040000, 0x00000010, 0x40004000, 0x01300808, 0x00002020,
            0xFF00FF00, 0x08060402, 0x00004668, 0x0204404A
        }},
        {set:{//ANR_custom: profile = capture, sensor mode = capture, iso = ISO_17
            0x01100133, 0x02410102, 0x89090808, 0x0010000A, 0x00E0A000, 0x0E101010, 0x1F1C0000, 0x00A06428, 0x10101010, 0x00000000,
            0x00000000, 0x00000000, 0x00C08040, 0x1C161010, 0x000C0C00, 0x140F0A05, 0x001E100A, 0x18120C06, 0x00080006, 0x46305302,
            0x0C50C0C5, 0x1430C506, 0x32302C24, 0x00000000, 0xB9193C78, 0x00060000, 0x00000000, 0x10000008, 0x00010010, 0x00000000,
            0x00200020, 0x0A050A1E, 0x1E051E1E, 0x0A050A1E, 0x00040000, 0x12040000, 0x00000010, 0x40004000, 0x01300808, 0x00002020,
            0xFF00FF00, 0x08060402, 0x00004668, 0x0204404A
        }},
        {set:{//ANR_custom: profile = capture, sensor mode = capture, iso = ISO_18
            0x01100133, 0x02410102, 0x89090808, 0x0010000A, 0x00E0A000, 0x0E101010, 0x1F1C0000, 0x00A06428, 0x10101010, 0x00000000,
            0x00000000, 0x00000000, 0x00C08040, 0x1C161010, 0x000C0C00, 0x140F0A05, 0x001E100A, 0x18120C06, 0x00080006, 0x46305302,
            0x0C50C0C5, 0x1430C506, 0x32302C24, 0x00000000, 0xB9193C78, 0x00060000, 0x00000000, 0x10000008, 0x00010010, 0x00000000,
            0x00200020, 0x0A050A1E, 0x1E051E1E, 0x0A050A1E, 0x00040000, 0x12040000, 0x00000010, 0x40004000, 0x01300808, 0x00002020,
            0xFF00FF00, 0x08060402, 0x00004668, 0x0204404A
        }},
        {set:{//ANR_custom: profile = capture, sensor mode = capture, iso = ISO_19
            0x01100133, 0x02410102, 0x89090808, 0x0010000A, 0x00E0A000, 0x0E101010, 0x1F1C0000, 0x00A06428, 0x10101010, 0x00000000,
            0x00000000, 0x00000000, 0x00C08040, 0x1C161010, 0x000C0C00, 0x140F0A05, 0x001E100A, 0x18120C06, 0x00080006, 0x46305302,
            0x0C50C0C5, 0x1430C506, 0x32302C24, 0x00000000, 0xB9193C78, 0x00060000, 0x00000000, 0x10000008, 0x00010010, 0x00000000,
            0x00200020, 0x0A050A1E, 0x1E051E1E, 0x0A050A1E, 0x00040000, 0x12040000, 0x00000010, 0x40004000, 0x01300808, 0x00002020,
            0xFF00FF00, 0x08060402, 0x00004668, 0x0204404A
        }}
    },

    //set 2
    {
        {set:{//ANR_custom: profile = capture, sensor mode = capture, iso = ISO_0
            0x01000113, 0x04410002, 0x89090000, 0x0010000A, 0x00A06428, 0x0C101010, 0x18180000, 0x00A06428, 0x10101010, 0x00000000,
            0x00000000, 0x00000000, 0x00C08040, 0x12101010, 0x00000000, 0x06040302, 0x001E100A, 0x08060402, 0x00080004, 0x10305405,
            0x00308003, 0x0A300300, 0x1613100D, 0x00000000, 0xB9193C78, 0x00060000, 0x00000000, 0x10001008, 0x00010010, 0x00000000,
            0x00200020, 0x1E051E0E, 0x2805280E, 0x4005400E, 0x00040000, 0x00000000, 0x00000000, 0x2D002D00, 0x01300000, 0x00002020,
            0xFF00FF00, 0x09070503, 0x00004668, 0x0204404A
        }},
        {set:{//ANR_custom: profile = capture, sensor mode = capture, iso = ISO_1
            0x01000113, 0x04410002, 0x89090000, 0x0010000A, 0x00A06428, 0x0C101010, 0x18170000, 0x00A06428, 0x10101010, 0x00000000,
            0x00000000, 0x00000000, 0x00C08040, 0x12101010, 0x00040000, 0x06040503, 0x001E100A, 0x08060402, 0x00080004, 0x10305405,
            0x00308003, 0x0A300300, 0x1613100D, 0x00000000, 0xB9193C78, 0x00060000, 0x00000000, 0x10001008, 0x00010010, 0x00000000,
            0x00200020, 0x1905190E, 0x2305230E, 0x3505350E, 0x00040000, 0x00000000, 0x00000000, 0x29002900, 0x01300000, 0x0000201C,
            0xFF00FF00, 0x0A080503, 0x00004668, 0x0204404A
        }},
        {set:{//ANR_custom: profile = capture, sensor mode = capture, iso = ISO_2
            0x01000113, 0x04410002, 0x89090000, 0x0010000A, 0x00A06428, 0x0C101010, 0x18180000, 0x00A06428, 0x10101010, 0x00000000,
            0x00000000, 0x00000000, 0x00C08040, 0x12101010, 0x00000000, 0x06040804, 0x001E100A, 0x08060402, 0x00080004, 0x10305405,
            0x00308003, 0x0A300300, 0x1613100D, 0x00000000, 0xB9193C78, 0x00060000, 0x00000000, 0x10001008, 0x00010010, 0x00000000,
            0x00200020, 0x0F050F0E, 0x1905190E, 0x1E051E0E, 0x00040000, 0x00000000, 0x00000000, 0x20002000, 0x01300000, 0x00002014,
            0xFF00FF00, 0x0C090603, 0x00004668, 0x0204404A
        }},
        {set:{//ANR_custom: profile = capture, sensor mode = capture, iso = ISO_3
            0x01000113, 0x04410002, 0x89090000, 0x0010000A, 0x00A06428, 0x0C101010, 0x18180000, 0x00A06428, 0x10101010, 0x00000000,
            0x00000000, 0x00000000, 0x00C08040, 0x1C161210, 0x000C0804, 0x0A080603, 0x001E100A, 0x0C090603, 0x00080008, 0x10305405,
            0x00308003, 0x0A300300, 0x1613100D, 0x00000000, 0xB9193C78, 0x00060000, 0x00000000, 0x10001008, 0x00010010, 0x00000000,
            0x00200020, 0x1405140E, 0x1E051E0E, 0x1E051E0E, 0x00040000, 0x00000000, 0x00000000, 0x40004000, 0x01300000, 0x00002014,
            0xFF00FF00, 0x0C090603, 0x00004668, 0x0204404A
        }},
        {set:{//ANR_custom: profile = capture, sensor mode = capture, iso = ISO_4
            0x01000113, 0x02410102, 0x89090000, 0x0010000A, 0x00A06428, 0x0C101010, 0x18180000, 0x00A06428, 0x10101010, 0x00000000,
            0x00000000, 0x00000000, 0x00C08040, 0x1C161010, 0x000C0C00, 0x08060402, 0x001E100A, 0x140F0A05, 0x00080004, 0x10305405,
            0x00308003, 0x0A300300, 0x1613100D, 0x00000000, 0xB9193C78, 0x00060000, 0x00000000, 0x10000008, 0x00010010, 0x00000000,
            0x00200020, 0x14051414, 0x14051414, 0x14051414, 0x00040010, 0x12041210, 0x00000010, 0x40004000, 0x01300000, 0x00002020,
            0xFF00FF00, 0x08060402, 0x00004668, 0x0204404A
        }},
        {set:{//ANR_custom: profile = capture, sensor mode = capture, iso = ISO_5
            0x01000113, 0x02410102, 0x89090000, 0x0010000A, 0x00A06428, 0x0C101010, 0x18180000, 0x00A06428, 0x10101010, 0x00000000,
            0x00000000, 0x00000000, 0x00C08040, 0x1C161010, 0x000C0C00, 0x08060402, 0x001E100A, 0x18120C06, 0x00080004, 0x10305405,
            0x00308003, 0x0A300300, 0x1613100D, 0x00000000, 0xB9193C78, 0x00060000, 0x00000000, 0x10000008, 0x00010010, 0x00000000,
            0x00200020, 0x14051414, 0x14051414, 0x14051414, 0x00040010, 0x12041210, 0x00000010, 0x40004000, 0x01300000, 0x00002020,
            0xFF00FF00, 0x08060402, 0x00004668, 0x0204404A
        }},
        {set:{//ANR_custom: profile = capture, sensor mode = capture, iso = ISO_6
            0x01000113, 0x02410102, 0x89090000, 0x0010000A, 0x00A06428, 0x0C101010, 0x18170000, 0x00A06428, 0x10101010, 0x00000000,
            0x00000000, 0x00000000, 0x00C08040, 0x1C161010, 0x000C0C00, 0x08060402, 0x001E100A, 0x18120C06, 0x00080004, 0x10305405,
            0x00308003, 0x0A300300, 0x1613100D, 0x00000000, 0xB9193C78, 0x00060000, 0x00000000, 0x10000008, 0x00010010, 0x00000000,
            0x00200020, 0x14051414, 0x14051414, 0x14051414, 0x00040010, 0x12041210, 0x00000010, 0x40004000, 0x01300000, 0x00002020,
            0xFF00FF00, 0x08060402, 0x00004668, 0x0204404A
        }},
        {set:{//ANR_custom: profile = capture, sensor mode = capture, iso = ISO_7
            0x01000113, 0x02410102, 0x89090000, 0x0010000A, 0x00A06428, 0x0C101010, 0x18170000, 0x00A06428, 0x10101010, 0x00000000,
            0x00000000, 0x00000000, 0x00C08040, 0x1C161010, 0x000C0C00, 0x08060402, 0x001E100A, 0x18120C06, 0x00080004, 0x10305405,
            0x00308003, 0x0A300300, 0x1613100D, 0x00000000, 0xB9193C78, 0x00060000, 0x00000000, 0x10000008, 0x00010010, 0x00000000,
            0x00200020, 0x14051414, 0x14051414, 0x14051414, 0x00040010, 0x12041210, 0x00000010, 0x40004000, 0x01300000, 0x00002020,
            0xFF00FF00, 0x08060402, 0x00004668, 0x0204404A
        }},
        {set:{//ANR_custom: profile = capture, sensor mode = capture, iso = ISO_8
            0x01000113, 0x02410102, 0x89090000, 0x0010000A, 0x00A06428, 0x0C101010, 0x18170000, 0x00A06428, 0x10101010, 0x00000000,
            0x00000000, 0x00000000, 0x00C08040, 0x1C161010, 0x000C0C00, 0x08060402, 0x001E100A, 0x18120C06, 0x00080004, 0x10305405,
            0x00308003, 0x0A300300, 0x1613100D, 0x00000000, 0xB9193C78, 0x00060000, 0x00000000, 0x10000008, 0x00010010, 0x00000000,
            0x00200020, 0x14051414, 0x14051414, 0x14051414, 0x00040010, 0x12041210, 0x00000010, 0x40004000, 0x01300000, 0x00002020,
            0xFF00FF00, 0x08060402, 0x00004668, 0x0204404A
        }},
        {set:{//ANR_custom: profile = capture, sensor mode = capture, iso = ISO_9
            0x01100133, 0x02410102, 0x89090808, 0x0010000A, 0x00E0A000, 0x0E101010, 0x1F1C0000, 0x00A06428, 0x10101010, 0x00000000,
            0x00000000, 0x00000000, 0x00C08040, 0x1C161010, 0x000C0C00, 0x140F0A05, 0x001E100A, 0x18120C06, 0x00080006, 0x3C305302,
            0x0C50C0C5, 0x1430C506, 0x32302C24, 0x00000000, 0xB9193C78, 0x00060000, 0x00000000, 0x10000008, 0x00010010, 0x00000000,
            0x00200020, 0x0A050A1E, 0x1E051E1E, 0x0A050A1E, 0x00040000, 0x12040000, 0x00000010, 0x40004000, 0x01300808, 0x00002020,
            0xFF00FF00, 0x08060402, 0x00004668, 0x0204404A
        }},
        {set:{//ANR_custom: profile = capture, sensor mode = capture, iso = ISO_10
            0x01100133, 0x02410102, 0x89090808, 0x0010000A, 0x00E0A000, 0x0E101010, 0x1F1C0000, 0x00A06428, 0x10101010, 0x00000000,
            0x00000000, 0x00000000, 0x00C08040, 0x1C161010, 0x000C0C00, 0x140F0A05, 0x001E100A, 0x18120C06, 0x00080006, 0x41305302,
            0x0C50C0C5, 0x1430C506, 0x32302C24, 0x00000000, 0xB9193C78, 0x00060000, 0x00000000, 0x10000008, 0x00010010, 0x00000000,
            0x00200020, 0x0A050A1E, 0x1E051E1E, 0x0A050A1E, 0x00040000, 0x12040000, 0x00000010, 0x40004000, 0x01300808, 0x00002020,
            0xFF00FF00, 0x08060402, 0x00004668, 0x0204404A
        }},
        {set:{//ANR_custom: profile = capture, sensor mode = capture, iso = ISO_11
            0x01100133, 0x02410102, 0x89090808, 0x0010000A, 0x00E0A000, 0x0E101010, 0x1F1C0000, 0x00A06428, 0x10101010, 0x00000000,
            0x00000000, 0x00000000, 0x00C08040, 0x1C161010, 0x000C0C00, 0x140F0A05, 0x001E100A, 0x18120C06, 0x00080006, 0x46305302,
            0x0C50C0C5, 0x1430C506, 0x32302C24, 0x00000000, 0xB9193C78, 0x00060000, 0x00000000, 0x10000008, 0x00010010, 0x00000000,
            0x00200020, 0x0A050A1E, 0x1E051E1E, 0x0A050A1E, 0x00040000, 0x12040000, 0x00000010, 0x40004000, 0x01300808, 0x00002020,
            0xFF00FF00, 0x08060402, 0x00004668, 0x0204404A
        }},
        {set:{//ANR_custom: profile = capture, sensor mode = capture, iso = ISO_12
            0x01100133, 0x02410102, 0x89090808, 0x0010000A, 0x00E0A000, 0x0E101010, 0x1F1C0000, 0x00A06428, 0x10101010, 0x00000000,
            0x00000000, 0x00000000, 0x00C08040, 0x1C161010, 0x000C0C00, 0x140F0A05, 0x001E100A, 0x18120C06, 0x00080006, 0x46305302,
            0x0C50C0C5, 0x1430C506, 0x32302C24, 0x00000000, 0xB9193C78, 0x00060000, 0x00000000, 0x10000008, 0x00010010, 0x00000000,
            0x00200020, 0x0A050A1E, 0x1E051E1E, 0x0A050A1E, 0x00040000, 0x12040000, 0x00000010, 0x40004000, 0x01300808, 0x00002020,
            0xFF00FF00, 0x08060402, 0x00004668, 0x0204404A
        }},
        {set:{//ANR_custom: profile = capture, sensor mode = capture, iso = ISO_13
            0x01100133, 0x02410102, 0x89090808, 0x0010000A, 0x00E0A000, 0x0E101010, 0x1F1C0000, 0x00A06428, 0x10101010, 0x00000000,
            0x00000000, 0x00000000, 0x00C08040, 0x1C161010, 0x000C0C00, 0x140F0A05, 0x001E100A, 0x18120C06, 0x00080006, 0x46305302,
            0x0C50C0C5, 0x1430C506, 0x32302C24, 0x00000000, 0xB9193C78, 0x00060000, 0x00000000, 0x10000008, 0x00010010, 0x00000000,
            0x00200020, 0x0A050A1E, 0x1E051E1E, 0x0A050A1E, 0x00040000, 0x12040000, 0x00000010, 0x40004000, 0x01300808, 0x00002020,
            0xFF00FF00, 0x08060402, 0x00004668, 0x0204404A
        }},
        {set:{//ANR_custom: profile = capture, sensor mode = capture, iso = ISO_14
            0x01100133, 0x02410102, 0x89090808, 0x0010000A, 0x00E0A000, 0x0E101010, 0x1F1C0000, 0x00A06428, 0x10101010, 0x00000000,
            0x00000000, 0x00000000, 0x00C08040, 0x1C161010, 0x000C0C00, 0x140F0A05, 0x001E100A, 0x18120C06, 0x00080006, 0x46305302,
            0x0C50C0C5, 0x1430C506, 0x32302C24, 0x00000000, 0xB9193C78, 0x00060000, 0x00000000, 0x10000008, 0x00010010, 0x00000000,
            0x00200020, 0x0A050A1E, 0x1E051E1E, 0x0A050A1E, 0x00040000, 0x12040000, 0x00000010, 0x40004000, 0x01300808, 0x00002020,
            0xFF00FF00, 0x08060402, 0x00004668, 0x0204404A
        }},
        {set:{//ANR_custom: profile = capture, sensor mode = capture, iso = ISO_15
            0x01100133, 0x02410102, 0x89090808, 0x0010000A, 0x00E0A000, 0x0E101010, 0x1F1C0000, 0x00A06428, 0x10101010, 0x00000000,
            0x00000000, 0x00000000, 0x00C08040, 0x1C161010, 0x000C0C00, 0x140F0A05, 0x001E100A, 0x18120C06, 0x00080006, 0x46305302,
            0x0C50C0C5, 0x1430C506, 0x32302C24, 0x00000000, 0xB9193C78, 0x00060000, 0x00000000, 0x10000008, 0x00010010, 0x00000000,
            0x00200020, 0x0A050A1E, 0x1E051E1E, 0x0A050A1E, 0x00040000, 0x12040000, 0x00000010, 0x40004000, 0x01300808, 0x00002020,
            0xFF00FF00, 0x08060402, 0x00004668, 0x0204404A
        }},
        {set:{//ANR_custom: profile = capture, sensor mode = capture, iso = ISO_16
            0x01100133, 0x02410102, 0x89090808, 0x0010000A, 0x00E0A000, 0x0E101010, 0x1F1C0000, 0x00A06428, 0x10101010, 0x00000000,
            0x00000000, 0x00000000, 0x00C08040, 0x1C161010, 0x000C0C00, 0x140F0A05, 0x001E100A, 0x18120C06, 0x00080006, 0x46305302,
            0x0C50C0C5, 0x1430C506, 0x32302C24, 0x00000000, 0xB9193C78, 0x00060000, 0x00000000, 0x10000008, 0x00010010, 0x00000000,
            0x00200020, 0x0A050A1E, 0x1E051E1E, 0x0A050A1E, 0x00040000, 0x12040000, 0x00000010, 0x40004000, 0x01300808, 0x00002020,
            0xFF00FF00, 0x08060402, 0x00004668, 0x0204404A
        }},
        {set:{//ANR_custom: profile = capture, sensor mode = capture, iso = ISO_17
            0x01100133, 0x02410102, 0x89090808, 0x0010000A, 0x00E0A000, 0x0E101010, 0x1F1C0000, 0x00A06428, 0x10101010, 0x00000000,
            0x00000000, 0x00000000, 0x00C08040, 0x1C161010, 0x000C0C00, 0x140F0A05, 0x001E100A, 0x18120C06, 0x00080006, 0x46305302,
            0x0C50C0C5, 0x1430C506, 0x32302C24, 0x00000000, 0xB9193C78, 0x00060000, 0x00000000, 0x10000008, 0x00010010, 0x00000000,
            0x00200020, 0x0A050A1E, 0x1E051E1E, 0x0A050A1E, 0x00040000, 0x12040000, 0x00000010, 0x40004000, 0x01300808, 0x00002020,
            0xFF00FF00, 0x08060402, 0x00004668, 0x0204404A
        }},
        {set:{//ANR_custom: profile = capture, sensor mode = capture, iso = ISO_18
            0x01100133, 0x02410102, 0x89090808, 0x0010000A, 0x00E0A000, 0x0E101010, 0x1F1C0000, 0x00A06428, 0x10101010, 0x00000000,
            0x00000000, 0x00000000, 0x00C08040, 0x1C161010, 0x000C0C00, 0x140F0A05, 0x001E100A, 0x18120C06, 0x00080006, 0x46305302,
            0x0C50C0C5, 0x1430C506, 0x32302C24, 0x00000000, 0xB9193C78, 0x00060000, 0x00000000, 0x10000008, 0x00010010, 0x00000000,
            0x00200020, 0x0A050A1E, 0x1E051E1E, 0x0A050A1E, 0x00040000, 0x12040000, 0x00000010, 0x40004000, 0x01300808, 0x00002020,
            0xFF00FF00, 0x08060402, 0x00004668, 0x0204404A
        }},
        {set:{//ANR_custom: profile = capture, sensor mode = capture, iso = ISO_19
            0x01100133, 0x02410102, 0x89090808, 0x0010000A, 0x00E0A000, 0x0E101010, 0x1F1C0000, 0x00A06428, 0x10101010, 0x00000000,
            0x00000000, 0x00000000, 0x00C08040, 0x1C161010, 0x000C0C00, 0x140F0A05, 0x001E100A, 0x18120C06, 0x00080006, 0x46305302,
            0x0C50C0C5, 0x1430C506, 0x32302C24, 0x00000000, 0xB9193C78, 0x00060000, 0x00000000, 0x10000008, 0x00010010, 0x00000000,
            0x00200020, 0x0A050A1E, 0x1E051E1E, 0x0A050A1E, 0x00040000, 0x12040000, 0x00000010, 0x40004000, 0x01300808, 0x00002020,
            0xFF00FF00, 0x08060402, 0x00004668, 0x0204404A
        }}
    }
};

static ISP_NVRAM_ANR2_T customANR2_main[FINE_TUNE_SUPPORT_NUM][NVRAM_ISP_REGS_ISO_SUPPORT_NUM] =
{
    //set 1
    {
        {set:{//ANR2_custom: profile = capture, sensor mode = capture, iso = ISO_0
            0x01000031, 0x00000000, 0x80000000, 0x00A06428, 0x10101010, 0x18000000, 0x00C08040, 0x10101010, 0x00000000, 0x0028100A,
            0x04030201, 0x00080004, 0x10305405, 0x083040C3, 0x0A308300, 0x1613100D, 0x00000000, 0x40004000, 0x40004000
        }},
        {set:{//ANR2_custom: profile = capture, sensor mode = capture, iso = ISO_1
            0x01000031, 0x00000000, 0x80000000, 0x00A06428, 0x10101010, 0x18000000, 0x00C08040, 0x10101010, 0x00000000, 0x0028100A,
            0x04030201, 0x00080004, 0x10305405, 0x083040C3, 0x0A308300, 0x1613100D, 0x00000000, 0x40004000, 0x40004000
        }},
        {set:{//ANR2_custom: profile = capture, sensor mode = capture, iso = ISO_2
            0x01000033, 0x00000000, 0x80000000, 0x00A06428, 0x10101010, 0x18000000, 0x00C08040, 0x10101010, 0x00000000, 0x0028100A,
            0x04030201, 0x00080004, 0x10305405, 0x083040C3, 0x0A308300, 0x1613100D, 0x00000000, 0x40004000, 0x40004000
        }},
        {set:{//ANR2_custom: profile = capture, sensor mode = capture, iso = ISO_3
            0x01000033, 0x00000000, 0x80000000, 0x00A06428, 0x10101010, 0x18000000, 0x00C08040, 0x10101010, 0x00000000, 0x0028100A,
            0x04030201, 0x00080004, 0x10305405, 0x083040C3, 0x0A308300, 0x1613100D, 0x00000000, 0x40004000, 0x40004000
        }},
        {set:{//ANR2_custom: profile = capture, sensor mode = capture, iso = ISO_4
            0x01000033, 0x00000100, 0x80000000, 0x00A06428, 0x10101010, 0x18000000, 0x00C08040, 0x18141010, 0x00040400, 0x001E100A,
            0x09060403, 0x00080004, 0x2D305405, 0x0C3040C3, 0x0A30C300, 0x1613100D, 0x00000000, 0x40004000, 0x40004000
        }},
        {set:{//ANR2_custom: profile = capture, sensor mode = capture, iso = ISO_5
            0x01000033, 0x00000100, 0x80000000, 0x00A06428, 0x10101010, 0x18000000, 0x00C08040, 0x18141010, 0x00040400, 0x001E100A,
            0x09060403, 0x00080004, 0x2D305405, 0x0C3040C3, 0x0A30C300, 0x1613100D, 0x00000000, 0x40004000, 0x40004000
        }},
        {set:{//ANR2_custom: profile = capture, sensor mode = capture, iso = ISO_6
            0x01000033, 0x00000100, 0x80000000, 0x00A06428, 0x10101010, 0x18000000, 0x00C08040, 0x18141010, 0x00080800, 0x001E100A,
            0x09060403, 0x00080004, 0x2D305405, 0x0C3040C3, 0x0A30C300, 0x1613100D, 0x00000000, 0x40004000, 0x40004000
        }},
        {set:{//ANR2_custom: profile = capture, sensor mode = capture, iso = ISO_7
            0x01000033, 0x00000100, 0x80000000, 0x00A06428, 0x10101010, 0x18000000, 0x00C08040, 0x18141010, 0x00080800, 0x001E100A,
            0x09060403, 0x00080004, 0x2D305405, 0x0C3040C3, 0x0A30C300, 0x1613100D, 0x00000000, 0x40004000, 0x40004000
        }},
        {set:{//ANR2_custom: profile = capture, sensor mode = capture, iso = ISO_8
            0x01000033, 0x00000100, 0x80000000, 0x00A06428, 0x10101010, 0x18000000, 0x00C08040, 0x18141010, 0x00080800, 0x001E100A,
            0x09060403, 0x00080004, 0x2D305405, 0x0C3040C3, 0x0A30C300, 0x1613100D, 0x00000000, 0x40004000, 0x40004000
        }},
        {set:{//ANR2_custom: profile = capture, sensor mode = capture, iso = ISO_9
            0x01000033, 0x00000100, 0x80000000, 0x00A07828, 0x08101010, 0x1E110000, 0x00C08040, 0x1C161010, 0x020C0C00, 0x00180A0A,
            0x0C090603, 0x00040006, 0x3C305302, 0x105180C5, 0x14310506, 0x32302C24, 0x00000000, 0x0A084000, 0x10084000
        }},
        {set:{//ANR2_custom: profile = capture, sensor mode = capture, iso = ISO_10
            0x01000033, 0x00000100, 0x80000000, 0x00A07828, 0x08101010, 0x1E110000, 0x00C08040, 0x1C161010, 0x020C0C00, 0x00180A0A,
            0x0E0B0704, 0x00040006, 0x3C305302, 0x105180C5, 0x14310506, 0x32302C24, 0x00000000, 0x0A084000, 0x10084000
        }},
        {set:{//ANR2_custom: profile = capture, sensor mode = capture, iso = ISO_11
            0x01000033, 0x00000100, 0x80000000, 0x00A07828, 0x08101010, 0x1E110000, 0x00C08040, 0x1C161010, 0x020C0C00, 0x00180A0A,
            0x100C0804, 0x00040006, 0x3C305302, 0x105180C5, 0x14310506, 0x32302C24, 0x00000000, 0x0A084000, 0x10084000
        }},
        {set:{//ANR2_custom: profile = capture, sensor mode = capture, iso = ISO_12
            0x010F1003, 0x00000100, 0x80000000, 0x00A07828, 0x08101010, 0x1E1C0000, 0x00C08040, 0x100C0A08, 0x02020101, 0x00180A8A,
            0x06040302, 0x00040006, 0x20305405, 0x1030C103, 0x0A310330, 0x1613100D, 0x00000000, 0x0A083000, 0x10083000
        }},
        {set:{//ANR2_custom: profile = capture, sensor mode = capture, iso = ISO_13
            0x010F1003, 0x00000100, 0x80000000, 0x00A07828, 0x08101010, 0x1E1C0000, 0x00C08040, 0x100C0A08, 0x02020101, 0x00180A8A,
            0x06040302, 0x00040006, 0x20305405, 0x1030C103, 0x0A310330, 0x1613100D, 0x00000000, 0x0A083000, 0x10083000
        }},
        {set:{//ANR2_custom: profile = capture, sensor mode = capture, iso = ISO_14
            0x010F1003, 0x00000100, 0x80000000, 0x00A07828, 0x08101010, 0x1E1C0000, 0x00C08040, 0x100C0A08, 0x02020101, 0x00180A8A,
            0x06040302, 0x00040006, 0x20305405, 0x1030C103, 0x0A310330, 0x1613100D, 0x00000000, 0x0A083000, 0x10083000
        }},
        {set:{//ANR2_custom: profile = capture, sensor mode = capture, iso = ISO_15
            0x010F1003, 0x00000100, 0x80000000, 0x00A07828, 0x08101010, 0x1E1C0000, 0x00C08040, 0x100C0A08, 0x02020101, 0x00180A8A,
            0x06040302, 0x00040006, 0x20305405, 0x1030C103, 0x0A310330, 0x1613100D, 0x00000000, 0x0A083000, 0x10083000
        }},
        {set:{//ANR2_custom: profile = capture, sensor mode = capture, iso = ISO_16
            0x010F1003, 0x00000100, 0x80000000, 0x00A07828, 0x08101010, 0x1E1C0000, 0x00C08040, 0x100C0A08, 0x02020101, 0x00180A8A,
            0x06040302, 0x00040006, 0x20305405, 0x1030C103, 0x0A310330, 0x1613100D, 0x00000000, 0x0A083000, 0x10083000
        }},
        {set:{//ANR2_custom: profile = capture, sensor mode = capture, iso = ISO_17
            0x010F1003, 0x00000100, 0x80000000, 0x00A07828, 0x08101010, 0x1E1C0000, 0x00C08040, 0x100C0A08, 0x02020101, 0x00180A8A,
            0x06040302, 0x00040006, 0x20305405, 0x1030C103, 0x0A310330, 0x1613100D, 0x00000000, 0x0A083000, 0x10083000
        }},
        {set:{//ANR2_custom: profile = capture, sensor mode = capture, iso = ISO_18
            0x010F1003, 0x00000100, 0x80000000, 0x00A07828, 0x08101010, 0x1E1C0000, 0x00C08040, 0x100C0A08, 0x02020101, 0x00180A8A,
            0x06040302, 0x00040006, 0x20305405, 0x1030C103, 0x0A310330, 0x1613100D, 0x00000000, 0x0A083000, 0x10083000
        }},
        {set:{//ANR2_custom: profile = capture, sensor mode = capture, iso = ISO_19
            0x010F1003, 0x00000100, 0x80000000, 0x00A07828, 0x08101010, 0x1E1C0000, 0x00C08040, 0x100C0A08, 0x02020101, 0x00180A8A,
            0x06040302, 0x00040006, 0x20305405, 0x1030C103, 0x0A310330, 0x1613100D, 0x00000000, 0x0A083000, 0x10083000
        }}
    },

    //set 2
    {
        {set:{//ANR2_custom: profile = capture, sensor mode = capture, iso = ISO_0
            0x01000031, 0x00000000, 0x80000000, 0x00A06428, 0x10101010, 0x18000000, 0x00C08040, 0x10101010, 0x00000000, 0x0028100A,
            0x04030201, 0x00080004, 0x10305405, 0x083040C3, 0x0A308300, 0x1613100D, 0x00000000, 0x40004000, 0x40004000
        }},
        {set:{//ANR2_custom: profile = capture, sensor mode = capture, iso = ISO_1
            0x01000031, 0x00000000, 0x80000000, 0x00A06428, 0x10101010, 0x18000000, 0x00C08040, 0x10101010, 0x00000000, 0x0028100A,
            0x04030201, 0x00080004, 0x10305405, 0x083040C3, 0x0A308300, 0x1613100D, 0x00000000, 0x40004000, 0x40004000
        }},
        {set:{//ANR2_custom: profile = capture, sensor mode = capture, iso = ISO_2
            0x01000033, 0x00000000, 0x80000000, 0x00A06428, 0x10101010, 0x18000000, 0x00C08040, 0x10101010, 0x00000000, 0x0028100A,
            0x04030201, 0x00080004, 0x10305405, 0x083040C3, 0x0A308300, 0x1613100D, 0x00000000, 0x40004000, 0x40004000
        }},
        {set:{//ANR2_custom: profile = capture, sensor mode = capture, iso = ISO_3
            0x01000033, 0x00000000, 0x80000000, 0x00A06428, 0x10101010, 0x18000000, 0x00C08040, 0x10101010, 0x00000000, 0x0028100A,
            0x04030201, 0x00080004, 0x10305405, 0x083040C3, 0x0A308300, 0x1613100D, 0x00000000, 0x40004000, 0x40004000
        }},
        {set:{//ANR2_custom: profile = capture, sensor mode = capture, iso = ISO_4
            0x01000033, 0x00000100, 0x80000000, 0x00A06428, 0x10101010, 0x18000000, 0x00C08040, 0x18141010, 0x00040400, 0x001E100A,
            0x09060403, 0x00080004, 0x2D305405, 0x0C3040C3, 0x0A30C300, 0x1613100D, 0x00000000, 0x40004000, 0x40004000
        }},
        {set:{//ANR2_custom: profile = capture, sensor mode = capture, iso = ISO_5
            0x01000033, 0x00000100, 0x80000000, 0x00A06428, 0x10101010, 0x18000000, 0x00C08040, 0x18141010, 0x00040400, 0x001E100A,
            0x09060403, 0x00080004, 0x2D305405, 0x0C3040C3, 0x0A30C300, 0x1613100D, 0x00000000, 0x40004000, 0x40004000
        }},
        {set:{//ANR2_custom: profile = capture, sensor mode = capture, iso = ISO_6
            0x01000033, 0x00000100, 0x80000000, 0x00A06428, 0x10101010, 0x18000000, 0x00C08040, 0x18141010, 0x00080800, 0x001E100A,
            0x09060403, 0x00080004, 0x2D305405, 0x0C3040C3, 0x0A30C300, 0x1613100D, 0x00000000, 0x40004000, 0x40004000
        }},
        {set:{//ANR2_custom: profile = capture, sensor mode = capture, iso = ISO_7
            0x01000033, 0x00000100, 0x80000000, 0x00A06428, 0x10101010, 0x18000000, 0x00C08040, 0x18141010, 0x00080800, 0x001E100A,
            0x09060403, 0x00080004, 0x2D305405, 0x0C3040C3, 0x0A30C300, 0x1613100D, 0x00000000, 0x40004000, 0x40004000
        }},
        {set:{//ANR2_custom: profile = capture, sensor mode = capture, iso = ISO_8
            0x01000033, 0x00000100, 0x80000000, 0x00A06428, 0x10101010, 0x18000000, 0x00C08040, 0x18141010, 0x00080800, 0x001E100A,
            0x09060403, 0x00080004, 0x2D305405, 0x0C3040C3, 0x0A30C300, 0x1613100D, 0x00000000, 0x40004000, 0x40004000
        }},
        {set:{//ANR2_custom: profile = capture, sensor mode = capture, iso = ISO_9
            0x01000033, 0x00000100, 0x80000000, 0x00A07828, 0x08101010, 0x1E110000, 0x00C08040, 0x1C161010, 0x020C0C00, 0x00180A0A,
            0x0C090603, 0x00040006, 0x3C305302, 0x105180C5, 0x14310506, 0x32302C24, 0x00000000, 0x0A084000, 0x10084000
        }},
        {set:{//ANR2_custom: profile = capture, sensor mode = capture, iso = ISO_10
            0x01000033, 0x00000100, 0x80000000, 0x00A07828, 0x08101010, 0x1E110000, 0x00C08040, 0x1C161010, 0x020C0C00, 0x00180A0A,
            0x0E0B0704, 0x00040006, 0x3C305302, 0x105180C5, 0x14310506, 0x32302C24, 0x00000000, 0x0A084000, 0x10084000
        }},
        {set:{//ANR2_custom: profile = capture, sensor mode = capture, iso = ISO_11
            0x01000033, 0x00000100, 0x80000000, 0x00A07828, 0x08101010, 0x1E110000, 0x00C08040, 0x1C161010, 0x020C0C00, 0x00180A0A,
            0x100C0804, 0x00040006, 0x3C305302, 0x105180C5, 0x14310506, 0x32302C24, 0x00000000, 0x0A084000, 0x10084000
        }},
        {set:{//ANR2_custom: profile = capture, sensor mode = capture, iso = ISO_12
            0x010F1003, 0x00000100, 0x80000000, 0x00A07828, 0x08101010, 0x1E1C0000, 0x00C08040, 0x100C0A08, 0x02020101, 0x00180A8A,
            0x06040302, 0x00040006, 0x20305405, 0x1030C103, 0x0A310330, 0x1613100D, 0x00000000, 0x0A083000, 0x10083000
        }},
        {set:{//ANR2_custom: profile = capture, sensor mode = capture, iso = ISO_13
            0x010F1003, 0x00000100, 0x80000000, 0x00A07828, 0x08101010, 0x1E1C0000, 0x00C08040, 0x100C0A08, 0x02020101, 0x00180A8A,
            0x06040302, 0x00040006, 0x20305405, 0x1030C103, 0x0A310330, 0x1613100D, 0x00000000, 0x0A083000, 0x10083000
        }},
        {set:{//ANR2_custom: profile = capture, sensor mode = capture, iso = ISO_14
            0x010F1003, 0x00000100, 0x80000000, 0x00A07828, 0x08101010, 0x1E1C0000, 0x00C08040, 0x100C0A08, 0x02020101, 0x00180A8A,
            0x06040302, 0x00040006, 0x20305405, 0x1030C103, 0x0A310330, 0x1613100D, 0x00000000, 0x0A083000, 0x10083000
        }},
        {set:{//ANR2_custom: profile = capture, sensor mode = capture, iso = ISO_15
            0x010F1003, 0x00000100, 0x80000000, 0x00A07828, 0x08101010, 0x1E1C0000, 0x00C08040, 0x100C0A08, 0x02020101, 0x00180A8A,
            0x06040302, 0x00040006, 0x20305405, 0x1030C103, 0x0A310330, 0x1613100D, 0x00000000, 0x0A083000, 0x10083000
        }},
        {set:{//ANR2_custom: profile = capture, sensor mode = capture, iso = ISO_16
            0x010F1003, 0x00000100, 0x80000000, 0x00A07828, 0x08101010, 0x1E1C0000, 0x00C08040, 0x100C0A08, 0x02020101, 0x00180A8A,
            0x06040302, 0x00040006, 0x20305405, 0x1030C103, 0x0A310330, 0x1613100D, 0x00000000, 0x0A083000, 0x10083000
        }},
        {set:{//ANR2_custom: profile = capture, sensor mode = capture, iso = ISO_17
            0x010F1003, 0x00000100, 0x80000000, 0x00A07828, 0x08101010, 0x1E1C0000, 0x00C08040, 0x100C0A08, 0x02020101, 0x00180A8A,
            0x06040302, 0x00040006, 0x20305405, 0x1030C103, 0x0A310330, 0x1613100D, 0x00000000, 0x0A083000, 0x10083000
        }},
        {set:{//ANR2_custom: profile = capture, sensor mode = capture, iso = ISO_18
            0x010F1003, 0x00000100, 0x80000000, 0x00A07828, 0x08101010, 0x1E1C0000, 0x00C08040, 0x100C0A08, 0x02020101, 0x00180A8A,
            0x06040302, 0x00040006, 0x20305405, 0x1030C103, 0x0A310330, 0x1613100D, 0x00000000, 0x0A083000, 0x10083000
        }},
        {set:{//ANR2_custom: profile = capture, sensor mode = capture, iso = ISO_19
            0x010F1003, 0x00000100, 0x80000000, 0x00A07828, 0x08101010, 0x1E1C0000, 0x00C08040, 0x100C0A08, 0x02020101, 0x00180A8A,
            0x06040302, 0x00040006, 0x20305405, 0x1030C103, 0x0A310330, 0x1613100D, 0x00000000, 0x0A083000, 0x10083000
        }}
    }

};

static ISP_NVRAM_EE_T customEE_main[FINE_TUNE_SUPPORT_NUM][NVRAM_ISP_REGS_ISO_SUPPORT_NUM] =
{
    //set 1
    {
        {set:{//EE_custom: profile = capture, sensor mode = capture, iso = ISO_0
            0x00000680, 0x64323214, 0x00000802, 0x00000000, 0x030C0C0C, 0x0014140A, 0x00FF00FF, 0x00FF00FF, 0x00FF00FF, 0x000000FF,
            0x0000FFFF, 0x00100020, 0x00000028, 0x00281050, 0x00643064, 0x00961096, 0x00000000, 0x0000C8C8
        }},
        {set:{//EE_custom: profile = capture, sensor mode = capture, iso = ISO_1
            0x00000680, 0x53643212, 0x00000802, 0x00000000, 0x030C0C0C, 0x0012140A, 0x00FF00FF, 0x00FF00FF, 0x00FF00FF, 0x000000FF,
            0x0000FFFF, 0x00100020, 0x00000028, 0x00281050, 0x00643064, 0x00961096, 0x00000000, 0x0000C8C8
        }},
        {set:{//EE_custom: profile = capture, sensor mode = capture, iso = ISO_2
            0x00000680, 0x32C8320F, 0x00000802, 0x00000000, 0x030C0C0C, 0x000F140A, 0x00FF00FF, 0x00FF00FF, 0x00FF00FF, 0x000000FF,
            0x0000FFFF, 0x00100020, 0x00000028, 0x00281050, 0x00643064, 0x00961096, 0x00000000, 0x0000C8C8
        }},
        {set:{//EE_custom: profile = capture, sensor mode = capture, iso = ISO_3
            0x00000680, 0x96323214, 0x00000802, 0x00000000, 0x030C0C0C, 0x00140F0A, 0x00FF00FF, 0x00FF00FF, 0x00FF00FF, 0x000000FF,
            0x0000AAD2, 0x00101020, 0x0000003C, 0x0028155A, 0x00646064, 0x00961096, 0x00000000, 0x0000C8C8
        }},
        {set:{//EE_custom: profile = capture, sensor mode = capture, iso = ISO_4
            0x00000680, 0xFF000014, 0x00000802, 0x00000000, 0x030C0C0C, 0x000A0A0A, 0x00FF00FF, 0x00FF00FF, 0x00FF00FF, 0x000000FF,
            0x0000FFFF, 0x00100020, 0x00140923, 0x00C8733C, 0x01321491, 0x0102F6DC, 0x0000B800, 0x0000FFFF
        }},
        {set:{//EE_custom: profile = capture, sensor mode = capture, iso = ISO_5
            0x00000680, 0xFF00000F, 0x00000802, 0x00000000, 0x030C0C0C, 0x000A0A0A, 0x00FF00FF, 0x00FF00FF, 0x00FF00FF, 0x000000FF,
            0x0000FFFF, 0x00100020, 0x00140923, 0x00C8733C, 0x01321491, 0x0102F6DC, 0x0000B800, 0x0000FFFF
        }},
        {set:{//EE_custom: profile = capture, sensor mode = capture, iso = ISO_6
            0x00000680, 0xFF00000F, 0x00000802, 0x00000000, 0x030C0C0C, 0x000A0A0A, 0x00FF00FF, 0x00FF00FF, 0x00FF00FF, 0x000000FF,
            0x0000FFFF, 0x00100020, 0x00140923, 0x00C8733C, 0x01321491, 0x0102F6DC, 0x0000B800, 0x0000FFFF
        }},
        {set:{//EE_custom: profile = capture, sensor mode = capture, iso = ISO_7
            0x00000680, 0xFF00000F, 0x00000802, 0x00000000, 0x030C0C0C, 0x000A0A0A, 0x00FF00FF, 0x00FF00FF, 0x00FF00FF, 0x000000FF,
            0x0000FFFF, 0x00100020, 0x00140923, 0x00C8733C, 0x01321491, 0x0102F6DC, 0x0000B800, 0x0000FFFF
        }},
        {set:{//EE_custom: profile = capture, sensor mode = capture, iso = ISO_8
            0x00000680, 0xFF00000F, 0x00000802, 0x00000000, 0x030C0C0C, 0x000A0A0A, 0x00FF00FF, 0x00FF00FF, 0x00FF00FF, 0x000000FF,
            0x0000FFFF, 0x00100020, 0x00140923, 0x00C8733C, 0x01321491, 0x0102F6DC, 0x0000B800, 0x0000FFFF
        }},
        {set:{//EE_custom: profile = capture, sensor mode = capture, iso = ISO_9
            0x00000680, 0xFF00000A, 0x00000802, 0x00000000, 0x030C0C0C, 0x00060606, 0x00FF00FF, 0x00FF00FF, 0x00FF00FF, 0x000000FF,
            0x0000FFFF, 0x00100020, 0x00140923, 0x00C8733C, 0x01321491, 0x0102F6DC, 0x0000B800, 0x0000FFFF
        }},
        {set:{//EE_custom: profile = capture, sensor mode = capture, iso = ISO_10
            0x00000680, 0xFF00000A, 0x00000802, 0x00000000, 0x030C0C0C, 0x00060606, 0x00FF00FF, 0x00FF00FF, 0x00FF00FF, 0x000000FF,
            0x0000FFFF, 0x00100020, 0x00140923, 0x00C8733C, 0x01321491, 0x0102F6DC, 0x0000B800, 0x0000FFFF
        }},
        {set:{//EE_custom: profile = capture, sensor mode = capture, iso = ISO_11
            0x00000680, 0xFF00000A, 0x00000802, 0x00000000, 0x030C0C0C, 0x00060606, 0x00FF00FF, 0x00FF00FF, 0x00FF00FF, 0x000000FF,
            0x0000FFFF, 0x00100020, 0x00140923, 0x00C8733C, 0x01321491, 0x0102F6DC, 0x0000B800, 0x0000FFFF
        }},
        {set:{//EE_custom: profile = capture, sensor mode = capture, iso = ISO_12
            0x00000680, 0xFF000040, 0x00000802, 0x00000000, 0x030C0C0C, 0x0101040E, 0x00FF00FF, 0x00FF00FF, 0x00FF00FF, 0x000000FF,
            0x0000FFFF, 0x00100020, 0x00140123, 0x0266183C, 0x0133F864, 0x0029FEDC, 0x0000FF00, 0x0000FFFF
        }},
        {set:{//EE_custom: profile = capture, sensor mode = capture, iso = ISO_13
            0x00000680, 0xFF000040, 0x00000802, 0x00000000, 0x030C0C0C, 0x0101040E, 0x00FF00FF, 0x00FF00FF, 0x00FF00FF, 0x000000FF,
            0x0000FFFF, 0x00100020, 0x00140123, 0x0266183C, 0x0133F864, 0x0029FEDC, 0x0000FF00, 0x0000FFFF
        }},
        {set:{//EE_custom: profile = capture, sensor mode = capture, iso = ISO_14
            0x00000680, 0xFF000040, 0x00000802, 0x00000000, 0x030C0C0C, 0x0101040E, 0x00FF00FF, 0x00FF00FF, 0x00FF00FF, 0x000000FF,
            0x0000FFFF, 0x00100020, 0x00140123, 0x0266183C, 0x0133F864, 0x0029FEDC, 0x0000FF00, 0x0000FFFF
        }},
        {set:{//EE_custom: profile = capture, sensor mode = capture, iso = ISO_15
            0x00000680, 0xFF000040, 0x00000802, 0x00000000, 0x030C0C0C, 0x0101040E, 0x00FF00FF, 0x00FF00FF, 0x00FF00FF, 0x000000FF,
            0x0000FFFF, 0x00100020, 0x00140123, 0x0266183C, 0x0133F864, 0x0029FEDC, 0x0000FF00, 0x0000FFFF
        }},
        {set:{//EE_custom: profile = capture, sensor mode = capture, iso = ISO_16
            0x00000680, 0xFF000040, 0x00000802, 0x00000000, 0x030C0C0C, 0x0101040E, 0x00FF00FF, 0x00FF00FF, 0x00FF00FF, 0x000000FF,
            0x0000FFFF, 0x00100020, 0x00140123, 0x0266183C, 0x0133F864, 0x0029FEDC, 0x0000FF00, 0x0000FFFF
        }},
        {set:{//EE_custom: profile = capture, sensor mode = capture, iso = ISO_17
            0x00000680, 0xFF000040, 0x00000802, 0x00000000, 0x030C0C0C, 0x0101040E, 0x00FF00FF, 0x00FF00FF, 0x00FF00FF, 0x000000FF,
            0x0000FFFF, 0x00100020, 0x00140123, 0x0266183C, 0x0133F864, 0x0029FEDC, 0x0000FF00, 0x0000FFFF
        }},
        {set:{//EE_custom: profile = capture, sensor mode = capture, iso = ISO_18
            0x00000680, 0xFF000040, 0x00000802, 0x00000000, 0x030C0C0C, 0x0101040E, 0x00FF00FF, 0x00FF00FF, 0x00FF00FF, 0x000000FF,
            0x0000FFFF, 0x00100020, 0x00140123, 0x0266183C, 0x0133F864, 0x0029FEDC, 0x0000FF00, 0x0000FFFF
        }},
        {set:{//EE_custom: profile = capture, sensor mode = capture, iso = ISO_19
            0x00000680, 0xFF000040, 0x00000802, 0x00000000, 0x030C0C0C, 0x0101040E, 0x00FF00FF, 0x00FF00FF, 0x00FF00FF, 0x000000FF,
            0x0000FFFF, 0x00100020, 0x00140123, 0x0266183C, 0x0133F864, 0x0029FEDC, 0x0000FF00, 0x0000FFFF
        }}
    },

    //set 2
    {
        {set:{//EE_custom: profile = capture, sensor mode = capture, iso = ISO_0
            0x00000680, 0x64323214, 0x00000802, 0x00000000, 0x030C0C0C, 0x0014140A, 0x00FF00FF, 0x00FF00FF, 0x00FF00FF, 0x000000FF,
            0x0000FFFF, 0x00100020, 0x00000028, 0x00281050, 0x00643064, 0x00961096, 0x00000000, 0x0000C8C8
        }},
        {set:{//EE_custom: profile = capture, sensor mode = capture, iso = ISO_1
            0x00000680, 0x53643212, 0x00000802, 0x00000000, 0x030C0C0C, 0x0012140A, 0x00FF00FF, 0x00FF00FF, 0x00FF00FF, 0x000000FF,
            0x0000FFFF, 0x00100020, 0x00000028, 0x00281050, 0x00643064, 0x00961096, 0x00000000, 0x0000C8C8
        }},
        {set:{//EE_custom: profile = capture, sensor mode = capture, iso = ISO_2
            0x00000680, 0x32C8320F, 0x00000802, 0x00000000, 0x030C0C0C, 0x000F140A, 0x00FF00FF, 0x00FF00FF, 0x00FF00FF, 0x000000FF,
            0x0000FFFF, 0x00100020, 0x00000028, 0x00281050, 0x00643064, 0x00961096, 0x00000000, 0x0000C8C8
        }},
        {set:{//EE_custom: profile = capture, sensor mode = capture, iso = ISO_3
            0x00000680, 0x96323214, 0x00000802, 0x00000000, 0x030C0C0C, 0x00140F0A, 0x00FF00FF, 0x00FF00FF, 0x00FF00FF, 0x000000FF,
            0x0000AAD2, 0x00101020, 0x0000003C, 0x0028155A, 0x00646064, 0x00961096, 0x00000000, 0x0000C8C8
        }},
        {set:{//EE_custom: profile = capture, sensor mode = capture, iso = ISO_4
            0x00000680, 0xFF000014, 0x00000802, 0x00000000, 0x030C0C0C, 0x000A0A0A, 0x00FF00FF, 0x00FF00FF, 0x00FF00FF, 0x000000FF,
            0x0000FFFF, 0x00100020, 0x00140923, 0x00C8733C, 0x01321491, 0x0102F6DC, 0x0000B800, 0x0000FFFF
        }},
        {set:{//EE_custom: profile = capture, sensor mode = capture, iso = ISO_5
            0x00000680, 0xFF00000F, 0x00000802, 0x00000000, 0x030C0C0C, 0x000A0A0A, 0x00FF00FF, 0x00FF00FF, 0x00FF00FF, 0x000000FF,
            0x0000FFFF, 0x00100020, 0x00140923, 0x00C8733C, 0x01321491, 0x0102F6DC, 0x0000B800, 0x0000FFFF
        }},
        {set:{//EE_custom: profile = capture, sensor mode = capture, iso = ISO_6
            0x00000680, 0xFF00000F, 0x00000802, 0x00000000, 0x030C0C0C, 0x000A0A0A, 0x00FF00FF, 0x00FF00FF, 0x00FF00FF, 0x000000FF,
            0x0000FFFF, 0x00100020, 0x00140923, 0x00C8733C, 0x01321491, 0x0102F6DC, 0x0000B800, 0x0000FFFF
        }},
        {set:{//EE_custom: profile = capture, sensor mode = capture, iso = ISO_7
            0x00000680, 0xFF00000F, 0x00000802, 0x00000000, 0x030C0C0C, 0x000A0A0A, 0x00FF00FF, 0x00FF00FF, 0x00FF00FF, 0x000000FF,
            0x0000FFFF, 0x00100020, 0x00140923, 0x00C8733C, 0x01321491, 0x0102F6DC, 0x0000B800, 0x0000FFFF
        }},
        {set:{//EE_custom: profile = capture, sensor mode = capture, iso = ISO_8
            0x00000680, 0xFF00000F, 0x00000802, 0x00000000, 0x030C0C0C, 0x000A0A0A, 0x00FF00FF, 0x00FF00FF, 0x00FF00FF, 0x000000FF,
            0x0000FFFF, 0x00100020, 0x00140923, 0x00C8733C, 0x01321491, 0x0102F6DC, 0x0000B800, 0x0000FFFF
        }},
        {set:{//EE_custom: profile = capture, sensor mode = capture, iso = ISO_9
            0x00000680, 0xFF00000A, 0x00000802, 0x00000000, 0x030C0C0C, 0x00060606, 0x00FF00FF, 0x00FF00FF, 0x00FF00FF, 0x000000FF,
            0x0000FFFF, 0x00100020, 0x00140923, 0x00C8733C, 0x01321491, 0x0102F6DC, 0x0000B800, 0x0000FFFF
        }},
        {set:{//EE_custom: profile = capture, sensor mode = capture, iso = ISO_10
            0x00000680, 0xFF00000A, 0x00000802, 0x00000000, 0x030C0C0C, 0x00060606, 0x00FF00FF, 0x00FF00FF, 0x00FF00FF, 0x000000FF,
            0x0000FFFF, 0x00100020, 0x00140923, 0x00C8733C, 0x01321491, 0x0102F6DC, 0x0000B800, 0x0000FFFF
        }},
        {set:{//EE_custom: profile = capture, sensor mode = capture, iso = ISO_11
            0x00000680, 0xFF00000A, 0x00000802, 0x00000000, 0x030C0C0C, 0x00060606, 0x00FF00FF, 0x00FF00FF, 0x00FF00FF, 0x000000FF,
            0x0000FFFF, 0x00100020, 0x00140923, 0x00C8733C, 0x01321491, 0x0102F6DC, 0x0000B800, 0x0000FFFF
        }},
        {set:{//EE_custom: profile = capture, sensor mode = capture, iso = ISO_12
            0x00000680, 0xFF000040, 0x00000802, 0x00000000, 0x030C0C0C, 0x0101040E, 0x00FF00FF, 0x00FF00FF, 0x00FF00FF, 0x000000FF,
            0x0000FFFF, 0x00100020, 0x00140123, 0x0266183C, 0x0133F864, 0x0029FEDC, 0x0000FF00, 0x0000FFFF
        }},
        {set:{//EE_custom: profile = capture, sensor mode = capture, iso = ISO_13
            0x00000680, 0xFF000040, 0x00000802, 0x00000000, 0x030C0C0C, 0x0101040E, 0x00FF00FF, 0x00FF00FF, 0x00FF00FF, 0x000000FF,
            0x0000FFFF, 0x00100020, 0x00140123, 0x0266183C, 0x0133F864, 0x0029FEDC, 0x0000FF00, 0x0000FFFF
        }},
        {set:{//EE_custom: profile = capture, sensor mode = capture, iso = ISO_14
            0x00000680, 0xFF000040, 0x00000802, 0x00000000, 0x030C0C0C, 0x0101040E, 0x00FF00FF, 0x00FF00FF, 0x00FF00FF, 0x000000FF,
            0x0000FFFF, 0x00100020, 0x00140123, 0x0266183C, 0x0133F864, 0x0029FEDC, 0x0000FF00, 0x0000FFFF
        }},
        {set:{//EE_custom: profile = capture, sensor mode = capture, iso = ISO_15
            0x00000680, 0xFF000040, 0x00000802, 0x00000000, 0x030C0C0C, 0x0101040E, 0x00FF00FF, 0x00FF00FF, 0x00FF00FF, 0x000000FF,
            0x0000FFFF, 0x00100020, 0x00140123, 0x0266183C, 0x0133F864, 0x0029FEDC, 0x0000FF00, 0x0000FFFF
        }},
        {set:{//EE_custom: profile = capture, sensor mode = capture, iso = ISO_16
            0x00000680, 0xFF000040, 0x00000802, 0x00000000, 0x030C0C0C, 0x0101040E, 0x00FF00FF, 0x00FF00FF, 0x00FF00FF, 0x000000FF,
            0x0000FFFF, 0x00100020, 0x00140123, 0x0266183C, 0x0133F864, 0x0029FEDC, 0x0000FF00, 0x0000FFFF
        }},
        {set:{//EE_custom: profile = capture, sensor mode = capture, iso = ISO_17
            0x00000680, 0xFF000040, 0x00000802, 0x00000000, 0x030C0C0C, 0x0101040E, 0x00FF00FF, 0x00FF00FF, 0x00FF00FF, 0x000000FF,
            0x0000FFFF, 0x00100020, 0x00140123, 0x0266183C, 0x0133F864, 0x0029FEDC, 0x0000FF00, 0x0000FFFF
        }},
        {set:{//EE_custom: profile = capture, sensor mode = capture, iso = ISO_18
            0x00000680, 0xFF000040, 0x00000802, 0x00000000, 0x030C0C0C, 0x0101040E, 0x00FF00FF, 0x00FF00FF, 0x00FF00FF, 0x000000FF,
            0x0000FFFF, 0x00100020, 0x00140123, 0x0266183C, 0x0133F864, 0x0029FEDC, 0x0000FF00, 0x0000FFFF
        }},
        {set:{//EE_custom: profile = capture, sensor mode = capture, iso = ISO_19
            0x00000680, 0xFF000040, 0x00000802, 0x00000000, 0x030C0C0C, 0x0101040E, 0x00FF00FF, 0x00FF00FF, 0x00FF00FF, 0x000000FF,
            0x0000FFFF, 0x00100020, 0x00140123, 0x0266183C, 0x0133F864, 0x0029FEDC, 0x0000FF00, 0x0000FFFF
        }}
    }
};

MUINT32 WEIGHTING(MUINT32 x, MUINT32 y, float w) {
    MUINT32 z = (((x)*(w))+((y)*(1.0f-(w))));
    return z;
}

MINT32 AIS_Profile2Stage(MUINT32 profile) {
    MINT32 stage = -1;
    switch(profile) {
        case EIspProfile_MFNR_Before_Blend:
            stage = 1;
            break;

        case EIspProfile_MFNR_Single:
            stage = 2;
            break;

        case EIspProfile_MFNR_MFB:
            stage = 3;
            break;

        case EIspProfile_MFNR_After_Blend:
            stage = 4;
            break;

        default:
            stage = -1;
    }

    return stage;
}


MUINT32 get_normal_SWNR_ENC_enable_ISO_threshold()
{
#define NORMAL_SWNR_ENC_ENABLE_ISO_THRESHOLD (800) // enable if ISO >= THRESHOLD
    return NORMAL_SWNR_ENC_ENABLE_ISO_THRESHOLD;
}

MUINT32 get_MFB_SWNR_ENC_enable_ISO_threshold()
{
#define MFB_SWNR_ENC_ENABLE_ISO_THRESHOLD (800) // enable if ISO >= THRESHOLD
    return MFB_SWNR_ENC_ENABLE_ISO_THRESHOLD;
}

/*******************************************************************************
*
*   rCamInfo
*       [in]    ISP Camera Info for RAW sensor. Its members are as below:
*
*           eIspProfile:
*               EIspProfile_Preview = 0,          // Preview
*               EIspProfile_Video,                // Video
*               EIspProfile_Capture,              // Capture
*               EIspProfile_ZSD_Capture,          // ZSD Capture
*               EIspProfile_VSS_Capture,          // VSS Capture
*               EIspProfile_PureRAW_Capture,      // Pure RAW Capture
*               EIspProfile_N3D_Preview,          // N3D Preview
*               EIspProfile_N3D_Video,            // N3D Video
*               EIspProfile_N3D_Capture,          // N3D Capture
*               EIspProfile_MFB_Capture_EE_Off,   // MFB capture: EE off
*               EIspProfile_MFB_Blending_All_Off, // MFB blending: all off
*               EIspProfile_MFB_PostProc_EE_Off,  // MFB post process: capture + EE off
*               EIspProfile_MFB_PostProc_ANR_EE,  // MFB post process: capture + ANR + EE
*               EIspProfile_MFB_PostProc_Mixing,  // MFB post process: mixing + all off
*               EIspProfile_VFB_PostProc,         // VFB post process: all off + ANR + CCR + PCA
*               EIspProfile_IHDR_Preview,         // IHDR preview
*               EIspProfile_IHDR_Video,           // IHDR video
*
*           eSensorMode:
*               ESensorMode_Preview = 0,
*               ESensorMode_Capture,
*               ESensorMode_Video,
*               ESensorMode_SlimVideo1,
*               ESensorMode_SlimVideo2,
*
*           eIdx_Scene:
*               MTK_CONTROL_SCENE_MODE_UNSUPPORTED = 0,
*               MTK_CONTROL_SCENE_MODE_FACE_PRIORITY,
*               MTK_CONTROL_SCENE_MODE_ACTION,
*               MTK_CONTROL_SCENE_MODE_PORTRAIT,
*               MTK_CONTROL_SCENE_MODE_LANDSCAPE,
*               MTK_CONTROL_SCENE_MODE_NIGHT,
*               MTK_CONTROL_SCENE_MODE_NIGHT_PORTRAIT,
*               MTK_CONTROL_SCENE_MODE_THEATRE,
*               MTK_CONTROL_SCENE_MODE_BEACH,
*               MTK_CONTROL_SCENE_MODE_SNOW,
*               MTK_CONTROL_SCENE_MODE_SUNSET,
*               MTK_CONTROL_SCENE_MODE_STEADYPHOTO,
*               MTK_CONTROL_SCENE_MODE_FIREWORKS,
*               MTK_CONTROL_SCENE_MODE_SPORTS,
*               MTK_CONTROL_SCENE_MODE_PARTY,
*               MTK_CONTROL_SCENE_MODE_CANDLELIGHT,
*               MTK_CONTROL_SCENE_MODE_BARCODE,
*               MTK_CONTROL_SCENE_MODE_NORMAL,
*               MTK_CONTROL_SCENE_MODE_HDR,
*
*           u4ISOValue:
*               ISO value to determine eISO.
*
*           eIdx_ISO:
*               eIDX_ISO_100 = 0,
*               eIDX_ISO_200,
*               eIDX_ISO_400,
*               eIDX_ISO_800,
*               eIDX_ISO_1200,
*               eIDX_ISO_1600,
*               eIDX_ISO_2000,
*               eIDX_ISO_2400,
*               eIDX_ISO_2800,
*               eIDX_ISO_3200,
*
*           eIdx_PCA_LUT:
*               eIDX_PCA_LOW  = 0,
*               eIDX_PCA_MIDDLE,
*               eIDX_PCA_HIGH,
*               eIDX_PCA_LOW_2,    // for video HDR only
*               eIDX_PCA_MIDDLE_2, // for video HDR only
*               eIDX_PCA_HIGH_2    // for video HDR only
*
*           eIdx_CCM:
*               eIDX_CCM_D65  = 0,
*               eIDX_CCM_TL84,
*               eIDX_CCM_CWF,
*               eIDX_CCM_A,
*
*           eIdx_Shading_CCT:
*               eIDX_Shading_CCT_BEGIN = 0,
*               eIDX_Shading_CCT_ALight = eIDX_Shading_CCT_BEGIN,
*               eIDX_Shading_CCT_CWF,
*               eIDX_Shading_CCT_D65,
*               eIDX_Shading_CCT_RSVD
*
*           rAWBInfo:
*               rProb; // Light source probability
*               rLightStat; // Light source statistics
*               rLightAWBGain; // Golden sample's AWB gain for multi-CCM
*               rCurrentAWBGain; // Current preview AWB gain
*               i4NeutralParentBlkNum; // Neutral parent block number
*               i4CCT; // CCT
*               i4FluorescentIndex; // Fluorescent index
*               i4DaylightFluorescentIndex; // Daylight fluorescent index
*               i4SceneLV; // Scene LV
*               i4AWBMode; // AWB mode
*               bAWBStable; // AWB stable
*
*           rAEInfo:
*               u4AETarget;
*               u4AECurrentTarget;
*               u4Eposuretime;
*               u4AfeGain;
*               u4IspGain;
*               u4RealISOValue;
*               i4LightValue_x10;
*               u4AECondition;
*               eAEMeterMode;
*               i2FlareOffset;
*               u2Histogrm[AE_HISTOGRAM_BIN];
*           rAFInfo:
*               i4AFPos
*
*           rFlashInfo:
*               flashMode;
*               isFlash; // 0: no flash, 1: image with flash
*
*           u4ZoomRatio_x100:
*               zoom ratio (x100)
*
*           i4LightValue_x10:
*               light value (x10)
*
*******************************************************************************/
MVOID
IspTuningCustom::
refine_CamInfo(RAWIspCamInfo& rCamInfo)
{
}

/*******************************************************************************
*
*   rCamInfo
*       [in]    ISP Camera Info for RAW sensor. Its members are as below:
*
*           eIspProfile:
*               EIspProfile_Preview = 0,          // Preview
*               EIspProfile_Video,                // Video
*               EIspProfile_Capture,              // Capture
*               EIspProfile_ZSD_Capture,          // ZSD Capture
*               EIspProfile_VSS_Capture,          // VSS Capture
*               EIspProfile_PureRAW_Capture,      // Pure RAW Capture
*               EIspProfile_N3D_Preview,          // N3D Preview
*               EIspProfile_N3D_Video,            // N3D Video
*               EIspProfile_N3D_Capture,          // N3D Capture
*               EIspProfile_MFB_Capture_EE_Off,   // MFB capture: EE off
*               EIspProfile_MFB_Blending_All_Off, // MFB blending: all off
*               EIspProfile_MFB_PostProc_EE_Off,  // MFB post process: capture + EE off
*               EIspProfile_MFB_PostProc_ANR_EE,  // MFB post process: capture + ANR + EE
*               EIspProfile_MFB_PostProc_Mixing,  // MFB post process: mixing + all off
*               EIspProfile_VFB_PostProc,         // VFB post process: all off + ANR + CCR + PCA
*               EIspProfile_IHDR_Preview,         // IHDR preview
*               EIspProfile_IHDR_Video,           // IHDR video
*
*           eSensorMode:
*               ESensorMode_Preview = 0,
*               ESensorMode_Capture,
*               ESensorMode_Video,
*               ESensorMode_SlimVideo1,
*               ESensorMode_SlimVideo2,
*
*           eIdx_Scene:
*               MTK_CONTROL_SCENE_MODE_UNSUPPORTED = 0,
*               MTK_CONTROL_SCENE_MODE_FACE_PRIORITY,
*               MTK_CONTROL_SCENE_MODE_ACTION,
*               MTK_CONTROL_SCENE_MODE_PORTRAIT,
*               MTK_CONTROL_SCENE_MODE_LANDSCAPE,
*               MTK_CONTROL_SCENE_MODE_NIGHT,
*               MTK_CONTROL_SCENE_MODE_NIGHT_PORTRAIT,
*               MTK_CONTROL_SCENE_MODE_THEATRE,
*               MTK_CONTROL_SCENE_MODE_BEACH,
*               MTK_CONTROL_SCENE_MODE_SNOW,
*               MTK_CONTROL_SCENE_MODE_SUNSET,
*               MTK_CONTROL_SCENE_MODE_STEADYPHOTO,
*               MTK_CONTROL_SCENE_MODE_FIREWORKS,
*               MTK_CONTROL_SCENE_MODE_SPORTS,
*               MTK_CONTROL_SCENE_MODE_PARTY,
*               MTK_CONTROL_SCENE_MODE_CANDLELIGHT,
*               MTK_CONTROL_SCENE_MODE_BARCODE,
*               MTK_CONTROL_SCENE_MODE_NORMAL,
*               MTK_CONTROL_SCENE_MODE_HDR,
*
*           u4ISOValue:
*               ISO value to determine eISO.
*
*           eIdx_ISO:
*               eIDX_ISO_100 = 0,
*               eIDX_ISO_200,
*               eIDX_ISO_400,
*               eIDX_ISO_800,
*               eIDX_ISO_1200,
*               eIDX_ISO_1600,
*               eIDX_ISO_2000,
*               eIDX_ISO_2400,
*               eIDX_ISO_2800,
*               eIDX_ISO_3200,
*
*           eIdx_PCA_LUT:
*               eIDX_PCA_LOW  = 0,
*               eIDX_PCA_MIDDLE,
*               eIDX_PCA_HIGH,
*               eIDX_PCA_LOW_2,    // for video HDR only
*               eIDX_PCA_MIDDLE_2, // for video HDR only
*               eIDX_PCA_HIGH_2    // for video HDR only
*
*           eIdx_CCM:
*               eIDX_CCM_D65  = 0,
*               eIDX_CCM_TL84,
*               eIDX_CCM_CWF,
*               eIDX_CCM_A,
*
*           eIdx_Shading_CCT:
*               eIDX_Shading_CCT_BEGIN = 0,
*               eIDX_Shading_CCT_ALight = eIDX_Shading_CCT_BEGIN,
*               eIDX_Shading_CCT_CWF,
*               eIDX_Shading_CCT_D65,
*               eIDX_Shading_CCT_RSVD
*
*           rAWBInfo:
*               rProb; // Light source probability
*               rLightStat; // Light source statistics
*               rLightAWBGain; // Golden sample's AWB gain for multi-CCM
*               rCurrentAWBGain; // Current preview AWB gain
*               i4NeutralParentBlkNum; // Neutral parent block number
*               i4CCT; // CCT
*               i4FluorescentIndex; // Fluorescent index
*               i4DaylightFluorescentIndex; // Daylight fluorescent index
*               i4SceneLV; // Scene LV
*               i4AWBMode; // AWB mode
*               bAWBStable; // AWB stable
*
*           rAEInfo:
*               u4AETarget;
*               u4AECurrentTarget;
*               u4Eposuretime;
*               u4AfeGain;
*               u4IspGain;
*               u4RealISOValue;
*               i4LightValue_x10;
*               u4AECondition;
*               eAEMeterMode;
*               i2FlareOffset;
*               u2Histogrm[AE_HISTOGRAM_BIN];
*           rAFInfo:
*               i4AFPos
*
*           rFlashInfo:
*               flashMode;
*               isFlash; // 0: no flash, 1: image with flash
*
*           u4ZoomRatio_x100:
*               zoom ratio (x100)
*
*           i4LightValue_x10:
*               light value (x10)
*
*   rIdxMgr:
*       [in]    The default ISP tuning index manager.
*       [out]   The ISP tuning index manager after customizing.
*
*
*******************************************************************************/
MVOID
IspTuningCustom::
evaluate_nvram_index(RAWIspCamInfo const& rCamInfo, IndexMgr& rIdxMgr)
{
//..............................................................................
    //  (1) Dump info. before customizing.
    char InputValue[PROPERTY_VALUE_MAX] = {'\0'};
    property_get("debug.nvram_idx.log", InputValue, "0");
    MUINT32 debugEn = atoi(InputValue);

#if ENABLE_MY_LOG
    if (debugEn) rCamInfo.dump();
#endif

#if 0
    LOGD("[+evaluate_nvram_index][before customizing]");
    rIdxMgr.dump();
#endif
//..............................................................................
    //  (2) Modify each index based on conditions.
    //
    //  setIdx_XXX() returns:
    //      MTURE: if successful
    //      MFALSE: if the input index is out of range.
    //
#if 0
    fgRet = rIdxMgr.setIdx_OBC(XXX);
    fgRet = rIdxMgr.setIdx_BPC(XXX);
    fgRet = rIdxMgr.setIdx_NR1(XXX);
    fgRet = rIdxMgr.setIdx_CFA(XXX);
    fgRet = rIdxMgr.setIdx_GGM(XXX);
    fgRet = rIdxMgr.setIdx_ANR(XXX);
    fgRet = rIdxMgr.setIdx_CCR(XXX);
    fgRet = rIdxMgr.setIdx_EE(XXX);
#endif
/*
if(rCamInfo.u4ISOValue > 500 && rCamInfo.u4ISOValue < 700)
	{
		switch(rCamInfo.eIspProfile)
		{
			case EIspProfile_Preview:
				if( rCamInfo.eSensorMode == ESensorMode_Preview)
				{
				    rIdxMgr.setIdx_CFA(9);
				    rIdxMgr.setIdx_ANR(9);
				    rIdxMgr.setIdx_EE(9);
				}
				else if( rCamInfo.eSensorMode == ESensorMode_Capture)
				{
					rIdxMgr.setIdx_CFA(19);
					rIdxMgr.setIdx_ANR(19);
					rIdxMgr.setIdx_EE(19);
				}
				break;
			case EIspProfile_Capture:
			case EIspProfile_ZSD_Capture:
				if( rCamInfo.eSensorMode == ESensorMode_Capture)
				{
					rIdxMgr.setIdx_CFA(119);
					rIdxMgr.setIdx_ANR(119);
					rIdxMgr.setIdx_EE(119);
				}
				break;
			case EIspProfile_MFB_PostProc_ANR_EE:
			case EIspProfile_MFB_PostProc_ANR_EE_SWNR:
				rIdxMgr.setIdx_ANR(199);
			break;
			case EIspProfile_MFB_PostProc_Mixing:
			case EIspProfile_MFB_PostProc_Mixing_SWNR:
				rIdxMgr.setIdx_EE(189);
			break;
			default:
				break;
		}
	}


	if(rCamInfo.eIdx_ISO == eIDX_ISO_3200)
	{
		switch(rCamInfo.eIspProfile)
		{
			case EIspProfile_Preview:
				if( rCamInfo.eSensorMode == ESensorMode_Preview)
				{
				    rIdxMgr.setIdx_CFA(8);
				    rIdxMgr.setIdx_ANR(8);
				    rIdxMgr.setIdx_EE(8);
				}
				else if( rCamInfo.eSensorMode == ESensorMode_Capture)
				{
					rIdxMgr.setIdx_CFA(18);
					rIdxMgr.setIdx_ANR(18);
					rIdxMgr.setIdx_EE(18);
				}
				break;
			case EIspProfile_Capture:
			case EIspProfile_ZSD_Capture:
				if( rCamInfo.eSensorMode == ESensorMode_Capture)
				{
					rIdxMgr.setIdx_CFA(118);
					rIdxMgr.setIdx_ANR(118);
					rIdxMgr.setIdx_EE(118);
				}
				break;
			default:
				break;
		}

	}

	if( rCamInfo.eSensorMode == ESensorMode_Capture)
	{
		if(rCamInfo.rAEInfo.u4AfeGain >= 6144)
		{
			rIdxMgr.setIdx_OBC(3);
		}
	}
	*/

#if 0
    if(TUNING_FOR_AIS) {
        if(IS_AIS) {
            if(rCamInfo.eIspProfile == EIspProfile_MFB_Capture_EE_Off
                || rCamInfo.eIspProfile == EIspProfile_VSS_MFB_Capture_EE_Off
                || rCamInfo.eIspProfile == EIspProfile_MFB_Capture_EE_Off_SWNR
                || rCamInfo.eIspProfile == EIspProfile_VSS_MFB_Capture_EE_Off_SWNR
                || rCamInfo.eIspProfile == EIspProfile_MFB_PostProc_EE_Off
                || rCamInfo.eIspProfile == EIspProfile_VSS_MFB_PostProc_EE_Off
                //|| rCamInfo.eIspProfile == EIspProfile_MFB_Blending_All_Off
                //|| rCamInfo.eIspProfile == EIspProfile_MFB_Blending_All_Off_SWNR
                || rCamInfo.eIspProfile == EIspProfile_MFB_PostProc_ANR_EE
                || rCamInfo.eIspProfile == EIspProfile_VSS_MFB_PostProc_ANR_EE
                || rCamInfo.eIspProfile == EIspProfile_MFB_PostProc_ANR_EE_SWNR
                || rCamInfo.eIspProfile == EIspProfile_VSS_MFB_PostProc_ANR_EE_SWNR
                || rCamInfo.eIspProfile == EIspProfile_MFB_PostProc_Mixing
                || rCamInfo.eIspProfile == EIspProfile_VSS_MFB_PostProc_Mixing
                || rCamInfo.eIspProfile == EIspProfile_MFB_PostProc_Mixing_SWNR
                || rCamInfo.eIspProfile == EIspProfile_VSS_MFB_PostProc_Mixing_SWNR
            )
            {
                MUINT32 stage = AIS_Profile2Stage(rCamInfo.eIspProfile);

                MUINT32 normalIso = rCamInfo.rAEInfo.u4OrgRealISOValue;
                MUINT32 aisIso = rCamInfo.rAEInfo.u4RealISOValue;
                MUINT32 cfaIso = WEIGHTING(normalIso, aisIso, AIS_NORMAL_CFA_RATIO[stage-1]);
                MUINT32 ynrIso = WEIGHTING(normalIso, aisIso, AIS_NORMAL_YNR_RATIO[stage-1]);
                MUINT32 eeIso = WEIGHTING(normalIso, aisIso, AIS_NORMAL_EE_RATIO[stage-1]);

                MINT32 normalIndex = map_ISO_value_to_index(normalIso, rCamInfo.eIspProfile);
                MINT32 aisIndex = map_ISO_value_to_index(aisIso, rCamInfo.eIspProfile);

                // base on AIS index
                MINT32 deltaUdm = map_ISO_value_to_index(cfaIso, rCamInfo.eIspProfile) - aisIndex;
                MINT32 deltaYnr = map_ISO_value_to_index(ynrIso, rCamInfo.eIspProfile) - aisIndex;
                MINT32 deltaEE = map_ISO_value_to_index(eeIso, rCamInfo.eIspProfile) - aisIndex;

                if(rIdxMgr.getIdx_UDM() != 0)   rIdxMgr.setIdx_UDM(rIdxMgr.getIdx_UDM() + deltaUdm);
                if(rIdxMgr.getIdx_ANR() != 0)   rIdxMgr.setIdx_ANR(rIdxMgr.getIdx_ANR() + deltaYnr);
                if(rIdxMgr.getIdx_EE() != 0)    rIdxMgr.setIdx_EE(rIdxMgr.getIdx_EE() + deltaEE);
            }
        }
    }
#endif
//..............................................................................
    //  (3) Finally, dump info. after modifying.
#if 0
    LOGD("[-evaluate_nvram_index][after customizing]");
    rIdxMgr.dump();
#endif
}

//++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
//   rIspRegMgr
//       m_rRegs: ISP NVRAM register
//           ISP_NVRAM_OBC_T    OBC[NVRAM_OBC_TBL_NUM];
//           ISP_NVRAM_BPC_T    BPC[NVRAM_BPC_TBL_NUM];
//           ISP_NVRAM_NR1_T    NR1[NVRAM_NR1_TBL_NUM];
//           ISP_NVRAM_LSC_T    LSC[NVRAM_LSC_TBL_NUM];
//           ISP_NVRAM_SL2_T    SL2[NVRAM_SL2_TBL_NUM];
//           ISP_NVRAM_CFA_T    CFA[NVRAM_CFA_TBL_NUM];
//           ISP_NVRAM_CCM_T    CCM[NVRAM_CCM_TBL_NUM];
//           ISP_NVRAM_GGM_T    GGM[NVRAM_GGM_TBL_NUM];
//           ISP_NVRAM_GGM_T    IHDR_GGM[NVRAM_IHDR_GGM_TBL_NUM];
//           ISP_NVRAM_ANR_T    ANR[NVRAM_ANR_TBL_NUM];
//           ISP_NVRAM_CCR_T    CCR[NVRAM_CCR_TBL_NUM];
//           ISP_NVRAM_EE_T     EE[NVRAM_EE_TBL_NUM];
//           ISP_NVRAM_NR3D_T   NR3D[NVRAM_NR3D_TBL_NUM];
//           ISP_NVRAM_MFB_T    MFB[NVRAM_MFB_TBL_NUM];
//       m_rIdx: current NVRAM index
//++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++


MINT32 GetDBSGNP(MINT32 Ratio)
{
     if(Ratio > 11)
         return 4;
    else if (Ratio > 5)
         return 3;
    else if(Ratio > 2)
         return 2;
    else if(Ratio > 1)
         return 1;
    else
         return 0;


}

MVOID
IspTuningCustom::
refine_SL2F(RAWIspCamInfo const& rCamInfo, IspNvramRegMgr & rIspRegMgr, ISP_NVRAM_SL2_T& rSL2F)
{

}

MVOID
IspTuningCustom::
refine_DBS(RAWIspCamInfo const& rCamInfo, IspNvramRegMgr & rIspRegMgr, ISP_NVRAM_DBS_T& rDBS)
{
        char cLogLevel[PROPERTY_VALUE_MAX];
    MINT32 debugDump = property_get_int32("debug.dbs.dump", 0);
        //> prepar register value

        MINT32 aeLeSeRatio = rCamInfo.rAEInfo.i4LESE_Ratio;
        MINT32 aeLeSeRatio_round = (rCamInfo.rAEInfo.i4LESE_Ratio + 50) / 100;

        // Use Formula to calculate
        if(aeLeSeRatio < 100)
        {
            aeLeSeRatio = 100;
            aeLeSeRatio_round = 1;
        }
        if(aeLeSeRatio > 1600)
        {
            aeLeSeRatio = 1600;
            aeLeSeRatio_round = 16;
        }
        MINT32 DBS_GN  = (aeLeSeRatio << 3) / 100;
        MINT32 DBS_GN2 = 6400 / aeLeSeRatio;
        MINT32 DBS_GNP = GetDBSGNP(aeLeSeRatio_round);

    MY_LOG_IF(debugDump, "refine_dbs, i4LESE_Ratio(%d)->asLeSeRatio_round(%d),GNP(%d), GAIN1(%d),GAIN2(%d)", aeLeSeRatio, aeLeSeRatio_round, DBS_GNP, DBS_GN, DBS_GN2);

        rDBS.ctl.val = (rDBS.ctl.val & 0xFFF8)
                                    | (DBS_GNP&0x07)
                                    | ((DBS_GN&0xFF) << 16)
                                    | ((DBS_GN2&0xFF) << 24);
    }




//++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++

//++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++



MVOID
IspTuningCustom::
refine_OBC(RAWIspCamInfo const& rCamInfo, IspNvramRegMgr & rIspRegMgr, ISP_NVRAM_OBC_T& rOBC)
{
    #if 0
    MY_LOG("%s()\n", __FUNCTION__);
    // TODO: Add your code below...

    if (getSensorDev() == ESensorDev_Main) { // main
    }
    else if (getSensorDev() == ESensorDev_Sub) { // sub
    }
    else { // main2
    }

    MY_LOG("rOBC.offst0 = 0x%8x", rOBC.offst0);
    MY_LOG("rOBC.offst1 = 0x%8x", rOBC.offst1);
    MY_LOG("rOBC.offst2 = 0x%8x", rOBC.offst2);
    MY_LOG("rOBC.offst3 = 0x%8x", rOBC.offst3);
    MY_LOG("rOBC.gain0 = 0x%8x", rOBC.gain0);
    MY_LOG("rOBC.gain1 = 0x%8x", rOBC.gain1);
    MY_LOG("rOBC.gain2 = 0x%8x", rOBC.gain2);
    MY_LOG("rOBC.gain3 = 0x%8x", rOBC.gain3);
    #endif
}

//++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
//
//++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
MVOID
IspTuningCustom::
refine_BPC(RAWIspCamInfo const& rCamInfo, IspNvramRegMgr & rIspRegMgr, ISP_NVRAM_BNR_BPC_T& rBPC)
{
    #if 0
    MY_LOG("%s()\n", __FUNCTION__);
    // TODO: Add your code below...

    MY_LOG("rBPC.con = 0x%8x", rBPC.con);
    MY_LOG("rBPC.th1 = 0x%8x", rBPC.th1);
    MY_LOG("rBPC.th2 = 0x%8x", rBPC.th2);
    MY_LOG("rBPC.th3 = 0x%8x", rBPC.th3);
    MY_LOG("rBPC.th4 = 0x%8x", rBPC.th4);
    MY_LOG("rBPC.dtc = 0x%8x", rBPC.dtc);
    MY_LOG("rBPC.cor = 0x%8x", rBPC.cor);
    MY_LOG("rBPC.tbli1 = 0x%8x", rBPC.tbli1);
    MY_LOG("rBPC.tbli2 = 0x%8x", rBPC.tbli2);
    MY_LOG("rBPC.th1_c = 0x%8x", rBPC.th1_c);
    MY_LOG("rBPC.th2_c = 0x%8x", rBPC.th2_c);
    MY_LOG("rBPC.th3_c = 0x%8x", rBPC.th3_c);
    #endif

}

//++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
//
//++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
MVOID
IspTuningCustom::
refine_NR1(RAWIspCamInfo const& rCamInfo, IspNvramRegMgr & rIspRegMgr, ISP_NVRAM_BNR_NR1_T& rNR1)
{
    #if 0
    MY_LOG("%s()\n", __FUNCTION__);
    // TODO: Add your code below...

    MY_LOG("rNR1.con = 0x%8x", rNR1.con);
    MY_LOG("rNR1.ct_con = 0x%8x", rNR1.ct_con);
    #endif
}

MVOID
IspTuningCustom::
refine_PDC(RAWIspCamInfo const& rCamInfo, IspNvramRegMgr & rIspRegMgr, ISP_NVRAM_BNR_PDC_T& rPDC)
{
    #if 0
    MY_LOG("%s()\n", __FUNCTION__);
    // TODO: Add your code below...

    MY_LOG("rNR1.con = 0x%8x", rNR1.con);
    MY_LOG("rNR1.ct_con = 0x%8x", rNR1.ct_con);
    #endif
}

MVOID
IspTuningCustom::
refine_RMM(RAWIspCamInfo const& rCamInfo, IspNvramRegMgr & rIspRegMgr, ISP_NVRAM_RMM_T& rRMM)
{
    #if 0
    MY_LOG("%s()\n", __FUNCTION__);
    // TODO: Add your code below...

    MY_LOG("rNR1.con = 0x%8x", rNR1.con);
    MY_LOG("rNR1.ct_con = 0x%8x", rNR1.ct_con);
    #endif
}


//++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
//
//++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
MVOID
IspTuningCustom::
refine_SL2(RAWIspCamInfo const& rCamInfo, IspNvramRegMgr & rIspRegMgr, ISP_NVRAM_SL2_T& rSL2)
{
    #if 0
    MY_LOG("%s()\n", __FUNCTION__);
    // TODO: Add your code below...

    MY_LOG("rSL2.cen = 0x%8x", rSL2.cen);
    MY_LOG("rSL2.max0_rr = 0x%8x", rSL2.max0_rr);
    MY_LOG("rSL2.max1_rr = 0x%8x", rSL2.max1_rr);
    MY_LOG("rSL2.max2_rr = 0x%8x", rSL2.max2_rr);
    #endif
}

//++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
//
//++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
MVOID
IspTuningCustom::
refine_RPG(RAWIspCamInfo const& rCamInfo, IspNvramRegMgr & rIspRegMgr, ISP_NVRAM_RPG_T& rRPG)
{
    #if 0
    MY_LOG("%s()\n", __FUNCTION__);
    // TODO: Add your code below...

    MY_LOG("rRPG.satu_1 = 0x%8x", rRPG.satu_1);
    MY_LOG("rRPG.satu_2 = 0x%8x", rRPG.satu_2);
    MY_LOG("rRPG.gain_1 = 0x%8x", rRPG.gain_1);
    MY_LOG("rRPG.gain_2 = 0x%8x", rRPG.gain_2);
    MY_LOG("rRPG.ofst_1 = 0x%8x", rRPG.ofst_1);
    MY_LOG("rRPG.ofst_2 = 0x%8x", rRPG.ofst_2);
    #endif
}

//++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
//
//++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
MVOID
IspTuningCustom::
refine_PGN(RAWIspCamInfo const& rCamInfo, IspNvramRegMgr & rIspRegMgr, ISP_NVRAM_PGN_T& rPGN)
{
    #if 0
    MY_LOG("%s()\n", __FUNCTION__);
    // TODO: Add your code below...

    MY_LOG("rPGN.satu_1 = 0x%8x", rPGN.satu_1);
    MY_LOG("rPGN.satu_2 = 0x%8x", rPGN.satu_2);
    MY_LOG("rPGN.gain_1 = 0x%8x", rPGN.gain_1);
    MY_LOG("rPGN.gain_2 = 0x%8x", rPGN.gain_2);
    MY_LOG("rPGN.ofst_1 = 0x%8x", rPGN.ofst_1);
    MY_LOG("rPGN.ofst_2 = 0x%8x", rPGN.ofst_2);
    #endif
}

//++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
//
//++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
MVOID
IspTuningCustom::
refine_RNR(RAWIspCamInfo const& rCamInfo, IspNvramRegMgr & rIspRegMgr, ISP_NVRAM_RNR_T& rRNR)
{

}
//++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
//
//++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
MVOID
IspTuningCustom::
refine_UDM(RAWIspCamInfo const& rCamInfo, IspNvramRegMgr & rIspRegMgr, ISP_NVRAM_UDM_T& rUDM)
{

    #if 0
    if(TUNING_FOR_AIS) {                          //0717forbuild
        if(IS_AIS) {
            rUDM.//rUDM.hf_comp.bits.DM_HF_LSC_GAIN0 = 8;
            rCFA.hf_comp.bits.DM_HF_LSC_GAIN1 = 8;
            rCFA.hf_comp.bits.DM_HF_LSC_GAIN2 = 8;
            rCFA.hf_comp.bits.DM_HF_LSC_GAIN3 = 8;
        }
    }
    #endif

    #if 0
    MY_LOG("%s()\n", __FUNCTION__);
    // TODO: Add your code below...

    MY_LOG("rCFA.byp = 0x%8x", rCFA.byp);
    MY_LOG("rCFA.ed_flat = 0x%8x", rCFA.ed_flat);
    MY_LOG("rCFA.ed_nyq = 0x%8x", rCFA.ed_nyq);
    MY_LOG("rCFA.ed_step = 0x%8x", rCFA.ed_step);
    MY_LOG("rCFA.rgb_hf = 0x%8x", rCFA.rgb_hf);
    MY_LOG("rCFA.dot = 0x%8x", rCFA.dot);
    MY_LOG("rCFA.f1_act = 0x%8x", rCFA.f1_act);
    MY_LOG("rCFA.f2_act = 0x%8x", rCFA.f2_act);
    MY_LOG("rCFA.f3_act = 0x%8x", rCFA.f3_act);
    MY_LOG("rCFA.f4_act = 0x%8x", rCFA.f4_act);
    MY_LOG("rCFA.f1_l = 0x%8x", rCFA.f1_l);
    MY_LOG("rCFA.f2_l = 0x%8x", rCFA.f2_l);
    MY_LOG("rCFA.f3_l = 0x%8x", rCFA.f3_l);
    MY_LOG("rCFA.f4_l = 0x%8x", rCFA.f4_l);
    MY_LOG("rCFA.hf_rb = 0x%8x", rCFA.hf_rb);
    MY_LOG("rCFA.hf_gain = 0x%8x", rCFA.hf_gain);
    MY_LOG("rCFA.hf_comp = 0x%8x", rCFA.hf_comp);
    MY_LOG("rCFA.hf_coring_th = 0x%8x", rCFA.hf_coring_th);
    MY_LOG("rCFA.act_lut = 0x%8x", rCFA.act_lut);
    MY_LOG("rCFA.spare = 0x%8x", rCFA.spare);
    MY_LOG("rCFA.bb = 0x%8x", rCFA.bb);
    #endif
}

//++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
//
//++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
static MINT32 Complement2(MUINT32 value, MUINT32 digit)
{
    MINT32 Result;

    if (((value >> (digit - 1)) & 0x1) == 1)    // negative
    {
        Result = 0 - (MINT32)((~value + 1) & ((1 << digit) - 1));
    }
    else
    {
        Result = (MINT32)(value & ((1 << digit) - 1));
    }

    return Result;
}

//++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
//
//++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
MVOID
IspTuningCustom::
refine_CCM(RAWIspCamInfo const& rCamInfo, IspNvramRegMgr & rIspRegMgr, ISP_NVRAM_CCM_T& rCCM)
{
    #if 0
    MY_LOG("%s()\n", __FUNCTION__);
    // TODO: Add your code below...

    MY_LOG("rCCM.cnv_1 = 0x%8x", rCCM.cnv_1);
    MY_LOG("rCCM.cnv_2 = 0x%8x", rCCM.cnv_2);
    MY_LOG("rCCM.cnv_3 = 0x%8x", rCCM.cnv_3);
    MY_LOG("rCCM.cnv_4 = 0x%8x", rCCM.cnv_4);
    MY_LOG("rCCM.cnv_5 = 0x%8x", rCCM.cnv_5);
    MY_LOG("rCCM.cnv_6 = 0x%8x", rCCM.cnv_6);
    #endif

#if 0
    if(rCamInfo.eIspProfile == EIspProfile_iHDR_Preview ||
       rCamInfo.eIspProfile == EIspProfile_iHDR_Video   ||
       rCamInfo.eIspProfile == EIspProfile_zHDR_Preview ||
       rCamInfo.eIspProfile == EIspProfile_zHDR_Video   ||
       rCamInfo.eIspProfile == EIspProfile_zHDR_Capture)
    {
        char debugFlag[PROPERTY_VALUE_MAX] = {'\0'};
        property_get("debug.vhdr.tuning", debugFlag, "0");

        MINT32 digit_00 = Complement2((rCCM.cnv_1.val & 0x1FFF),13);
        MINT32 digit_01 = Complement2(((rCCM.cnv_1.val & 0x1FFF0000) >> 16),13);
        MINT32 digit_02 = Complement2((rCCM.cnv_2.val & 0x1FFF),13);
        MINT32 digit_10 = Complement2((rCCM.cnv_3.val & 0x1FFF),13);
        MINT32 digit_11 = Complement2(((rCCM.cnv_3.val & 0x1FFF0000) >> 16),13);
        MINT32 digit_12 = Complement2((rCCM.cnv_4.val & 0x1FFF),13);
        MINT32 digit_20 = Complement2((rCCM.cnv_5.val & 0x1FFF),13);
        MINT32 digit_21 = Complement2(((rCCM.cnv_5.val & 0x1FFF0000) >> 16),13);
        MINT32 digit_22 = Complement2((rCCM.cnv_6.val & 0x1FFF),13);

        if(debugFlag[0] == '1')
        {
            MY_LOG("ori:00(%d),01(%d),02(%d)",digit_00,digit_01,digit_02);
            MY_LOG("ori:10(%d),11(%d),12(%d)",digit_10,digit_11,digit_12);
            MY_LOG("ori:20(%d),21(%d),22(%d)",digit_20,digit_21,digit_22);
        }

        MFLOAT temp_01 = 0.7 * (MFLOAT)digit_01;
        MFLOAT temp_02 = 0.7 * (MFLOAT)digit_02;
        MFLOAT temp_10 = 0.7 * (MFLOAT)digit_10;
        MFLOAT temp_12 = 0.7 * (MFLOAT)digit_12;
        MFLOAT temp_20 = 0.7 * (MFLOAT)digit_20;
        MFLOAT temp_21 = 0.7 * (MFLOAT)digit_21;

        MINT32 final_00 = ((MFLOAT)digit_00-512.0) * 0.7 + 0.5 + 512.0;
        MINT32 final_01 = (temp_01 > 0) ? temp_01 + 0.5 : temp_01 - 0.5;
        MINT32 final_02 = (temp_02 > 0) ? temp_02 + 0.5 : temp_02 - 0.5;
        MINT32 final_10 = (temp_10 > 0) ? temp_10 + 0.5 : temp_10 - 0.5;
        MINT32 final_11 = ((MFLOAT)digit_11-512.0) * 0.7 + 0.5 + 512.0;
        MINT32 final_12 = (temp_12 > 0) ? temp_12 + 0.5 : temp_12 - 0.5;
        MINT32 final_20 = (temp_20 > 0) ? temp_20 + 0.5 : temp_20 - 0.5;
        MINT32 final_21 = (temp_21 > 0) ? temp_21 + 0.5 : temp_21 - 0.5;
        MINT32 final_22 = ((MFLOAT)digit_22-512.0) * 0.7 + 0.5 + 512.0;

        //> check sum = 512 at each raw

        const MINT32 sumValue = 512;

        final_00 += sumValue - (final_00 + final_01 + final_02);
        final_11 += sumValue - (final_10 + final_11 + final_12);
        final_22 += sumValue - (final_20 + final_21 + final_22);

        if(debugFlag[0] == '1')
        {
            MY_LOG("final:00(%d),01(%d),02(%d)",final_00,final_01,final_02);
            MY_LOG("final:10(%d),11(%d),12(%d)",final_10,final_11,final_12);
            MY_LOG("final:20(%d),21(%d),22(%d)",final_20,final_21,final_22);
        }

        rCCM.cnv_1.val = 0 | (final_00 & 0x1FFF) | ((final_01 & 0x1FFF) << 16);
        rCCM.cnv_2.val = 0 | (final_02 & 0x1FFF);
        rCCM.cnv_3.val = 0 | (final_10 & 0x1FFF) | ((final_11 & 0x1FFF) << 16);
        rCCM.cnv_4.val = 0 | (final_12 & 0x1FFF);
        rCCM.cnv_5.val = 0 | (final_20 & 0x1FFF) | ((final_21 & 0x1FFF) << 16);
        rCCM.cnv_6.val = 0 | (final_22 & 0x1FFF);

        if(debugFlag[0] == '1')
        {
            MY_LOG("final:rCCM.cnv_1 = 0x%08x", rCCM.cnv_1.val);
            MY_LOG("final:rCCM.cnv_2 = 0x%08x", rCCM.cnv_2.val);
            MY_LOG("final:rCCM.cnv_3 = 0x%08x", rCCM.cnv_3.val);
            MY_LOG("final:rCCM.cnv_4 = 0x%08x", rCCM.cnv_4.val);
            MY_LOG("final:rCCM.cnv_5 = 0x%08x", rCCM.cnv_5.val);
            MY_LOG("final:rCCM.cnv_6 = 0x%08x", rCCM.cnv_6.val);
        }
    }
	#endif
}

//++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
//
//++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
MVOID
IspTuningCustom::
refine_GGM(RAWIspCamInfo const& rCamInfo, IspNvramRegMgr & rIspRegMgr, ISP_NVRAM_GGM_T& rGGM)
{
    #if 0
    MY_LOG("%s()\n", __FUNCTION__);
    // TODO: Add your code below...

    MY_LOG("rGGM.lut_rb.lut[0] = 0x%8x", rGGM.lut_rb.lut[0]);
    MY_LOG("rGGM.lut_g.lut[0] = 0x%8x", rGGM.lut_g.lut[0]);
    #endif
}

//++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
//
//++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
MVOID
IspTuningCustom::
refine_ANR(RAWIspCamInfo const& rCamInfo, IspNvramRegMgr & rIspRegMgr, ISP_NVRAM_ANR_T& rANR)
{
    char InputValue[PROPERTY_VALUE_MAX] = {'\0'};
    property_get("debug.refine_ANR", InputValue, "0");
    MUINT32 debugEn = atoi(InputValue);



//    if (debugEn) MY_LOG("rANR.con1.val(0x%08x)", rANR.con1.val);


#if 0

    if (rCamInfo.eIspProfile == EIspProfile_VFB_PostProc) {
            rANR.con1.bits.ANR1_ENC = 0;
            rANR.con1.bits.ANR1_ENY = 1;
            rANR.con1.bits.ANR_SCALE_MODE = 1;
            rANR.con1.bits.ANR1_MEDIAN_EN = 0;
            rANR.con1.bits.ANR1_LCE_LINK = 0;
            rANR.con1.bits.ANR1_TABLE_EN = 0;
            rANR.con1.bits.ANR1_TBL_PRC = 0;

            rANR.con2.bits.ANR1_IMPL_MODE = 2;
            rANR.con2.bits.ANR1_C_MED_EN = 0;
            rANR.con2.bits.ANR1_C_SM_EDGE = 1;
            rANR.con2.bits.ANR1_FLT_C = 0;
            rANR.con2.bits.ANR1_LR = 1;       // Set 1 to reduce contour
            rANR.con2.bits.ANR1_ALPHA = 4;    // when ANR1_LR == 1, ANR1_ALPHA = 4 is LR result, ANR1_ALPHA = 0 is original result

            rANR.yad1.bits.ANR1_CEN_GAIN_LO_TH = 1;
            rANR.yad1.bits.ANR1_CEN_GAIN_HI_TH = 5;
            rANR.yad1.bits.ANR1_K_LO_TH = 0;
            rANR.yad1.bits.ANR1_K_HI_TH = 9;
            rANR.yad1.bits.ANR1_K_TH_C = 8;

            rANR.yad2.bits.ANR1_PTY_VGAIN = 10;
            rANR.yad2.bits.ANR1_PTY_GAIN_TH = 10;

            rANR.lut1.bits.ANR1_Y_CPX1 = 40;
           rANR.lut1.bits.ANR1_Y_CPX2 = 100;
            rANR.lut1.bits.ANR1_Y_CPX3 = 160;

            rANR.lut2.bits.ANR1_Y_SCALE_CPY0 = 16;
            rANR.lut2.bits.ANR1_Y_SCALE_CPY1 = 16;
            rANR.lut2.bits.ANR1_Y_SCALE_CPY2 = 16;
            rANR.lut2.bits.ANR1_Y_SCALE_CPY3 = 8;

            rANR.lut3.bits.ANR1_Y_SCALE_SP0 = 0;
            rANR.lut3.bits.ANR1_Y_SCALE_SP1 = 0;
            rANR.lut3.bits.ANR1_Y_SCALE_SP2 = 0x1C;
            rANR.lut3.bits.ANR1_Y_SCALE_SP3 = 0x1E;

            // CAM_ANR_C4LUT1
            rANR.clut1.bits.ANR1_C_CPX1 = 40;
            rANR.clut1.bits.ANR1_C_CPX2 = 100;
            rANR.clut1.bits.ANR1_C_CPX3 = 160;

            // CAM_ANR_C4LUT2
            rANR.clut2.bits.ANR1_C_SCALE_CPY0 = 16;
            rANR.clut2.bits.ANR1_C_SCALE_CPY1 = 16;
            rANR.clut2.bits.ANR1_C_SCALE_CPY2 = 16;
            rANR.clut2.bits.ANR1_C_SCALE_CPY3 = 16;

            // CAM_ANR_C4LUT3
            rANR.clut3.bits.ANR1_C_SCALE_SP0 = 0;
            rANR.clut3.bits.ANR1_C_SCALE_SP1 = 0;
            rANR.clut3.bits.ANR1_C_SCALE_SP2 = 0;
            rANR.clut3.bits.ANR1_C_SCALE_SP3 = 0;

            // CAM_ANR_A4LUT2
            rANR.alut2.bits.ANR1_Y_ACT_CPY0 = 0;
            rANR.alut2.bits.ANR1_Y_ACT_CPY1 = 0;
            rANR.alut2.bits.ANR1_Y_ACT_CPY2 = 0;
            rANR.alut2.bits.ANR1_Y_ACT_CPY3 = 0;

            // CAM_ANR_A4LUT3
            rANR.alut3.bits.ANR1_Y_ACT_SP0 = 0;
            rANR.alut3.bits.ANR1_Y_ACT_SP1 = 0;
            rANR.alut3.bits.ANR1_Y_ACT_SP2 = 0;
            rANR.alut3.bits.ANR1_Y_ACT_SP3 = 0;

            // CAM_ANR_L4LUT1
            rANR.llut1.bits.ANR1_LCE_X1 = 64;
            rANR.llut1.bits.ANR1_LCE_X2 = 128;
            rANR.llut1.bits.ANR1_LCE_X3 = 192;

            // CAM_ANR_L4LUT2
            rANR.llut2.bits.ANR1_LCE_GAIN0 = 16;
            rANR.llut2.bits.ANR1_LCE_GAIN1 = 18;
            rANR.llut2.bits.ANR1_LCE_GAIN2 = 20;
            rANR.llut2.bits.ANR1_LCE_GAIN3 = 24;

            // CAM_ANR_L4LUT3
            rANR.llut3.bits.ANR1_LCE_SP0 = 4;
            rANR.llut3.bits.ANR1_LCE_SP1 = 4;
            rANR.llut3.bits.ANR1_LCE_SP2 = 8;
            rANR.llut3.bits.ANR1_LCE_SP3 = 4;

            rANR.pty.bits.ANR1_PTY1 = 12;
            rANR.pty.bits.ANR1_PTY2 = 34;
            rANR.pty.bits.ANR1_PTY3 = 58;
            rANR.pty.bits.ANR1_PTY4 = 72;

            rANR.cad.bits.ANR1_PTC_VGAIN = 10;
            rANR.cad.bits.ANR1_PTC_GAIN_TH = 6;
            rANR.cad.bits.ANR1_C_L_DIFF_TH = 28;

            rANR.ptc.bits.ANR1_PTC1 = 2;
            rANR.ptc.bits.ANR1_PTC2 = 3;
            rANR.ptc.bits.ANR1_PTC3 = 4;
            rANR.ptc.bits.ANR1_PTC4 = 6;

            rANR.lce.bits.ANR1_LCE_C_GAIN = 6;
            rANR.lce.bits.ANR1_LCE_SCALE_GAIN = 0;

            // CAM_ANR_MED1
            rANR.med1.bits.ANR1_COR_TH = 5;
            rANR.med1.bits.ANR1_COR_SL = 4;
            rANR.med1.bits.ANR1_MCD_TH = 5;
            rANR.med1.bits.ANR1_MCD_SL = 3;
            rANR.med1.bits.ANR1_LCL_TH = 24;

            // CAM_ANR_MED2
            rANR.med2.bits.ANR1_LCL_SL = 3;
            rANR.med2.bits.ANR1_LCL_LV = 16;
            rANR.med2.bits.ANR1_SCL_TH = 12;
            rANR.med2.bits.ANR1_SCL_SL = 3;
            rANR.med2.bits.ANR1_SCL_LV = 16;

            // CAM_ANR_MED3
            rANR.med3.bits.ANR1_NCL_TH = 255;
            rANR.med3.bits.ANR1_NCL_SL = 3;
            rANR.med3.bits.ANR1_NCL_LV = 16;
            rANR.med3.bits.ANR1_VAR = 3;
            rANR.med3.bits.ANR1_Y0 = 10;

            // CAM_ANR_MED4
            rANR.med4.bits.ANR1_Y1 = 13;
            rANR.med4.bits.ANR1_Y2 = 16;
            rANR.med4.bits.ANR1_Y3 = 19;
            rANR.med4.bits.ANR1_Y4 = 22;

            rANR.hp1.bits.ANR1_HP_A = 120;
            rANR.hp1.bits.ANR1_HP_B = 0x3C;
            rANR.hp1.bits.ANR1_HP_C = 0x19;
            rANR.hp1.bits.ANR1_HP_D = 0x9;
            rANR.hp1.bits.ANR1_HP_E = 0xB;

            rANR.hp2.bits.ANR1_HP_S1 = 0;
            rANR.hp2.bits.ANR1_HP_S2 = 0;
            rANR.hp2.bits.ANR1_HP_X1 = 0;
            rANR.hp2.bits.ANR1_HP_F = 0x2;

            rANR.hp3.bits.ANR1_HP_Y_GAIN_CLIP = 119;
            rANR.hp3.bits.ANR1_HP_Y_SP = 6;
            rANR.hp3.bits.ANR1_HP_Y_LO = 120;
            rANR.hp3.bits.ANR1_HP_CLIP = 0;

            rANR.acty.bits.ANR1_ACT_TH_Y = 16;
            rANR.acty.bits.ANR1_ACT_BLD_BASE_Y = 48;
            rANR.acty.bits.ANR1_ACT_SLANT_Y = 14;
            rANR.acty.bits.ANR1_ACT_BLD_TH_Y = 48;

            // CAM_ANR_ACT1 CAM+A64H
            rANR.act1.bits.ANR1_ACT_LCE_GAIN = 8;
            rANR.act1.bits.ANR1_ACT_SCALE_OFT = 0;
            rANR.act1.bits.ANR1_ACT_SCALE_GAIN = 0;
            rANR.act1.bits.ANR1_ACT_DIF_GAIN = 0;
            rANR.act1.bits.ANR1_ACT_DIF_LO_TH = 16;

            // CAM_ANR_ACT2 CAM+A64H
            rANR.act2.bits.ANR1_ACT_SIZE_GAIN = 0;
            rANR.act2.bits.ANR1_ACT_SIZE_LO_TH = 16;
            rANR.act2.bits.ANR1_COR_TH1 = 0;
            rANR.act2.bits.ANR1_COR_SL1 = 0;

            // CAM_ANR_ACT3 CAM+A64H
            rANR.act3.bits.ANR1_COR_ACT_TH = 0;
            rANR.act3.bits.ANR1_COR_ACT_SL1 = 0;
            rANR.act3.bits.ANR1_COR_ACT_SL2 = 0;

            rANR.actc.bits.ANR1_ACT_TH_C = 0;
            rANR.actc.bits.ANR1_ACT_BLD_BASE_C = 0;
            rANR.actc.bits.ANR1_ACT_SLANT_C = 0;
            rANR.actc.bits.ANR1_ACT_BLD_TH_C = 0;


    }
#endif

#if 0
    if(TUNING_FOR_AIS) {
        // enable sl2b for ais stage3/4
        if(IS_AIS) {
            if (rCamInfo.eIspProfile == EIspProfile_MFB_PostProc_ANR_EE
                || rCamInfo.eIspProfile == EIspProfile_VSS_MFB_PostProc_ANR_EE
                || rCamInfo.eIspProfile == EIspProfile_MFB_PostProc_ANR_EE_SWNR
                || rCamInfo.eIspProfile == EIspProfile_VSS_MFB_PostProc_ANR_EE_SWNR
                || rCamInfo.eIspProfile == EIspProfile_MFB_PostProc_Mixing
                || rCamInfo.eIspProfile == EIspProfile_VSS_MFB_PostProc_Mixing
                || rCamInfo.eIspProfile == EIspProfile_MFB_PostProc_Mixing_SWNR
                || rCamInfo.eIspProfile == EIspProfile_VSS_MFB_PostProc_Mixing_SWNR  )
            {
                    rANR.con1.bits.ANR_LCE_LINK = 1;
            }
        }

        //stage1 use high iso, stage2/3/4 use low iso
        // use low iso luma anr for ais stage3/4
        // use high iso chroma anr for ais stage3/4 <- this
        if(IS_AIS) {
            if (rCamInfo.eIspProfile == EIspProfile_MFB_PostProc_ANR_EE
                || rCamInfo.eIspProfile == EIspProfile_VSS_MFB_PostProc_ANR_EE
                || rCamInfo.eIspProfile == EIspProfile_MFB_PostProc_ANR_EE_SWNR
                || rCamInfo.eIspProfile == EIspProfile_VSS_MFB_PostProc_ANR_EE_SWNR
                || rCamInfo.eIspProfile == EIspProfile_MFB_PostProc_Mixing
                || rCamInfo.eIspProfile == EIspProfile_VSS_MFB_PostProc_Mixing
                || rCamInfo.eIspProfile == EIspProfile_MFB_PostProc_Mixing_SWNR
                || rCamInfo.eIspProfile == EIspProfile_VSS_MFB_PostProc_Mixing_SWNR   )
            {
                MUINT32 stage = AIS_Profile2Stage(rCamInfo.eIspProfile);

                MUINT32 aisIso = rCamInfo.u4ISOValue;
                MUINT32 normalIso = rCamInfo.rAEInfo.u4OrgRealISOValue;
                MUINT32 ynrIso = WEIGHTING(normalIso, aisIso, AIS_NORMAL_YNR_RATIO[stage-1]);
                MUINT32 cnrIso = WEIGHTING(normalIso, aisIso, AIS_NORMAL_CNR_RATIO[stage-1]);

                MINT32 aisIndex = map_ISO_value_to_index(aisIso);
                MINT32 normalIndex = map_ISO_value_to_index(normalIso);
                MINT32 ynrIndex = map_ISO_value_to_index(ynrIso);

                //base on YNR
                MINT32 deltaCnr = map_ISO_value_to_index(cnrIso) - ynrIndex;

                ISP_NVRAM_ANR_T rAnrSettingForCnr = rIspRegMgr.getANR(rIspRegMgr.getIdx_ANR() + deltaCnr);

                if (debugEn) MY_LOG("rANR.con1.val(0x%08x), rAnrSettingForCnr.con1.val(0x%08x)", rANR.con1.val, rAnrSettingForCnr.con1.val);
                //con1
                rANR.con1.bits.ANR_ENC = rAnrSettingForCnr.con1.bits.ANR_ENC;
                rANR.con1.bits.ANR_SCALE_MODE = rAnrSettingForCnr.con1.bits.ANR_SCALE_MODE;
                //con3
                rANR.con3.bits.ANR_C_SM_EDGE = rAnrSettingForCnr.con3.bits.ANR_C_SM_EDGE;
                //cad(all)
                //rANR.cad.bits.ANR_PTC_VGAIN = rAnrSettingForCnr.cad.bits.ANR_PTC_VGAIN;
                //rANR.cad.bits.ANR_PTC_GAIN_TH = rAnrSettingForCnr.cad.bits.ANR_PTC_GAIN_TH;
                //rANR.cad.bits.ANR_C_L_DIFF_TH = rAnrSettingForCnr.cad.bits.ANR_C_L_DIFF_TH;
                rANR.cad.val = rAnrSettingForCnr.cad.val;
                //ptc(all)
                //rANR.ptc.bits.ANR_PTC1 = rAnrSettingForCnr.ptc.bits.ANR_PTC1;
                //rANR.ptc.bits.ANR_PTC2 = rAnrSettingForCnr.ptc.bits.ANR_PTC2;
                //rANR.ptc.bits.ANR_PTC3 = rAnrSettingForCnr.ptc.bits.ANR_PTC3;
                //rANR.ptc.bits.ANR_PTC4 = rAnrSettingForCnr.ptc.bits.ANR_PTC4;
                rANR.ptc.val = rAnrSettingForCnr.ptc.val;
                //lce1
                rANR.lce1.bits.ANR_LCE_C_GAIN = rAnrSettingForCnr.lce1.bits.ANR_LCE_C_GAIN;
                //lce2(all)
                //rANR.lce2.bits.ANR_LCE_GAIN0 = rAnrSettingForCnr.lce2.bits.ANR_LCE_GAIN0;
                //rANR.lce2.bits.ANR_LCE_GAIN1 = rAnrSettingForCnr.lce2.bits.ANR_LCE_GAIN1;
                //rANR.lce2.bits.ANR_LCE_GAIN2 = rAnrSettingForCnr.lce2.bits.ANR_LCE_GAIN2;
                //rANR.lce2.bits.ANR_LCE_GAIN3 = rAnrSettingForCnr.lce2.bits.ANR_LCE_GAIN3;
                rANR.lce2.val = rAnrSettingForCnr.lce2.val;
                //actc(all)
                //rANR.actc.bits.ANR_ACT_TH_C = rAnrSettingForCnr.actc.bits.ANR_ACT_TH_C;
                //rANR.actc.bits.ANR_ACT_BLD_BASE_C = rAnrSettingForCnr.actc.bits.ANR_ACT_BLD_BASE_C;
                //rANR.actc.bits.ANR_ACT_SLANT_C = rAnrSettingForCnr.actc.bits.ANR_ACT_SLANT_C;
                //rANR.actc.bits.ANR_ACT_BLD_TH_C = rAnrSettingForCnr.actc.bits.ANR_ACT_BLD_TH_C;
                rANR.actc.val = rAnrSettingForCnr.actc.val;
            }
        }
    }

    #endif

    #if 0
    MY_LOG("%s()\n", __FUNCTION__);
    // TODO: Add your code below...

    MY_LOG("rANR.con1 = 0x%8x", rANR.con1);
    MY_LOG("rANR.con2 = 0x%8x", rANR.con2);
    MY_LOG("rANR.con3 = 0x%8x", rANR.con3);
    MY_LOG("rANR.yad1 = 0x%8x", rANR.yad1);
    MY_LOG("rANR.yad2 = 0x%8x", rANR.yad2);
    MY_LOG("rANR.lut1 = 0x%8x", rANR.lut1);
    MY_LOG("rANR.lut2 = 0x%8x", rANR.lut2);
    MY_LOG("rANR.lut3 = 0x%8x", rANR.lut3);
    MY_LOG("rANR.pty = 0x%8x", rANR.pty);
    MY_LOG("rANR.cad = 0x%8x", rANR.cad);
    MY_LOG("rANR.ptc = 0x%8x", rANR.ptc);
    MY_LOG("rANR.lce1 = 0x%8x", rANR.lce1);
    MY_LOG("rANR.lce2 = 0x%8x", rANR.lce2);
    MY_LOG("rANR.hp1 = 0x%8x", rANR.hp1);
    MY_LOG("rANR.hp2 = 0x%8x", rANR.hp2);
    MY_LOG("rANR.hp3 = 0x%8x", rANR.hp3);
    MY_LOG("rANR.acty = 0x%8x", rANR.acty);
    MY_LOG("rANR.actc = 0x%8x", rANR.actc);
    #endif
}

//++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
//
//++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
MVOID
IspTuningCustom::
refine_ANR2(RAWIspCamInfo const& rCamInfo, IspNvramRegMgr & rIspRegMgr, ISP_NVRAM_ANR2_T& rANR)
{

    #if 0
    MY_LOG("%s()\n", __FUNCTION__);
    // TODO: Add your code below...

    MY_LOG("rCCR.con = 0x%8x", rCCR.con);
    MY_LOG("rCCR.ylut = 0x%8x", rCCR.ylut);
    MY_LOG("rCCR.uvlut = 0x%8x", rCCR.uvlut);
    MY_LOG("rCCR.ylut2 = 0x%8x", rCCR.ylut2);
    MY_LOG("rCCR.sat_ctrl = 0x%8x", rCCR.sat_ctrl);
    MY_LOG("rCCR.uvlut_sp = 0x%8x", rCCR.uvlut_sp);
    #endif
}

//++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
//
//++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++

MVOID
IspTuningCustom::
refine_CCR(RAWIspCamInfo const& rCamInfo, IspNvramRegMgr & rIspRegMgr, ISP_NVRAM_CCR_T& rCCR)
{

/*
    if (rCamInfo.eIspProfile == EIspProfile_VFB_PostProc) {
       rCCR.con.bits.CCR_EN = 0;
    }
*/

    #if 0
    MY_LOG("%s()\n", __FUNCTION__);
    // TODO: Add your code below...

    MY_LOG("rCCR.con = 0x%8x", rCCR.con);
    MY_LOG("rCCR.ylut = 0x%8x", rCCR.ylut);
    MY_LOG("rCCR.uvlut = 0x%8x", rCCR.uvlut);
    MY_LOG("rCCR.ylut2 = 0x%8x", rCCR.ylut2);
    MY_LOG("rCCR.sat_ctrl = 0x%8x", rCCR.sat_ctrl);
    MY_LOG("rCCR.uvlut_sp = 0x%8x", rCCR.uvlut_sp);
    #endif
}


//++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
//
//++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
MVOID
IspTuningCustom::
refine_HFG(RAWIspCamInfo const& rCamInfo, IspNvramRegMgr & rIspRegMgr, ISP_NVRAM_HFG_T& rHFG)
{

}


//++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
//
//++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
MVOID
IspTuningCustom::
refine_EE(RAWIspCamInfo const& rCamInfo, IspNvramRegMgr & rIspRegMgr, ISP_NVRAM_EE_T& rEE)
{

     #if 0

     if(TUNING_FOR_AIS) {
        if(IS_AIS) {
            rEE.glut_ctrl_07.bits.SEEE_GLUT_LLINK_EN = 0;
        }
    }


    MY_LOG("%s()\n", __FUNCTION__);
    // TODO: Add your code below...

    MY_LOG("rEE.srk_ctrl = 0x%8x", rEE.srk_ctrl);
    MY_LOG("rEE.clip_ctrl = 0x%8x", rEE.clip_ctrl);
    MY_LOG("rEE.flt_ctrl_1 = 0x%8x", rEE.flt_ctrl_1);
    MY_LOG("rEE.flt_ctrl_2 = 0x%8x", rEE.flt_ctrl_2);
    MY_LOG("rEE.glut_ctrl_01 = 0x%8x", rEE.glut_ctrl_01);
    MY_LOG("rEE.glut_ctrl_02 = 0x%8x", rEE.glut_ctrl_02);
    MY_LOG("rEE.glut_ctrl_03 = 0x%8x", rEE.glut_ctrl_03);
    MY_LOG("rEE.glut_ctrl_04 = 0x%8x", rEE.glut_ctrl_04);
    MY_LOG("rEE.glut_ctrl_05 = 0x%8x", rEE.glut_ctrl_05);
    MY_LOG("rEE.glut_ctrl_06 = 0x%8x", rEE.glut_ctrl_06);
    MY_LOG("rEE.edtr_ctrl = 0x%8x", rEE.edtr_ctrl);
    MY_LOG("rEE.glut_ctrl_07 = 0x%8x", rEE.glut_ctrl_07);
    MY_LOG("rEE.glut_ctrl_08 = 0x%8x", rEE.glut_ctrl_08);
    MY_LOG("rEE.glut_ctrl_09 = 0x%8x", rEE.glut_ctrl_09);
    MY_LOG("rEE.glut_ctrl_10 = 0x%8x", rEE.glut_ctrl_10);
    MY_LOG("rEE.glut_ctrl_11 = 0x%8x", rEE.glut_ctrl_11);
    #endif
}

//++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
//
//++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++

/*

MVOID
IspTuningCustom::
refine_BOK(RAWIspCamInfo const& rCamInfo, IspNvramRegMgr & rIspRegMgr, ISP_NVRAM_BOK_T& rBOK)
{
    #if 0
    MY_LOG("%s()\n", __FUNCTION__);
    // TODO: Add your code below...

    MY_LOG("rCCR.con = 0x%8x", rCCR.con);
    MY_LOG("rCCR.ylut = 0x%8x", rCCR.ylut);
    MY_LOG("rCCR.uvlut = 0x%8x", rCCR.uvlut);
    MY_LOG("rCCR.ylut2 = 0x%8x", rCCR.ylut2);
    MY_LOG("rCCR.sat_ctrl = 0x%8x", rCCR.sat_ctrl);
    MY_LOG("rCCR.uvlut_sp = 0x%8x", rCCR.uvlut_sp);
    #endif
}

*/

//++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
//
//++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++

// Use 3DNR for frame rate improvement
// Ratio|
//  100%|         /-----
//      |        /
//      |       /
//      |      /
//      |     /
//   0% +----|----|------
//      TH_LOW  TH_HIGH
#define LIMITER(Input, LowerBound, UpperBound)  do { if (Input > UpperBound){Input = UpperBound;} if (Input < LowerBound){Input = LowerBound;} } while (0)

// Note: X2 must be larger than or equal to X1.
inline MINT32 Nr3dLmtInterpolation(MINT32 TargetX, MINT32 X1, MINT32 Y1, MINT32 X2, MINT32 Y2)
{
    MINT32 TargetY = 0;
	MINT32 TempValue = 0;
	MINT32 RoundingValue = 0;

    if (X1 == X2)
    {
        TargetY = Y1;
    }
	else if ( TargetX <= X1 )
	{
		TargetY = Y1;
	}
	else if ( TargetX >= X2 )
	{
		TargetY = Y2;
	}
	else    // if (X1 <= TargetX <= X2), then interpolation.
	{
		TempValue = (TargetX - X1) * (Y2 - Y1);
		RoundingValue = (X2 - X1) >> 1;
		TargetY = (TempValue + RoundingValue) / (X2 - X1) + Y1;
	}

    return TargetY;

}

/*
MVOID
IspTuningCustom::
refine_NR3D(RAWIspCamInfo const& rCamInfo, IspNvramRegMgr & rIspRegMgr, ISP_NVRAM_NR3D_T& rNR3D)
{



}
*/
//++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
//
//++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
MVOID
IspTuningCustom::
refine_MFB(RAWIspCamInfo const& rCamInfo, IspNvramRegMgr & rIspRegMgr, ISP_NVRAM_MFB_T& rMFB)
{


    if(rCamInfo.eIspProfile == EIspProfile_MFNR_After_Blend) {
        rMFB.con.bits.BLD_LL_DB_EN = 0;
        rMFB.con.bits.BLD_LL_BRZ_EN = 0;
    }

#if 0
    MY_LOG("%s()\n", __FUNCTION__);
    MY_LOG("rMFB.profile = %d", rCamInfo.eIspProfile);
    MY_LOG("rMFB.con = 0x%8x", rMFB.con.val);
    MY_LOG("rMFB.ll_con1 = 0x%8x", rMFB.ll_con1.val);
    MY_LOG("rMFB.ll_con2 = 0x%8x", rMFB.ll_con2.val);
    MY_LOG("rMFB.ll_con4 = 0x%8x", rMFB.ll_con4.val);
#endif
}

//++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
//
//++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
MVOID
IspTuningCustom::
refine_MIXER3(RAWIspCamInfo const& rCamInfo, IspNvramRegMgr & rIspRegMgr, ISP_NVRAM_MIXER3_T& rMIXER3)
{
    #if 0
    MY_LOG("%s()\n", __FUNCTION__);
    // TODO: Add your code below...

    rMIXER3.ctrl_0.bits.MIX3_WT_SEL = 1;
    rMIXER3.ctrl_0.bits.MIX3_B0 = 0;
    rMIXER3.ctrl_0.bits.MIX3_B1 = 0xFF;
    rMIXER3.ctrl_0.bits.MIX3_DT = 1;

    rMIXER3.ctrl_1.bits.MIX3_M0 = 0;
    rMIXER3.ctrl_1.bits.MIX3_M1 = 0xFF;

    MY_LOG("rMIXER3.ctrl_0 = 0x%8x", rMIXER3.ctrl_0);
    MY_LOG("rMIXER3.ctrl_1 = 0x%8x", rMIXER3.ctrl_1);
    MY_LOG("rMIXER3.spare = 0x%8x", rMIXER3.spare);
    #endif
}

//++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
//
//++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
MVOID
IspTuningCustom::
refine_LCE(RAWIspCamInfo const& rCamInfo, IspNvramRegMgr & rIspRegMgr, ISP_NVRAM_LCE_T& rLCE)
{
    #if 0
    MY_LOG("%s()\n", __FUNCTION__);
    MY_LOG("rLCE.qua = 0x%8x", rLCE.qua);
    #endif
}

//++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
//
//++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
EIndex_CCM_T
IspTuningCustom::
evaluate_CCM_index(RAWIspCamInfo const& rCamInfo)
{
    #if 0
    MY_LOG(
        "[+evaluate_CCM_index]"
        "(eIdx_CCM, i4CCT, i4FluorescentIndex)=(%d, %d, %d)"
        , rCamInfo.eIdx_CCM
        , rCamInfo.rAWBInfo.i4CCT
        , rCamInfo.rAWBInfo.i4FluorescentIndex);
    #endif

    EIndex_CCM_T eIdx_CCM_new = rCamInfo.eIdx_CCM;

//    -----------------|---|---|--------------|---|---|------------------
//                                THA TH1 THB              THC TH2  THD

    MINT32 const THA = 3318;
    MINT32 const TH1 = 3484;
    MINT32 const THB = 3667;
    MINT32 const THC = 4810;
    MINT32 const TH2 = 5050;
    MINT32 const THD = 5316;
    MINT32 const F_IDX_TH1 = 25;
    MINT32 const F_IDX_TH2 = -25;

    switch  (rCamInfo.eIdx_CCM)
    {
    case eIDX_CCM_TL84:
        if  ( rCamInfo.rAWBInfo.i4CCT < THB )
        {
            eIdx_CCM_new = eIDX_CCM_TL84;
        }
        else if ( rCamInfo.rAWBInfo.i4CCT < THD )
        {
            if  ( rCamInfo.rAWBInfo.i4FluorescentIndex < F_IDX_TH2 )
                eIdx_CCM_new = eIDX_CCM_CWF;
            else
                eIdx_CCM_new = eIDX_CCM_TL84;
        }
        else
        {
            eIdx_CCM_new = eIDX_CCM_D65;
        }
        break;
    case eIDX_CCM_CWF:
        if  ( rCamInfo.rAWBInfo.i4CCT < THA )
        {
            eIdx_CCM_new = eIDX_CCM_TL84;
        }
        else if ( rCamInfo.rAWBInfo.i4CCT < THD )
        {
            if  ( rCamInfo.rAWBInfo.i4FluorescentIndex > F_IDX_TH1 )
                eIdx_CCM_new = eIDX_CCM_TL84;
            else
                eIdx_CCM_new = eIDX_CCM_CWF;
        }
        else
        {
            eIdx_CCM_new = eIDX_CCM_D65;
        }
        break;
    case eIDX_CCM_D65:
        if  ( rCamInfo.rAWBInfo.i4CCT > THC )
        {
	        eIdx_CCM_new = eIDX_CCM_D65;
        }
        else if ( rCamInfo.rAWBInfo.i4CCT > TH1 )
        {
            if(rCamInfo.rAWBInfo.i4FluorescentIndex > F_IDX_TH2)
                eIdx_CCM_new = eIDX_CCM_TL84;
            else
                eIdx_CCM_new = eIDX_CCM_CWF;
        }
        else
        {
            eIdx_CCM_new = eIDX_CCM_TL84;
        }
        break;
    default:
        break;
    }

    if  ( rCamInfo.eIdx_CCM != eIdx_CCM_new )
    {
        MY_LOG(
            "[-evaluate_CCM_index] CCM Idx(old,new)=(%d,%d)"
            , rCamInfo.eIdx_CCM, eIdx_CCM_new
        );
    }

    return  eIdx_CCM_new;
}

//++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
//
//++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
MBOOL
IspTuningCustom::
is_to_invoke_smooth_ccm_with_preference_gain(RAWIspCamInfo const& rCamInfo)
{
    return MTRUE;
}

//++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
//
//++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
MBOOL
IspTuningCustom::
is_to_invoke_isp_interpolation(RAWIspCamInfo const& rCamInfo)
{
#if 0
    if(TUNING_FOR_AIS) {
        if(IS_AIS) {
            if(
                   rCamInfo.eIspProfile == EIspProfile_MFB_Capture_EE_Off
                || rCamInfo.eIspProfile == EIspProfile_MFB_Capture_EE_Off_SWNR
                || rCamInfo.eIspProfile == EIspProfile_MFB_PostProc_EE_Off
                || rCamInfo.eIspProfile == EIspProfile_MFB_Blending_All_Off
                || rCamInfo.eIspProfile == EIspProfile_MFB_Blending_All_Off_SWNR
                || rCamInfo.eIspProfile == EIspProfile_MFB_PostProc_ANR_EE
                || rCamInfo.eIspProfile == EIspProfile_MFB_PostProc_ANR_EE_SWNR
                || rCamInfo.eIspProfile == EIspProfile_MFB_PostProc_Mixing
                || rCamInfo.eIspProfile == EIspProfile_MFB_PostProc_Mixing_SWNR
                || rCamInfo.eIspProfile == EIspProfile_VSS_MFB_Capture_EE_Off
                || rCamInfo.eIspProfile == EIspProfile_VSS_MFB_Capture_EE_Off_SWNR
                || rCamInfo.eIspProfile == EIspProfile_VSS_MFB_PostProc_EE_Off
                || rCamInfo.eIspProfile == EIspProfile_VSS_MFB_Blending_All_Off
                || rCamInfo.eIspProfile == EIspProfile_VSS_MFB_Blending_All_Off_SWNR
                || rCamInfo.eIspProfile == EIspProfile_VSS_MFB_PostProc_ANR_EE
                || rCamInfo.eIspProfile == EIspProfile_VSS_MFB_PostProc_ANR_EE_SWNR
                || rCamInfo.eIspProfile == EIspProfile_VSS_MFB_PostProc_Mixing
                || rCamInfo.eIspProfile == EIspProfile_VSS_MFB_PostProc_Mixing_SWNR

            )
			{
	            return MFALSE;
			}
       }
    }
#endif
    return MTRUE;
}

//++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
//
//++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
MINT32
IspTuningCustom::
get_CCM_smooth_method(RAWIspCamInfo const& rCamInfo)
{
    // 0: CCM (without flash info)
    // 1: enable flash CCM
    return 0;
}

MINT32
IspTuningCustom::
get_CCM_proj_method(RAWIspCamInfo const& rCamInfo)
{
    //I4ProjMethod=0, the same with known smooth CCM full projection method
    //I4ProjMethod=1, for test only, the range of the i4ProjMaxdist is 1~100
    //I4ProjMethod=2, for Sony Dx0 test using CWF light source, the range of the i4ProjMaxdist is 1~300
#if 0
    if (getSensorDev() == ESensorDev_Sub) {
        return 2;
    }
    else if (getSensorDev() == ESensorDev_Main){
        return 1;
    }
    else{
      return 0;
    }
#endif

    return 0;
}


MINT32
IspTuningCustom::
get_CCM_proj_max(RAWIspCamInfo const& rCamInfo)
{
#if 0
    if (getSensorDev() == ESensorDev_Sub) {
        return 300;
    }
    else if (getSensorDev() == ESensorDev_Main){
        return 200;
    }
    else{
      return 1;
    }
#endif

    return 1;

}



//++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
//
//++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
/*
MUINT32
IspTuningCustom::
get_SWNR_ENC_enable_ISO_threshold(RAWIspCamInfo const& rCamInfo)
{
    if ((rCamInfo.eIspProfile == EIspProfile_Capture_SWNR) ||
        (rCamInfo.eIspProfile == EIspProfile_VSS_Capture_SWNR) ||
        (rCamInfo.eIspProfile == EIspProfile_PureRAW_Capture_SWNR)) {
         return get_normal_SWNR_ENC_enable_ISO_threshold();
    }
    else if (
             (rCamInfo.eIspProfile == EIspProfile_MFB_Capture_EE_Off_SWNR) ||
             (rCamInfo.eIspProfile == EIspProfile_MFB_PostProc_ANR_EE_SWNR) ||
             (rCamInfo.eIspProfile == EIspProfile_MFB_PostProc_Mixing_SWNR) ||
             (rCamInfo.eIspProfile == EIspProfile_VSS_MFB_Capture_EE_Off_SWNR) ||
             (rCamInfo.eIspProfile == EIspProfile_VSS_MFB_PostProc_ANR_EE_SWNR) ||
             (rCamInfo.eIspProfile == EIspProfile_VSS_MFB_PostProc_Mixing_SWNR)

            )
    {
         return get_MFB_SWNR_ENC_enable_ISO_threshold();
    }

    return 0;
}
*/
//++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
//
//++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
EIndex_PCA_LUT_T
IspTuningCustom::
evaluate_PCA_LUT_index(RAWIspCamInfo const& rCamInfo)
{
    //MY_LOG("%s()\n", __FUNCTION__);

    // TODO: Add your code below...

/*
    MY_LOG(
        "[+evaluate_PCA_LUT_index]"
        "(rCamInfo.eIdx_PCA_LUT, rCamInfo.rAWBInfo.i4CCT, rCamInfo.rAWBInfo.i4FluorescentIndex)=(%d, %d, %d)"
        , rCamInfo.eIdx_PCA_LUT, rCamInfo.rAWBInfo.i4CCT, rCamInfo.rAWBInfo.i4FluorescentIndex
    );
*/
    EIndex_PCA_LUT_T eIdx_PCA_LUT_new = rCamInfo.eIdx_PCA_LUT;

//    -----------------|-------|--------------|-------|------------------
//                    THA     THB            THC     THD

    MINT32 const THA = 3318;
    MINT32 const THB = 3667;
    MINT32 const THC = 4810;
    MINT32 const THD = 5316;

    switch  (rCamInfo.eIdx_PCA_LUT)
    {
    case eIDX_PCA_HIGH_2:
        eIdx_PCA_LUT_new = eIDX_PCA_HIGH;
        break;
    case eIDX_PCA_MIDDLE_2:
        eIdx_PCA_LUT_new = eIDX_PCA_MIDDLE;
        break;
    case eIDX_PCA_LOW_2:
        eIdx_PCA_LUT_new = eIDX_PCA_LOW;
        break;
    case eIDX_PCA_HIGH:
        if  ( rCamInfo.rAWBInfo.i4CCT < THA )
        {
            eIdx_PCA_LUT_new = eIDX_PCA_LOW;
        }
        else if ( rCamInfo.rAWBInfo.i4CCT < THC )
        {
            eIdx_PCA_LUT_new = eIDX_PCA_MIDDLE;
        }
        else
        {
            eIdx_PCA_LUT_new = eIDX_PCA_HIGH;
        }
        break;
    case eIDX_PCA_MIDDLE:
        if  ( rCamInfo.rAWBInfo.i4CCT > THD )
        {
            eIdx_PCA_LUT_new = eIDX_PCA_HIGH;
        }
        else if ( rCamInfo.rAWBInfo.i4CCT < THA )
        {
            eIdx_PCA_LUT_new = eIDX_PCA_LOW;
        }
        else
        {
            eIdx_PCA_LUT_new = eIDX_PCA_MIDDLE;
        }
        break;
    case eIDX_PCA_LOW:
        if  ( rCamInfo.rAWBInfo.i4CCT > THD )
        {
	        eIdx_PCA_LUT_new = eIDX_PCA_HIGH;
        }
        else if ( rCamInfo.rAWBInfo.i4CCT > THB )
        {
            eIdx_PCA_LUT_new = eIDX_PCA_MIDDLE;
        }
        else
        {
            eIdx_PCA_LUT_new = eIDX_PCA_LOW;
        }
        break;
    }

    if  ( rCamInfo.eIdx_PCA_LUT != eIdx_PCA_LUT_new )
    {
        MY_LOG(
            "[-evaluate_PCA_LUT_index] PCA_LUT_index(old,new)=(%d,%d)"
            , rCamInfo.eIdx_PCA_LUT, eIdx_PCA_LUT_new
        );
    }

    return eIdx_PCA_LUT_new;
}

//++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
//
//++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
EIndex_PCA_LUT_T
IspTuningCustom::
evaluate_PCA_LUT_index_for_IHDR(RAWIspCamInfo const& rCamInfo)
{
    MY_LOG("%s()\n", __FUNCTION__);

    // TODO: Add your code below...


    MY_LOG(
        "[+evaluate_PCA_LUT_index]"
        "(rCamInfo.eIdx_PCA_LUT, rCamInfo.rAWBInfo.i4CCT, rCamInfo.rAWBInfo.i4FluorescentIndex)=(%d, %d, %d)"
        , rCamInfo.eIdx_PCA_LUT, rCamInfo.rAWBInfo.i4CCT, rCamInfo.rAWBInfo.i4FluorescentIndex
    );

    EIndex_PCA_LUT_T eIdx_PCA_LUT_new = rCamInfo.eIdx_PCA_LUT;

//    -----------------|-------|--------------|-------|------------------
//                    THA     THB            THC     THD

    MINT32 const THA = 3318;
    MINT32 const THB = 3667;
    MINT32 const THC = 4810;
    MINT32 const THD = 5316;

    switch  (rCamInfo.eIdx_PCA_LUT)
    {
    case eIDX_PCA_HIGH:
        eIdx_PCA_LUT_new = eIDX_PCA_HIGH_2;
        break;
    case eIDX_PCA_MIDDLE:
        eIdx_PCA_LUT_new = eIDX_PCA_MIDDLE_2;
        break;
    case eIDX_PCA_LOW:
        eIdx_PCA_LUT_new = eIDX_PCA_LOW_2;
        break;
    case eIDX_PCA_HIGH_2:
        if  ( rCamInfo.rAWBInfo.i4CCT < THA )
        {
            eIdx_PCA_LUT_new = eIDX_PCA_LOW_2;
        }
        else if ( rCamInfo.rAWBInfo.i4CCT < THC )
        {
            eIdx_PCA_LUT_new = eIDX_PCA_MIDDLE_2;
        }
        else
        {
            eIdx_PCA_LUT_new = eIDX_PCA_HIGH_2;
        }
        break;
    case eIDX_PCA_MIDDLE_2:
        if  ( rCamInfo.rAWBInfo.i4CCT > THD )
        {
            eIdx_PCA_LUT_new = eIDX_PCA_HIGH_2;
        }
        else if ( rCamInfo.rAWBInfo.i4CCT < THA )
        {
            eIdx_PCA_LUT_new = eIDX_PCA_LOW_2;
        }
        else
        {
            eIdx_PCA_LUT_new = eIDX_PCA_MIDDLE_2;
        }
        break;
    case eIDX_PCA_LOW_2:
        if  ( rCamInfo.rAWBInfo.i4CCT > THD )
        {
	        eIdx_PCA_LUT_new = eIDX_PCA_HIGH_2;
        }
        else if ( rCamInfo.rAWBInfo.i4CCT > THB )
        {
            eIdx_PCA_LUT_new = eIDX_PCA_MIDDLE_2;
        }
        else
        {
            eIdx_PCA_LUT_new = eIDX_PCA_LOW_2;
        }
        break;
    }

    if  ( rCamInfo.eIdx_PCA_LUT != eIdx_PCA_LUT_new )
    {
        MY_LOG(
            "[-evaluate_PCA_LUT_index] PCA_LUT_index(old,new)=(%d,%d)"
            , rCamInfo.eIdx_PCA_LUT, eIdx_PCA_LUT_new
        );
    }

    return eIdx_PCA_LUT_new;
}

EIndex_PCA_LUT_T
IspTuningCustom::
evaluate_SmoothPCA_LUT_idx(RAWIspCamInfo const& rCamInfo, MBOOL &inter, MUINT32 &CTlower, MUINT32 &CTupper)
{

//    -----------------|-------|--------------|-------
//                    THA     THB            THC

    MINT32 const THA = 2856;  //Low
    MINT32 const THB = 4000;  //Mid
    MINT32 const THC = 6500;  //High

    MBOOL fgHDR = MFALSE;
    if ((rCamInfo.eIspProfile == EIspProfile_iHDR_Preview)                     ||
       (rCamInfo.eIspProfile == EIspProfile_zHDR_Preview)                     ||
       (rCamInfo.eIspProfile == EIspProfile_mHDR_Preview)                     ||
       (rCamInfo.eIspProfile == EIspProfile_iHDR_Video)                       ||
       (rCamInfo.eIspProfile == EIspProfile_zHDR_Video)                       ||
       (rCamInfo.eIspProfile == EIspProfile_mHDR_Video)                       ||
       (rCamInfo.eIspProfile == EIspProfile_iHDR_Preview_VSS)                 ||
       (rCamInfo.eIspProfile == EIspProfile_zHDR_Preview_VSS)                 ||
       (rCamInfo.eIspProfile == EIspProfile_mHDR_Preview_VSS)                 ||
       (rCamInfo.eIspProfile == EIspProfile_iHDR_Video_VSS)                   ||
       (rCamInfo.eIspProfile == EIspProfile_zHDR_Video_VSS)                   ||
       (rCamInfo.eIspProfile == EIspProfile_mHDR_Video_VSS)                   ||
       (rCamInfo.eIspProfile == EIspProfile_zHDR_Capture)                     ||
       (rCamInfo.eIspProfile == EIspProfile_mHDR_Capture)                     ||
       (rCamInfo.eIspProfile == EIspProfile_Auto_iHDR_Preview)                ||
       (rCamInfo.eIspProfile == EIspProfile_Auto_zHDR_Preview)                ||
       (rCamInfo.eIspProfile == EIspProfile_Auto_mHDR_Preview)                ||
       (rCamInfo.eIspProfile == EIspProfile_Auto_iHDR_Video)                  ||
       (rCamInfo.eIspProfile == EIspProfile_Auto_zHDR_Video)                  ||
       (rCamInfo.eIspProfile == EIspProfile_Auto_mHDR_Video)                  ||
       (rCamInfo.eIspProfile == EIspProfile_Auto_iHDR_Preview_VSS)            ||
       (rCamInfo.eIspProfile == EIspProfile_Auto_zHDR_Preview_VSS)            ||
       (rCamInfo.eIspProfile == EIspProfile_Auto_mHDR_Preview_VSS)            ||
       (rCamInfo.eIspProfile == EIspProfile_Auto_iHDR_Video_VSS)              ||
       (rCamInfo.eIspProfile == EIspProfile_Auto_zHDR_Video_VSS)              ||
       (rCamInfo.eIspProfile == EIspProfile_Auto_mHDR_Video_VSS)              ||
       (rCamInfo.eIspProfile == EIspProfile_Auto_zHDR_Capture)                ||
       (rCamInfo.eIspProfile == EIspProfile_Auto_mHDR_Capture)                ||
       (rCamInfo.eIspProfile == EIspProfile_zHDR_Capture_MFNR_Before_Blend)   ||
       (rCamInfo.eIspProfile == EIspProfile_zHDR_Capture_MFNR_Single)         ||
       (rCamInfo.eIspProfile == EIspProfile_zHDR_Capture_MFNR_MFB)            ||
       (rCamInfo.eIspProfile == EIspProfile_zHDR_Capture_MFNR_After_Blend)    ||
       (rCamInfo.eIspProfile == EIspProfile_EIS_iHDR_Preview)                 ||
       (rCamInfo.eIspProfile == EIspProfile_EIS_zHDR_Preview)                 ||
       (rCamInfo.eIspProfile == EIspProfile_EIS_mHDR_Preview)                 ||
       (rCamInfo.eIspProfile == EIspProfile_EIS_iHDR_Video)                   ||
       (rCamInfo.eIspProfile == EIspProfile_EIS_zHDR_Video)                   ||
       (rCamInfo.eIspProfile == EIspProfile_EIS_mHDR_Video)                   ||
       (rCamInfo.eIspProfile == EIspProfile_EIS_Auto_iHDR_Preview)            ||
       (rCamInfo.eIspProfile == EIspProfile_EIS_Auto_zHDR_Preview)            ||
       (rCamInfo.eIspProfile == EIspProfile_EIS_Auto_mHDR_Preview)            ||
       (rCamInfo.eIspProfile == EIspProfile_EIS_Auto_iHDR_Video)              ||
       (rCamInfo.eIspProfile == EIspProfile_EIS_Auto_zHDR_Video)              ||
       (rCamInfo.eIspProfile == EIspProfile_EIS_Auto_mHDR_Video))
    {    fgHDR = MTRUE;
    }

    if(rCamInfo.rAWBInfo.i4CCT < THA){

        inter = MFALSE;
        CTlower = THA;
        CTupper = THA;

        if(fgHDR){
            return eIDX_PCA_LOW_2;
        }
        else{
            return eIDX_PCA_LOW;
        }
    }

    else if(rCamInfo.rAWBInfo.i4CCT < THB){

        inter = MTRUE;
        CTlower = THA;
        CTupper = THB;

        if(fgHDR){ 
            return eIDX_PCA_LOW_2;
        }
        else{
            return eIDX_PCA_LOW;
        }
    }

    else if(rCamInfo.rAWBInfo.i4CCT < THC){

        inter = MTRUE;
        CTlower = THB;
        CTupper = THC;

        if(fgHDR){ 
            return eIDX_PCA_MIDDLE_2;
        }
        else{
            return eIDX_PCA_MIDDLE;
        }
    }

    else{

        inter = MFALSE;
        CTlower = THC;
        CTupper = THC;

        if(fgHDR){ 
            return eIDX_PCA_HIGH_2;
        }
        else{
            return eIDX_PCA_HIGH;
        }
    }

}

EIndex_ANR_TBL_ISO_T
IspTuningCustom::
evaluate_ANR_TBL_ISO_index(RAWIspCamInfo const& rCamInfo)
{
    EIndex_ANR_TBL_ISO_T eIdx_ANR_TBL_ISO = static_cast<EIndex_ANR_TBL_ISO_T>(ANR_TBL_SUPPORT_ISO_NUM - 1) ;

    MUINT32* tempISO;

    switch (getSensorDev()){

        case ESensorDev_Main:
            tempISO = &(ANR_TBL_SUPPORT_ISO[ISO_TOP_SUPPORT_MODE_main][0]);
            break;

        case ESensorDev_Sub:
            tempISO = &(ANR_TBL_SUPPORT_ISO[ISO_TOP_SUPPORT_MODE_sub][0]);
            break;

        case ESensorDev_MainSecond:
            tempISO = &(ANR_TBL_SUPPORT_ISO[ISO_TOP_SUPPORT_MODE_main2][0]);
            break;

        default:
            tempISO = &(ANR_TBL_SUPPORT_ISO[ISO_TOP_SUPPORT_MODE_main][0]);
            break;
    }

    for(int i=0; i<ANR_TBL_SUPPORT_ISO_NUM; i++){
        if (rCamInfo.u4ISOValue < tempISO[i]){
            eIdx_ANR_TBL_ISO = static_cast<EIndex_ANR_TBL_ISO_T> (i);
            break;
        }
    }

    if (rCamInfo.u4ISOValue >= tempISO[ANR_TBL_SUPPORT_ISO_NUM-1])
    {
        eIdx_ANR_TBL_ISO = static_cast<EIndex_ANR_TBL_ISO_T> (ANR_TBL_SUPPORT_ISO_NUM-1);
    }

    return eIdx_ANR_TBL_ISO;
}

EIndex_ANR_TBL_CT_T
IspTuningCustom::
evaluate_ANR_TBL_CT_index(RAWIspCamInfo const& rCamInfo)
{

    EIndex_ANR_TBL_CT_T eIdx_ANR_TBL_new = rCamInfo.eIdx_ANR_TBL_CT;

    MINT32 const THA = 4317;

    if( rCamInfo.rAWBInfo.i4CCT < THA){
        eIdx_ANR_TBL_new = eIDX_ANR_TBL_CCT_LOW;
    }
    else{
        eIdx_ANR_TBL_new = eIDX_ANR_TBL_CCT_HIGH;
    }

    if  ( rCamInfo.eIdx_ANR_TBL_CT != eIdx_ANR_TBL_new )
    {
        MY_LOG(
            "[-evaluate_ANR_TBL_CT_index] ANR_TBL_CT_index(old,new)=(%d,%d)"
            , rCamInfo.eIdx_ANR_TBL_CT, eIdx_ANR_TBL_new
        );
    }

    return eIdx_ANR_TBL_new;
}

/*******************************************************************************
*
* eIdx_Shading_CCT_old:
*   [in] the previous color temperature index
*           eIDX_Shading_CCT_ALight
*           eIDX_Shading_CCT_CWF
*           eIDX_Shading_CCT_D65
*
* i4CCT:
*   [in] the current color temperature from 3A.
*
*
* return:
*   [out] the current color temperature index
*           eIDX_Shading_CCT_ALight
*           eIDX_Shading_CCT_CWF
*           eIDX_Shading_CCT_D65
*
*******************************************************************************/
EIndex_Shading_CCT_T
IspTuningCustom::
evaluate_Shading_CCT_index  (
        RAWIspCamInfo const& rCamInfo
)   const
{
    MINT32 i4CCT = rCamInfo.rAWBInfo.i4CCT;

    EIndex_Shading_CCT_T eIdx_Shading_CCT_new = rCamInfo.eIdx_Shading_CCT;

//    -----------------|----|----|--------------|----|----|------------------
//                   THH2  TH2  THL2                   THH1  TH1  THL1

    MINT32 const THL1 = 3257;
    MINT32 const THH1 = 3484;
    MINT32 const TH1 = (THL1+THH1)/2; //(THL1 +THH1)/2
    MINT32 const THL2 = 4673;
    MINT32 const THH2 = 5155;
    MINT32 const TH2 = (THL2+THH2)/2;//(THL2 +THH2)/2

    switch  (rCamInfo.eIdx_Shading_CCT)
    {
    case eIDX_Shading_CCT_ALight:
        if  ( i4CCT < THH1 )
        {
            eIdx_Shading_CCT_new = eIDX_Shading_CCT_ALight;
        }
        else if ( i4CCT <  TH2)
        {
            eIdx_Shading_CCT_new = eIDX_Shading_CCT_CWF;
        }
        else
        {
            eIdx_Shading_CCT_new = eIDX_Shading_CCT_D65;
        }
        break;
    case eIDX_Shading_CCT_CWF:
        if  ( i4CCT < THL1 )
        {
            eIdx_Shading_CCT_new = eIDX_Shading_CCT_ALight;
        }
        else if ( i4CCT < THH2 )
        {
            eIdx_Shading_CCT_new = eIDX_Shading_CCT_CWF;
        }
        else
        {
            eIdx_Shading_CCT_new = eIDX_Shading_CCT_D65;
        }
        break;
    case eIDX_Shading_CCT_D65:
        if  ( i4CCT < TH1 )
        {
            eIdx_Shading_CCT_new = eIDX_Shading_CCT_ALight;
        }
        else if ( i4CCT < THL2 )
        {
            eIdx_Shading_CCT_new = eIDX_Shading_CCT_CWF;
        }
        else
        {
            eIdx_Shading_CCT_new = eIDX_Shading_CCT_D65;
        }
        break;
    }

    if  ( rCamInfo.eIdx_Shading_CCT != eIdx_Shading_CCT_new )
    {
        MY_LOG(
            "[-evaluate_Shading_CCT_index] Shading CCT Idx(old,new)=(%d,%d), i4CCT = %d\n"
            , rCamInfo.eIdx_Shading_CCT, eIdx_Shading_CCT_new,i4CCT
        );
    }

    return  eIdx_Shading_CCT_new;
}

MVOID
IspTuningCustom::
reset_ISO_SmoothBuffer()
{
    total_RA_num_frames_= 0;
    MY_LOG("reset_ISO total_RA_num_frames_=0");
    memset(ISO_Buffer_, 6, sizeof(ISO_Buffer_));
    MY_LOG("[%s] total_RA_num_frames_(%d)", __FUNCTION__, total_RA_num_frames_ );
    MY_LOG("[%s] ISO_Buffer_[] = {%d, %d, %d, %d, %d, %d, %d, %d, %d, %d\n}", __FUNCTION__,
        ISO_Buffer_[0], ISO_Buffer_[1], ISO_Buffer_[2], ISO_Buffer_[3], ISO_Buffer_[4],
        ISO_Buffer_[5], ISO_Buffer_[6], ISO_Buffer_[7], ISO_Buffer_[8], ISO_Buffer_[9] );
}

static MINT32 ratioMapping(MINT32 i4Iso)
{
#define LERP(x, lo_x, lo_y, hi_x, hi_y)\
    (((hi_x) - (x))*(lo_y) + ((x) - (lo_x))*(hi_y)) / ((hi_x) - (lo_x))

    static const MINT32 iso[10] =
    {100, 200, 400, 800, 1200, 1600, 2000, 2400, 2800, 3200};

    static const MINT32 rto[10] =
    //{24, 22, 20, 18, 16, 14, 12, 10, 8, 6}; //Tower modify for iso1600 Noise 2014-12-26
    //{30, 28, 26, 24, 22, 20, 18, 16, 14, 12};
    {32, 32, 32, 30, 28, 26, 24, 22, 20, 18};

    MINT32 i = 0;
    MINT32 i4Rto = 32;

    if (i4Iso < iso[0])
    {
        i4Rto = rto[0];
    }
    else if (i4Iso >= iso[9])
    {
        i4Rto = rto[9];
    }
    else
    {
        for (i = 1; i < 10; i++)
        {
            if (i4Iso < iso[i])
                break;
        }
        i4Rto = LERP(i4Iso, iso[i-1], rto[i-1], iso[i], rto[i]);
    }
    return i4Rto;
}

MINT32
IspTuningCustom::
evaluate_Shading_Ratio  (
        RAWIspCamInfo const& rCamInfo
)
{
    /*
        Sample code for evaluate shading ratio.
        The shading ratio is an integer ranging from 0(0%) to 32(100%).
        All informations can be obtained via rCamInfo.
        The following sample code shows a shading ratio evaluated by ISO value with temporal smoothness.
    */
    MINT32 Avg_Frm_Cnt = 5;
    MINT32 i = 0;
    MINT32 i4Rto = 8; //32;
    MINT32 i4Iso = rCamInfo.rAEInfo.u4RealISOValue;

    int idx = total_RA_num_frames_ % Avg_Frm_Cnt;
    int *p_global_Ra = ISO_Buffer_;
    int n_frames, avgISO;

    ISO_Buffer_[idx] = i4Iso;

    // to prevent total frames overflow
    if (total_RA_num_frames_ >= 65535){
        total_RA_num_frames_ = 0;
    }
    total_RA_num_frames_++;
    if (total_RA_num_frames_ < 0){
        avgISO = 32;
        MY_LOG("[%s] first avgISO = %d\n", __FUNCTION__, avgISO);
    } else {
        // smooth
        n_frames = ( total_RA_num_frames_ <  Avg_Frm_Cnt) ? (total_RA_num_frames_) : (Avg_Frm_Cnt);
        avgISO = 0;
        for (int k = 0; k < n_frames; k++) {
            avgISO += ISO_Buffer_[k];
        }
        avgISO /= n_frames;
#if 0
        MY_LOG("[%s] ISO_Buffer_[] = {%d, %d, %d, %d, %d, %d, %d, %d, %d, %d\n}", __FUNCTION__,
        ISO_Buffer_[0], ISO_Buffer_[1], ISO_Buffer_[2], ISO_Buffer_[3], ISO_Buffer_[4],
        ISO_Buffer_[5], ISO_Buffer_[6], ISO_Buffer_[7], ISO_Buffer_[8], ISO_Buffer_[9] );
        MY_LOG("[%s] avgISO = %d", __FUNCTION__, avgISO);
#endif
        if (rCamInfo.rFlashInfo.isFlash == 2)
        {
            i4Rto = ratioMapping(i4Iso);
            MY_LOG("[%s] Main flash iso(%d), ratio(%d)", __FUNCTION__, i4Iso, i4Rto);
        }
        else
        {
            i4Rto = ratioMapping(avgISO);
        }
    }
    return i4Rto;
}

//++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
// Sample code for sub sensor customization
//++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
#if 0
EIndex_Shading_CCT_T
CTIspTuningCustom<ESensorDev_Sub>::
evaluate_Shading_CCT_index(RAWIspCamInfo const& rCamInfo) const
{
    MY_LOG("CTIspTuningCustom<ESensorDev_Main> %s()\n", __FUNCTION__);

    MINT32 i4CCT = rCamInfo.rAWBInfo.i4CCT;

    EIndex_Shading_CCT_T eIdx_Shading_CCT_new = rCamInfo.eIdx_Shading_CCT;

//    -----------------|----|----|--------------|----|----|------------------
//                   THH2  TH2  THL2                   THH1  TH1  THL1

    MINT32 const THL1 = 2500;//3257;
    MINT32 const THH1 = 2800;//3484;
    MINT32 const TH1 = (THL1+THH1)/2; //(THL1 +THH1)/2
    MINT32 const THL2 = 4673;
    MINT32 const THH2 = 5155;
    MINT32 const TH2 = (THL2+THH2)/2;//(THL2 +THH2)/2

    switch  (rCamInfo.eIdx_Shading_CCT)
    {
    case eIDX_Shading_CCT_ALight:
        if  ( i4CCT < THH1 )
        {
            eIdx_Shading_CCT_new = eIDX_Shading_CCT_ALight;
        }
        else if ( i4CCT <  TH2)
        {
            eIdx_Shading_CCT_new = eIDX_Shading_CCT_CWF;
        }
        else
        {
            eIdx_Shading_CCT_new = eIDX_Shading_CCT_D65;
        }
        break;
    case eIDX_Shading_CCT_CWF:
        if  ( i4CCT < THL1 )
        {
            eIdx_Shading_CCT_new = eIDX_Shading_CCT_ALight;
        }
        else if ( i4CCT < THH2 )
        {
            eIdx_Shading_CCT_new = eIDX_Shading_CCT_CWF;
        }
        else
        {
            eIdx_Shading_CCT_new = eIDX_Shading_CCT_D65;
        }
        break;
    case eIDX_Shading_CCT_D65:
        if  ( i4CCT < TH1 )
        {
         eIdx_Shading_CCT_new = eIDX_Shading_CCT_ALight;
        }
        else if ( i4CCT < THL2 )
        {
            eIdx_Shading_CCT_new = eIDX_Shading_CCT_CWF;
        }
        else
        {
            eIdx_Shading_CCT_new = eIDX_Shading_CCT_D65;
        }
        break;
    }

    if  ( rCamInfo.eIdx_Shading_CCT != eIdx_Shading_CCT_new )
    {
        MY_LOG(
            "[-evaluate_Shading_CCT_index] Shading CCT Idx(old,new)=(%d,%d), i4CCT = %d\n"
            , rCamInfo.eIdx_Shading_CCT, eIdx_Shading_CCT_new,i4CCT
        );
    }

    return  eIdx_Shading_CCT_new;
}
#endif


//++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
//
//++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++

MVOID*
IspTuningCustom::
get_ANR_TBL_ISO_IDX_info(ESensorDev_T eSensorDev) const
{
    switch (eSensorDev)
    {
    case ESensorDev_Main: //main
             return &ANR_TBL_SUPPORT_ISO[ISO_TOP_SUPPORT_MODE_main];
             break;

    case ESensorDev_Sub: //sub
             return &ANR_TBL_SUPPORT_ISO[ISO_TOP_SUPPORT_MODE_sub];
             break;

    case ESensorDev_MainSecond: //main
             return &ANR_TBL_SUPPORT_ISO[ISO_TOP_SUPPORT_MODE_main2];
             break;
    default:
        return &ANR_TBL_SUPPORT_ISO[ISO_TOP_SUPPORT_MODE_main];
    }
}

MUINT32
IspTuningCustom::
get_ANR_TBL_SUPPORT_info() const
{
    return ANR_TBL_SUPPORT_ISO_NUM;
}


//++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
//
//++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
MUINT32
IspTuningCustom::
remap_ISO_value(MUINT32 const u4Iso) const
{
    MUINT32 remapIso = u4Iso;

    //add your remap ISO code here

    //MY_LOG("[%s] ISO: in(%d), out(%d)", __FUNCTION__, u4Iso, remapIso);
    return remapIso;
}

MVOID*
IspTuningCustom::
get_feature_control(ESensorDev_T eSensorDev) const
{
    switch (eSensorDev)
    {
    case ESensorDev_Main: //main
        return &ispTopCtl;
    case ESensorDev_Sub: //sub
        return &ispTopCtl;
    case ESensorDev_MainSecond: //main2
        return &ispTopCtl;
    default:
        return &ispTopCtl;
    }

}

//++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
//
//++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
MBOOL
IspTuningCustom::
is_to_invoke_fine_tune(ESensorDev_T const eSensorDev)
{
    switch (eSensorDev)
    {
    case ESensorDev_Main: //main
        return MFALSE;
    case ESensorDev_Sub: //sub
        return MFALSE;
    case ESensorDev_MainSecond: //main2
        return MFALSE;
    default:
        return MFALSE;

    }
}

//++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
//
//++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
MVOID
IspTuningCustom::
fine_tune_ANR(RAWIspCamInfo const& rCamInfo,
                    NSIspExifDebug::IspExifDebugInfo_T::IspGmaInfo const& GmaExifInfo,
                    NSIspExifDebug::IspExifDebugInfo_T::IspLceInfo const& LceExifInfo,
                    ISP_NVRAM_ANR_T const& rANR_U, ISP_NVRAM_ANR_T const& rANR_L,
                    ISP_NVRAM_ANR_T& rANR)
{
// Sensor Mode : rCamInfo.eSensorMode
// ISO Value : rCamInfo.rAEInfo.u4RealISOValue
// ISP Profile : rCamInfo.eIspProfile
// FD ratio : rCamInfo.FDRatio
// Bin enable : rCamInfo.BinInfo.fgBIN
// RRZ ratio (include bin) : rCamInfo.rCropRzInfo
// GMA info : GmaExifInfo
// LCE info : LceExifInfo
// Upper ANR : rANR_U, customANR_main[0][rCamInfo.eIdx_ISO_U], customANR_main[1][rCamInfo.eIdx_ISO_U]
// Lower ANR : rANR_L, customANR_main[0][rCamInfo.eIdx_ISO_L], customANR_main[1][rCamInfo.eIdx_ISO_L]
// After interpolation & Output : rANR


}

//++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
//
//++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
MVOID
IspTuningCustom::
fine_tune_ANR2(RAWIspCamInfo const& rCamInfo,
                     NSIspExifDebug::IspExifDebugInfo_T::IspGmaInfo const& GmaExifInfo,
                     NSIspExifDebug::IspExifDebugInfo_T::IspLceInfo const& LceExifInfo,
                     ISP_NVRAM_ANR2_T const& rANR2_U, ISP_NVRAM_ANR2_T const& rANR2_L,
                     ISP_NVRAM_ANR2_T& rANR2)
{
// Sensor Mode : rCamInfo.eSensorMode
// ISO Value : rCamInfo.rAEInfo.u4RealISOValue
// ISP Profile : rCamInfo.eIspProfile
// FD ratio : rCamInfo.FDRatio
// Bin enable : rCamInfo.BinInfo.fgBIN
// RRZ ratio (include bin) : rCamInfo.rCropRzInfo
// GMA info : GmaExifInfo
// LCE info : LceExifInfo
// Upper ANR2 : rANR2_U, customANR2_main[0][rCamInfo.eIdx_ISO_U], customANR2_main[1][rCamInfo.eIdx_ISO_U]
// Lower ANR2 : rANR2_L, customANR2_main[0][rCamInfo.eIdx_ISO_L], customANR2_main[1][rCamInfo.eIdx_ISO_L]
// After interpolation & Output : rANR2


}


//++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
//
//++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
MVOID
IspTuningCustom::
fine_tune_EE(RAWIspCamInfo const& rCamInfo,
                  NSIspExifDebug::IspExifDebugInfo_T::IspGmaInfo const& GmaExifInfo,
                  NSIspExifDebug::IspExifDebugInfo_T::IspLceInfo const& LceExifInfo,
                  ISP_NVRAM_EE_T const& rEE_U, ISP_NVRAM_EE_T const& rEE_L,
                  ISP_NVRAM_EE_T& rEE)
{
// Sensor Mode : rCamInfo.eSensorMode
// ISO Value : rCamInfo.rAEInfo.u4RealISOValue
// ISP Profile : rCamInfo.eIspProfile
// FD ratio : rCamInfo.FDRatio
// Bin enable : rCamInfo.BinInfo.fgBIN
// RRZ ratio (include bin) : rCamInfo.rCropRzInfo
// GMA info : GmaExifInfo
// LCE info : LceExifInfo
// Upper EE : rEE_U, customEE_main[0][rCamInfo.eIdx_ISO_U], customEE_main[1][rCamInfo.eIdx_ISO_U]
// Lower EE : rEE_L, customEE_main[0][rCamInfo.eIdx_ISO_L], customEE_main[1][rCamInfo.eIdx_ISO_L]
// After interpolation & Output : rEE


}


//++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
//
//++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
MBOOL
IspTuningCustom::
is_to_invoke_remap_ISP_ISO_with_LV(RAWIspCamInfo const& rCamInfo)
{
    return MFALSE;
}

//++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
//
//++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
MUINT32
IspTuningCustom::
remap_ISP_ISO_with_LV(ISP_NVRAM_ISO_INTERVAL_STRUCT const& rISO_ENV, MUINT32 const LV, MUINT32 const realISO, MBOOL enable) //(float isoIndex, int lvValue, bool enable)
{
    MUINT32 i = 0;
    MUINT32 lvIndex = 0;
    MUINT32 upperIsoIndex = 0;
    MUINT32 lowerIsoIndex = 0;
    float isoRefineIndex = 0;
    float isoIndex = 0;
    MUINT32 isoRefine = 0;

    if(enable == 0 || rISO_ENV.u2Length != 20 || realISO > rISO_ENV.IDX_Partition[rISO_ENV.u2Length-1]   || realISO < rISO_ENV.IDX_Partition[0]   )
      return realISO;

    float LV_LUT[20][20] = {

      //LV  0      1      2     3     4     5     6     7    8   9     10    11    12    13    14    15    16    17    18    19
         { 0.8 , 0.7  ,  0.6 , 0.5 , 0.4 , 0.3 , 0.2 , 0.1 , 0 , 0   , 0   , 0   , 0   , 0   , 0   , 0   , 0   , 0   , 0   , 0 } , //iso index 0
         { 1.8 , 1.7  ,  1.6 , 1.5 , 1.4 , 1.3 , 1.2 , 1.1 , 1 , 1   , 1   , 1   , 0.9 , 0.8 , 0.7 , 0.6 , 0.5 , 0.4 , 0.3 , 0.2 } , //iso index 1
         { 2.8 , 2.7  ,  2.6 , 2.5 , 2.4 , 2.3 , 2.2 , 2.1 , 2 , 2   , 2   , 2   , 1.9 , 1.8 , 1.7 , 1.6 , 1.5 , 1.4 , 1.3 , 1.2 } , //iso index 2
         { 3.8 , 3.7  ,  3.6 , 3.5 , 3.4 , 3.3 , 3.2 , 3.1 , 3 , 3   , 3   , 3   , 2.9 , 2.8 , 2.7 , 2.6 , 2.5 , 2.4 , 2.3 , 2.2 } , //iso index 3
         { 4.8 , 4.7  ,  4.6 , 4.5 , 4.4 , 4.3 , 4.2 , 4.1 , 4 , 4   , 4   , 4   , 3.9 , 3.8 , 3.7 , 3.6 , 3.5 , 3.4 , 3.3 , 3.2 } , //iso index 4
         { 5.8 , 5.7  ,  5.6 , 5.5 , 5.4 , 5.3 , 5.2 , 5.1 , 5 , 5   , 5   , 5   , 4.9 , 4.8 , 4.7 , 4.6 , 4.5 , 4.4 , 4.3 , 4.2 } , //iso index 5
         { 6.8 , 6.7  ,  6.6 , 6.5 , 6.4 , 6.3 , 6.2 , 6.1 , 6 , 6   , 6   , 6   , 5.9 , 5.8 , 5.7 , 5.6 , 5.5 , 5.4 , 5.3 , 5.2 } , //iso index 6
         { 7.8 , 7.7  ,  7.6 , 7.5 , 7.4 , 7.3 , 7.2 , 7.1 , 7 , 7   , 7   , 7   , 6.9 , 6.8 , 6.7 , 6.6 , 6.5 , 6.4 , 6.3 , 6.2 } , //iso index 7
         { 8.8 , 8.7  ,  8.6 , 8.5 , 8.4 , 8.3 , 8.2 , 8.1 , 8 , 8   , 8   , 8   , 7.9 , 7.8 , 7.7 , 7.6 , 7.5 , 7.4 , 7.3 , 7.2 } , //iso index 8
         { 9.8 , 9.7  ,  9.6 , 9.5 , 9.4 , 9.3 , 9.2 , 9.1 , 9 , 9   , 9   , 9   , 8.9 , 8.8 , 8.7 , 8.6 , 8.5 , 8.4 , 8.3 , 8.2 } , //iso index 9
         { 10.8, 10.7 ,  10.6, 10.5, 10.4, 10.3, 10.2, 10.1, 10, 10  , 10  , 10  , 9.9 , 9.8 , 9.7 , 9.6 , 9.5 , 9.4 , 9.3 , 9.2 } , //iso index 10
         { 11.8, 11.7 ,  11.6, 11.5, 11.4, 11.3, 11.2, 11.1, 11, 11  , 11  , 11  , 10.9, 10.8, 10.7, 10.6, 10.5, 10.4, 10.3, 10.2} , //iso index 11
         { 12.8, 12.7 ,  12.6, 12.5, 12.4, 12.3, 12.2, 12.1, 12, 12  , 12  , 12  , 11.9, 11.8, 11.7, 11.6, 11.5, 11.4, 11.3, 11.2} , //iso index 12
         { 13.8, 13.7 ,  13.6, 13.5, 13.4, 13.3, 13.2, 13.1, 13, 13  , 13  , 13  , 12.9, 12.8, 12.7, 12.6, 12.5, 12.4, 12.3, 12.2} , //iso index 13
         { 14.8, 14.7 ,  14.6, 14.5, 14.4, 14.3, 14.2, 14.1, 14, 14  , 14  , 14  , 13.9, 13.8, 13.7, 13.6, 13.5, 13.4, 13.3, 13.2} , //iso index 14
         { 15.8, 15.7 ,  15.6, 15.5, 15.4, 15.3, 15.2, 15.1, 15, 15  , 15  , 15  , 14.9, 14.8, 14.7, 14.6, 14.5, 14.4, 14.3, 14.2} , //iso index 15
         { 16.8, 16.7 ,  16.6, 16.5, 16.4, 16.3, 16.2, 16.1, 16, 16  , 16  , 16  , 15.9, 15.8, 15.7, 15.6, 15.5, 15.4, 15.3, 15.2} , //iso index 16
         { 17.8, 17.7 ,  17.6, 17.5, 17.4, 17.3, 17.2, 17.1, 17, 17  , 17  , 17  , 16.9, 16.8, 16.7, 16.6, 16.5, 16.4, 16.3, 16.2} , //iso index 17
         { 18.8, 18.7 ,  18.6, 18.5, 18.4, 18.3, 18.2, 18.1, 18, 18  , 18  , 18  , 17.9, 17.8, 17.7, 17.6, 17.5, 17.4, 17.3, 17.2} , //iso index 18
         { 19  , 19   ,  19  , 19  , 19  , 19  , 19  , 19  , 19, 19  , 19  , 19  , 18.9, 18.8, 18.7, 18.6, 18.5, 18.4, 18.3, 18.2} , //iso index 19


    };

    //compute iso index
    for(i = 0; i <rISO_ENV.u2Length; ++i ){
        if(rISO_ENV.IDX_Partition[i] == realISO) {
          lowerIsoIndex = i;
          upperIsoIndex = i;
          isoIndex = i;
          break;
        }
        else if(rISO_ENV.IDX_Partition[i] > realISO)
        {
          lowerIsoIndex = i-1;
          upperIsoIndex = i;

          if(rISO_ENV.IDX_Partition[upperIsoIndex] == rISO_ENV.IDX_Partition[lowerIsoIndex] )
            isoIndex = lowerIsoIndex;
          else
            isoIndex = lowerIsoIndex + (float)(realISO - rISO_ENV.IDX_Partition[lowerIsoIndex])/(rISO_ENV.IDX_Partition[upperIsoIndex] -rISO_ENV.IDX_Partition[lowerIsoIndex] );
          break;
        }
    }
    MY_LOG("isoIndex = %d", isoIndex);

    upperIsoIndex = CEIL(isoIndex);
    lowerIsoIndex = FLOOR(isoIndex);

    upperIsoIndex = (upperIsoIndex > 19)? 19: upperIsoIndex;
    upperIsoIndex = (upperIsoIndex < 0)? 0: upperIsoIndex;

    lowerIsoIndex = (lowerIsoIndex > 19)? 19: lowerIsoIndex;
    lowerIsoIndex = (lowerIsoIndex < 0)? 0: lowerIsoIndex;

    MY_LOG("upperIsoIndex = %d, lowerIsoIndex = %d", upperIsoIndex, lowerIsoIndex);

    lvIndex = (LV+ 5)/10;
    lvIndex = (lvIndex > 19)? 19: lvIndex;
    lvIndex = (lvIndex <0 )? 0: lvIndex;

    MY_LOG("lvIndex = %d", lvIndex);

    if (lowerIsoIndex == upperIsoIndex)
      isoRefineIndex =  LV_LUT[(int)lowerIsoIndex][lvIndex];
    else
      isoRefineIndex = (LV_LUT[(int)lowerIsoIndex][lvIndex]*(upperIsoIndex - isoIndex) + LV_LUT[(int)upperIsoIndex][lvIndex] * (isoIndex-lowerIsoIndex)) / (upperIsoIndex - lowerIsoIndex);

    upperIsoIndex = CEIL(isoRefineIndex);
    lowerIsoIndex = FLOOR(isoRefineIndex);

    upperIsoIndex = (upperIsoIndex > 19)? 19: upperIsoIndex;
    upperIsoIndex = (upperIsoIndex < 0)? 0: upperIsoIndex;

    lowerIsoIndex = (lowerIsoIndex > 19)? 19: lowerIsoIndex;
    lowerIsoIndex = (lowerIsoIndex < 0)? 0: lowerIsoIndex;

    isoRefine =  rISO_ENV.IDX_Partition[lowerIsoIndex] +  (isoRefineIndex - lowerIsoIndex)*(rISO_ENV.IDX_Partition[upperIsoIndex] -rISO_ENV.IDX_Partition[lowerIsoIndex] ) ;

    MY_LOG("isoRefineIndex = %d, isoRefine = %d", isoRefineIndex, isoRefine);

    return isoRefine;
}
