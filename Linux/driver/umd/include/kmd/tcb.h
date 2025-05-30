// Copyright (C) 2023-2024 Arm Technology (China) Co. Ltd.
//
// SPDX-License-Identifier: Apache-2.0


#ifndef __TCB_H__
#define __TCB_H__

#include <stdint.h>

namespace aipudrv
{
/**
 * #reserved[8:11], dtcm_en[7], rd_en[6], wr_en[5], size[0:4]
 */
#define ASID_WR (1 << 5)
#define ASID_RD (1 << 6)
#define DTCM_EN (1 << 7)

#if (defined ZHOUYI_V3)
enum
{
    TCB_INIT = 0,
    TCB_TASK,
    TCB_LOOP
};

enum
{
    TCB_NO_DEP = 0,
    TCB_IMMIDIATE_DEP,
    TCB_PRE_ALL_DEP
};

union addr64_t
{
    uint64_t v64;
    struct
    {
        uint32_t lo;
        uint32_t hi;
    } v32;
};

struct smmu_conf_t
{
    uint32_t ctrl;
    uint32_t remap;
    struct
    {
        uint32_t ctrl0;
        uint32_t ctrl1;
    } segs[4];
};

struct tcb_t
{
    uint32_t flag;
    uint32_t next;
    union
    {
        struct
        {
            uint32_t loop_count;
            uint32_t spc;
            uint32_t interrupt;
            uint16_t groupid;
            uint16_t gridid;
            uint16_t rsvd0;
            uint16_t taskid;
            uint16_t grid_dim_x;
            uint16_t grid_dim_y;
            uint16_t grid_dim_z;
            uint16_t group_dim_x;
            uint16_t group_dim_y;
            uint16_t group_dim_z;
            uint16_t group_id_x;
            uint16_t group_id_y;
            uint16_t group_id_z;
            uint16_t task_id_x;
            uint16_t task_id_y;
            uint16_t task_id_z;
            uint32_t sp;
            uint32_t pp;
            uint32_t dp;
            uint32_t cp;
            uint32_t pprint;
            uint32_t pprofiler;
            uint16_t coreid;
            uint16_t clusterid;
            uint16_t rsvd1;
            uint16_t tecid;
            uint32_t fmdp;
            uint32_t tap;
            uint32_t dap;
            uint32_t pap;
            uint32_t idp;
            uint32_t dsize;
            uint32_t tcbp;
            uint32_t global_param;
            uint32_t rsvd2[3];
        } noninit;
        union
        {
            struct
            {
                uint32_t rsvd0[2];
                uint32_t gm_ctrl;
                uint32_t grid_id;
                uint32_t gm_rgnx_ctrl[2];
                addr64_t gm_rgnx_addr[2];
                addr64_t asids[4];
                addr64_t dtcm_addr;
                uint32_t rsvd2[10];
            } clst;

