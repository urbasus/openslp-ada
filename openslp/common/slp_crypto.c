/*-------------------------------------------------------------------------
 * Copyright (C) 2000 Caldera Systems, Inc
 * All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions
 * are met:
 *
 *    Redistributions of source code must retain the above copyright
 *    notice, this list of conditions and the following disclaimer.
 *
 *    Redistributions in binary form must reproduce the above copyright
 *    notice, this list of conditions and the following disclaimer in the
 *    documentation and/or other materials provided with the distribution.
 *
 *    Neither the name of Caldera Systems nor the names of its
 *    contributors may be used to endorse or promote products derived
 *    from this software without specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS
 * `AS IS'' AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT
 * LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR
 * A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE CALDERA
 * SYSTEMS OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL,
 * SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT
 * LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES;  LOSS OF USE,
 * DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON
 * ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
 * (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
 * OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 *-------------------------------------------------------------------------*/

/** Primitive crypto functions to support DSA signature of SHA1 digests. 
 *
 * The current implementation uses the OpenSSL (http://www.openssl.org) 
 * cryptographic library.
 *
 * @file       slp_crypto.c
 * @author     Matthew Peterson, John Calcote (jcalcote@novell.com)
 * @attention  Please submit patches to http://www.openslp.org
 * @ingroup    CommonCode
 */

#include "slp_crypto.h"
#include "slp_message.h"
  
/** Generate a SHA1 digest for the specified block data.
 *
 * @param[in] data - The data block to be hashed.
 * @param[in] datalen - The length of @p data in bytes.
 * @param[out] digest - The address of storage for the digest.
 * 
 * @return Zero on success, or a non-zero error code.
 *
 * @remarks The @p digest parameter must point to a buffer of at least 
 *    20 bytes.
 */
int SLPCryptoSHA1Digest(const unsigned char* data,
                        int datalen,
                        unsigned char* digest)
{
    if(SHA1(data,datalen,digest))
    {
        return 0;
    }

    return -1;
}

/** Duplicates a key.
 *
 * @param[in] dsa - The key to be duplicated.
 * 
 * @return A pointer to a duplicate of @p dsa.
 *
 * @remarks The caller must use SLPCryptoDSAKeyDestroy to release the 
 *    resources used by this key.
 */
SLPCryptoDSAKey* SLPCryptoDSAKeyDup(SLPCryptoDSAKey* dsa)
{
    SLPCryptoDSAKey* result;
    
    result =  DSA_new();
    if(result)
    {
        result->p = BN_dup(dsa->p);
        result->q = BN_dup(dsa->q);
        result->g = BN_dup(dsa->g);
        result->priv_key = BN_dup(dsa->priv_key);
        result->pub_key = BN_dup(dsa->pub_key);
    }

    return result;
}

/** Destroy a key that was created by SLPCryptoDSAKeyCreate.
 *
 * @param[in] dsa - The key to be destroyed.
 * 
 * @remarks Care should be taken to make sure all private keys 
 *    are destroyed.
 */
void SLPCryptoDSAKeyDestroy(SLPCryptoDSAKey* dsa)
{
    DSA_free(dsa);
}

/** Determine the length of a signatures produced with specified key.
 *
 * @param[in] key - The key that will be used for signing.
 * 
 * @return The length of all signatures generated by this key.
 */
int SLPCryptoDSASignLen(SLPCryptoDSAKey* key)
{
    return DSA_size(key);
}

/** Signs the specified digest with the specified DSA key.
 *
 * @param[in] key - The key to sign with.
 * @param[in] digest - The digest to be signed.
 * @param[in] digestlen - The length of @p digest.
 * @param[out] signature - The address of storage for the signature.
 * @param[in] signaturelen - The length of @p signature.
 * 
 * @return Zero on success, or a non-zero error code.
 *
 * @remarks The caller should call SLPCryptoDSASignLen(key) to determine
 *    how large signature should be.
 */
int SLPCryptoDSASign(SLPCryptoDSAKey* key,
                     const unsigned char* digest,
                     int digestlen,
                     unsigned char* signature,
                     int* signaturelen)
{
    return DSA_sign(0, /* it does not look like the type param is used? */
                    digest,
                    digestlen,
                    signature,
                    signaturelen,
                    key) == 0;
}

/** Verifies a DSA signature to ensure it matches the specified digest.
 *
 * @param[in] key - The verifying key.
 * @param[in] digest - The digest buffer to be verified.
 * @param[in] digestlen - The length of @p digest.
 * @param[in] signature - The ASN.1 DER encoded signature.
 * @param[in] signaturelen - The length of @p signature.
 * 
 * @return True (1) if the signature is value; False (0) if not.
 */
int SLPCryptoDSAVerify(SLPCryptoDSAKey* key,
                       const unsigned char* digest,
                       int digestlen,
                       const unsigned char* signature,
                       int signaturelen)
{
    return DSA_verify(0, /* it does not look like the type param is used? */
                      digest,
                      digestlen,
                      (unsigned char*)signature,  /* broken DSA_verify() declaration */
                      signaturelen,
                      key);
}

/*=========================================================================*/
