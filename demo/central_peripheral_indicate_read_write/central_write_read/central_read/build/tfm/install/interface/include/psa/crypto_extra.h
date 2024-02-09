/*
 * Copyright (c) 2018-2023, Arm Limited. All rights reserved.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 *
 */
/**
 * \file psa/crypto_extra.h
 *
 * \brief PSA cryptography module: vendor extensions
 *
 * \note This file may not be included directly. Applications must
 * include psa/crypto.h.
 *
 * This file is reserved for vendor-specific definitions.
 */

#ifndef PSA_CRYPTO_EXTRA_H
#define PSA_CRYPTO_EXTRA_H

#include "crypto_types.h"
#include "crypto_compat.h"

#ifdef __cplusplus
extern "C" {
#endif

/** \addtogroup crypto_types
 * @{
 */

/** DSA public key.
 *
 * The import and export format is the
 * representation of the public key `y = g^x mod p` as a big-endian byte
 * string. The length of the byte string is the length of the base prime `p`
 * in bytes.
 */
#define PSA_KEY_TYPE_DSA_PUBLIC_KEY                 ((psa_key_type_t) 0x4002)

/** DSA key pair (private and public key).
 *
 * The import and export format is the
 * representation of the private key `x` as a big-endian byte string. The
 * length of the byte string is the private key size in bytes (leading zeroes
 * are not stripped).
 *
 * Deterministic DSA key derivation with psa_generate_derived_key follows
 * FIPS 186-4 &sect;B.1.2: interpret the byte string as integer
 * in big-endian order. Discard it if it is not in the range
 * [0, *N* - 2] where *N* is the boundary of the private key domain
 * (the prime *p* for Diffie-Hellman, the subprime *q* for DSA,
 * or the order of the curve's base point for ECC).
 * Add 1 to the resulting integer and use this as the private key *x*.
 *
 */
#define PSA_KEY_TYPE_DSA_KEY_PAIR                    ((psa_key_type_t) 0x7002)

/** Whether a key type is a DSA key (pair or public-only). */
#define PSA_KEY_TYPE_IS_DSA(type)                                       \
    (PSA_KEY_TYPE_PUBLIC_KEY_OF_KEY_PAIR(type) == PSA_KEY_TYPE_DSA_PUBLIC_KEY)

#define PSA_ALG_DSA_BASE                        ((psa_algorithm_t) 0x06000400)
/** DSA signature with hashing.
 *
 * This is the signature scheme defined by FIPS 186-4,
 * with a random per-message secret number (*k*).
 *
 * \param hash_alg      A hash algorithm (\c PSA_ALG_XXX value such that
 *                      #PSA_ALG_IS_HASH(\p hash_alg) is true).
 *                      This includes #PSA_ALG_ANY_HASH
 *                      when specifying the algorithm in a usage policy.
 *
 * \return              The corresponding DSA signature algorithm.
 * \return              Unspecified if \p hash_alg is not a supported
 *                      hash algorithm.
 */
#define PSA_ALG_DSA(hash_alg)                             \
    (PSA_ALG_DSA_BASE | ((hash_alg) & PSA_ALG_HASH_MASK))
#define PSA_ALG_DETERMINISTIC_DSA_BASE          ((psa_algorithm_t) 0x06000500)
#define PSA_ALG_DSA_DETERMINISTIC_FLAG PSA_ALG_ECDSA_DETERMINISTIC_FLAG
/** Deterministic DSA signature with hashing.
 *
 * This is the deterministic variant defined by RFC 6979 of
 * the signature scheme defined by FIPS 186-4.
 *
 * \param hash_alg      A hash algorithm (\c PSA_ALG_XXX value such that
 *                      #PSA_ALG_IS_HASH(\p hash_alg) is true).
 *                      This includes #PSA_ALG_ANY_HASH
 *                      when specifying the algorithm in a usage policy.
 *
 * \return              The corresponding DSA signature algorithm.
 * \return              Unspecified if \p hash_alg is not a supported
 *                      hash algorithm.
 */
#define PSA_ALG_DETERMINISTIC_DSA(hash_alg)                             \
    (PSA_ALG_DETERMINISTIC_DSA_BASE | ((hash_alg) & PSA_ALG_HASH_MASK))
#define PSA_ALG_IS_DSA(alg)                                             \
    (((alg) & ~PSA_ALG_HASH_MASK & ~PSA_ALG_DSA_DETERMINISTIC_FLAG) ==  \
     PSA_ALG_DSA_BASE)
#define PSA_ALG_DSA_IS_DETERMINISTIC(alg)               \
    (((alg) & PSA_ALG_DSA_DETERMINISTIC_FLAG) != 0)
#define PSA_ALG_IS_DETERMINISTIC_DSA(alg)                       \
    (PSA_ALG_IS_DSA(alg) && PSA_ALG_DSA_IS_DETERMINISTIC(alg))
#define PSA_ALG_IS_RANDOMIZED_DSA(alg)                          \
    (PSA_ALG_IS_DSA(alg) && !PSA_ALG_DSA_IS_DETERMINISTIC(alg))


/* We need to expand the sample definition of this macro from
 * the API definition. */
#undef PSA_ALG_IS_VENDOR_HASH_AND_SIGN
#define PSA_ALG_IS_VENDOR_HASH_AND_SIGN(alg)    \
    PSA_ALG_IS_DSA(alg)

#if defined(MBEDTLS_ECP_C)
#include <mbedtls/ecp.h>

/** Convert an ECC curve identifier from the Mbed TLS encoding to PSA.
 *
 * \note This function is provided solely for the convenience of
 *       Mbed TLS and may be removed at any time without notice.
 *
 * \param grpid         An Mbed TLS elliptic curve identifier
 *                      (`MBEDTLS_ECP_DP_xxx`).
 * \param[out] bits     On success, the bit size of the curve.
 *
 * \return              The corresponding PSA elliptic curve identifier
 *                      (`PSA_ECC_FAMILY_xxx`).
 * \return              \c 0 on failure (\p grpid is not recognized).
 */
static inline psa_ecc_family_t mbedtls_ecc_group_to_psa( mbedtls_ecp_group_id grpid,
                                                        size_t *bits )
{
    switch( grpid )
    {
        case MBEDTLS_ECP_DP_SECP192R1:
            *bits = 192;
            return( PSA_ECC_FAMILY_SECP_R1 );
        case MBEDTLS_ECP_DP_SECP224R1:
            *bits = 224;
            return( PSA_ECC_FAMILY_SECP_R1 );
        case MBEDTLS_ECP_DP_SECP256R1:
            *bits = 256;
            return( PSA_ECC_FAMILY_SECP_R1 );
        case MBEDTLS_ECP_DP_SECP384R1:
            *bits = 384;
            return( PSA_ECC_FAMILY_SECP_R1 );
        case MBEDTLS_ECP_DP_SECP521R1:
            *bits = 521;
            return( PSA_ECC_FAMILY_SECP_R1 );
        case MBEDTLS_ECP_DP_BP256R1:
            *bits = 256;
            return( PSA_ECC_FAMILY_BRAINPOOL_P_R1 );
        case MBEDTLS_ECP_DP_BP384R1:
            *bits = 384;
            return( PSA_ECC_FAMILY_BRAINPOOL_P_R1 );
        case MBEDTLS_ECP_DP_BP512R1:
            *bits = 512;
            return( PSA_ECC_FAMILY_BRAINPOOL_P_R1 );
        case MBEDTLS_ECP_DP_CURVE25519:
            *bits = 255;
            return( PSA_ECC_FAMILY_MONTGOMERY );
        case MBEDTLS_ECP_DP_SECP192K1:
            *bits = 192;
            return( PSA_ECC_FAMILY_SECP_K1 );
        case MBEDTLS_ECP_DP_SECP224K1:
            *bits = 224;
            return( PSA_ECC_FAMILY_SECP_K1 );
        case MBEDTLS_ECP_DP_SECP256K1:
            *bits = 256;
            return( PSA_ECC_FAMILY_SECP_K1 );
        case MBEDTLS_ECP_DP_CURVE448:
            *bits = 448;
            return( PSA_ECC_FAMILY_MONTGOMERY );
        default:
            *bits = 0;
            return( 0 );
    }
}

/** Convert an ECC curve identifier from the PSA encoding to Mbed TLS.
 *
 * \note This function is provided solely for the convenience of
 *       Mbed TLS and may be removed at any time without notice.
 *
 * \param curve         A PSA elliptic curve identifier
 *                      (`PSA_ECC_FAMILY_xxx`).
 * \param byte_length   The byte-length of a private key on \p curve.
 *
 * \return              The corresponding Mbed TLS elliptic curve identifier
 *                      (`MBEDTLS_ECP_DP_xxx`).
 * \return              #MBEDTLS_ECP_DP_NONE if \c curve is not recognized.
 * \return              #MBEDTLS_ECP_DP_NONE if \p byte_length is not
 *                      correct for \p curve.
 */
mbedtls_ecp_group_id mbedtls_ecc_group_of_psa( psa_ecc_family_t curve,
                                               size_t byte_length );
#endif /* MBEDTLS_ECP_C */


/**@}*/

#ifdef __cplusplus
}
#endif

#endif /* PSA_CRYPTO_EXTRA_H */
