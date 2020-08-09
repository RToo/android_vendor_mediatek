/* Copyright Statement:
 *
 * This software/firmware and related documentation ("MediaTek Software") are
 * protected under relevant copyright laws. The information contained herein
 * is confidential and proprietary to MediaTek Inc. and/or its licensors.
 * Without the prior written permission of MediaTek inc. and/or its licensors,
 * any reproduction, modification, use or disclosure of MediaTek Software,
 * and information contained herein, in whole or in part, shall be strictly prohibited.
 */
/* MediaTek Inc. (C) 2017. All rights reserved.
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
#ifndef _STEREO_SYNC2A_TUNING_H_
#define _STEREO_SYNC2A_TUNING_H_

#define MAX_MAPPING_DELTABV_ISPRATIO 30
typedef enum
{
    SYNC_DUAL_CAM_DENOISE_BMDN = 0,
    SYNC_DUAL_CAM_DENOISE_MFNR,
    SYNC_DUAL_CAM_DENOISE_MAX
} SYNC_DUAL_CAM_DENOISE_ENUM;
typedef struct
{
	MUINT32 EVOffset_main[2]; //EVOffset main
	MUINT32 EVOffset_main2[2]; //EVOffset main2
	MUINT32 RGB2YCoef_main[3]; //RGB2YCoef main
	MUINT32 RGB2YCoef_main2[3]; //RGB2YCoef main2
	MUINT32 FixSyncGain;
	MBOOL isDoGainRegression;
	MUINT16 SyncWhichEye;
        MUINT32 pDeltaBVtoRatioArray[SYNC_DUAL_CAM_DENOISE_MAX][MAX_MAPPING_DELTABV_ISPRATIO];
}strSyncAEInitInfo;

//MUINT32 getSyncAePolicy();
MUINT32 getSyncAwbMode();

const MUINT32* getSyncCCTDiffTh();


const MUINT32* getSyncGainRatioTh();

MUINT32 getSynAwbStatYTh();
MUINT32 getSyncAwbStatNumRatio();
const strSyncAEInitInfo* getSyncAEInitInfo();

#endif //  _STEREO_SYNC2A_TUNING_H_
