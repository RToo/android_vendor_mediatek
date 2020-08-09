/* Copyright Statement:
 *
 * This software/firmware and related documentation ("MediaTek Software") are
 * protected under relevant copyright laws. The information contained herein
 * is confidential and proprietary to MediaTek Inc. and/or its licensors.
 * Without the prior written permission of MediaTek inc. and/or its licensors,
 * any reproduction, modification, use or disclosure of MediaTek Software,
 * and information contained herein, in whole or in part, shall be strictly prohibited.
 */
/* MediaTek Inc. (C) 2019. All rights reserved.
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
 *     TO REVISE OR REPLACE THE MEDIATEK SOFTWARE AT ISSUE, OR REFUND ANY SOFTWARE LICENSE
 *     FEES OR SERVICE CHARGE PAID BY BUYER TO MEDIATEK FOR SUCH MEDIATEK SOFTWARE AT ISSUE.
 *
 *     THE TRANSACTION CONTEMPLATED HEREUNDER SHALL BE CONSTRUED IN ACCORDANCE WITH THE LAWS
 *     OF THE STATE OF CALIFORNIA, USA, EXCLUDING ITS CONFLICT OF LAWS PRINCIPLES.
 ************************************************************************************************/
#define NVRAM_SLK_TBL_NUM         (10)
#define NVRAM_DM_TBL_NUM         (1600)
#define NVRAM_YNR_TBL_NUM         (2000)
#define NVRAM_YNR_LCE_LINK_TBL_NUM         (1200)
#define NVRAM_YNR_FD_TBL_NUM         (1200)
#define NVRAM_CNR_CNR_TBL_NUM         (2000)
#define NVRAM_EE_TBL_NUM         (1600)
#define NVRAM_NR3D_TBL_NUM         (1200)
#define NVRAM_HFG_TBL_NUM         (1200)
#define NVRAM_DSDN_TBL_NUM         (5)
#define NVRAM_OBC_TBL_NUM         (200)
#define NVRAM_BPC_BPC_TBL_NUM         (200)
#define NVRAM_BPC_CT_TBL_NUM         (200)
#define NVRAM_BPC_PDC_TBL_NUM         (200)
#define NVRAM_LDNR_TBL_NUM         (200)
#define NVRAM_CNR_CCR_TBL_NUM         (100)
#define NVRAM_CNR_ABF_TBL_NUM         (100)
#define NVRAM_SWNR_THRES_TBL_NUM         (30)
#define NVRAM_SWNR_TBL_NUM         (300)
#define NVRAM_LPCNR_TBL_NUM         (300)
#define NVRAM_FUS_TBL_NUM         (100)
#define NVRAM_ZFUS_TBL_NUM         (100)
#define NVRAM_MFNR_THRES_TBL_NUM         (10)
#define NVRAM_MFNR_TBL_NUM         (200)
#define NVRAM_BSS_TBL_NUM         (200)
#define NVRAM_MFB_TBL_NUM         (200)
#define NVRAM_MIX_TBL_NUM         (200)
#define NVRAM_AINR_THRES_TBL_NUM         (30)
#define NVRAM_AINR_TBL_NUM         (100)
#define NVRAM_YNR_TBL_TBL_NUM         (200)
#define NVRAM_CA_LTM_TBL_NUM         (30)
#define NVRAM_ClearZoom_TBL_NUM         (7)
#define NVRAM_YNRS_TBL_NUM         (100)
#define NVRAM_CCM_TBL_NUM         (288)
#define NVRAM_COLOR_PARAM_TBL_NUM         (48)
#define NVRAM_COLOR_TBL_NUM         (2880)
#define NVRAM_AE_TBL_NUM         (20)
#define NVRAM_AF_TBL_NUM         (10)
#define NVRAM_AWB_TBL_NUM         (10)
#define NVRAM_Flash_AE_TBL_NUM         (4)
#define NVRAM_Flash_AWB_TBL_NUM         (4)
#define NVRAM_Flash_Calibration_TBL_NUM         (4)
#define NVRAM_GMA_TBL_NUM         (60)
#define NVRAM_LCE_TBL_NUM         (60)
#define NVRAM_DCE_TBL_NUM         (60)
#define NVRAM_LTM_TBL_NUM         (60)
#define NVRAM_HLR_TBL_NUM         (60)