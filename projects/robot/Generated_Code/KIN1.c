/* ###################################################################
**     This component module is generated by Processor Expert. Do not modify it.
**     Filename    : KIN1.c
**     Project     : robot
**     Processor   : MK22FX512VLK12
**     Component   : KinetisTools
**     Version     : Component 01.032, Driver 01.00, CPU db: 3.00.000
**     Repository  : My Components
**     Compiler    : GNU C Compiler
**     Date/Time   : 2016-10-20, 15:41, # CodeGen: 1
**     Abstract    :
**
**     Settings    :
**          Component name                                 : KIN1
**          Utility                                        : UTIL1
**          SDK                                            : KSDK1
**          Shell                                          : Disabled
**     Contents    :
**         SoftwareReset          - void KIN1_SoftwareReset(void);
**         UIDGet                 - uint8_t KIN1_UIDGet(KIN1_UID *uid);
**         UIDSame                - bool KIN1_UIDSame(const KIN1_UID *src, const KIN1_UID *dst);
**         UIDtoString            - uint8_t KIN1_UIDtoString(const KIN1_UID *uid, uint8_t *buf, size_t bufSize);
**         GetKinetisFamilyString - KIN1_ConstCharPtr KIN1_GetKinetisFamilyString(void);
**         GetPC                  - void* KIN1_GetPC(void);
**         GetSP                  - void* KIN1_GetSP(void);
**         SetPSP                 - void KIN1_SetPSP(void *setval);
**         SetLR                  - void KIN1_SetLR(uint32_t setval);
**
**     * Copyright (c) 2014-2016, Erich Styger
**      * Web:         https://mcuoneclipse.com
**      * SourceForge: https://sourceforge.net/projects/mcuoneclipse
**      * Git:         https://github.com/ErichStyger/McuOnEclipse_PEx
**      * All rights reserved.
**      *
**      * Redistribution and use in source and binary forms, with or without modification,
**      * are permitted provided that the following conditions are met:
**      *
**      * - Redistributions of source code must retain the above copyright notice, this list
**      *   of conditions and the following disclaimer.
**      *
**      * - Redistributions in binary form must reproduce the above copyright notice, this
**      *   list of conditions and the following disclaimer in the documentation and/or
**      *   other materials provided with the distribution.
**      *
**      * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND
**      * ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED
**      * WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
**      * DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE FOR
**      * ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES
**      * (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES;
**      * LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON
**      * ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
**      * (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS
**      * SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
** ###################################################################*/
/*!
** @file KIN1.c
** @version 01.00
** @brief
**
*/         
/*!
**  @addtogroup KIN1_module KIN1 module documentation
**  @{
*/         

/* MODULE KIN1. */

#include "KIN1.h"
#include <stddef.h> /* for size_t */
#if KSDK1_SDK_VERSION_USED == KSDK1_SDK_VERSION_2_0
  #include "fsl_sim.h" /* system integration module */
#endif

#if KIN1_CPU_IS_M4
static const unsigned char *KinetisM4FamilyStrings[] =
{ /* FAMID (3 bits) are used as index */
  (const unsigned char *)"K10 or K12 Family",          /* 000 */
  (const unsigned char *)"K20 or K22 Family",          /* 001 */
  (const unsigned char *)"K30, K11 or K61 Family",     /* 010 */
  (const unsigned char *)"K40 or K21 Family",          /* 011 */
  (const unsigned char *)"K60 or K62 Family",          /* 100 */
  (const unsigned char *)"K70 Family",                 /* 101 */
  (const unsigned char *)"Reserved",                   /* 110 */
  (const unsigned char *)"Reserved"                    /* 111 */
};
#endif
#if KIN1_CPU_IS_M0_PLUS
static const unsigned char *KinetisM0FamilyStrings[] =
{ /* FAMID (3 bits) are used as index */
  (const unsigned char *)"KL0x",          /* 000 */
  (const unsigned char *)"KL1x",          /* 001 */
  (const unsigned char *)"KL2x",          /* 010 */
  (const unsigned char *)"KL3x",          /* 011 */
  (const unsigned char *)"KL4x",          /* 100 */
  (const unsigned char *)"Reserved",      /* 101 */
  (const unsigned char *)"Reserved",      /* 110 */
  (const unsigned char *)"Reserved"       /* 111 */
};
#endif


