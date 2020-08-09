/*
 * Generated by asn1c-0.9.29 (http://lionet.info/asn1c)
 * From ASN.1 module "MDMI"
 * 	found in "MDMI-MIB v2.8.2.asn"
 */

#ifndef _EmbmsRegisteredAppListEvent_H_
#define _EmbmsRegisteredAppListEvent_H_


#include <asn_application.h>

/* Including external dependencies */
#include "RegisteredAppListEvent.h"

#ifdef __cplusplus
extern "C" {
#endif

/* EmbmsRegisteredAppListEvent */
typedef RegisteredAppListEvent_t     EmbmsRegisteredAppListEvent_t;

/* Implementation */
extern asn_TYPE_descriptor_t asn_DEF_EmbmsRegisteredAppListEvent;
asn_struct_free_f EmbmsRegisteredAppListEvent_free;
asn_struct_print_f EmbmsRegisteredAppListEvent_print;
asn_constr_check_f EmbmsRegisteredAppListEvent_constraint;
ber_type_decoder_f EmbmsRegisteredAppListEvent_decode_ber;
der_type_encoder_f EmbmsRegisteredAppListEvent_encode_der;
xer_type_decoder_f EmbmsRegisteredAppListEvent_decode_xer;
xer_type_encoder_f EmbmsRegisteredAppListEvent_encode_xer;

#ifdef __cplusplus
}
#endif

#endif  /* _EmbmsRegisteredAppListEvent_H_ */
#include <asn_internal.h>