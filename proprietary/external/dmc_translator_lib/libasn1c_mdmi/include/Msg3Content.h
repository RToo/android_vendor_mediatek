/*
 * Generated by asn1c-0.9.29 (http://lionet.info/asn1c)
 * From ASN.1 module "MDMI"
 * 	found in "MDMI-MIB v2.8.2.asn"
 */

#ifndef _Msg3Content_H_
#define _Msg3Content_H_


#include <asn_application.h>

/* Including external dependencies */
#include <NativeInteger.h>
#include "ModulationType.h"
#include <constr_SEQUENCE.h>

#ifdef __cplusplus
extern "C" {
#endif

/* Msg3Content */
typedef struct Msg3Content {
    long     mcs;
    ModulationType_t     modulationType;
    long     startRb;
    long     numRb;
    long     tbSizeIndex;

    /* Context for parsing across buffer boundaries */
    asn_struct_ctx_t _asn_ctx;
} Msg3Content_t;

/* Implementation */
extern asn_TYPE_descriptor_t asn_DEF_Msg3Content;

#ifdef __cplusplus
}
#endif

#endif  /* _Msg3Content_H_ */
#include <asn_internal.h>