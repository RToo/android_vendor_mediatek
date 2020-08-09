/* Copyright Statement:
*
* This software/firmware and related documentation ("MediaTek Software") are
* protected under relevant copyright laws. The information contained herein
* is confidential and proprietary to MediaTek Inc. and/or its licensors.
* Without the prior written permission of MediaTek inc. and/or its licensors,
* any reproduction, modification, use or disclosure of MediaTek Software,
* and information contained herein, in whole or in part, shall be strictly prohibited.
*/
/* MediaTek Inc. (C) 2018. All rights reserved.
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
* have been modified by MediaTek Inc. All revisions are subject to any receiver\'s
* applicable license agreements with MediaTek Inc.
*/

#include <string.h>
#include "pal_typedefs.h"
#include "rsa_oaep.h"
#include "oaep.h"
#include "rsa2048_export.h"
#include "pal_log.h"

/* ENABLE_GOLDEN_PATTERN = 1: rsa_oaep_decrypt() will be tested
 *                            by default golden data.
 * ENABLE_GOLDEN_PATTERN = 0: rsa_oaep_decrypt() will decrypt the
 *                            data encrypted by rsa_oaep_encrypt().
 * If ENABLE_GOLDEN_PATTERN is set to 0, please assign at least
 * 0xA0000 to SEC_LIB_HEAP_LENGTH in src/core/inc/dram_buffer.h
 */
#define ENABLE_GOLDEN_PATTERN (1)

/* GOLDEN_EMSG_SHA256 = 1: sha256 algorithm will be used in MGF.
 * GOLDEN_EMSG_SHA1   = 1: sha1 algorithm will be used in MGF.
 */
#define GOLDEN_EMSG_SHA256 (1)
#define GOLDEN_EMSG_SHA1   (0)

#define RSA_TST_KEY_N     0x9B, 0xB7, 0x34, 0x77, 0x44, 0x43, 0xD7, 0x75, 0x57, 0xA7, 0x6E, 0x24, 0xB1, 0x07, 0x33, 0x78, \
                          0x77, 0x50, 0xD9, 0x0D, 0x09, 0xC8, 0x69, 0xCD, 0x60, 0x6D, 0x54, 0xF2, 0x89, 0x78, 0xEA, 0x62, \
                          0x20, 0xDC, 0x99, 0x48, 0xB3, 0xC9, 0xE8, 0x92, 0x84, 0xF8, 0x55, 0x1D, 0x61, 0x66, 0xF3, 0x75, \
                          0x4B, 0x6A, 0x3B, 0x89, 0x0A, 0xC9, 0xCD, 0xA9, 0xE3, 0x7D, 0xFA, 0xA0, 0xC1, 0x31, 0x7E, 0x35, \
                          0x1C, 0xE5, 0x10, 0x7C, 0x42, 0x73, 0x79, 0x59, 0x49, 0xC6, 0xCC, 0xE6, 0x38, 0x31, 0x4A, 0xB1, \
                          0xA3, 0x45, 0x38, 0x5D, 0x76, 0x42, 0xCB, 0x8D, 0x05, 0x5A, 0x1F, 0x41, 0x0C, 0x7D, 0x7E, 0x24, \
                          0xA6, 0xF0, 0xA2, 0xAA, 0xB8, 0x18, 0x4E, 0x77, 0x3D, 0x21, 0xB3, 0x75, 0x4A, 0x94, 0x75, 0x41, \
                          0x68, 0x0F, 0x2C, 0x1A, 0x8D, 0x6B, 0xA5, 0xBE, 0xFD, 0x3B, 0x6E, 0x1F, 0xC2, 0x8E, 0xC0, 0xB6, \
                          0x1D, 0x55, 0xB1, 0x45, 0x43, 0x83, 0xF2, 0xC3, 0xE8, 0xBD, 0x27, 0x17, 0x0A, 0x25, 0x97, 0x86, \
                          0x08, 0xF6, 0x78, 0x8B, 0x90, 0xA2, 0xFC, 0x34, 0xF0, 0xCE, 0x35, 0x05, 0x6B, 0xF7, 0x52, 0x07, \
                          0x95, 0xC8, 0xC6, 0x02, 0x32, 0xCB, 0xBC, 0x0B, 0x03, 0x99, 0x36, 0x7A, 0xF9, 0x37, 0x86, 0x9C, \
                          0xA4, 0x5C, 0xF7, 0x37, 0xA8, 0xA0, 0x66, 0x12, 0x78, 0x93, 0xE9, 0x31, 0x66, 0xC4, 0x33, 0x29, \
                          0x8D, 0xD6, 0xFD, 0x00, 0x9E, 0x67, 0x90, 0xE7, 0x43, 0xB3, 0x39, 0x2A, 0xCA, 0x8E, 0xA9, 0x9F, \
                          0x61, 0xDF, 0xC7, 0x7B, 0xD9, 0x94, 0x16, 0xDD, 0xA4, 0xB8, 0xA9, 0xD7, 0xE4, 0xDA, 0x24, 0x21, \
                          0x74, 0x27, 0xF3, 0x58, 0x41, 0x19, 0xA4, 0x93, 0x20, 0x16, 0xF1, 0x73, 0x5C, 0xC6, 0x3B, 0x12, \
                          0x65, 0x0F, 0xDD, 0xDA, 0x73, 0xC8, 0xFC, 0xFB, 0xC7, 0x9E, 0x05, 0x8F, 0x36, 0x21, 0x9D, 0x3D

