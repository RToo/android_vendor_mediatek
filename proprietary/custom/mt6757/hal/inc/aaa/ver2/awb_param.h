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

#ifndef _AWB_PARAM_H
#define _AWB_PARAM_H

#include <awb_feature.h>
#include <aaa_types.h>
#include <string.h>

#define AWB_PARAM_REVISION 6511001

//____Parent Block Config____
#define AWB_WINDOW_NUM_X (120) // To be removed
#define AWB_WINDOW_NUM_Y (90)  // To be removed
#define AWB_WINDOW_NUM (AWB_WINDOW_NUM_X * AWB_WINDOW_NUM_Y) // To be removed

// Parent block size
#define AWB_PARENT_BLK_SIZE_X (5)
#define AWB_PARENT_BLK_SIZE_Y (5)
#define AWB_PARENT_BLK_SIZE (AWB_PARENT_BLK_SIZE_X * AWB_PARENT_BLK_SIZE_Y)

#define AWB_WINDOW_NUM_MAX_X (125)
#define AWB_WINDOW_NUM_MAX_Y (125)
#define AWB_PARENT_BLK_NUM_MAX_X (AWB_WINDOW_NUM_MAX_X/AWB_PARENT_BLK_SIZE_X)
#define AWB_PARENT_BLK_NUM_MAX_Y (AWB_WINDOW_NUM_MAX_Y/AWB_PARENT_BLK_SIZE_Y)
#define AWB_PARENT_BLK_NUM_MAX (AWB_PARENT_BLK_NUM_MAX_X * AWB_PARENT_BLK_NUM_MAX_Y)

#define AWB_SPEED_MIN (0)
#define AWB_SPEED_MAX (100)

#define AWB_LIGHT_WEIGHT_UNIT (256) // 1.0 = 256

//____Auto White Balance Config____
#define AWB_SCALE_UNIT (512) // 1.0 = 512

//____Preset White Balance Config____
#define PWB_LIGHT_AREA_NUM (2)
#define PWB_NEUTRAL_AREA_INDEX (0)
#define PWB_REFERENCE_AREA_INDEX (1)

//____AWB Temporal Predictor____
#define AWB_TEMPORAL_BUFF_SIZE              (8)
#define AWB_TEMPORAL_BUFF_HALF_SIZE         (AWB_TEMPORAL_BUFF_SIZE >> 1)


//Face Detection for AWB
#define AWB_WIN_OFFSET          1000   // for android window define


// AWB mode
typedef enum
{
	AWB_SENSOR_MODE_PREVIEW = 0,
    AWB_SENSOR_MODE_CAPTURE,
   	AWB_SENSOR_MODE_VIDEO,
   	AWB_SENSOR_MODE_SLIM_VIDEO1,
   	AWB_SENSOR_MODE_SLIM_VIDEO2,
	AWB_SENSOR_MODE_NUM
} AWB_SENSOR_MODE_T;

// Strobe mode
typedef enum
{
    AWB_STROBE_MODE_ON = 0,
    AWB_STROBE_MODE_OFF,
    AWB_STROBE_MODE_NUM
} AWB_STROBE_MODE_T;

// Strobe mode
typedef enum
{
    AWB_STATE_PREVIEW = 0,
    AWB_STATE_AF,
    AWB_STATE_PRECAPTURE,
    AWB_STATE_CAPTURE,
    AWB_STATE_NUM
} AWB_STATE_T;

// Light source definition
typedef enum
{
    AWB_LIGHT_STROBE = 0,                  // Strobe
    AWB_LIGHT_TUNGSTEN,                    // Tungsten
    AWB_LIGHT_WARM_FLUORESCENT,            // Warm fluorescent
    AWB_LIGHT_FLUORESCENT,                 // Fluorescent (TL84)
    AWB_LIGHT_CWF,                         // CWF
    AWB_LIGHT_DAYLIGHT,                    // Daylight
    AWB_LIGHT_SHADE,                       // Shade
    AWB_LIGHT_DAYLIGHT_FLUORESCENT,        // Daylight fluorescent
    AWB_LIGHT_NUM,                         // Light source number
    AWB_LIGHT_NONE = AWB_LIGHT_NUM,        // None: not neutral block
    //AWB_LIGHT_DONT_CARE = 0xFF             // Don't care: don't care the light source of block
} AWB_LIGHT_T;


