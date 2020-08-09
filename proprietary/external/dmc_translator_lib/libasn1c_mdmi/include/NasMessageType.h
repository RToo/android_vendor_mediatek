/*
 * Generated by asn1c-0.9.29 (http://lionet.info/asn1c)
 * From ASN.1 module "MDMI"
 * 	found in "MDMI-MIB v2.8.2.asn"
 */

#ifndef _NasMessageType_H_
#define _NasMessageType_H_


#include <asn_application.h>

/* Including external dependencies */
#include <NativeInteger.h>

#ifdef __cplusplus
extern "C" {
#endif

/* Dependencies */
typedef enum NasMessageType {
    NasMessageType_esm  = 1,
    NasMessageType_emm  = 2
} e_NasMessageType;

/* NasMessageType */
typedef long     NasMessageType_t;

/* Implementation */
extern asn_TYPE_descriptor_t asn_DEF_NasMessageType;
asn_struct_free_f NasMessageType_free;
asn_struct_print_f NasMessageType_print;
asn_constr_check_f NasMessageType_constraint;
ber_type_decoder_f NasMessageType_decode_ber;
der_type_encoder_f NasMessageType_encode_der;
xer_type_decoder_f NasMessageType_decode_xer;
xer_type_encoder_f NasMessageType_encode_xer;

#ifdef __cplusplus
}
#endif

#endif  /* _NasMessageType_H_ */
#include <asn_internal.h>