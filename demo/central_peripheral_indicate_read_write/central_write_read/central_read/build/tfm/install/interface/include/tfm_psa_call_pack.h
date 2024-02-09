/*
 * Copyright (c) 2021-2022, Arm Limited. All rights reserved.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 *
 */

#ifndef __TFM_PSA_CALL_PACK_H__
#define __TFM_PSA_CALL_PACK_H__

#include "psa/client.h"

#ifdef __cplusplus
extern "C" {
#endif

#define TYPE_OFFSET     16U
#define TYPE_MASK       (0xFFFFUL << TYPE_OFFSET)
#define IN_LEN_OFFSET   8U
#define IN_LEN_MASK     (0xFFUL << IN_LEN_OFFSET)
#define OUT_LEN_OFFSET  0U
#define OUT_LEN_MASK    (0xFFUL << OUT_LEN_OFFSET)

#define PARAM_PACK(type, in_len, out_len)                        \
        (((((uint32_t)type) << TYPE_OFFSET) & TYPE_MASK)       | \
         ((((uint32_t)in_len) << IN_LEN_OFFSET) & IN_LEN_MASK) | \
         ((((uint32_t)out_len) << OUT_LEN_OFFSET) & OUT_LEN_MASK))

#define PARAM_UNPACK_TYPE(ctrl_param) \
        ((int32_t)(((ctrl_param) & TYPE_MASK) >> TYPE_OFFSET))

#define PARAM_UNPACK_IN_LEN(ctrl_param) \
        ((size_t)(((ctrl_param) & IN_LEN_MASK) >> IN_LEN_OFFSET))

#define PARAM_UNPACK_OUT_LEN(ctrl_param) \
        ((size_t)(((ctrl_param) & OUT_LEN_MASK) >> OUT_LEN_OFFSET))

psa_status_t tfm_psa_call_pack(psa_handle_t handle,
                               uint32_t ctrl_param,
                               const psa_invec *in_vec,
                               psa_outvec *out_vec);

#ifdef __cplusplus
}
#endif

#endif /* __TFM_PSA_CALL_PACK_H__ */