// Preset white balance light source definition
typedef enum
{
    PWB_DAYLIGHT = 0,         // Preset white balance: Daylight
    PWB_CLOUDY_DAYLIGHT,      // Preset white balance: Cloudy daylight
    PWB_SHADE,                // Preset white balance: Shade
    PWB_TWILIGHT,             // Preset white balance: Twilight
    PWB_FLUORESCENT,          // Preset white balance: Fluorescent
    PWB_WARM_FLUORESCENT,     // Preset white balance: Warm fluorescent
    PWB_INCANDESCENT,         // Preset white balance: Incandescent
    PWB_MANUAL,               // Preset white balance: Manual
    PWB_LIGHT_NUM             // Preset white balance light source number
} PWB_LIGHT_T;

// Light source probability
typedef struct
{
    MINT32 i4P0[AWB_LIGHT_NUM]; // Probability 0
    MINT32 i4P1[AWB_LIGHT_NUM]; // Probability 1
    MINT32 i4P2[AWB_LIGHT_NUM]; // Probability 2
    MINT32 i4P[AWB_LIGHT_NUM];  // Probability
} AWB_LIGHT_PROBABILITY_T;

// AWB speed mode definition
typedef enum
{
    AWB_SPEED_MODE_ONESHOT = 0, // full AWB compensation for half-push/capture
    AWB_SPEED_MODE_SMOOTH_TRANSITION = 1  // partial AWB compensation for preview/movie
} AWB_SPEED_MODE_T;


// Parent block statistics
typedef struct
{
    MINT32 i4SumR[AWB_PARENT_BLK_NUM_MAX_Y][AWB_PARENT_BLK_NUM_MAX_X]; // R summation of specified light source of specified parent block
    MINT32 i4SumG[AWB_PARENT_BLK_NUM_MAX_Y][AWB_PARENT_BLK_NUM_MAX_X]; // G summation of specified light source of specified parent block
    MINT32 i4SumB[AWB_PARENT_BLK_NUM_MAX_Y][AWB_PARENT_BLK_NUM_MAX_X]; // B summation of specified light source of specified parent block
    MINT32 i4ChildBlkNum[AWB_PARENT_BLK_NUM_MAX_Y][AWB_PARENT_BLK_NUM_MAX_X]; // Child block number of specified light source of specified parent block
    MINT32 i4Light[AWB_PARENT_BLK_NUM_MAX_Y][AWB_PARENT_BLK_NUM_MAX_X]; // Light source of specified parent block
	XY_COORDINATE_T rXY[AWB_PARENT_BLK_NUM_MAX_Y][AWB_PARENT_BLK_NUM_MAX_X];    // XrYr of specified parent block
} AWB_PARENT_BLK_STAT_T;

// Statistics of child blocks within specified parent block
typedef struct
{
    MINT32 i4SumR[AWB_LIGHT_NUM+1];   // Child block R summation of specified light (+1 for non-neutral color)
    MINT32 i4SumG[AWB_LIGHT_NUM+1];   // Child block G summation of specified light (+1 for non-neutral color)
    MINT32 i4SumB[AWB_LIGHT_NUM+1];   // Child block B summation of specified light (+1 for non-neutral color)
    MINT32 i4BlkNum[AWB_LIGHT_NUM+1]; // Child block number of specified light (+1 for non-neutral color)
} AWB_CHILD_BLK_STAT_T;

// AWB window statistics: RGBX
typedef struct
{
    MUINT8 ucR; // R average of specified AWB window
    MUINT8 ucG; // G average of specified AWB window
    MUINT8 ucB; // B average of specified AWB window
    MUINT8 ucEL; // error number + light source info
} AWB_MAIN_STAT_T;

typedef union
{
    AWB_MAIN_STAT_T rMainStat;
    MUINT32 u4MainStat;
} AWB_WINDOW_T;

