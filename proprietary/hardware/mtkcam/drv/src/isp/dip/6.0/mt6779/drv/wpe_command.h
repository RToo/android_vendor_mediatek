/*TODO: update WPE register map*/
typedef struct {
    unsigned int WPE_CTL_MOD_EN;
    unsigned int WPE_CTL_DMA_EN;
    unsigned int WPE_CTL_CFG;
    unsigned int WPE_CTL_FMT_SEL;
    unsigned int WPE_CTL_INT_EN;
    unsigned int WPE_CTL_INT_STATUS;
    unsigned int WPE_CTL_INT_STATUSX;
    unsigned int WPE_CTL_TDR_TILE;
    unsigned int WPE_CTL_TDR_DBG_STATUS;
    unsigned int WPE_CTL_TDR_TCM_EN;
    unsigned int WPE_CTL_SW_CTL;
    unsigned int WPE_CTL_SPARE0;
    unsigned int WPE_CTL_SPARE1;
    unsigned int WPE_CTL_SPARE2;
    unsigned int WPE_CTL_DONE_SEL;
    unsigned int WPE_CTL_DBG_SET;
    unsigned int WPE_CTL_DBG_PORT;
    unsigned int WPE_CTL_DATE_CODE;
    unsigned int WPE_CTL_PROJ_CODE;
    unsigned int WPE_CTL_WPE_DCM_DIS;
    unsigned int WPE_CTL_DMA_DCM_DIS;
    unsigned int WPE_CTL_WPE_DCM_STATUS;
    unsigned int WPE_CTL_DMA_DCM_STATUS;
    unsigned int WPE_CTL_WPE_REQ_STATUS;
    unsigned int WPE_CTL_DMA_REQ_STATUS;
    unsigned int WPE_CTL_WPE_RDY_STATUS;
    unsigned int WPE_CTL_DMA_RDY_STATUS;
    unsigned int WPE_VGEN_CTL;
    unsigned int WPE_VGEN_IN_IMG;
    unsigned int WPE_VGEN_OUT_IMG ;
    unsigned int WPE_VGEN_HORI_STEP ;
    unsigned int WPE_VGEN_VERT_STEP;
    unsigned int WPE_VGEN_HORI_INT_OFST;
    unsigned int WPE_VGEN_HORI_SUB_OFST;
    unsigned int WPE_VGEN_VERT_INT_OFST ;
    unsigned int WPE_VGEN_VERT_SUB_OFST;
    unsigned int WPE_VGEN_POST_CTL;
    unsigned int WPE_VGEN_POST_COMP_X;
    unsigned int WPE_VGEN_POST_COMP_Y;
    unsigned int WPE_VGEN_MAX_VEC;
    unsigned int WPE_VFIFO_CTL;
    unsigned int WPE_CFIFO_CTL;
    unsigned int WPE_RWCTL_CTL;
    unsigned int WPE_CACHI_SPECIAL_FUN_EN;
    unsigned int WPE_C24_TILE_EDGE;
    unsigned int WPE_MDP_CROP_X;
    unsigned int WPE_MDP_CROP_Y;
    unsigned int WPE_ISPCROP_CON1;
    unsigned int WPE_ISPCROP_CON2;
    unsigned int WPE_PSP_CTL;
    unsigned int WPE_PSP2_CTL;
    unsigned int WPE_ADDR_GEN_SOFT_RSTSTAT_0;
    unsigned int WPE_ADDR_GEN_BASE_ADDR_0;
    unsigned int WPE_ADDR_GEN_OFFSET_ADDR_0;
    unsigned int WPE_ADDR_GEN_STRIDE_0;
    unsigned int WPE_CACHI_CON_0;
    unsigned int WPE_CACHI_CON2_0;
    unsigned int WPE_CACHI_CON3_0;
    unsigned int WPE_ADDR_GEN_ERR_CTRL_0;
    unsigned int WPE_ADDR_GEN_ERR_STAT_0;
    unsigned int WPE_ADDR_GEN_RSV1_0;
    unsigned int WPE_ADDR_GEN_DEBUG_SEL_0;
    unsigned int WPE_ADDR_GEN_SOFT_RSTSTAT_1;
    unsigned int WPE_ADDR_GEN_BASE_ADDR_1;
    unsigned int WPE_ADDR_GEN_OFFSET_ADDR_1;
    unsigned int WPE_ADDR_GEN_STRIDE_1;
    unsigned int WPE_CACHI_CON_1;
    unsigned int WPE_CACHI_CON2_1;
    unsigned int WPE_CACHI_CON3_1;
    unsigned int WPE_ADDR_GEN_ERR_CTRL_1;
    unsigned int WPE_ADDR_GEN_ERR_STAT_1;
    unsigned int WPE_ADDR_GEN_RSV1_1;
    unsigned int WPE_ADDR_GEN_DEBUG_SEL_1;
    unsigned int WPE_ADDR_GEN_SOFT_RSTSTAT_2;
    unsigned int WPE_ADDR_GEN_BASE_ADDR_2;
    unsigned int WPE_ADDR_GEN_OFFSET_ADDR_2;
    unsigned int WPE_ADDR_GEN_STRIDE_2;
    unsigned int WPE_CACHI_CON_2;
    unsigned int WPE_CACHI_CON2_2;
    unsigned int WPE_CACHI_CON3_2;
    unsigned int WPE_ADDR_GEN_ERR_CTRL_2;
    unsigned int WPE_ADDR_GEN_ERR_STAT_2;
    unsigned int WPE_ADDR_GEN_RSV1_2;
    unsigned int WPE_ADDR_GEN_DEBUG_SEL_2;
    unsigned int WPE_ADDR_GEN_SOFT_RSTSTAT_3;
    unsigned int WPE_ADDR_GEN_BASE_ADDR_3;
    unsigned int WPE_ADDR_GEN_OFFSET_ADDR_3;
    unsigned int WPE_ADDR_GEN_STRIDE_3;
    unsigned int WPE_CACHI_CON_3;
    unsigned int WPE_CACHI_CON2_3;
    unsigned int WPE_CACHI_CON3_3;
    unsigned int WPE_ADDR_GEN_ERR_CTRL_3;
    unsigned int WPE_ADDR_GEN_ERR_STAT_3;
    unsigned int WPE_ADDR_GEN_RSV1_3;
    unsigned int WPE_ADDR_GEN_DEBUG_SEL_3;
    unsigned int WPE_DMA_SOFT_RSTSTAT;
    unsigned int WPE_TDRI_BASE_ADDR;
    unsigned int WPE_TDRI_OFST_ADDR;
    unsigned int WPE_TDRI_XSIZE;
    unsigned int WPE_VERTICAL_FLIP_EN;
    unsigned int WPE_DMA_SOFT_RESET;
    unsigned int WPE_LAST_ULTRA_EN;
    unsigned int WPE_SPECIAL_FUN_EN;
    unsigned int WPE_WPEO_BASE_ADDR;
    unsigned int WPE_WPEO_OFST_ADDR;
    unsigned int WPE_WPEO_XSIZE;
    unsigned int WPE_WPEO_YSIZE;
    unsigned int WPE_WPEO_STRIDE;
    unsigned int WPE_WPEO_CON;
    unsigned int WPE_WPEO_CON2;
    unsigned int WPE_WPEO_CON3;
    unsigned int WPE_WPEO_CROP;
    unsigned int WPE_MSKO_BASE_ADDR;
    unsigned int WPE_MSKO_OFST_ADDR;
    unsigned int WPE_MSKO_XSIZE;
    unsigned int WPE_MSKO_YSIZE;
    unsigned int WPE_MSKO_STRIDE;
    unsigned int WPE_MSKO_CON;
    unsigned int WPE_MSKO_CON2;
    unsigned int WPE_MSKO_CON3;
    unsigned int WPE_MSKO_CROP;
    unsigned int WPE_VECI_BASE_ADDR ;
    unsigned int WPE_VECI_OFST_ADDR ;
    unsigned int WPE_VECI_XSIZE;
    unsigned int WPE_VECI_YSIZE;
    unsigned int WPE_VECI_STRIDE;
    unsigned int WPE_VECI_CON;
    unsigned int WPE_VECI_CON2;
    unsigned int WPE_VECI_CON3;
    unsigned int WPE_VEC2I_BASE_ADDR;
    unsigned int WPE_VEC2I_OFST_ADDR;
    unsigned int WPE_VEC2I_XSIZE;
    unsigned int WPE_VEC2I_YSIZE;
    unsigned int WPE_VEC2I_STRIDE;
    unsigned int WPE_VEC2I_CON;
    unsigned int WPE_VEC2I_CON2;
    unsigned int WPE_VEC2I_CON3;
    unsigned int WPE_VEC3I_BASE_ADDR;
    unsigned int WPE_VEC3I_OFST_ADDR;
    unsigned int WPE_VEC3I_XSIZE;
    unsigned int WPE_VEC3I_YSIZE;
    unsigned int WPE_VEC3I_STRIDE;
    unsigned int WPE_VEC3I_CON;
    unsigned int WPE_VEC3I_CON2;
    unsigned int WPE_VEC3I_CON3;
    unsigned int WPE_DMA_ERR_CTRL;
    unsigned int WPE_WPEO_ERR_STAT ;
    unsigned int WPE_MSKO_ERR_STAT ;
    unsigned int WPE_VECI_ERR_STAT;
    unsigned int WPE_VEC2I_ERR_STAT;
    unsigned int WPE_VEC3I_ERR_STAT;
    unsigned int WPE_DMA_DEBUG_ADDR;
    unsigned int WPE_DMA_DEBUG_SEL;
} WPE_Command;