            struct
            {
                uint32_t rsvd0[4];
                smmu_conf_t smmu;
                uint32_t rsvd1[6];
                smmu_conf_t next_core_smmu;
            } core;
        } init;
    } __data;
};

#define loop_count   __data.noninit.loop_count
#define spc          __data.noninit.spc
#define interrupt    __data.noninit.interrupt
#define groupid      __data.noninit.groupid
#define gridid       __data.noninit.gridid
#define taskid       __data.noninit.taskid
#define grid_dim_x   __data.noninit.grid_dim_x
#define grid_dim_y   __data.noninit.grid_dim_y
#define grid_dim_z   __data.noninit.grid_dim_z
#define group_dim_x  __data.noninit.group_dim_x
#define group_dim_y  __data.noninit.group_dim_y
#define group_dim_z  __data.noninit.group_dim_z
#define group_id_x   __data.noninit.group_id_x
#define group_id_y   __data.noninit.group_id_y
#define group_id_z   __data.noninit.group_id_z
#define task_id_x    __data.noninit.task_id_x
#define task_id_y    __data.noninit.task_id_y
#define task_id_z    __data.noninit.task_id_z
#define sp           __data.noninit.sp
#define pp           __data.noninit.pp
#define dp           __data.noninit.dp
#define cp           __data.noninit.cp
#define pprint       __data.noninit.pprint
#define pprofiler    __data.noninit.pprofiler
#define coreid       __data.noninit.coreid
#define clusterid    __data.noninit.clusterid
#define tecid        __data.noninit.tecid
#define fmdp         __data.noninit.fmdp
#define tap          __data.noninit.tap
#define dap          __data.noninit.dap
#define pap          __data.noninit.pap
#define idp          __data.noninit.idp
#define dsize        __data.noninit.dsize
#define tcbp         __data.noninit.tcbp
#define global_param __data.noninit.global_param

#define gm_ctl       __data.init.clst.gm_ctrl
#define igrid_id     __data.init.clst.grid_id
#define asids        __data.init.clst.asids
#define gm_rgnx_ctrl __data.init.clst.gm_rgnx_ctrl
#define gm_rgnx_addr __data.init.clst.gm_rgnx_addr
#define dtcm_addr    __data.init.clst.dtcm_addr

#define smmu           __data.init.core.smmu
#define next_core_smmu __data.init.core.next_core_smmu

#define TCB_FLAG_TASK_TYPE_INIT        0
#define TCB_FLAG_TASK_TYPE_TASK        1
#define TCB_FLAG_TASK_TYPE_LOOP_TASK   2

#define TCB_FLAG_DEP_TYPE_NONE         0
#define TCB_FLAG_DEP_TYPE_IMMEDIATE    (1 << 4)
#define TCB_FLAG_DEP_TYPE_PRE_ALL      (2 << 4)

#define TCB_FLAG_END_TYPE_NOT_END      0
#define TCB_FLAG_END_TYPE_GROUP_END    (1 << 6)
#define TCB_FLAG_END_TYPE_GRID_END     (1 << 7)
#define TCB_FLAG_END_TYPE_END_WITH_DESTROY     (1 << 8)

#define EN_INTERRUPT_DONE              1
#define EN_INTERRUPT_EXCEPTION         (1 << 2)
#define EN_INTERRUPT_FAULT             (1 << 3)
#define EN_INTERRUPT_ERROR             (1 << 4)
#define EN_INTERRUPT_SIGNAL            (1 << 5)
#define EN_INTERRUPT_ALL_TYPE          \
    (EN_INTERRUPT_DONE | EN_INTERRUPT_EXCEPTION | \
     EN_INTERRUPT_FAULT | EN_INTERRUPT_ERROR | EN_INTERRUPT_SIGNAL)
#define EN_INTERRUPT_TEC               (1 << 8)
#define EN_INTERRUPT_CORE              (1 << 9)
#define EN_INTERRUPT_CLUSTER           (1 << 10)
#define EN_INTERRUPT_POOL              (1 << 11)

/**
 * GM data sync direction
 * GM_REGION_CTRL_SYNC_TO_GM: DDR to GM region
 * GM_REGION_CTRL_SYNC_TO_DDR: GM region to DDR
 */
#define GM_CTRL_TSM_IGNORE_CFG              (0xf)
#define GM_REGION_CTRL_ONLY_UPDATE_REG    (0 << 30)
#define GM_REGION_CTRL_SYNC_TO_GM         (1 << 30)
#define GM_REGION_CTRL_SYNC_TO_DDR        (2UL << 30)
#define GM_REGION_CTRL_IGNORE_CFG         (3UL << 30)

#define GM_CTRL_REMAP_BOTH_REGION_DEN     (0x0)
#define GM_CTRL_REMAP_REGION0_EN          (0x1)
#define GM_CTRL_REMAP_BOTH_REGION_EN      (0x2)

#else

enum
{
    TCB_GRID_INIT = 0,
    TCB_GROUP_INIT,
    TCB_TASK
};

enum
{
    TCB_NO_DEP = 0,
    TCB_GROUP_DEP,
    TCB_PRE_ALL_DEP
};

union addr64_t
{
    uint64_t v64;
    struct
    {
        uint32_t lo;
        uint32_t hi;
    } v32;
};

union config64_t
{
    uint64_t v64;
    struct
    {
        uint32_t ctrl0;
        uint32_t ctrl1;
    } v32;
};

struct smmu_conf_t
{
    uint32_t ctrl;
    uint32_t remap;
    struct
    {
        uint32_t ctrl0;
        uint32_t ctrl1;
    } segs[4];
};

struct tcb_t
{
    uint32_t flag;
    union
    {
        struct
        {
            uint32_t next;
            uint32_t rsvd0[1];
            uint32_t spc;
            uint32_t interrupt_en;
            uint16_t group_id;
            uint16_t grid_id;
            uint16_t task_id;
            uint16_t warmup_len;
            uint16_t grid_dim_x;
            uint16_t grid_dim_y;
            uint16_t grid_dim_z;
            uint16_t group_dim_x;
            uint16_t group_dim_y;
            uint16_t group_dim_z;
            uint16_t group_id_x;
            uint16_t group_id_y;
            uint16_t group_id_z;
            uint16_t task_id_x;
            uint16_t task_id_y;
            uint16_t task_id_z;
            uint32_t sp;
            uint32_t pp;
            uint32_t dp;
            uint32_t cp;
            uint32_t pprint;
            uint32_t pprofiler;
            uint16_t core_id;
            uint16_t cluster_id;
            uint16_t tec_id;
            uint16_t rsvd1;
            uint32_t rsvd2[5];
            uint32_t dsize;
            uint32_t tcbp;
            uint32_t global_param;
            uint32_t rsvd3;
        } noninit;