#define RSA_TST_KEY_E     0x01, 0x00, 0x01

#define RSA_TST_KEY_D     0x92, 0x8D, 0x2A, 0x63, 0xD5, 0x6B, 0xC4, 0x2C, 0x3C, 0x02, 0xE8, 0x36, 0xC0, 0x25, 0xF6, 0xDB, \
                          0x39, 0xF0, 0x6D, 0x57, 0x48, 0x0D, 0xC7, 0x05, 0xF3, 0xEA, 0xF2, 0x38, 0x12, 0x0A, 0x2B, 0x0D, \
                          0x8F, 0xC0, 0x0E, 0xC3, 0xCD, 0xF2, 0x09, 0x61, 0x5C, 0xA4, 0x1B, 0xF7, 0x3A, 0xE4, 0x99, 0xDD, \
                          0x9A, 0xCC, 0xB0, 0x9B, 0x99, 0xFB, 0xB4, 0x04, 0x60, 0x87, 0x00, 0x8A, 0xAB, 0x48, 0xD9, 0x6F, \
                          0x43, 0x72, 0x92, 0xC1, 0x60, 0x09, 0x2D, 0xFC, 0x4A, 0xE3, 0x3F, 0x94, 0xAC, 0xEE, 0x37, 0x4E, \
                          0x58, 0x4B, 0x9D, 0x70, 0xD9, 0x0C, 0xA4, 0x66, 0x64, 0xD3, 0x1C, 0xF7, 0x2B, 0xBA, 0x30, 0x2E, \
                          0x6E, 0xAF, 0xAB, 0x68, 0xE1, 0x3A, 0x7F, 0x1E, 0xD9, 0xFA, 0x0D, 0xDC, 0x26, 0x04, 0xF3, 0x16, \
                          0x4B, 0xC9, 0x7B, 0xC8, 0xC1, 0xB0, 0xBE, 0x9D, 0xB1, 0xD7, 0x6D, 0x1D, 0x97, 0x0E, 0xA3, 0x6F, \
                          0x4A, 0xF8, 0xAD, 0x2C, 0xE0, 0xAB, 0x5B, 0x7C, 0xA6, 0xB4, 0xF9, 0x9C, 0x13, 0x3F, 0xA3, 0xF8, \
                          0xFF, 0x9C, 0x92, 0xDA, 0x87, 0x43, 0x94, 0xD0, 0xE8, 0xF1, 0xBD, 0xBB, 0x83, 0xC8, 0xE8, 0x11, \
                          0xA3, 0x44, 0xA5, 0xB5, 0xA7, 0x25, 0x1C, 0x9B, 0x4F, 0xB4, 0xAD, 0x0A, 0xC4, 0x94, 0xA2, 0xC5, \
                          0x0A, 0x1A, 0x79, 0xFA, 0x9B, 0x39, 0x92, 0xD7, 0x49, 0x53, 0x5B, 0x69, 0x1D, 0xC2, 0xF0, 0x16, \
                          0xCE, 0xE4, 0x93, 0xE4, 0x1C, 0xD2, 0x03, 0x31, 0x90, 0xC4, 0xC0, 0x49, 0x7F, 0x68, 0x9E, 0x48, \
                          0xBA, 0xE5, 0xD3, 0xAD, 0x28, 0xCB, 0x0E, 0x8D, 0x17, 0xDA, 0xE7, 0xF4, 0xE8, 0xC0, 0x34, 0xAB, \
                          0x60, 0xAB, 0x33, 0x0F, 0x67, 0x8F, 0xBF, 0xDD, 0x2C, 0x97, 0x16, 0xBF, 0xA6, 0xA6, 0x17, 0x33, \
                          0x9D, 0x24, 0x8B, 0x46, 0xDF, 0x15, 0x93, 0xD5, 0x31, 0x7B, 0x3A, 0xF4, 0x4A, 0x86, 0x46, 0x41