typedef struct
{
    MUINT16 u2R; // R average of specified AWB window
    MUINT16 u2G; // G average of specified AWB window
    MUINT16 u2B; // B average of specified AWB window
    MUINT16 u2EL; // error number + light source info
} AWB_MAIN_STAT_2B_T;

typedef union
{
    AWB_MAIN_STAT_2B_T rMainStat;
    UINT64 u8MainStat;
} AWB_WINDOW_2B_T;

// Light source statistics
typedef struct
{
    MINT32 i4WeightedSumR[AWB_LIGHT_NUM];         // Weighted R summation of specified light source
    MINT32 i4WeightedSumG[AWB_LIGHT_NUM];         // Weighted G summation of specified light source
    MINT32 i4WeightedSumB[AWB_LIGHT_NUM];         // Weighted B summation of specified light source
    MINT32 i4WeightedWindowNum[AWB_LIGHT_NUM];    // Weighted window number of specified light source
    MINT32 i4ParentBlkNum[AWB_LIGHT_NUM];         // Parent block number of specified light source
    MINT32 i4WeightedParentBlkNum[AWB_LIGHT_NUM]; // Weighted parent block number of specified light source
    XY_COORDINATE_T rRotatedXY[AWB_LIGHT_NUM];    // Rotated XY coordinate (for debug purpose)
    MUINT32 u4CalibratedWeightedSumR[AWB_LIGHT_NUM]; // Calibrated weighted R summation of specified light source
    MUINT32 u4CalibratedWeightedSumG[AWB_LIGHT_NUM]; // Calibrated weighted G summation of specified light source
    MUINT32 u4CalibratedWeightedSumB[AWB_LIGHT_NUM]; // Calibrated weighted B summation of specified light source
} AWB_LIGHT_STAT_T;

// customization statistic
typedef struct
{
    AWB_GAIN_T rAwbGainSmooth;
    AWB_GAIN_T rAwbGainOut;
} AWB_CUST_INFO_OUT_T;

// customization statistic
typedef struct
{
    MINT32 i4TotalParentBlkNum;
    MINT32 i4ParentBlkNum[AWB_LIGHT_NUM];
    MINT32 i4AvgR[AWB_LIGHT_NUM];
    MINT32 i4AvgG[AWB_LIGHT_NUM];
    MINT32 i4AvgB[AWB_LIGHT_NUM];
    AWB_GAIN_T rAwbGain[AWB_LIGHT_NUM];
    AWB_GAIN_T rPrefGain[AWB_LIGHT_NUM];
    AWB_GAIN_T rSpatGain[AWB_LIGHT_NUM];
} AWB_CUST_INFO_T;

// customization statistic
typedef struct
{
    MINT32 i4ErrorRate;
    MINT32 i4FrmAvgR;
    MINT32 i4FrmAvgG;
    MINT32 i4FrmAvgB;
    AWB_GAIN_T rAwbGainAvg;
} AWB_CUST_INFO_FRAME_T;

#define AWB_LIGHT_AREA_NUM (10)