        union
        {
            struct
            {
                uint32_t rsvd0;
                uint32_t group_num;
                uint32_t rsvd1;
                uint32_t _interrupt_en;
                uint16_t _group_id;
                uint16_t _grid_id;
                uint32_t rsvd2[6];
                uint32_t gm_ctrl;
                uint32_t gm_sync;
                uint32_t gm_addr_low;
                uint32_t gm_addr_high;
                uint32_t rsvd3[14];
            } grid_init;

            struct
            {
                uint32_t segmmu_ctrl;
                uint32_t segmmu_remap_ctrl0;
                uint32_t segmmu_remap_ctrl1;
                uint32_t __interrupt_en;
                uint16_t __group_id;
                uint16_t __grid_id;
                uint32_t segmmu_seg_ctrl[16];
                uint32_t asids[8];
            } group_init;
        } init;
    } __data;
    uint16_t group_deps[4];
};

/* task tcb */
#define _next        __data.noninit.next
#define spc          __data.noninit.spc
#define interrupt_en __data.noninit.interrupt_en
#define groupid      __data.noninit.group_id
#define gridid       __data.noninit.grid_id
#define taskid       __data.noninit.task_id
#define ica_warmup_len   __data.noninit.warmup_len
#define grid_dim_x   __data.noninit.grid_dim_x
#define grid_dim_y   __data.noninit.grid_dim_y
#define grid_dim_z   __data.noninit.grid_dim_z
#define group_dim_x  __data.noninit.group_dim_x
#define group_dim_y  __data.noninit.group_dim_y
#define group_dim_z  __data.noninit.group_dim_z
#define group_id_x   __data.noninit.group_id_x
#define group_id_y   __data.noninit.group_id_y
#define group_id_z   __data.noninit.group_id_z
#define task_id_x    __data.noninit.task_id_x
#define task_id_y    __data.noninit.task_id_y
#define task_id_z    __data.noninit.task_id_z
#define sp           __data.noninit.sp
#define pp           __data.noninit.pp
#define dp           __data.noninit.dp
#define cp           __data.noninit.cp
#define pprint       __data.noninit.pprint
#define pprofiler    __data.noninit.pprofiler
#define coreid       __data.noninit.core_id
#define clusterid    __data.noninit.cluster_id
#define tecid        __data.noninit.tec_id
#define dsize        __data.noninit.dsize
#define tcbp         __data.noninit.tcbp
#define global_param __data.noninit.global_param

/* grid init tcb */
#define group_num           __data.init.grid_init.group_num
#define grid_intrrupt_en    __data.init.grid_init._interrupt_en
#define grid_groupid        __data.init.grid_init._group_id
#define grid_gridid         __data.init.grid_init._grid_id
#define gm_ctrl             __data.init.grid_init.gm_ctrl
#define gm_sync             __data.init.grid_init.gm_sync
#define gm_addr_low         __data.init.grid_init.gm_addr_low
#define gm_addr_high        __data.init.grid_init.gm_addr_high

/* group init tcb */
#define segmmu_ctrl         __data.init.group_init.segmmu_ctrl
#define segmmu_remap_ctrl0  __data.init.group_init.segmmu_remap_ctrl0
#define segmmu_remap_ctrl1  __data.init.group_init.segmmu_remap_ctrl1
#define group_interrupt_en  __data.init.group_init.__interrupt_en
#define group_groupid       __data.init.group_init.__group_id
#define group_gridid        __data.init.group_init.__grid_id
#define segmmu_seg_ctrl     __data.init.group_init.segmmu_seg_ctrl
#define asids               __data.init.group_init.asids

#define TCB_FLAG_TASK_TYPE(flag)       (flag & 0xF)
#define TCB_FLAG_TASK_TYPE_GRID_INIT   0
#define TCB_FLAG_TASK_TYPE_GROUP_INIT  1
#define TCB_FLAG_TASK_TYPE_TASK        2

#define TCB_FLAG_DEP_TYPE(flag)        (flag & 0x30)
#define TCB_FLAG_DEP_TYPE_NONE         0
#define TCB_FLAG_DEP_TYPE_GROUP        (1 << 4)
#define TCB_FLAG_DEP_TYPE_PRE_ALL      (2 << 4)

#define TCB_FLAG_END_TYPE_NOT_END      0
#define TCB_FLAG_END_TYPE_GROUP_END    (1 << 6)
#define TCB_FLAG_END_TYPE_GRID_END     (1 << 7)
#define TCB_FLAG_END_TYPE_POOL_END     (1 << 8)

#define TCB_FLAG_CORE_NUM(n)           ((n & 0xF) << 16)
#define TCB_FLAG_BROADCAST_START       (1 << 20)
#define TCB_FLAG_GRID_INIT             (1 << 21)
#define TCB_FLAG_L2D_FLUSH             (1 << 22)

/* task tcb interrupt */
#define EN_INTERRUPT_TEC_DONE          (1 << 0)
#define EN_INTERRUPT_TEC_SIGNAL        (1 << 1)
#define EN_INTERRUPT_TEC_EXCEPTION     (1 << 2)
#define EN_INTERRUPT_TEC_FAULT         (1 << 3)
#define EN_INTERRUPT_TEC_ALL \
        (EN_INTERRUPT_TEC_DONE | EN_INTERRUPT_TEC_SIGNAL | \
        EN_INTERRUPT_TEC_EXCEPTION | EN_INTERRUPT_TEC_FAULT)

/* grid tcb interrupt */
#define EN_INTERRUPT_GRID_DONE         (1 << 0)
#define EN_INTERRUPT_GRID_GM_FALUT     (1 << 3)
#define EN_INTERRUPT_GRID_ALL (EN_INTERRUPT_GRID_DONE | EN_INTERRUPT_GRID_GM_FALUT)

/* group tcb interrupt */
#define EN_INTERRUPT_GROUP_DONE        (1 << 0)

#define GM_CTRL_REMAP_EN               (0x1)
#define GM_CTRL_REMAP_MODE_RES_PRIOR   (0x2)
#define GM_MAX_SIZE                    (8 << 20)

#define GM_SYNC_ONLY_UPDATE_REG        (0 << 30)
#define GM_SYNC_DDR_TO_GM              (1UL << 30)
#define GM_SYNC_IGNORE_CFG             (3UL << 30)

#define EN_GROUP_DEPEND                (1 << 15)

#endif
}

#endif //!__TCB_H__
