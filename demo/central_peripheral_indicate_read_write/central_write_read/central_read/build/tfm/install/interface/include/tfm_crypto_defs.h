/*
 * Copyright (c) 2018-2022, Arm Limited. All rights reserved.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 *
 */

#ifndef __TFM_CRYPTO_DEFS_H__
#define __TFM_CRYPTO_DEFS_H__

#ifdef __cplusplus
extern "C" {
#endif

#include <stdint.h>
#include "psa/crypto.h"
#ifdef PLATFORM_DEFAULT_CRYPTO_KEYS
#include "crypto_keys/tfm_builtin_key_ids.h"
#else
#include "tfm_builtin_key_ids.h"
#endif /* PLATFORM_DEFAULT_CRYPTO_KEYS */

/**
 * \brief The maximum supported length of a nonce through the TF-M
 *        interfaces
 */
#define TFM_CRYPTO_MAX_NONCE_LENGTH (16u)

/**
 * \brief This type is used to overcome a limitation in the number of maximum
 *        IOVECs that can be used especially in psa_aead_encrypt and
 *        psa_aead_decrypt. By using this type we pack the nonce and the actual
 *        nonce_length at part of the same structure
 */
struct tfm_crypto_aead_pack_input {
    uint8_t nonce[TFM_CRYPTO_MAX_NONCE_LENGTH];
    uint32_t nonce_length;
};

/**
 * \brief Structure used to pack non-pointer types in a call to PSA Crypto APIs
 *
 */
struct tfm_crypto_pack_iovec {
    psa_key_id_t key_id;     /*!< Key id */
    psa_algorithm_t alg;     /*!< Algorithm */
    uint32_t op_handle;      /*!< Frontend context handle associated to a
                              *   multipart operation
                              */
    size_t capacity;         /*!< Key derivation capacity */
    size_t ad_length;        /*!< Additional Data length for multipart AEAD */
    size_t plaintext_length; /*!< Plaintext length for multipart AEAD */

    struct tfm_crypto_aead_pack_input aead_in; /*!< Packs AEAD-related inputs */

    uint16_t function_id;    /*!< Used to identify the function in the
                              *   API dispatcher to the service backend
                              *   See tfm_crypto_func_sid for detail
                              */
    uint16_t step;           /*!< Key derivation step */
};

/**
 * \brief Type associated to the group of a function encoding. There can be
 *        nine groups (Random, Key management, Hash, MAC, Cipher, AEAD,
 *        Asym sign, Asym encrypt, Key derivation).
 */
enum tfm_crypto_group_id {
    TFM_CRYPTO_GROUP_ID_RANDOM = 0x0,
    TFM_CRYPTO_GROUP_ID_KEY_MANAGEMENT,
    TFM_CRYPTO_GROUP_ID_HASH,
    TFM_CRYPTO_GROUP_ID_MAC,
    TFM_CRYPTO_GROUP_ID_CIPHER,
    TFM_CRYPTO_GROUP_ID_AEAD,
    TFM_CRYPTO_GROUP_ID_ASYM_SIGN,
    TFM_CRYPTO_GROUP_ID_ASYM_ENCRYPT,
    TFM_CRYPTO_GROUP_ID_KEY_DERIVATION,
};

/* Set of X macros describing each of the available PSA Crypto APIs */
#define KEY_MANAGEMENT_FUNCS                       \
    X(TFM_CRYPTO_GET_KEY_ATTRIBUTES)               \
    X(TFM_CRYPTO_RESET_KEY_ATTRIBUTES)             \
    X(TFM_CRYPTO_OPEN_KEY)                         \
    X(TFM_CRYPTO_CLOSE_KEY)                        \
    X(TFM_CRYPTO_IMPORT_KEY)                       \
    X(TFM_CRYPTO_DESTROY_KEY)                      \
    X(TFM_CRYPTO_EXPORT_KEY)                       \
    X(TFM_CRYPTO_EXPORT_PUBLIC_KEY)                \
    X(TFM_CRYPTO_PURGE_KEY)                        \
    X(TFM_CRYPTO_COPY_KEY)                         \
    X(TFM_CRYPTO_GENERATE_KEY)

#define HASH_FUNCS                                 \
    X(TFM_CRYPTO_HASH_COMPUTE)                     \
    X(TFM_CRYPTO_HASH_COMPARE)                     \
    X(TFM_CRYPTO_HASH_SETUP)                       \
    X(TFM_CRYPTO_HASH_UPDATE)                      \
    X(TFM_CRYPTO_HASH_CLONE)                       \
    X(TFM_CRYPTO_HASH_FINISH)                      \
    X(TFM_CRYPTO_HASH_VERIFY)                      \
    X(TFM_CRYPTO_HASH_ABORT)

#define MAC_FUNCS                                  \
    X(TFM_CRYPTO_MAC_COMPUTE)                      \
    X(TFM_CRYPTO_MAC_VERIFY)                       \
    X(TFM_CRYPTO_MAC_SIGN_SETUP)                   \
    X(TFM_CRYPTO_MAC_VERIFY_SETUP)                 \
    X(TFM_CRYPTO_MAC_UPDATE)                       \
    X(TFM_CRYPTO_MAC_SIGN_FINISH)                  \
    X(TFM_CRYPTO_MAC_VERIFY_FINISH)                \
    X(TFM_CRYPTO_MAC_ABORT)

#define CIPHER_FUNCS                               \
    X(TFM_CRYPTO_CIPHER_ENCRYPT)                   \
    X(TFM_CRYPTO_CIPHER_DECRYPT)                   \
    X(TFM_CRYPTO_CIPHER_ENCRYPT_SETUP)             \
    X(TFM_CRYPTO_CIPHER_DECRYPT_SETUP)             \
    X(TFM_CRYPTO_CIPHER_GENERATE_IV)               \
    X(TFM_CRYPTO_CIPHER_SET_IV)                    \
    X(TFM_CRYPTO_CIPHER_UPDATE)                    \
    X(TFM_CRYPTO_CIPHER_FINISH)                    \
    X(TFM_CRYPTO_CIPHER_ABORT)

#define AEAD_FUNCS                                 \
    X(TFM_CRYPTO_AEAD_ENCRYPT)                     \
    X(TFM_CRYPTO_AEAD_DECRYPT)                     \
    X(TFM_CRYPTO_AEAD_ENCRYPT_SETUP)               \
    X(TFM_CRYPTO_AEAD_DECRYPT_SETUP)               \
    X(TFM_CRYPTO_AEAD_GENERATE_NONCE)              \
    X(TFM_CRYPTO_AEAD_SET_NONCE)                   \
    X(TFM_CRYPTO_AEAD_SET_LENGTHS)                 \
    X(TFM_CRYPTO_AEAD_UPDATE_AD)                   \
    X(TFM_CRYPTO_AEAD_UPDATE)                      \
    X(TFM_CRYPTO_AEAD_FINISH)                      \
    X(TFM_CRYPTO_AEAD_VERIFY)                      \
    X(TFM_CRYPTO_AEAD_ABORT)

#define ASYMMETRIC_SIGN_FUNCS                      \
    X(TFM_CRYPTO_ASYMMETRIC_SIGN_MESSAGE)          \
    X(TFM_CRYPTO_ASYMMETRIC_VERIFY_MESSAGE)        \
    X(TFM_CRYPTO_ASYMMETRIC_SIGN_HASH)             \
    X(TFM_CRYPTO_ASYMMETRIC_VERIFY_HASH)

#define AYSMMETRIC_ENCRYPT_FUNCS                   \
    X(TFM_CRYPTO_ASYMMETRIC_ENCRYPT)               \
    X(TFM_CRYPTO_ASYMMETRIC_DECRYPT)

#define KEY_DERIVATION_FUNCS                       \
    X(TFM_CRYPTO_RAW_KEY_AGREEMENT)                \
    X(TFM_CRYPTO_KEY_DERIVATION_SETUP)             \
    X(TFM_CRYPTO_KEY_DERIVATION_GET_CAPACITY)      \
    X(TFM_CRYPTO_KEY_DERIVATION_SET_CAPACITY)      \
    X(TFM_CRYPTO_KEY_DERIVATION_INPUT_BYTES)       \
    X(TFM_CRYPTO_KEY_DERIVATION_INPUT_KEY)         \
    X(TFM_CRYPTO_KEY_DERIVATION_KEY_AGREEMENT)     \
    X(TFM_CRYPTO_KEY_DERIVATION_OUTPUT_BYTES)      \
    X(TFM_CRYPTO_KEY_DERIVATION_OUTPUT_KEY)        \
    X(TFM_CRYPTO_KEY_DERIVATION_ABORT)

#define RANDOM_FUNCS                               \
    X(TFM_CRYPTO_GENERATE_RANDOM)

/**
 * \brief Define function IDs in each group. The function ID will be encoded into
 *        tfm_crypto_func_sid below. Each group is defined as a dedicated enum
 */
#define X(func_id)    func_id,
enum tfm_crypto_key_management_func_id {
    KEY_MANAGEMENT_FUNCS
};
enum tfm_crypto_hash_func_id {
    HASH_FUNCS
};
enum tfm_crypto_mac_func_id {
    MAC_FUNCS
};
enum tfm_crypto_cipher_func_id {
    CIPHER_FUNCS
};
enum tfm_crypto_aead_func_id {
    AEAD_FUNCS
};
enum tfm_crypto_asym_sign_func_id {
    ASYMMETRIC_SIGN_FUNCS
};
enum tfm_crypto_asym_encrypt_func_id {
    AYSMMETRIC_ENCRYPT_FUNCS
};
enum tfm_crypto_key_derivation_func_id {
    KEY_DERIVATION_FUNCS
};
enum tfm_crypto_random_func_id {
    RANDOM_FUNCS
};
#undef X

/**
 * \brief This macro is used to encode a function ID in a 16-bit value as the
 *        top 8 bits towards the MSB
 */
#define FUNC_ID(func_id)    (((func_id) & 0xFF) << 8)

/**
 * \brief This macro is used to extract the group_id from an encoded function_id
 *        by accessing the low 8 bits towards the LSB
 */
#define TFM_CRYPTO_GET_GROUP_ID(_function_id)    \
                        ((enum tfm_crypto_group_id)((_function_id) & 0xFF))
/**
 * \brief This type defines numerical progressive values identifying a function API
 *        exposed through the interfaces (S or NS). It's used to dispatch the requests
 *        from S/NS to the corresponding API implementation in the Crypto service backend.
 *
 * \note Each function SID is encoded as uint16_t.
 *        +-----------+-----------+
 *        |  Func ID  |  Group ID |
 *        +-----------+-----------+
 *   (MSB)15         8 7          0(LSB)
 *
 *       where Func ID is defined in each group tfm_crypto_*_func_id enum above and Group ID
 *       is defined in \ref tfm_crypto_group_id.
 */
enum tfm_crypto_func_sid {

#define X(func_id)      func_id ## _SID = (uint16_t)((FUNC_ID(func_id)) | \
                                   (TFM_CRYPTO_GROUP_ID_KEY_MANAGEMENT & 0xFF)),

    KEY_MANAGEMENT_FUNCS

#undef X
#define X(func_id)      func_id ## _SID = (uint16_t)((FUNC_ID(func_id)) | \
                                            (TFM_CRYPTO_GROUP_ID_HASH & 0xFF)),
    HASH_FUNCS