// AWB statistics config
typedef struct
{
    // Main stat window size
    // [1] must be even
    // [2] max number of pixels in a single window must <= 256K
    // [3] max number of pixels of any single color per window must <= 128K
    MINT32 i4WindowSizeX; // Main stat window horizontal size: min = 4
    MINT32 i4WindowSizeY; // Main stat window vertical size: min = 2

    // Main stat window pitch
    MINT32 i4WindowPitchX; // Main stat window horizontal pitch
    MINT32 i4WindowPitchY; // Main stat window vertical pitch

    // Start coordinate of the first window (upper left)
    MINT32 i4WindowOriginX; // Horizontal origin 1st main stat window
    MINT32 i4WindowOriginY; // Vertical origin of 1st main stat window

    // Number of AWB windows: 1x1 ~ 128x128
    MINT32 i4WindowNumX; // Number of horizontal AWB windows
    MINT32 i4WindowNumY; // Number of vertical AWB windows

    // Thresholds
    MINT32 i4LowThresholdR;  // Error pixel low threshold of R (8-bit)
    MINT32 i4LowThresholdG;  // Error pixel low threshold of G (8-bit)
    MINT32 i4LowThresholdB;  // Error pixel lowthreshold of B (8-bit)

    MINT32 i4HighThresholdR; // Error pixel high threshold of R (8-bit)
    MINT32 i4HighThresholdG; // Error pixel high threshold of G (8-bit)
    MINT32 i4HighThresholdB; // Error pixel high threshold of B (8-bit)

    // Pixel count
    MINT32 i4PixelCountR; // ROUND((1<<24)/number of red pixels in a window)
    MINT32 i4PixelCountG; // ROUND((1<<24)/number of green pixels in a window)
    MINT32 i4PixelCountB; // ROUND((1<<24)/number of blue pixels in a window)

    // Pre-gain maximum limit clipping
    MINT32 i4PreGainLimitR; // Maximum limit clipping for R color (12-bit: 0xFFF)
    MINT32 i4PreGainLimitG; // Maximum limit clipping for G color (12-bit: 0xFFF)
    MINT32 i4PreGainLimitB; // Maximum limit clipping for B color (12-bit: 0xFFF)

    // Pre-gain values
    MINT32 i4PreGainR; // Pre-gain Value for R color (format: 4.9; 1x = 512)
    MINT32 i4PreGainG; // Pre-gain Value for G color (format: 4.9; 1x = 512)
    MINT32 i4PreGainB; // Pre-gain Value for B color (format: 4.9; 1x = 512)

    // AWB error threshold
    MINT32 i4ErrorThreshold; // error pixel threshold for the allowed total

    // AWB Motion error threshold
    MINT32 i4MoErrorThreshold; // motion error pixel threshold for the allowed total
    // over-exposed and under-exposed pixels in one main stat window

    // AWB error count shift bits
    MINT32 i4ErrorShiftBits; // Programmable error count shift bits: 0 ~ 7
    // Note: AWB statistics provide 4-bits error count output only

    // AWB rotation matrix
    MINT32 i4Cos; // COS x 256
    MINT32 i4Sin; // SIN x 256

    // AWB light area
    MINT32 i4AWBXY_WINR[AWB_LIGHT_AREA_NUM]; // Right bound
    MINT32 i4AWBXY_WINL[AWB_LIGHT_AREA_NUM]; // Left bound
    MINT32 i4AWBXY_WIND[AWB_LIGHT_AREA_NUM]; // Lower bound
    MINT32 i4AWBXY_WINU[AWB_LIGHT_AREA_NUM]; // Upper bound

} AWB_STAT_CONFIG_T;

// AWB statistics parameter
typedef struct
{
    // Number of AWB windows
    MINT32 i4WindowNumX; // Number of horizontal AWB windows
    MINT32 i4WindowNumY; // Number of vertical AWB windows

    // Thresholds
    MINT32 i4LowThresholdR;  // Low threshold of R
    MINT32 i4LowThresholdG;  // Low threshold of G
    MINT32 i4LowThresholdB;  // Low threshold of B

    MINT32 i4HighThresholdR; // High threshold of R
    MINT32 i4HighThresholdG; // High threshold of G
    MINT32 i4HighThresholdB; // High threshold of B

    // Pre-gain maximum limit clipping
    MINT32 i4PreGainLimitR; // Maximum limit clipping for R color
    MINT32 i4PreGainLimitG; // Maximum limit clipping for G color
    MINT32 i4PreGainLimitB; // Maximum limit clipping for B color

    // AWB error threshold
    MINT32 i4ErrorThreshold; // Programmable threshold for the allowed total
    // over-exposured and under-exposered pixels in one main stat window

    // AWB error count shift bits
    MINT32 i4ErrorShiftBits; // Programmable error count shift bits: 0 ~ 7
    // Note: AWB statistics provide 4-bits error count output only

    // AWB error pixel ratio
    MINT32 i4ErrorRatio; // Programmable error pixel count by AWB window size (base : 256)

    // AWB motion error pixel ratio
    MINT32 i4MoErrorRatio; // Programmable motion error pixel count by AWB window size (base : 256)

} AWB_STAT_PARAM_T;


