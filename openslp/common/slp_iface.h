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

/** Header file for functions to obtain network interface information.
 *
 * @file       slp_iface.h
 * @author     Matthew Peterson, John Calcote (jcalcote@novell.com)
 * @attention  Please submit patches to http://www.openslp.org
 * @ingroup    CommonCode
 */

#ifndef SLP_IFACE_H_INCLUDED
#define SLP_IFACE_H_INCLUDED

/*!@defgroup CommonCodeNetIfc Interface */

/*!@addtogroup CommonCodeNetIfc
 * @ingroup CommonCodeNetwork
 * @{
 */

#ifdef HAVE_CONFIG_H
# include "config.h"
#endif
#ifdef HAVE_SYS_TYPES_H
# include <sys/types.h>
#endif
#ifdef HAVE_STDINT_H
# include <stdint.h>
#endif
#ifdef HAVE_NETINET_IN_H
# include <netinet/in.h>
#endif
#ifdef _WIN32
# define WIN32_LEAN_AND_MEAN
# include <winsock2.h>
# include <ws2tcpip.h>
#endif

#include "slp_net.h"

#define SLP_MAX_IFACES 100

typedef struct _SLPInterfaceInfo
{
    int iface_count;
    int bcast_count;
    struct sockaddr_storage iface_addr[SLP_MAX_IFACES];
    struct sockaddr_storage bcast_addr[SLP_MAX_IFACES];
} SLPIfaceInfo;

int SLPIfaceGetInfo(const char* useifaces,
                    SLPIfaceInfo* ifaceinfo, int family);

int SLPIfaceSockaddrsToString(const struct sockaddr_storage* addrs,
                              int addrcount,
                              char** addrstr);

int SLPIfaceStringToSockaddrs(const char* addrstr,
                              struct sockaddr_storage* addrs,
                              int* addrcount);

/*! @} */

#endif   /* SLP_IFACE_H_INCLUDED */

/*=========================================================================*/
