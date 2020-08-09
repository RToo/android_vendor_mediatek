/*
 * Generated by asn1c-0.9.29 (http://lionet.info/asn1c)
 * From ASN.1 module "MDMI"
 * 	found in "MDMI-MIB v2.8.2.asn"
 */

#ifndef _AppRegisteredEvent_H_
#define _AppRegisteredEvent_H_


#include <asn_application.h>

/* Including external dependencies */
#include "AppInstanceID.h"
#include <asn_SEQUENCE_OF.h>
#include <constr_SEQUENCE_OF.h>
#include <constr_SEQUENCE.h>

#ifdef __cplusplus
extern "C" {
#endif

/* AppRegisteredEvent */
typedef struct AppRegisteredEvent {
    struct appRegisteredEvent {
        A_SEQUENCE_OF(AppInstanceID_t) list;

        /* Context for parsing across buffer boundaries */
        asn_struct_ctx_t _asn_ctx;
    } appRegisteredEvent;

    /* Context for parsing across buffer boundaries */
    asn_struct_ctx_t _asn_ctx;
} AppRegisteredEvent_t;

/* Implementation */
extern asn_TYPE_descriptor_t asn_DEF_AppRegisteredEvent;

#ifdef __cplusplus
}
#endif

#endif  /* _AppRegisteredEvent_H_ */
#include <asn_internal.h>