#define TST_EMSG_SHA256   0x99, 0x5C, 0x33, 0x15, 0xFB, 0xCA, 0xCA, 0x57, 0xEA, 0xA6, 0xA8, 0xA7, 0x59, 0x11, 0x2B, 0x27, \
                          0x7C, 0x56, 0xD8, 0x28, 0x1A, 0x4B, 0x3B, 0x22, 0x18, 0xB0, 0x55, 0x81, 0x9B, 0x24, 0x9A, 0xB6, \
                          0x10, 0x49, 0x26, 0x84, 0xA4, 0xD4, 0x89, 0x86, 0xD3, 0x03, 0x97, 0x72, 0xE2, 0x63, 0x53, 0x54, \
                          0xAD, 0xC4, 0x07, 0x44, 0x3E, 0x90, 0xB3, 0x84, 0x64, 0x50, 0xDD, 0xD2, 0xA5, 0x26, 0x04, 0x11, \
                          0x70, 0x60, 0xCE, 0x19, 0xD0, 0x28, 0x26, 0x1D, 0x24, 0x75, 0xF7, 0x23, 0x41, 0x96, 0xAE, 0xBB, \
                          0x30, 0xF7, 0x06, 0xBD, 0x71, 0x68, 0x34, 0x8D, 0x99, 0x4B, 0x18, 0x16, 0x47, 0x38, 0x4D, 0x4C, \
                          0xB5, 0x9B, 0x85, 0x0E, 0x29, 0xA1, 0x12, 0xD2, 0x7E, 0xB9, 0x15, 0xE5, 0x87, 0xD7, 0x50, 0x39, \
                          0xC3, 0x8C, 0x02, 0x59, 0x40, 0x9C, 0x9E, 0x33, 0x4B, 0x0C, 0x81, 0x04, 0xE3, 0xC5, 0x06, 0x59, \
                          0x23, 0xAE, 0x6E, 0x0D, 0x6B, 0xE4, 0x67, 0xF8, 0xAD, 0x1F, 0x85, 0x35, 0x23, 0x76, 0x7E, 0x27, \
                          0x58, 0x85, 0x27, 0x6E, 0x9F, 0x01, 0xD8, 0x37, 0x4F, 0xD4, 0xEA, 0x49, 0x21, 0xBE, 0x25, 0x76, \
                          0x7E, 0x86, 0xCA, 0x06, 0x15, 0x0C, 0xE3, 0x49, 0x50, 0x0B, 0x0E, 0x9F, 0x17, 0x8F, 0x8E, 0x79, \
                          0x2D, 0x55, 0x47, 0x4D, 0xBA, 0xDC, 0x43, 0x2C, 0x90, 0x67, 0xBB, 0x93, 0xDE, 0x90, 0x96, 0x21, \
                          0x7C, 0x37, 0x97, 0x44, 0x67, 0x30, 0x5B, 0x92, 0x49, 0xC1, 0x2F, 0xD2, 0x2C, 0xB0, 0xC0, 0xDC, \
                          0xF7, 0xB1, 0xD0, 0xE5, 0x95, 0x42, 0xCB, 0x02, 0x36, 0xBC, 0x15, 0x07, 0xBA, 0xF6, 0x74, 0x85, \
                          0xCC, 0x7E, 0xAC, 0x89, 0xF6, 0x06, 0x74, 0x1E, 0xF3, 0x7E, 0x81, 0x4E, 0x98, 0x72, 0x68, 0xF3, \
                          0xD0, 0xCE, 0x18, 0x49, 0x08, 0xD2, 0xBE, 0x0A, 0xFC, 0xA3, 0x27, 0xEC, 0xD4, 0x8D, 0x51, 0x0E

