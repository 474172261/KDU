/*******************************************************************************
*
*  (C) COPYRIGHT AUTHORS, 2020 - 2022
*
*  TITLE:       SUP.H
*
*  VERSION:     1.20
*
*  DATE:        10 Feb 2022
*
*  Support routines header file.
*
* THIS CODE AND INFORMATION IS PROVIDED "AS IS" WITHOUT WARRANTY OF
* ANY KIND, EITHER EXPRESSED OR IMPLIED, INCLUDING BUT NOT LIMITED
* TO THE IMPLIED WARRANTIES OF MERCHANTABILITY AND/OR FITNESS FOR A
* PARTICULAR PURPOSE.
*
*******************************************************************************/
#pragma once

//#define VERBOSE_FUNCTION_LOG

typedef NTSTATUS(NTAPI* PENUMOBJECTSCALLBACK)(POBJECT_DIRECTORY_INFORMATION Entry, PVOID CallbackParam);

#define USER_TO_KERNEL_HANDLE(Handle) { Handle += 0xffffffff80000000; }
#define NTQSI_MAX_BUFFER_LENGTH (512 * 1024 * 1024)

typedef BOOL(CALLBACK* pfnOpenProcessCallback)(
    _In_ HANDLE DeviceHandle,
    _In_ HANDLE ProcessId,
    _In_ ACCESS_MASK DesiredAccess,
    _Out_ PHANDLE ProcessHandle);

typedef BOOL(CALLBACK* pfnDuplicateHandleCallback)(
    _In_ HANDLE DeviceHandle,
    _In_ HANDLE SourceProcessId, //some drivers need pid not handle
    _In_ HANDLE SourceProcessHandle,
    _In_ HANDLE SourceHandle,
    _Out_ PHANDLE TargetHandle,
    _In_ ACCESS_MASK DesiredAccess,
    _In_ ULONG HandleAttributes,
    _In_ ULONG Options);

typedef NTSTATUS(NTAPI* PFN_NTQUERYROUTINE)(
    _In_ HANDLE ObjectHandle,
    _In_ DWORD InformationClass,
    _Out_writes_bytes_(ObjectInformationLength) PVOID ObjectInformation,
    _In_ ULONG ObjectInformationLength,
    _Out_opt_ PULONG ReturnLength);

typedef PVOID(CALLBACK* PNTSUPMEMALLOC)(
    _In_ SIZE_T NumberOfBytes);

typedef BOOL(CALLBACK* PNTSUPMEMFREE)(
    _In_ PVOID Memory);

typedef NTSTATUS(CALLBACK* pfnLoadDriverCallback)(
    _In_ PUNICODE_STRING RegistryPath,
    _In_opt_ PVOID Param
    );

typedef struct _OBJSCANPARAM {
    PCWSTR Buffer;
    ULONG BufferSize;
} OBJSCANPARAM, * POBJSCANPARAM;

#ifdef VERBOSE_FUNCTION_LOG
#define FUNCTION_ENTER_MSG(lpFunctionName) printf_s("[>] Entering %s\r\n", lpFunctionName)
#define FUNCTION_LEAVE_MSG(lpFunctionName) printf_s("[<] Leaving %s\r\n", lpFunctionName)
#else
#define FUNCTION_ENTER_MSG(lpFunctionName) 
#define FUNCTION_LEAVE_MSG(lpFunctionName)
#endif

typedef enum _KDU_EVENT_TYPE {
    kduEventNone = 0,
    kduEventError,
    kduEventInformation,
    kduEventMax
} KDU_EVENT_TYPE, * PKDU_EVENT_TYPE;

PVOID FORCEINLINE supHeapAlloc(
    _In_ SIZE_T Size);

BOOL FORCEINLINE supHeapFree(
    _In_ PVOID Memory);

PVOID supMapPhysicalMemory(
    _In_ HANDLE SectionHandle,
    _In_ ULONG_PTR PhysicalAddress,
    _In_ ULONG NumberOfBytes,
    _In_ BOOL MapForWrite);

VOID supUnmapPhysicalMemory(
    _In_ PVOID BaseAddress);

BOOL WINAPI supReadWritePhysicalMemory(
    _In_ HANDLE SectionHandle,
    _In_ ULONG_PTR PhysicalAddress,
    _In_reads_bytes_(NumberOfBytes) PVOID Buffer,
    _In_ ULONG NumberOfBytes,
    _In_ BOOLEAN DoWrite);