#undef X
#define X(func_id)      func_id ## _SID = (uint16_t)((FUNC_ID(func_id)) | \
                                            (TFM_CRYPTO_GROUP_ID_MAC & 0xFF)),
    MAC_FUNCS

#undef X
#define X(func_id)      func_id ## _SID = (uint16_t)((FUNC_ID(func_id)) | \
                                           (TFM_CRYPTO_GROUP_ID_CIPHER & 0xFF)),
    CIPHER_FUNCS

#undef X
#define X(func_id)      func_id ## _SID = (uint16_t)((FUNC_ID(func_id)) | \
                                             (TFM_CRYPTO_GROUP_ID_AEAD & 0xFF)),
    AEAD_FUNCS

#undef X
#define X(func_id)      func_id ## _SID = (uint16_t)((FUNC_ID(func_id)) | \
                                        (TFM_CRYPTO_GROUP_ID_ASYM_SIGN & 0xFF)),
    ASYMMETRIC_SIGN_FUNCS

#undef X
#define X(func_id)      func_id ## _SID = (uint16_t)((FUNC_ID(func_id)) | \
                                     (TFM_CRYPTO_GROUP_ID_ASYM_ENCRYPT & 0xFF)),
    AYSMMETRIC_ENCRYPT_FUNCS

#undef X
#define X(func_id)      func_id ## _SID = (uint16_t)((FUNC_ID(func_id)) | \
                                   (TFM_CRYPTO_GROUP_ID_KEY_DERIVATION & 0xFF)),
    KEY_DERIVATION_FUNCS

#undef X
#define X(func_id)      func_id ## _SID = (uint16_t)((FUNC_ID(func_id)) | \
                                           (TFM_CRYPTO_GROUP_ID_RANDOM & 0xFF)),
    RANDOM_FUNCS

};
#undef X

#ifdef __cplusplus
}
#endif

#endif /* __TFM_CRYPTO_DEFS_H__ */
