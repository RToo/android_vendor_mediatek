/*
 * Generated by asn1c-0.9.28 (http://lionet.info/asn1c)
 * From ASN.1 module "MTK-EM"
 * 	found in "MAPI-MIB v2.0.asn"
 * 	`asn1c -fcompound-names`
 */

#ifndef _MAPIC2kOtaEventInfo_H_
#define _MAPIC2kOtaEventInfo_H_


#include <asn_application.h>

/* Including external dependencies */
#include "MAPIOTAMsgType.h"
#include <constr_SEQUENCE.h>

#ifdef __cplusplus
extern "C" {
#endif

/* MAPIC2kOtaEventInfo */
typedef struct MAPIC2kOtaEventInfo {
    MAPIOTAMsgType_t    *otaMsgType /* OPTIONAL */;
	/*
	 * This type is extensible,
	 * possible extensions are below.
	 */

    /* Context for parsing across buffer boundaries */
    asn_struct_ctx_t _asn_ctx;
} MAPIC2kOtaEventInfo_t;

/* Implementation */
extern asn_TYPE_descriptor_t asn_DEF_MAPIC2kOtaEventInfo;

#ifdef __cplusplus
}
#endif

#endif  /* _MAPIC2kOtaEventInfo_H_ */
#include <asn_internal.h>