BOOL WINAPI supOpenPhysicalMemory(
    _In_ HANDLE DeviceHandle,
    _In_ pfnOpenProcessCallback OpenProcessCallback,
    _In_ pfnDuplicateHandleCallback DuplicateHandleCallback,
    _Out_ PHANDLE PhysicalMemoryHandle);

BOOL supCallDriver(
    _In_ HANDLE DeviceHandle,
    _In_ ULONG IoControlCode,
    _In_ PVOID InputBuffer,
    _In_ ULONG InputBufferLength,
    _In_opt_ PVOID OutputBuffer,
    _In_opt_ ULONG OutputBufferLength);

NTSTATUS supEnablePrivilege(
    _In_ DWORD Privilege,
    _In_ BOOL Enable);

NTSTATUS supLoadDriverEx(
    _In_ LPCWSTR DriverName,
    _In_ LPCWSTR DriverPath,
    _In_ BOOLEAN UnloadPreviousInstance,
    _In_opt_ pfnLoadDriverCallback Callback,
    _In_opt_ PVOID CallbackParam);

NTSTATUS supLoadDriver(
    _In_ LPCWSTR DriverName,
    _In_ LPCWSTR DriverPath,
    _In_ BOOLEAN UnloadPreviousInstance);

NTSTATUS supUnloadDriver(
    _In_ LPCWSTR DriverName,
    _In_ BOOLEAN fRemove);

NTSTATUS supOpenDriverEx(
    _In_ LPCWSTR DriverName,
    _In_ ACCESS_MASK DesiredAccess,
    _Out_opt_ PHANDLE DeviceHandle);

NTSTATUS supOpenDriver(
    _In_ LPCWSTR DriverName,
    _In_ ACCESS_MASK DesiredAccess,
    _Out_ PHANDLE DeviceHandle);

PVOID supGetLoadedModulesList(
    _In_ BOOL ExtendedOutput,
    _Out_opt_ PULONG ReturnLength);

PVOID supGetSystemInfo(
    _In_ SYSTEM_INFORMATION_CLASS SystemInformationClass);

ULONG_PTR supGetNtOsBase(
    VOID);

PBYTE supQueryResourceData(
    _In_ ULONG_PTR ResourceId,
    _In_ PVOID DllHandle,
    _In_ PULONG DataSize);

PBYTE supReadFileToBuffer(
    _In_ LPWSTR lpFileName,
    _Inout_opt_ LPDWORD lpBufferSize);

SIZE_T supWriteBufferToFile(
    _In_ PCWSTR lpFileName,
    _In_ PVOID Buffer,
    _In_ SIZE_T Size,
    _In_ BOOL Flush,
    _In_ BOOL Append,
    _Out_opt_ NTSTATUS* Result);

ULONG_PTR supGetProcAddress(
    _In_ ULONG_PTR KernelBase,
    _In_ ULONG_PTR KernelImage,
    _In_ LPCSTR FunctionName);

VOID supResolveKernelImport(
    _In_ ULONG_PTR Image,
    _In_ ULONG_PTR KernelImage,
    _In_ ULONG_PTR KernelBase);

BOOLEAN supIsObjectExists(
    _In_ LPCWSTR RootDirectory,
    _In_ LPCWSTR ObjectName);

BOOL supQueryObjectFromHandle(
    _In_ HANDLE hOject,
    _Out_ ULONG_PTR* Address);

BOOL supGetCommandLineOption(
    _In_ LPCTSTR OptionName,
    _In_ BOOL IsParametric,
    _Inout_opt_ LPTSTR OptionValue,
    _In_ ULONG ValueSize,
    _Out_opt_ PULONG ParamLength);

BOOLEAN supQueryHVCIState(
    _Out_ PBOOLEAN pbHVCIEnabled,
    _Out_ PBOOLEAN pbHVCIStrictMode,
    _Out_ PBOOLEAN pbHVCIIUMEnabled);

DWORD supExpandEnvironmentStrings(
    _In_ LPCWSTR lpSrc,
    _Out_writes_to_opt_(nSize, return) LPWSTR lpDst,
    _In_ DWORD nSize);

BOOLEAN supQuerySecureBootState(
    _Out_ PBOOLEAN pbSecureBoot);