/*
** ===================================================================
**     Method      :  KIN1_SoftwareReset (component KinetisTools)
**     Description :
**         Performs a reset of the device
**     Parameters  : None
**     Returns     : Nothing
** ===================================================================
*/
void KIN1_SoftwareReset(void)
{
  /* Generic way to request a reset from software for ARM Cortex */
  /* See https://community.freescale.com/thread/99740
     To write to this register, you must write 0x5FA to the VECTKEY field, otherwise the processor ignores the write.
     SYSRESETREQ will cause a system reset asynchronously, so need to wait afterwards.
   */
#if KIN1_IS_USING_KINETIS_SDK
  SCB->AIRCR = (0x5FA<<SCB_AIRCR_VECTKEY_Pos)|SCB_AIRCR_SYSRESETREQ_Msk;
#else
  SCB_AIRCR = SCB_AIRCR_VECTKEY(0x5FA) | SCB_AIRCR_SYSRESETREQ_MASK;
#endif
  for(;;) {
    /* wait until reset */
  }
}

/*
** ===================================================================
**     Method      :  KIN1_UIDGet (component KinetisTools)
**     Description :
**         Return the 128bit UID of the device
**     Parameters  :
**         NAME            - DESCRIPTION
**       * uid             - Pointer to 
**     Returns     :
**         ---             - Error code
** ===================================================================
*/
/*!
 * \brief Return the 80bit UID of the device
 * \param uid Pointer to the buffer where to store the UID
 * \return Error code, ERR_OK if everything is ok.
 */
uint8_t KIN1_UIDGet(KIN1_UID *uid)
{
#if KSDK1_SDK_VERSION_USED == KSDK1_SDK_VERSION_2_0
  sim_uid_t tmp;
  int i, j;

  SIM_GetUniqueId(&tmp);
  /* init */
  for(i=0;i<sizeof(KIN1_UID);i++) {
    uid->id[i] = 0;
  }
  if (sizeof(sim_uid_t)>sizeof(KIN1_UID)) {
    return ERR_OVERFLOW;
  }
  /* copy into our own structure, data is right justified */
  for(i=0,j=sizeof(KIN1_UID)-sizeof(sim_uid_t);i<sizeof(sim_uid_t)&&i<sizeof(KIN1_UID);i++,j++) {
    uid->id[j] = ((uint8_t*)&tmp)[i];
  }
#else
#ifdef SIM_UIDMH /* 80 or 128 bit UUID: SIM_UIDMH, SIM_UIDML and SIM_UIDL */
#ifdef SIM_UIDH
  uid->id[0] = (SIM_UIDH>>24)&0xff;
  uid->id[1] = (SIM_UIDH>>16)&0xff;
  uid->id[2] = (SIM_UIDH>>8)&0xff;
  uid->id[3] = SIM_UIDH&0xff;
#else
  uid->id[0] = 0;
  uid->id[1] = 0;
  uid->id[2] = 0;
  uid->id[3] = 0;
#endif
  uid->id[4] = (SIM_UIDML>>24)&0xff;
  uid->id[5] = (SIM_UIDML>>16)&0xff;
  uid->id[6] = (SIM_UIDMH>>8)&0xff;
  uid->id[7] = SIM_UIDMH&0xff;
  uid->id[8] = (SIM_UIDML>>24)&0xff;
  uid->id[9] = (SIM_UIDML>>16)&0xff;
  uid->id[10] = (SIM_UIDML>>8)&0xff;
  uid->id[11] = SIM_UIDML&0xff;
  uid->id[12] = (SIM_UIDL>>24)&0xff;
  uid->id[13] = (SIM_UIDL>>16)&0xff;
  uid->id[14] = (SIM_UIDL>>8)&0xff;
  uid->id[15] = SIM_UIDL&0xff;
#elif defined(SIM_UUIDMH) /* KE06Z: SIM_UUIDMH, SIM_UUIDML and SIM_UUIDL */
  uid->id[0] = 0;
  uid->id[1] = 0;
  uid->id[2] = 0;
  uid->id[3] = 0;
  uid->id[4] = 0;
  uid->id[5] = 0;
  uid->id[6] = (SIM_UUIDMH>>8)&0xff;
  uid->id[7] = SIM_UUIDMH&0xff;
  uid->id[8] = (SIM_UUIDML>>24)&0xff;
  uid->id[9] = (SIM_UUIDML>>16)&0xff;
  uid->id[10] = (SIM_UUIDML>>8)&0xff;
  uid->id[11] = SIM_UUIDML&0xff;
  uid->id[12] = (SIM_UUIDL>>24)&0xff;
  uid->id[13] = (SIM_UUIDL>>16)&0xff;
  uid->id[14] = (SIM_UUIDL>>8)&0xff;
  uid->id[15] = SIM_UUIDL&0xff;
#else /* some devices like the KE02Z only have 64bit UUID: only SIM_UUIDH and SIM_UUIDL */
  uid->id[0] = 0;
  uid->id[1] = 0;
  uid->id[2] = 0;
  uid->id[3] = 0;
  uid->id[4] = 0;
  uid->id[5] = 0;
  uid->id[6] = 0;
  uid->id[7] = 0;
  uid->id[8] = (SIM_UUIDH>>24)&0xff;
  uid->id[9] = (SIM_UUIDH>>16)&0xff;
  uid->id[10] = (SIM_UUIDH>>8)&0xff;
  uid->id[11] = SIM_UUIDH&0xff;
  uid->id[12] = (SIM_UUIDL>>24)&0xff;
  uid->id[13] = (SIM_UUIDL>>16)&0xff;
  uid->id[14] = (SIM_UUIDL>>8)&0xff;
  uid->id[15] = SIM_UUIDL&0xff;
#endif
#endif /* SDK V2.0 */
  return ERR_OK;
}