// AWB info for ISP tuning
typedef struct
{
    AWB_LIGHT_PROBABILITY_T rProb; // Light source probability
    AWB_LIGHT_STAT_T rLightStat; // Light source statistics
    AWB_LIGHT_SOURCE_AWB_GAIN_T rLightAWBGain; // Golden sample's AWB gain for multi-CCM
    AWB_GAIN_T rAlgGain;
    AWB_GAIN_T rCurrentAWBGain; // Current AWB gain
    AWB_GAIN_T rCurrentFullAWBGain; // Current AWB gain without preference
    MINT32 i4NeutralParentBlkNum; // Neutral parent block number
    MINT32 i4CCT; // CCT
    MINT32 i4FluorescentIndex; // Fluorescent index
    MINT32 i4DaylightFluorescentIndex; // Daylight fluorescent index
    MINT32 i4SceneLV; // Scene LV
    MINT32 i4AWBMode; // AWB mode
    MBOOL  bAWBStable; // AWB stable
    AWB_GAIN_T rPregain1; // AA pregain1
    MBOOL  bCscEnable; // AA CCM enable
    MINT32 rCscCCM[9]; // AA CCM
    MBOOL bAWBLock;
    //-----Not Related to MTK AWB algo and won't maintain this part-----//
    AWB_GAIN_T rRPG;
    AWB_GAIN_T rPGN;
} AWB_INFO_T;

// AWB gain output (for debug purpose)
typedef struct
{
    AWB_GAIN_T rAWBGain;
    AWB_GAIN_T rRAWPreGain2;
    AWB_GAIN_T rFullWBGain;
    AWB_GAIN_T rTargetAWBGain;
    AWB_GAIN_T rAWBGainRwb;
    AWB_GAIN_T rRAWPreGain2Rwb;
    AWB_GAIN_T rFullWBGainRwb;
    AWB_GAIN_T rTargetAWBGainRwb;
} AWB_GAIN_OUTPUT_T;

// AWB Specific algorithm data
typedef struct
{
    MINT32 i4RG;                            // Statistic R/G
    MINT32 i4BG;                            // Statistic B/G
    MINT32 i4LogRG;                         // Statistic LOG(R/G)
    MINT32 i4LogBG;                         // Statistic LOG(B/G)
    MINT32 i4DaylightLocusRG;               // Statistic projection R/G
    MINT32 i4DaylightLocusBG;               // Statistic projection B/G
    MINT32 i4DaylightLocusLogRG;            // Statistic projection LOG(R/G)
    MINT32 i4DaylightLocusLogBG;            // Statistic projection LOG(B/G)
	MINT32 i4TargetRG;                      // Target for AWB compensation
	MINT32 i4TargetBG;                      // Target for AWB compensation
	MINT32 i4TargetLogRG;                   // Target for AWB compensation LOG(R/G)
	MINT32 i4TargetLogBG;                   // Target for AWB compensation LOG(B/G)
	MINT32 i4GMOffsetRG;                    // Green or Magenta offset R/G
	MINT32 i4GMOffsetBG;                    // Green or Magenta offset B/G
	MINT32 i4GMOffsetLogRG;                 // Green or Magenta offset LOG(R/G)
	MINT32 i4GMOffsetLogBG;                 // Green or Magenta offset LOG(B/G)
    MINT32 i4IsAboveDaylightLocus;          // 1: above or on daylight locus (Magenta), 0: below daylight locus (Green)
	MINT32 i4GMOffsetThr;                   // Green or Magenta offset threshold
    MINT32 i4GMOffset;                      // Green or Magenta offset
    MINT32 i4DaylightLocusOffset;       // Daylight locus offset
	MINT32 i4NewOffset;                     // New offset = Daylight locus offset - Offset threshold
	MINT32 i4LvOffsetRatio;                 // Lv offset ratio
	MINT32 i4RatioOffset;                   // Ratio offset = New offset * Lv offset ratio(LV)
	MINT32 i4LutOffset;                     // LUT offset = LUT(Ratio offset)
    MINT32 i4Weight;                        // weight
} AWB_ALGO_DATA_T;

