/*******************************************************************************
*
*  (C) COPYRIGHT AUTHORS, 2020 - 2022
*
*  TITLE:       CONSTS.H
*
*  VERSION:     1.20
*
*  DATE:        15 Feb 2022
*
*  Global consts.
*
* THIS CODE AND INFORMATION IS PROVIDED "AS IS" WITHOUT WARRANTY OF
* ANY KIND, EITHER EXPRESSED OR IMPLIED, INCLUDING BUT NOT LIMITED
* TO THE IMPLIED WARRANTIES OF MERCHANTABILITY AND/OR FITNESS FOR A
* PARTICULAR PURPOSE.
*
*******************************************************************************/

#pragma once

#define KDU_VERSION_MAJOR       1
#define KDU_VERSION_MINOR       2
#define KDU_VERSION_REVISION    0
#define KDU_VERSION_BUILD       2202

#define KDU_BASE_ID             0xff123456
#define KDU_SYNC_MUTANT         0xabcd

#define NT_REG_PREP             L"\\Registry\\Machine"
#define DRIVER_REGKEY           L"%wS\\System\\CurrentControlSet\\Services\\%wS"

#define PROCEXP152              L"PROCEXP152"
#define PROCEXP_DESC            L"Process Explorer"

#define NTOSKRNL_EXE            L"ntoskrnl.exe"
#define CI_DLL                  L"CI.dll"

#define DRV64DLL                L"drv64.dll"
#define DUMMYDLL                L"SB_SMBUS_SDK.dll"

#define WINIO_BASE_DESC         "WinIo by Yariv Kaplan"
#define WINRING0_BASE_DESC      "WinRing0 by Noriyuki Miyazaki"
#define MAPMEM_BASE_DESC        "MapMem from NTDDK 3.51"
#define PHYMEM_BASE_DESC        "PhyMem by akui"

#define SHELL_POOL_TAG          '  oI'

#define PROVIDER_RES_KEY        ' owo' // Giving you enough uwu's.

#define SYSTEM_PID_MAGIC             4
