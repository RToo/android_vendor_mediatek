const ISP_NVRAM_DM_T DM_%04d = {
    .intp_crs  ={.bits ={.DM_CDG_SL=4, .DM_CDG_OFST=20, .DM_CDG_RAT=16, .DM_CD_KNL=0, .rsv_18=0, .DM_BYP=0, .DM_MN_MODE=0}},
    .intp_nat  ={.bits ={.DM_HL_OFST=0, .DM_L0_SL=6, .DM_L0_OFST=0, .DM_CD_SLL=3, .DM_CD_SLC=0, .rsv_30=0}},
    .intp_aug  ={.bits ={.DM_DN_OFST=0, .DM_L2_SL=6, .DM_L2_OFST=0, .DM_L1_SL=6, .DM_L1_OFST=0}},
    .luma_lut1 ={.bits ={.DM_LM_Y2=256, .DM_LM_Y1=384, .DM_LM_Y0=511, .rsv_27=0}},
    .luma_lut2 ={.bits ={.DM_LM_Y5=32, .DM_LM_Y4=64, .DM_LM_Y3=128, .rsv_27=0}},
    .sl_ctl    ={.bits ={.DM_SL_EN=0, .DM_SL_HR=16, .DM_SL_Y2=255, .DM_SL_Y1=255, .rsv_22=0}},
    .hftd_ctl  ={.bits ={.DM_CORE_TH1=0, .DM_HD_GN2=0, .DM_HD_GN1=16, .DM_HT_GN2=16, .DM_HT_GN1=16, .rsv_28=0}},
    .nr_str    ={.bits ={.DM_N2_STR=0, .DM_N1_STR=0, .DM_N0_STR=0, .DM_XTK_SL=5, .DM_XTK_OFST=32, .DM_XTK_RAT=16}},
    .nr_act    ={.bits ={.DM_NGR=0, .DM_NSL=5, .DM_N2_OFST=0, .DM_N1_OFST=0, .DM_N0_OFST=0}},
    .hf_str    ={.bits ={.DM_CORE_TH2=0, .DM_HI_RAT=0, .DM_H3_GN=0, .DM_H2_GN=0, .DM_H1_GN=0, .DM_HA_STR=0}},
    .hf_act1   ={.bits ={.DM_H2_UPB=220, .DM_H2_LWB=52, .DM_H1_UPB=80, .DM_H1_LWB=20}},
    .hf_act2   ={.bits ={.DM_HSLL=5, .DM_HSLR=5, .DM_H3_UPB=255, .DM_H3_LWB=52, .rsv_24=0}},
    .clip      ={.bits ={.DM_CLIP_TH=100, .DM_UN_TH=32, .DM_OV_TH=223, .rsv_24=0}},
    .p1_act    ={.bits ={.DM_P1_UPB=85, .DM_P1_LWB=85, .rsv_16=0}},
    .lr_rat    ={.bits ={.DM_LR_RAT=15, .rsv_4=0}},
    .hftd_ctl2 ={.bits ={.DM_HD_GN3=16, .DM_HFRB_GN=16, .rsv_12=0}},
    .est_ctl   ={.bits ={.DM_P2_CLIP=0, .DM_P1_BLD=0, .rsv_6=0}},
    .int_ctl   ={.bits ={.DM_INT_LTH=3, .DM_INT_CDTH=5, .rsv_8=0}},
    .ee        ={.bits ={.DM_HPOS_GN=16, .DM_HNEG_GN=16, .rsv_10=0}},
    .lmt       ={.bits ={.DM_LMT_UPB=148, .DM_LMT_LWB=120, .DM_LMT_EN=1, .rsv_17=0}},
    .rccc      ={.bits ={.DM_RCDM_SL=192, .DM_RCCC_EN=0, .rsv_14=0}},
};