// AWB algorithm related data structure
typedef struct
{
    MINT32 i4WBGainR[AWB_LIGHT_NUM]; // Gain R
    MINT32 i4WBGainG[AWB_LIGHT_NUM]; // Gain G
    MINT32 i4WBGainB[AWB_LIGHT_NUM]; // Gain B
    AWB_ALGO_DATA_T rAlgoData[AWB_LIGHT_NUM];
} AWB_ALGORITHM_DATA_T;

// Strobe AWB algorithm related data structure
typedef struct
{
    AWB_GAIN_T rPresetGain; // Preset strobe AWB gain after AWB calibration
    AWB_GAIN_T rNonStrobeGain; // Non-strobe AWB gain
    AWB_GAIN_T rAWBGain; // Strobe AWB gain
} STROBE_AWB_ALGORITHM_DATA_T;

// PWB algorithm statistics
typedef struct
{
    MINT32 i4NeutralAreaParentBlkNum; // Parent block number of neutral area
    MINT32 i4ReferenceAreaParentBlkNum; // Parent block number of reference area
    MINT32 i4ParentBlkNum;  // Parent block number of neutral area + reference area
    AWB_GAIN_T rDefaultGain; // Default gain
    AWB_GAIN_T rWBGain_NeutralArea; // WB gain of neutral area
    AWB_GAIN_T rWBGain_ReferenceArea; // WB gain of reference area
    PWB_LIGHT_T ePWBLight; // PWB light source
    MINT32 i4Xor; // reference area original X coordinate (rotated)
    MINT32 i4Yor; // reference area original Y coordinate (rotated)
    MINT32 i4Xpr; // reference area projected X coordinate (rotated)
    MINT32 i4Ypr; // reference area projected Y coordinate (rotated)
    MINT32 i4MwbCCT;
    MINT32 i4MwbXr;
    MINT32 i4MwbYr;
    MINT32 i4MwbRangeX;
    MINT32 i4MwbRangeY;    
} PWB_ALGORITHM_DATA_T;

// CCT estimation
typedef struct
{
    MINT32 i4LogRG;
    MINT32 i4LogBG;
    MINT32 i4X;
    MINT32 i4Y;
    MINT32 i4XR;
    MINT32 i4YR;
    MINT32 i4MIRED_H;
    MINT32 i4MIRED_L;
    MINT32 i4MIRED;
    MINT32 i4YR_TL84;
    MINT32 i4YR_CWF;
    MINT32 i4YR_Mean_F;
    MINT32 i4YR_D65;
    MINT32 i4YR_DF;
    MINT32 i4YR_Mean_DF;
} CCT_INFO_T;

//____AWB Algorithm Parameters____

// Chip dependent parameter
typedef struct
{
    MINT32 i4AWBGainOutputScaleUnit; // AWB gain output scale unit
    MINT32 i4AWBGainOutputUpperLimit; // AWB gain output upper limit
    MINT32 i4RotationMatrixUnit; // Rotation matrix unit
} AWB_CHIP_PARAM_T;


// Parent block weight parameter used in light source statistics
typedef struct
{
	MINT32 bEnable; // Enable parent block weight
	MINT32 i4ScalingFactor; // 6: 1~12, 7: 1~6, 8: 1~3, 9: 1~2, >=10: 1
} AWB_PARENT_BLOCK_WEIGHT_T;


// AWB one-shot parameter
typedef struct
{
    MBOOL bSmoothEnable; // Enable smooth one-shot AWB for dark environment: take weighted average of one-shot AWB gain and preview AWB gain based on scene LV
    MINT32 i4LVThrL; // Low LV threshold: take preview AWB gain only when scene LV <= i4LVThrL
    MINT32 i4LVThrH; // High LV threshold: take one-shot AWB gain only when scene LV >= i4LVThrH
    // Perform interpolation when i4LVThrH > LV > i4LVThrL
} AWB_ONE_SHOT_T;

