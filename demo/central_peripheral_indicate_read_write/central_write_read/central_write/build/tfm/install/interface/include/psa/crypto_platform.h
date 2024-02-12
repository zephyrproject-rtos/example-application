
/*
 * Copyright (c) 2023, Arm Limited. All rights reserved.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 *
 */
/**
 * \file psa/crypto_platform.h
 *
 * \brief PSA cryptography module: TF-M platform definitions
 *
 * \note This file may not be included directly. Applications must
 * include psa/crypto.h.
 *
 * This file contains platform-dependent type definitions.
 *
 * In implementations with isolation between the application and the
 * cryptography module, implementers should take care to ensure that
 * the definitions that are exposed to applications match what the
 * module implements.
 */

#ifndef PSA_CRYPTO_PLATFORM_H
#define PSA_CRYPTO_PLATFORM_H

/* PSA requires several types which C99 provides in stdint.h. */
#include <stdint.h>

/* No particular platform definition is currently required for the
 * TF-M client view of the PSA Crytpo APIs, but we keep this header
 * available for reference and future compatibility
 */

#endif /* PSA_CRYPTO_PLATFORM_H */
