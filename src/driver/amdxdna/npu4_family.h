/* SPDX-License-Identifier: GPL-2.0 */
/*
 * Copyright (C) 2024-2025, Advanced Micro Devices, Inc.
 */

#ifndef _NPU4_FAMILY_H_
#define _NPU4_FAMILY_H_

#include "drm_local/amdxdna_accel.h"
#include "aie2_msg_priv.h"
#include "aie2_pci.h"

extern const struct dpm_clk_freq npu4_dpm_clk_table[];
extern const struct rt_config npu4_default_rt_cfg[];
extern const struct msg_op_ver npu4_msg_op_tbl[];
extern const struct rt_cfg_ver npu4_rt_cfg_tbl[];
extern const struct amdxdna_dev_priv npu4_dev_priv;

/* NPU Public Registers on MpNPUAxiXbar (refer to Diag npu_registers.h) */
#define MPNPU_PUB_SEC_INTR             0x3010060
#define MPNPU_PUB_PWRMGMT_INTR         0x3010064
#define MPNPU_PUB_SCRATCH0             0x301006C
#define MPNPU_PUB_SCRATCH1             0x3010070
#define MPNPU_PUB_SCRATCH2             0x3010074
#define MPNPU_PUB_SCRATCH3             0x3010078
#define MPNPU_PUB_SCRATCH4             0x301007C
#define MPNPU_PUB_SCRATCH5             0x3010080
#define MPNPU_PUB_SCRATCH6             0x3010084
#define MPNPU_PUB_SCRATCH7             0x3010088
#define MPNPU_PUB_SCRATCH8             0x301008C
#define MPNPU_PUB_SCRATCH9             0x3010090
#define MPNPU_PUB_SCRATCH10            0x3010094
#define MPNPU_PUB_SCRATCH11            0x3010098
#define MPNPU_PUB_SCRATCH12            0x301009C
#define MPNPU_PUB_SCRATCH13            0x30100A0
#define MPNPU_PUB_SCRATCH14            0x30100A4
#define MPNPU_PUB_SCRATCH15            0x30100A8
#define MP0_C2PMSG_73                  0x3810A24
#define MP0_C2PMSG_123                 0x3810AEC

#define MP1_C2PMSG_0                   0x3B10900
#define MP1_C2PMSG_60                  0x3B109F0
#define MP1_C2PMSG_61                  0x3B109F4

#define MPNPU_SRAM_X2I_MAILBOX_0       0x3600000
#define MPNPU_SRAM_X2I_MAILBOX_15      0x361E000
#define MPNPU_SRAM_X2I_MAILBOX_31      0x363E000
#define MPNPU_SRAM_I2X_MAILBOX_31      0x363F000

#define MMNPU_APERTURE0_BASE           0x3000000
#define MMNPU_APERTURE1_BASE           0x3600000
#define MMNPU_APERTURE3_BASE           0x3810000
#define MMNPU_APERTURE4_BASE           0x3B10000

/* PCIe BAR Index for NPU4 */
#define NPU4_REG_BAR_INDEX	0
#define NPU4_MBOX_BAR_INDEX	0
#define NPU4_PSP_BAR_INDEX	4
#define NPU4_SMU_BAR_INDEX	5
#define NPU4_SRAM_BAR_INDEX	2
/* Associated BARs and Apertures */
#define NPU4_REG_BAR_BASE	MMNPU_APERTURE0_BASE
#define NPU4_MBOX_BAR_BASE	MMNPU_APERTURE0_BASE
#define NPU4_PSP_BAR_BASE	MMNPU_APERTURE3_BASE
#define NPU4_SMU_BAR_BASE	MMNPU_APERTURE4_BASE
#define NPU4_SRAM_BAR_BASE	MMNPU_APERTURE1_BASE

