/***************************************************************************/
/*                                                                         */
/* Project:     OpenSLP - OpenSource implementation of Service Location    */
/*              Protocol Version 2                                         */
/*                                                                         */
/* File:        slpd_knownda.c                                             */
/*                                                                         */
/* Abstract:    Keeps track of known DAs                                   */
/*                                                                         */
/*-------------------------------------------------------------------------*/
/*                                                                         */
/* Copyright (c) 1995, 1999  Caldera Systems, Inc.                         */
/*                                                                         */
/* This program is free software; you can redistribute it and/or modify it */
/* under the terms of the GNU Lesser General Public License as published   */
/* by the Free Software Foundation; either version 2.1 of the License, or  */
/* (at your option) any later version.                                     */
/*                                                                         */
/*     This program is distributed in the hope that it will be useful,     */
/*     but WITHOUT ANY WARRANTY; without even the implied warranty of      */
/*     MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the       */
/*     GNU Lesser General Public License for more details.                 */
/*                                                                         */
/*     You should have received a copy of the GNU Lesser General Public    */
/*     License along with this program; see the file COPYING.  If not,     */
/*     please obtain a copy from http://www.gnu.org/copyleft/lesser.html   */
/*                                                                         */
/*-------------------------------------------------------------------------*/
/*                                                                         */
/*     Please submit patches to http://www.openslp.org                     */
/*                                                                         */
/***************************************************************************/

#include "slpd.h"

/*=========================================================================*/
SLPDAEntry* G_KnownDAListHead = 0;                                         
/* The list of DAs known to slpd.                                          */
/*=========================================================================*/

/*=========================================================================*/
int SLPDKnownDAInit()
/* Initializes the KnownDA list.  Removes all entries and adds entries     */
/* that are statically configured.                                         */
/*                                                                         */
/* returns  zero on success, Non-zero on failure                           */
/*=========================================================================*/
{
    return 0;    
}


/*=========================================================================*/
SLPDAEntry* SLPDKnownDAAddition(struct in_addr* addr,
                                unsigned long bootstamp,
                                const char* scopelist,
                                int scopelistlen)
/* Adds a DA to the known DA list.  If DA already exists, entry is updated */
/*                                                                         */
/* addr     (IN) pointer to in_addr of the DA to add                       */
/*                                                                         */
/* scopelist (IN) scope list of the DA to add                              */
/*                                                                         */
/* scopelistlen (IN) the length of the scope list                          */
/*                                                                         */
/* returns  Pointer to the added or updated                                */
/*=========================================================================*/
{
    SLPDAEntry* entry;

    /* Iterate through the list looking for an identical entry */
    entry = G_KnownDAListHead;
    while(entry)
    {
        /* for now assume entries are the same if in_addrs match */
        if (memcmp(&entry->daaddr,addr,sizeof(struct in_addr)) == 0)
        {
            /* Update an existing entry */
            if(entry->bootstamp < bootstamp)
            {
                entry->bootstamp = bootstamp;
            }
            else
            {
                /* set bootstamp to zero so that slpd will re-register with */
                /* this DA                                                  */
                bootstamp = 0;
            }
            entry->scopelist = realloc(entry->scopelist,scopelistlen);
            if(entry->scopelist)
            {
                memcpy(entry->scopelist,scopelist,scopelistlen);
                entry->scopelistlen = scopelistlen;
            }
            else
            {
                free(entry);
                entry = 0;
            }

            return entry;
        }
    }

    /* Create and link in a new entry */    
    bootstamp = 0;  /* make sure we re-register with new entries */
    entry = SLPDAEntryCreate(addr,bootstamp,scopelist,scopelistlen);
    ListLink((PListItem*)&G_KnownDAListHead,(PListItem)entry);
    
    return entry;
}