//MWB window size parameter
typedef struct
{
	MINT32 i4WindowSizeX;
	MINT32 i4WindowSizeY;
}AWB_MWB_PARAM_T;

// AWB algorithm parameter
typedef struct
{
    AWB_CHIP_PARAM_T rChipParam;
    AWB_PARENT_BLOCK_WEIGHT_T rParentBlkWeightParam;
    AWB_ONE_SHOT_T rOneShotParam;
    AWB_MWB_PARAM_T rMwbParam;
} AWB_PARAM_T;

// AWB ASD info data structure
typedef struct
{
    AWB_GAIN_T rLightPrefGain[AWB_LIGHT_NUM][AWB_LV_INDEX_NUM];
} AWB_PARAM2_T;


// AWB ASD info data structure
typedef struct
{
    MINT32 i4AWBRgain_X128;     // AWB Rgain
    MINT32 i4AWBBgain_X128;     // AWB Bgain
    MINT32 i4AWBRgain_D65_X128; // AWB Rgain (D65; golden sample)
    MINT32 i4AWBBgain_D65_X128; // AWB Bgain (D65; golden sample)
    MINT32 i4AWBRgain_CWF_X128; // AWB Rgain (CWF; golden sample)
    MINT32 i4AWBBgain_CWF_X128; // AWB Bgain (CWF; golden sample)
    MBOOL bAWBStable;           // AWB stable
} AWB_ASD_INFO_T;

typedef struct
{
    MUINT32 i4SensorPreviewWidth;
    MUINT32 i4SensorPreviewHeight;
    MUINT32 i4SensorCaptureWidth;
    MUINT32 i4SensorCaptureHeight;
    MUINT32 i4SensorVideoWidth;
    MUINT32 i4SensorVideoHeight;
    MUINT32 i4SensorVideo1Width;
    MUINT32 i4SensorVideo1Height;
    MUINT32 i4SensorVideo2Width;
    MUINT32 i4SensorVideo2Height;
} AWB_SENSOR_RESOLUTION_INFO_T;


typedef struct
{
    int i4GainSetNum;
    AWB_GAIN_T rGoldenGain2;
    AWB_GAIN_T rGoldenGain3;
    AWB_GAIN_T rUnitGain2;
    AWB_GAIN_T rUnitGain3;
}AWB_MULTI_ILLUMINACE_INFO;


// AWB frame-based input parameters
typedef struct
{
    MVOID* pAWBStatBuf; // Statistic buffer address
    MINT32 i4StatLineSize; // Statistic buffer line size
	AWB_SPEED_MODE_T eAWBSpeedMode; // AWB speed mode
	MINT32 i4SceneLV; // Scene LV
	MINT32 i4SensorMode; // Sensor mode
	MINT32 i4AWBState; // AWB state
	MBOOL bIsStrobeFired; // AWB for AF lamp or pre-flash

     MUINT32 u4AWBAAOMode; // 0:8/12bits, 1:14bits
     MBOOL bAWBLock;
} AWB_INPUT_T;

typedef struct
{
    AWB_GAIN_T rAlgGain;
    AWB_GAIN_T rCurrentGain;
    AWB_GAIN_T rTargetGain;
    MINT32 i4CCT;
    MINT32 i4LightMode;
    MINT32 i4SceneLV;
    AWB_LIGHT_PROBABILITY_T rLightProb;
    AWB_GAIN_T rAwbGainNoPref; //AWB gain without preference
    AWB_PARENT_BLK_STAT_T rAwbParentStatBlk; //New add
    MINT32 ParentBlkNumX; //New add
    MINT32 ParentBlkNumY; //New add
} AWB_SYNC_INPUT_N3D_T;

typedef struct
{
    AWB_GAIN_T rAWBGain;
    MINT32 i4CCT;
} AWB_SYNC_OUTPUT_N3D_T;

