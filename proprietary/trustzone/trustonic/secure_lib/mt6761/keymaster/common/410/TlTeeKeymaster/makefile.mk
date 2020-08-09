OUTPUT_NAME := tlTeeKeymaster
TRUSTLET_UUID := 0706000000000000000000000000004d
TRUSTLET_MEMTYPE := 2
TRUSTLET_NO_OF_THREADS := 1
TRUSTLET_SERVICE_TYPE := 3
TRUSTLET_FLAGS := 0
TA_KEYFILE := pairVendorTltSig.pem
GP_ENTRYPOINTS := N
TA_INTERFACE_VERSION := 79.0
TBASE_API_LEVEL := 7
HEAP_SIZE_INIT := 32768
HEAP_SIZE_MAX := 131072
TRUSTLET_DIR ?= Locals/Code
TLSDK_DIR_SRC ?= $(TLSDK_DIR)
include $(TLSDK_DIR)/trustlet_release.mk