#define NPU4_COMMON_DEV_PRIV \
	.rt_config	= npu4_default_rt_cfg,							\
	.optional_msg	= npu4_msg_op_tbl,							\
	.optional_cfg	= npu4_rt_cfg_tbl,							\
	.dpm_clk_tbl	= npu4_dpm_clk_table,							\
	.priv_load_cfg = { 5, 0, AIE2_RT_CFG_INIT },						\
	.col_opc	= 4096,									\
	.mbox_dev_addr  = NPU4_MBOX_BAR_BASE,							\
	.mbox_size      = 0, /* Use BAR size */							\
	.sram_dev_addr  = NPU4_SRAM_BAR_BASE,							\
	.hwctx_limit	= 16,									\
	.ctx_limit	= 32,									\
	.temporal_only	= 1,									\
	.sram_offs      = {									\
		DEFINE_BAR_OFFSET(MBOX_CHANN_OFF, NPU4_SRAM, MPNPU_SRAM_X2I_MAILBOX_0),		\
		DEFINE_BAR_OFFSET(FW_ALIVE_OFF,   NPU4_SRAM, MPNPU_SRAM_X2I_MAILBOX_15),	\
	},											\
	.psp_regs_off   = {									\
		DEFINE_BAR_OFFSET(PSP_CMD_REG,    NPU4_PSP, MP0_C2PMSG_123),			\
		DEFINE_BAR_OFFSET(PSP_ARG0_REG,   NPU4_REG, MPNPU_PUB_SCRATCH3),		\
		DEFINE_BAR_OFFSET(PSP_ARG1_REG,   NPU4_REG, MPNPU_PUB_SCRATCH4),		\
		DEFINE_BAR_OFFSET(PSP_ARG2_REG,   NPU4_REG, MPNPU_PUB_SCRATCH9),		\
		DEFINE_BAR_OFFSET(PSP_INTR_REG,   NPU4_PSP, MP0_C2PMSG_73),			\
		DEFINE_BAR_OFFSET(PSP_STATUS_REG, NPU4_PSP, MP0_C2PMSG_123),			\
		DEFINE_BAR_OFFSET(PSP_RESP_REG,   NPU4_REG, MPNPU_PUB_SCRATCH3),		\
	},											\
	.smu_regs_off   = {									\
		DEFINE_BAR_OFFSET(SMU_CMD_REG,  NPU4_SMU, MP1_C2PMSG_0),			\
		DEFINE_BAR_OFFSET(SMU_ARG_REG,  NPU4_SMU, MP1_C2PMSG_60),			\
		DEFINE_BAR_OFFSET(SMU_INTR_REG, NPU4_SMU, MMNPU_APERTURE4_BASE),		\
		DEFINE_BAR_OFFSET(SMU_RESP_REG, NPU4_SMU, MP1_C2PMSG_61),			\
		DEFINE_BAR_OFFSET(SMU_OUT_REG,  NPU4_SMU, MP1_C2PMSG_60),			\
	},											\
	.hw_ops		= {									\
		.set_dpm = npu4_set_dpm,							\
		.get_tops = npu4_get_tops,							\
	}

#define NPU4_COMMON_DEV_INFO \
	.reg_bar           = NPU4_REG_BAR_INDEX,						\
	.mbox_bar          = NPU4_MBOX_BAR_INDEX,						\
	.sram_bar          = NPU4_SRAM_BAR_INDEX,						\
	.psp_bar           = NPU4_PSP_BAR_INDEX,						\
	.smu_bar           = NPU4_SMU_BAR_INDEX,						\
	.first_col         = 0,									\
	.dev_mem_buf_shift = 15, /* 32 KiB aligned */						\
	.dev_mem_base      = AIE2_DEVM_BASE,							\
	.dev_mem_size      = AIE2_DEVM_SIZE,							\
	.device_type       = AMDXDNA_DEV_TYPE_KMQ,						\
	.ops               = &aie2_ops

#endif /* _NPU4_FAMILY_H_ */