typedef struct
{
    AWB_GAIN_T rPreviewAWBGain;              // AWB gain for DIP
    AWB_GAIN_T rPreviewStrobeAWBGain;        // AWB gain for DIP
    AWB_GAIN_T rPreviewFullWBGain;           // AWB gain without preference
    AWB_GAIN_T rCaptureAWBGain;              // AWB gain for DIP
    AWB_GAIN_T rCaptureFullWBGain;           // AWB gain without preference
    AWB_GAIN_T rPreviewRAWPreGain2;          // AWB gain for AE statistics
    AWB_GAIN_T rPreviewStrobeRAWPreGain2;    // AWB gain for AE statistics
    AWB_INFO_T rAWBInfo;                     // AWB info
    AWB_SYNC_INPUT_N3D_T rAWBSyncInput_N3D;  // N3D AWB info
} AWB_OUTPUT_T;


//Face AWB Structure
#define MAX_AWB_METER_AREAS  9
//#define MAX_AWB_TEMPO_TARGET_BUFF_SIZE 8

typedef struct
{
    MINT32 i4Left;
    MINT32 i4Top;
    MINT32 i4Right;
    MINT32 i4Bottom;
    MINT32 i4Weight;
	MINT32 i4Id;
	MINT32 i4Type; // 0:GFD, 1:LFD, 2:OT
	MINT32 i4Motion[2];
	MINT32 i4Landmark[3][4]; // index 0: left eye, index 1: right eye, index 2:mouth
    MINT32 i4LandmarkCV;
} AWBMeterArea_T;

typedef struct
{
    AWBMeterArea_T rAreas[MAX_AWB_METER_AREAS];
    MUINT32 u4Count;
} AWBMeteringArea_T;

typedef struct
{
    MUINT32 u4XLeft;
    MUINT32 u4XRight;
    MUINT32 u4YLow;
    MUINT32 u4YHi;
    MUINT32 u4Weight;
	MINT32 i4Id;
	MINT32 i4Type; // 0:GFD, 1:LFD, 2:OT
	MINT32 i4Motion[2];
	MINT32 i4Landmark[3][4]; // index 0: left eye, index 1: right eye, index 2:mouth
    MINT32 i4LandmarkCV;
} AWB_BLOCK_WINDOW_T;

typedef struct
{
    MBOOL bZoomChange;
    MUINT32 u4XOffset;
    MUINT32 u4YOffset;
    MUINT32 u4XWidth;
    MUINT32 u4YHeight;
} AWBZOOM_WINDOW_T;
///////////////////////////


// AWB init input parameters
typedef struct AWB_INIT_INPUT_S
{
    AWB_NVRAM_T rAWBNVRAM;       // AWB NVRAM param
    AWB_PARAM_T rAWBParam;       // AWB param
    AWB_STAT_PARAM_T rAWBStatParam; // AWB statistics param
    LIB3A_AWB_MODE_T eAWBMode; // AWB mode
	MINT32 i4SensorMode;           // Sensor mode
    AWB_SENSOR_RESOLUTION_INFO_T rSensorResolution; // Sensor resolution info
    AWB_MULTI_ILLUMINACE_INFO rMultiIlluminance;
	MINT32 i4RcCnvSupport; // 0 or 1, 3x3 matrix support

#ifdef PC_SIMU
    MINT32 i4AWBTmpGainR;
    MINT32 i4AWBTmpGainG;
    MINT32 i4AWBTmpGainB;
	AWB_BLOCK_WINDOW_T rFDInfo;
#endif

	CAM_SCENARIO_T eAWBScenario;

	AWB_INIT_INPUT_S()
	{
		memset(&rAWBNVRAM, 0, sizeof(rAWBNVRAM));
		memset(&rAWBParam, 0, sizeof(rAWBParam));
		memset(&rAWBStatParam, 0, sizeof(rAWBStatParam));
		memset(&eAWBMode, 0, sizeof(eAWBMode));
		memset(&i4SensorMode, 0, sizeof(i4SensorMode));
		memset(&rSensorResolution, 0, sizeof(rSensorResolution));
		memset(&rMultiIlluminance, 0, sizeof(rMultiIlluminance));
		memset(&i4RcCnvSupport, 0, sizeof(i4RcCnvSupport));
		memset(&eAWBScenario, 0, sizeof(eAWBScenario));
	}
} AWB_INIT_INPUT_T;


#endif