#define TST_EMSG_SHA1     0x49, 0x3F, 0x2E, 0x70, 0xE3, 0x0D, 0x56, 0x1D, 0x03, 0xA6, 0xBE, 0xAA, 0x04, 0xDC, 0x41, 0xBF, \
                          0x74, 0xDE, 0x8D, 0xDA, 0xB4, 0x37, 0xB7, 0x5E, 0xC8, 0x9D, 0xA4, 0x09, 0xBA, 0xF9, 0x28, 0xE6, \
                          0x85, 0xAE, 0x41, 0x36, 0x3C, 0xEA, 0x9B, 0x7D, 0x19, 0xF4, 0x09, 0xB7, 0x1A, 0x9C, 0x90, 0xDC, \
                          0xB2, 0x2C, 0x34, 0xD5, 0x98, 0x5C, 0xAD, 0x9C, 0xD1, 0xEC, 0xB9, 0x00, 0x7A, 0xB6, 0x5B, 0xC4, \
                          0x97, 0xDB, 0xFD, 0x22, 0xD5, 0xDC, 0xF3, 0x06, 0xF1, 0x20, 0x19, 0x69, 0x61, 0x4E, 0xEC, 0x72, \
                          0xFE, 0xBF, 0xE8, 0xEF, 0xF1, 0x14, 0x00, 0x7B, 0x36, 0x47, 0x70, 0x52, 0x98, 0xF1, 0x04, 0x51, \
                          0xD4, 0x52, 0xEE, 0x41, 0x57, 0xBE, 0xAA, 0x4E, 0x5C, 0x0D, 0xD0, 0xBD, 0x4F, 0x73, 0xFF, 0x9E, \
                          0x74, 0x8F, 0x55, 0x6D, 0xE0, 0x02, 0x13, 0x1A, 0x76, 0xDA, 0xE3, 0xE8, 0xC2, 0x87, 0x63, 0xA7, \
                          0x6E, 0x2C, 0xFE, 0x7E, 0x34, 0x86, 0x64, 0x26, 0x10, 0x06, 0x54, 0x19, 0x65, 0x85, 0x5B, 0xDB, \
                          0x86, 0xCE, 0x21, 0x17, 0xE2, 0xD0, 0x0F, 0x83, 0x01, 0x8D, 0x59, 0x3A, 0x9C, 0x78, 0xE6, 0x3D, \
                          0x37, 0x41, 0xEC, 0x38, 0x29, 0x27, 0xBE, 0xB3, 0xA2, 0xFD, 0x75, 0xDC, 0x4F, 0x57, 0xA4, 0xD3, \
                          0x9D, 0x5D, 0xC2, 0xE8, 0x17, 0xDE, 0x56, 0xEA, 0x06, 0xB5, 0x2A, 0x40, 0x8A, 0x5D, 0x62, 0x16, \
                          0xAE, 0x46, 0x28, 0x3B, 0x88, 0x90, 0xC5, 0xD5, 0x7C, 0x60, 0x61, 0x69, 0xD4, 0xC5, 0xEE, 0x7D, \
                          0xDC, 0x73, 0x5A, 0xD5, 0x9E, 0xEE, 0xE7, 0x66, 0x82, 0xE1, 0x74, 0xE0, 0x51, 0xB9, 0xF2, 0x31, \
                          0x62, 0xA0, 0x44, 0x35, 0xA9, 0xAD, 0x15, 0xE9, 0x2D, 0x72, 0x13, 0x0F, 0x30, 0xF5, 0x5E, 0xA4, \
                          0x5C, 0x90, 0x8C, 0x2A, 0x74, 0x79, 0xFE, 0x6B, 0x5D, 0x7B, 0x4A, 0x0F, 0x15, 0xC5, 0xBD, 0x5F