/*
** ===================================================================
**     Method      :  KIN1_UIDSame (component KinetisTools)
**     Description :
**         Compares two UID
**     Parameters  :
**         NAME            - DESCRIPTION
**       * src             - Pointer to 
**         Variable_1      - 
**     Returns     :
**         ---             - TRUE if the same, FALSE otherwise
** ===================================================================
*/
/*!
 * \brief Compares two UID
 * \param src One UID
 * \param dst The other UID
 * \return TRUE if the two UID's are the same
 */
bool KIN1_UIDSame(const KIN1_UID *src, const KIN1_UID *dst)
{
  unsigned int i;

  for(i=0; i<sizeof(KIN1_UID); i++) {
    if (src->id[i]!=dst->id[i]) {
      return FALSE; /* no match */
    }
  }
  return TRUE;
}

/*
** ===================================================================
**     Method      :  KIN1_UIDtoString (component KinetisTools)
**     Description :
**         Returns the value of the UID as string
**     Parameters  :
**         NAME            - DESCRIPTION
**         uid             - 
**       * buf             - Pointer to 
**         bufSize         - 
**     Returns     :
**         ---             - Error code
** ===================================================================
*/
/*!
 * \brief Transforms the 80bit UID into a string
 * \param id Pointer to the buffer where to store the string
 * \param bufSize Size of buffer in bytes
 * \return Error code, ERR_OK if everything is ok.
 */
uint8_t KIN1_UIDtoString(const KIN1_UID *uid, uint8_t *buf, size_t bufSize)
{
  unsigned int i;

  UTIL1_strcpy(buf, bufSize, (unsigned char*)"{");
  for(i=0;i<sizeof(KIN1_UID);i++) {
    UTIL1_strcat(buf, bufSize, (unsigned char*)"0x");
    UTIL1_strcatNum8Hex(buf, bufSize, uid->id[i]);
    if (i<sizeof(KIN1_UID)-1) {
      UTIL1_strcat(buf, bufSize, (unsigned char*)",");
    }
  }
  UTIL1_strcat(buf, bufSize, (unsigned char*)"}");
  return ERR_OK;
}