NTSTATUS supGetFirmwareType(
    _Out_ PFIRMWARE_TYPE FirmwareType);

ULONG_PTR supQueryMaximumUserModeAddress();

DWORD supCalculateCheckSumForMappedFile(
    _In_ PVOID BaseAddress,
    _In_ ULONG FileLength);

BOOLEAN supVerifyMappedImageMatchesChecksum(
    _In_ PVOID BaseAddress,
    _In_ ULONG FileLength,
    _Out_opt_ PULONG HeaderChecksum,
    _Out_opt_ PULONG CalculatedChecksum);

BOOL supReplaceDllEntryPoint(
    _In_ PVOID DllImage,
    _In_ ULONG SizeOfDllImage,
    _In_ LPCSTR lpEntryPointName,
    _In_ BOOL fConvertToExe);

ULONG_PTR supGetPML4FromLowStub1M(
    _In_ ULONG_PTR pbLowStub1M);

NTSTATUS supCreateSystemAdminAccessSD(
    _Out_ PSECURITY_DESCRIPTOR * SecurityDescriptor,
    _Out_ PACL * DefaultAcl);

ULONG supGetTimeAsSecondsSince1970();

ULONG_PTR supGetModuleBaseByName(
    _In_ LPCWSTR ModuleName,
    _Out_opt_ PULONG ImageSize);

BOOL supManageDummyDll(
    _In_ LPCWSTR lpDllName,
    _In_ BOOLEAN fRemove);

ULONG supSelectNonPagedPoolTag(
    VOID);

NTSTATUS supRegWriteValueString(
    _In_ HANDLE RegistryHandle,
    _In_ LPCWSTR ValueName,
    _In_ LPCWSTR ValueData);

NTSTATUS supLoadFileForMapping(
    _In_ LPCWSTR PayloadFileName,
    _Out_ PVOID * LoadBase);

VOID supPrintfEvent(
    _In_ KDU_EVENT_TYPE Event,
    _Printf_format_string_ LPCSTR Format,
    ...);

NTSTATUS supQueryImageSize(
    _In_ PVOID ImageBase,
    _Out_ PSIZE_T ImageSize);

NTSTATUS supConvertToAnsi(
    _In_ LPCWSTR UnicodeString,
    _Inout_ PANSI_STRING AnsiString);

NTSTATUS supQueryObjectInformation(
    _In_ HANDLE ObjectHandle,
    _In_ OBJECT_INFORMATION_CLASS ObjectInformationClass,
    _Out_ PVOID * Buffer,
    _Out_opt_ PULONG ReturnLength,
    _In_ PNTSUPMEMALLOC AllocMem,
    _In_ PNTSUPMEMFREE FreeMem);

VOID supGenerateSharedObjectName(
    _In_ WORD ObjectId,
    _Inout_ LPWSTR lpBuffer);

BOOL supSetupInstallDriverFromInf(
    _In_ LPCWSTR InfName,
    _In_ BYTE * HardwareId,
    _In_ ULONG HardwareIdLength,
    _Out_ HDEVINFO * DeviceInfo,
    _Inout_ SP_DEVINFO_DATA * DeviceInfoData);

BOOL supSetupRemoveDriver(
    _In_ HDEVINFO DeviceInfo,
    _In_ SP_DEVINFO_DATA * DeviceInfoData);

BOOL supExtractFileFromDB(
    _In_ HMODULE ImageBase,
    _In_ LPCWSTR FileName,
    _In_ ULONG FileId);

VOID supExtractFileToTemp(
    _In_opt_ HMODULE ImageBase,
    _In_opt_ ULONG FileResourceId,
    _In_ LPCWSTR lpTempPath,
    _In_ LPCWSTR lpFileName,
    _In_ BOOL fDelete);

BOOL supDeleteFileWithWait(
    _In_ ULONG WaitMilliseconds,
    _In_ ULONG NumberOfAttempts,
    _In_ LPCWSTR lpFileName);

PVOID supMapFileAsImage(
    _In_ LPWSTR lpImagePath);

PVOID supGetEntryPointForMappedFile(
    _In_ PVOID ImageBase);

NTSTATUS supInjectPayload(
    _In_ PVOID pvTargetImage,
    _In_ PVOID pvShellCode,
    _In_ ULONG cbShellCode,
    _In_ LPWSTR lpTargetModule,
    _Out_ PHANDLE phZombieProcess);