#define TST_MSG_SZ  (16)

#if (GOLDEN_EMSG_SHA256)
#define HASH_ALGO ATTR_SHA256
#define DECRYPT_GOLDEN_PATTERN TST_EMSG_SHA256
#elif (GOLDEN_EMSG_SHA1)
#define HASH_ALGO ATTR_SHA1
#define DECRYPT_GOLDEN_PATTERN TST_EMSG_SHA1
#else
#error please select sha1 or sha256 algorithm
#endif

static void dump_buf(unsigned char *buf, unsigned int buf_sz)
{
	unsigned int idx = 0;

	if (buf_sz < 4)
		return;

	for (idx = 0; idx <= buf_sz - 4; idx += 4) {
		pal_log_err("[0x%x] 0x%x 0x%x 0x%x 0x%x\n", idx,
								*(buf + idx),
								*(buf + idx + 1),
								*(buf + idx + 2),
								*(buf + idx + 3));
	}

	return;
}

void rsa_oaep_sample(void)
{
	unsigned int ret = 0;
	unsigned int idx = 0;

	unsigned char key_n[] = {RSA_TST_KEY_N};
	unsigned char key_e[] = {RSA_TST_KEY_E};
	unsigned char key_d[] = {RSA_TST_KEY_D};

	unsigned long key_n_sz = sizeof(key_n);
	unsigned long key_e_sz = sizeof(key_e);
	unsigned long key_d_sz = sizeof(key_d);

	unsigned char msg[TST_MSG_SZ] = {0};
	unsigned long msg_sz = TST_MSG_SZ;
	unsigned long emsg_sz = RSA_MODULUS_SZ;
#if ENABLE_GOLDEN_PATTERN
	int i = 0;
	unsigned char emsg[RSA_MODULUS_SZ] = {DECRYPT_GOLDEN_PATTERN};
#else
	unsigned char emsg[RSA_MODULUS_SZ] = {0};
#endif


#if ENABLE_GOLDEN_PATTERN
	pal_log_err("RSA_OAEP golden pattern test...\n");
	pal_log_err("dmsg should be equal to ");
	for (i = 0; i < TST_MSG_SZ; i++) {
			pal_log_err("0x%x ", i);
	}
	pal_log_err("\n");
#else
	pal_log_err("msg = ");
	for (idx = 0; idx < TST_MSG_SZ; idx++) {
		msg[idx] = idx % 16;
		pal_log_err("0x%x ", msg[idx]);
	}
	pal_log_err("\n");

	ret = rsa_oaep_encrypt(msg, msg_sz, key_e, key_e_sz, key_n, key_n_sz,
					emsg, &emsg_sz, HASH_ALGO);
	if (ret) {
		pal_log_err("rsa_oaep_encrypt test fail(0x%x)\n", ret);
		return;
	}
#endif

	pal_log_err("emsg = ");
	for (idx = 0; idx < TST_MSG_SZ; idx++) {
		pal_log_err("0x%x ", emsg[idx]);
	}
	pal_log_err("\n");

	memset(msg, 0x0, TST_MSG_SZ);
	ret = rsa_oaep_decrypt(emsg, emsg_sz, key_d, key_d_sz, key_n, key_n_sz,
					msg, &msg_sz, HASH_ALGO);
	if (ret) {
		pal_log_err("rsa_oaep_decrypt test fail(0x%x)\n", ret);
		return;
	}

	pal_log_err("dmsg = ");
	for (idx = 0; idx < TST_MSG_SZ; idx++) {
		pal_log_err("0x%x ", msg[idx]);
	}
	pal_log_err("\n");

	for (idx = 0; idx < TST_MSG_SZ; idx++) {
		if (msg[idx] != (idx % 16)) {
			pal_log_err("rsa_oaep test failed\n");
			return;
		}
	}

	pal_log_err("rsa_oaep test pass\n");
	return;
}