/*
** ===================================================================
**     Method      :  KIN1_GetKinetisFamilyString (component KinetisTools)
**     Description :
**         Determines the Kinetis Familiy based on SIM_SDID register
**     Parameters  : None
**     Returns     :
**         ---             - String describing the Kinetis Family
** ===================================================================
*/
KIN1_ConstCharPtr KIN1_GetKinetisFamilyString(void)
{
#if KIN1_CPU_IS_M0_PLUS
#ifdef SIM_SDID /* normal Kinetis define this */
  int32_t val;

  val = (SIM_SDID>>28)&0x3; /* bits 30..28 */
  return KinetisM0FamilyStrings[val];
#elif defined(SIM_SRSID_FAMID) /* MKE02Z4 defines this, hopefully all other KE too... */
  return "KE0x Family"; /* 0000 only KE0x supported */
#else
  #error "Unknown architecture!"
  return (KIN1_ConstCharPtr)"ERROR";
#endif
#elif KIN1_CPU_IS_M4
  int32_t val;

  val = (SIM_SDID>>4)&0x3; /* bits 6..4 */
  return KinetisM4FamilyStrings[val];
#elif KIN1_CPU_IS_M7
  return (KIN1_ConstCharPtr)"Cortex-M7";
#else
  #error "Unknown architecture!"
  return (KIN1_ConstCharPtr)"ERROR";
#endif
}

/*
** ===================================================================
**     Method      :  KIN1_GetPC (component KinetisTools)
**     Description :
**         returns the program counter
**     Parameters  : None
**     Returns     :
**         ---             - program counter
** ===================================================================
*/
void* KIN1_GetPC(void)
{
#ifdef __GNUC__
  void *pc;

  __asm__ __volatile__ ("mov %0, pc" : "=r"(pc));
  return pc;
#else
  #warning "only for GCC"
  return NULL;
#endif
}

/*
** ===================================================================
**     Method      :  KIN1_GetSP (component KinetisTools)
**     Description :
**         returns the stack pointer
**     Parameters  : None
**     Returns     :
**         ---             - stack pointer
** ===================================================================
*/
void* KIN1_GetSP(void)
{
#ifdef __GNUC__
  void *sp;

  __asm__ __volatile__ ("mrs %0, msp" : "=r"(sp));
  return sp;
#else
  #warning "only for GCC"
  return NULL;
#endif
}

/*
** ===================================================================
**     Method      :  KIN1_SetPSP (component KinetisTools)
**     Description :
**         sets the process stack pointer
**     Parameters  :
**         NAME            - DESCRIPTION
**         setval          - new PSP value
**     Returns     : Nothing
** ===================================================================
*/
void KIN1_SetPSP(void *setval)
{
#ifdef __GNUC__
  __asm__ volatile ("msr psp, %[value]\n\t""dmb\n\t""dsb\n\t""isb\n\t"::[value]"r"(setval):);
  __asm__ volatile ("" ::: "memory");
#else
  #warning "only for GCC"
#endif
}

/*
** ===================================================================
**     Method      :  KIN1_SetLR (component KinetisTools)
**     Description :
**         Sets the link register
**     Parameters  :
**         NAME            - DESCRIPTION
**         setval          - new LR value
**     Returns     : Nothing
** ===================================================================
*/
void KIN1_SetLR(uint32_t setval)
{
#ifdef __GNUC__
  __asm__ volatile ("mov lr, %[value]\n\t"::[value]"r"(setval):);
  __asm__ volatile ("" ::: "memory");
#else
  #warning "only for GCC"
#endif
}

/* END KIN1. */

/*!
** @}
*/
/*
** ###################################################################
**
**     This file was created by Processor Expert 10.5 [05.21]
**     for the Freescale Kinetis series of microcontrollers.
**
** ###################################################################
*/
