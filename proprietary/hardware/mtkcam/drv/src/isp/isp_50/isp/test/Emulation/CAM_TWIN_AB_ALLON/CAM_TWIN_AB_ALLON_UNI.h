


  ptr->writeReg(0x0f04, 0x0, UNI_A);            //CAM.CAM_UNI.ADL.CTL
  ptr->writeReg(0x0f40, 0x0, UNI_A);            //CAM.CAM_UNI.ADL.DMA_ST
  ptr->writeReg(0x0f00, 0x1, UNI_A);            //CAM.CAM_UNI.ADL.RESET
  ptr->writeReg(0x0f48, 0x0, UNI_A);            //CAM.CAM_UNI.ADL.DCM_ST
  ptr->writeReg(0x0f30, 0x0, UNI_A);            //CAM.CAM_UNI.ADL.CROP_IN_START
  ptr->writeReg(0x0f34, 0x0, UNI_A);            //CAM.CAM_UNI.ADL.CROP_IN_END
  ptr->writeReg(0x0f38, 0x0, UNI_A);            //CAM.CAM_UNI.ADL.CROP_OUT_START
  ptr->writeReg(0x0f3c, 0x0, UNI_A);            //CAM.CAM_UNI.ADL.CROP_OUT_END
  ptr->writeReg(0x0f4c, 0x4000600, UNI_A);      //CAM.CAM_UNI.ADL.DMA_ERR_ST
  ptr->writeReg(0x0c10, 0xff, UNI_A);           //CAM.CAM_UNI.ADL.DMA_A_SPECIAL_FUN_EN
  ptr->writeReg(0x0f44, 0x0, UNI_A);            //CAM.CAM_UNI.ADL.DCM_DIS
  ptr->writeReg(0x0ca8, 0x0, UNI_A);            //CAM.CAM_UNI.ADL.DMA_A_IPUI_STRIDE
  ptr->writeReg(0x0f50, 0x0, UNI_A);            //CAM.CAM_UNI.ADL.DMA_0_DEBUG
  ptr->writeReg(0x0f54, 0x0, UNI_A);            //CAM.CAM_UNI.ADL.DMA_1_DEBUG
  ptr->writeReg(0x0d28, 0x0, UNI_A);            //CAM.CAM_UNI.ADL.DMA_A_DMA_DEBUG_SEL
  ptr->writeReg(0x0f70, 0x0, UNI_A);            //CAM.CAM_UNI.ADL.SPARE
  ptr->writeReg(0x0f80, 0x0, UNI_A);            //CAM.CAM_UNI.ADL.INFO00
  ptr->writeReg(0x0f84, 0x0, UNI_A);            //CAM.CAM_UNI.ADL.INFO01
  ptr->writeReg(0x0f88, 0x0, UNI_A);            //CAM.CAM_UNI.ADL.INFO02
  ptr->writeReg(0x0f8c, 0x0, UNI_A);            //CAM.CAM_UNI.ADL.INFO03
  ptr->writeReg(0x0f90, 0x0, UNI_A);            //CAM.CAM_UNI.ADL.INFO04
  ptr->writeReg(0x0f94, 0x0, UNI_A);            //CAM.CAM_UNI.ADL.INFO05
  ptr->writeReg(0x0f98, 0x0, UNI_A);            //CAM.CAM_UNI.ADL.INFO06
  ptr->writeReg(0x0f9c, 0x0, UNI_A);            //CAM.CAM_UNI.ADL.INFO07
  ptr->writeReg(0x0fa0, 0x0, UNI_A);            //CAM.CAM_UNI.ADL.INFO08
  ptr->writeReg(0x0fa4, 0x0, UNI_A);            //CAM.CAM_UNI.ADL.INFO09
  ptr->writeReg(0x0fa8, 0x0, UNI_A);            //CAM.CAM_UNI.ADL.INFO10
  ptr->writeReg(0x0fac, 0x0, UNI_A);            //CAM.CAM_UNI.ADL.INFO11
  ptr->writeReg(0x0fb0, 0x0, UNI_A);            //CAM.CAM_UNI.ADL.INFO12
  ptr->writeReg(0x0fb4, 0x0, UNI_A);            //CAM.CAM_UNI.ADL.INFO13
  ptr->writeReg(0x0fb8, 0x0, UNI_A);            //CAM.CAM_UNI.ADL.INFO14
  ptr->writeReg(0x0fbc, 0x0, UNI_A);            //CAM.CAM_UNI.ADL.INFO15
  ptr->writeReg(0x0fc0, 0x0, UNI_A);            //CAM.CAM_UNI.ADL.INFO16
  ptr->writeReg(0x0fc4, 0x0, UNI_A);            //CAM.CAM_UNI.ADL.INFO17
  ptr->writeReg(0x0fc8, 0x0, UNI_A);            //CAM.CAM_UNI.ADL.INFO18
  ptr->writeReg(0x0fcc, 0x0, UNI_A);            //CAM.CAM_UNI.ADL.INFO19
  ptr->writeReg(0x0fd0, 0x0, UNI_A);            //CAM.CAM_UNI.ADL.INFO20
  ptr->writeReg(0x0fd4, 0x0, UNI_A);            //CAM.CAM_UNI.ADL.INFO21
  ptr->writeReg(0x0fd8, 0x0, UNI_A);            //CAM.CAM_UNI.ADL.INFO22
  ptr->writeReg(0x0fdc, 0x0, UNI_A);            //CAM.CAM_UNI.ADL.INFO23
  ptr->writeReg(0x0fe0, 0x0, UNI_A);            //CAM.CAM_UNI.ADL.INFO24
  ptr->writeReg(0x0fe4, 0x0, UNI_A);            //CAM.CAM_UNI.ADL.INFO25
  ptr->writeReg(0x0fe8, 0x0, UNI_A);            //CAM.CAM_UNI.ADL.INFO26
  ptr->writeReg(0x0fec, 0x0, UNI_A);            //CAM.CAM_UNI.ADL.INFO27
  ptr->writeReg(0x0ff0, 0x0, UNI_A);            //CAM.CAM_UNI.ADL.INFO28
  ptr->writeReg(0x0ff4, 0x0, UNI_A);            //CAM.CAM_UNI.ADL.INFO29
  ptr->writeReg(0x0ff8, 0x0, UNI_A);            //CAM.CAM_UNI.ADL.INFO30
  ptr->writeReg(0x0ffc, 0x0, UNI_A);            //CAM.CAM_UNI.ADL.INFO31
  ptr->writeReg(0x0c00, 0x10001, UNI_A);        //CAM.CAM_UNI.ADL.DMA_A_DMA_SOFT_RSTSTAT
  ptr->writeReg(0x0c04, 0x0, UNI_A);            //CAM.CAM_UNI.ADL.DMA_A_VERTICAL_FLIP_EN
  ptr->writeReg(0x0c08, 0x0, UNI_A);            //CAM.CAM_UNI.ADL.DMA_A_DMA_SOFT_RESET
  ptr->writeReg(0x0c0c, 0x0, UNI_A);            //CAM.CAM_UNI.ADL.DMA_A_LAST_ULTRA_EN
  ptr->writeReg(0x0c14, 0xff, UNI_A);           //CAM.CAM_UNI.ADL.DMA_A_IPUO_RING
  ptr->writeReg(0x0c18, 0xff, UNI_A);           //CAM.CAM_UNI.ADL.DMA_A_IPUI_RING
  ptr->writeReg(0x0c30, 0x0, UNI_A);            //CAM.CAM_UNI.ADL.DMA_A_IPUO_BASE_ADDR
  ptr->writeReg(0x0c38, 0x0, UNI_A);            //CAM.CAM_UNI.ADL.DMA_A_IPUO_OFST_ADDR
  ptr->writeReg(0x0c40, 0x0, UNI_A);            //CAM.CAM_UNI.ADL.DMA_A_IPUO_XSIZE
  ptr->writeReg(0x0c44, 0x0, UNI_A);            //CAM.CAM_UNI.ADL.DMA_A_IPUO_YSIZE
  ptr->writeReg(0x0c48, 0x0, UNI_A);            //CAM.CAM_UNI.ADL.DMA_A_IPUO_STRIDE
  ptr->writeReg(0x0c4c, 0x80000040, UNI_A);     //CAM.CAM_UNI.ADL.DMA_A_IPUO_CON
  ptr->writeReg(0x0c50, 0x400040, UNI_A);       //CAM.CAM_UNI.ADL.DMA_A_IPUO_CON2
  ptr->writeReg(0x0c54, 0x400040, UNI_A);       //CAM.CAM_UNI.ADL.DMA_A_IPUO_CON3
  ptr->writeReg(0x0c90, 0x0, UNI_A);            //CAM.CAM_UNI.ADL.DMA_A_IPUI_BASE_ADDR
  ptr->writeReg(0x0c98, 0x0, UNI_A);            //CAM.CAM_UNI.ADL.DMA_A_IPUI_OFST_ADDR
  ptr->writeReg(0x0ca0, 0x0, UNI_A);            //CAM.CAM_UNI.ADL.DMA_A_IPUI_XSIZE
  ptr->writeReg(0x0ca4, 0x0, UNI_A);            //CAM.CAM_UNI.ADL.DMA_A_IPUI_YSIZE
  ptr->writeReg(0x0cac, 0x80000040, UNI_A);     //CAM.CAM_UNI.ADL.DMA_A_IPUI_CON
  ptr->writeReg(0x0cb0, 0x400040, UNI_A);       //CAM.CAM_UNI.ADL.DMA_A_IPUI_CON2
  ptr->writeReg(0x0cb4, 0x400040, UNI_A);       //CAM.CAM_UNI.ADL.DMA_A_IPUI_CON3
  ptr->writeReg(0x0d00, 0x0, UNI_A);            //CAM.CAM_UNI.ADL.DMA_A_DMA_ERR_CTRL
  ptr->writeReg(0x0d04, 0xffff0000, UNI_A);     //CAM.CAM_UNI.ADL.DMA_A_IPUO_ERR_STAT
  ptr->writeReg(0x0d08, 0xffff0000, UNI_A);     //CAM.CAM_UNI.ADL.DMA_A_IPUI_ERR_STAT
  ptr->writeReg(0x0d0c, 0x0, UNI_A);            //CAM.CAM_UNI.ADL.DMA_A_DMA_DEBUG_ADDR
  ptr->writeReg(0x0d10, 0x0, UNI_A);            //CAM.CAM_UNI.ADL.DMA_A_DMA_RSV1
  ptr->writeReg(0x0d14, 0xffffffff, UNI_A);     //CAM.CAM_UNI.ADL.DMA_A_DMA_RSV2
  ptr->writeReg(0x0d18, 0x0, UNI_A);            //CAM.CAM_UNI.ADL.DMA_A_DMA_RSV3
  ptr->writeReg(0x0d1c, 0xffffffff, UNI_A);     //CAM.CAM_UNI.ADL.DMA_A_DMA_RSV4
  ptr->writeReg(0x0d20, 0x0, UNI_A);            //CAM.CAM_UNI.ADL.DMA_A_DMA_RSV5
  ptr->writeReg(0x0d24, 0xffffffff, UNI_A);     //CAM.CAM_UNI.ADL.DMA_A_DMA_RSV6
  ptr->writeReg(0x0d2c, 0x0, UNI_A);            //CAM.CAM_UNI.ADL.DMA_A_DMA_BW_SELF_TEST
  ptr->writeReg(0x0200, 0x0, UNI_A);            //CAM.CAM_UNI.UNP2_A.OFST
  ptr->writeReg(0x0110, 0x1800018, UNI_A);      //CAM.CAM_UNI.DMA.SPECIAL_FUN_EN
  ptr->writeReg(0x0138, 0x1f003c, UNI_A);       //CAM.CAM_UNI.DMA.RAWI_STRIDE
  ptr->writeReg(0x0174, 0x89e56e, UNI_A);       //CAM.CAM_UNI.DMA.DEBUG_SEL
  ptr->writeReg(0x012c, 0x8b5d07ca, UNI_A);     //CAM.CAM_UNI.DMA.RAWI_DRS
  ptr->writeReg(0x0100, 0x10000, UNI_A);        //CAM.CAM_UNI.DMA.SOFT_RSTSTAT
  ptr->writeReg(0x0104, 0x0, UNI_A);            //CAM.CAM_UNI.DMA.VERTICAL_FLIP_EN
  ptr->writeReg(0x0108, 0x0, UNI_A);            //CAM.CAM_UNI.DMA.SOFT_RESET
  ptr->writeReg(0x010c, 0x10000, UNI_A);        //CAM.CAM_UNI.DMA.LAST_ULTRA_EN
  ptr->writeReg(0x0114, 0xce45, UNI_A);         //CAM.CAM_UNI.DMA.SPECIAL_FUN2_EN
  ptr->writeReg(0x0120, 0x0, UNI_A);            //CAM.CAM_UNI.DMA.RAWI_BASE_ADDR
  ptr->writeReg(0x0128, 0x0, UNI_A);            //CAM.CAM_UNI.DMA.RAWI_OFST_ADDR
  ptr->writeReg(0x0130, 0x0, UNI_A);            //CAM.CAM_UNI.DMA.RAWI_XSIZE
  ptr->writeReg(0x0134, 0x0, UNI_A);            //CAM.CAM_UNI.DMA.RAWI_YSIZE
  ptr->writeReg(0x013c, 0x10000040, UNI_A);     //CAM.CAM_UNI.DMA.RAWI_CON
  ptr->writeReg(0x0140, 0x70005, UNI_A);        //CAM.CAM_UNI.DMA.RAWI_CON2
  ptr->writeReg(0x0144, 0x3d0031, UNI_A);       //CAM.CAM_UNI.DMA.RAWI_CON3
  ptr->writeReg(0x014c, 0x1ca0195, UNI_A);      //CAM.CAM_UNI.DMA.RAWI_CON4
  ptr->writeReg(0x0150, 0x0, UNI_A);            //CAM.CAM_UNI.DMA.ERR_CTRL
  ptr->writeReg(0x0154, 0xc6e90000, UNI_A);     //CAM.CAM_UNI.DMA.RAWI_ERR_STAT
  ptr->writeReg(0x0158, 0xa587aa4d, UNI_A);     //CAM.CAM_UNI.DMA.DEBUG_ADDR
  ptr->writeReg(0x015c, 0x96ac6e4c, UNI_A);     //CAM.CAM_UNI.DMA.RSV1
  ptr->writeReg(0x0160, 0x66a21d5f, UNI_A);     //CAM.CAM_UNI.DMA.RSV2
  ptr->writeReg(0x0164, 0x66353efc, UNI_A);     //CAM.CAM_UNI.DMA.RSV3
  ptr->writeReg(0x0168, 0xba5f31c, UNI_A);      //CAM.CAM_UNI.DMA.RSV4
  ptr->writeReg(0x016c, 0xdbdf2263, UNI_A);     //CAM.CAM_UNI.DMA.RSV5
  ptr->writeReg(0x0170, 0x7bf2463b, UNI_A);     //CAM.CAM_UNI.DMA.RSV6
  ptr->writeReg(0x0000, 0x300, UNI_A);          //CAM.CAM_UNI.TOP.CTL
  ptr->writeReg(0x0040, 0x10000123, UNI_A);     //CAM.CAM_UNI.TOP.MOD_DCM_DIS
  ptr->writeReg(0x0004, 0x10, UNI_A);           //CAM.CAM_UNI.TOP.MISC
  ptr->writeReg(0x0008, 0x2, UNI_A);            //CAM.CAM_UNI.TOP.SW_CTL
  ptr->writeReg(0x000c, 0x0, UNI_A);            //CAM.CAM_UNI.TOP.RAWI_TRIG
  ptr->writeReg(0x0010, 0x20000000, UNI_A);     //CAM.CAM_UNI.TOP.MOD_EN
  ptr->writeReg(0x0014, 0x0, UNI_A);            //CAM.CAM_UNI.TOP.DMA_EN
  ptr->writeReg(0x001c, 0x8000010b, UNI_A);     //CAM.CAM_UNI.TOP.FMT_SEL
  ptr->writeReg(0x0020, 0x20000000, UNI_A);     //CAM.CAM_UNI.TOP.DMA_INT_EN
  ptr->writeReg(0x0024, 0x0, UNI_A);            //CAM.CAM_UNI.TOP.DMA_INT_STATUS
  ptr->writeReg(0x0028, 0x0, UNI_A);            //CAM.CAM_UNI.TOP.DMA_INT_STATUSX
  ptr->writeReg(0x002c, 0x1557, UNI_A);         //CAM.CAM_UNI.TOP.DBG_SET
  ptr->writeReg(0x0030, 0x0, UNI_A);            //CAM.CAM_UNI.TOP.DBG_PORT
  ptr->writeReg(0x0034, 0x20000000, UNI_A);     //CAM.CAM_UNI.TOP.DMA_CCU_INT_EN
  ptr->writeReg(0x0038, 0x0, UNI_A);            //CAM.CAM_UNI.TOP.DMA_CCU_INT_STATUS
  ptr->writeReg(0x0044, 0x1, UNI_A);            //CAM.CAM_UNI.TOP.DMA_DCM_DIS
  ptr->writeReg(0x0050, 0x0, UNI_A);            //CAM.CAM_UNI.TOP.MOD_DCM_STATUS
  ptr->writeReg(0x0054, 0x0, UNI_A);            //CAM.CAM_UNI.TOP.DMA_DCM_STATUS
  ptr->writeReg(0x0060, 0x0, UNI_A);            //CAM.CAM_UNI.TOP.MOD_REQ_STATUS
  ptr->writeReg(0x0064, 0x0, UNI_A);            //CAM.CAM_UNI.TOP.DMA_REQ_STATUS
  ptr->writeReg(0x0070, 0x30000001, UNI_A);     //CAM.CAM_UNI.TOP.MOD_RDY_STATUS
  ptr->writeReg(0x0074, 0x1, UNI_A);            //CAM.CAM_UNI.TOP.DMA_RDY_STATUS