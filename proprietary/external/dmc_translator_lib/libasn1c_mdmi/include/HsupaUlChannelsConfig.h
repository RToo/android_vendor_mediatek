/*
 * Generated by asn1c-0.9.29 (http://lionet.info/asn1c)
 * From ASN.1 module "MDMI"
 * 	found in "MDMI-MIB v2.8.2.asn"
 */

#ifndef _HsupaUlChannelsConfig_H_
#define _HsupaUlChannelsConfig_H_


#include <asn_application.h>

/* Including external dependencies */
#include "Tti.h"
#include <NativeInteger.h>
#include "SpreadingFactor.h"
#include <constr_SEQUENCE.h>

#ifdef __cplusplus
extern "C" {
#endif

/* HsupaUlChannelsConfig */
typedef struct HsupaUlChannelsConfig {
    Tti_t    tti;
    long     harqSoftCombiningMethod;
    long     slotFormat;
    SpreadingFactor_t    maxEulSpreadingFactor;
    SpreadingFactor_t    maxUeSpreadingFactor;
    long     edpchStart;
    long     edpchEnd;
    long     edpcchPowerOffset;
    long     edpdchIQ;
    long     servingGrant;
    long     grantType;
    long     etfciThreshold2Step;
    long     etfciThreshold3Step;
    long     maxTxPower;

    /* Context for parsing across buffer boundaries */
    asn_struct_ctx_t _asn_ctx;
} HsupaUlChannelsConfig_t;

/* Implementation */
extern asn_TYPE_descriptor_t asn_DEF_HsupaUlChannelsConfig;

#ifdef __cplusplus
}
#endif

#endif  /* _HsupaUlChannelsConfig_H_ */
#include <asn_internal.h>