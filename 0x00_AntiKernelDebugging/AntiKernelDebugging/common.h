#pragma once
#include <ntifs.h>

/*//////////////////////////////////////////////
# File : common.h
# Desc : 모든 함수와 구조체, 전역변수 등으 선언
*///////////////////////////////////////////////


#define PROCESS_TERMINATE       0x0001
#define PROCESS_VM_OPERATION    0x0008
#define PROCESS_VM_READ         0x0010
#define PROCESS_VM_WRITE        0x0020

//============================================//
//======= DriverEntry & Unload Routine =======//
//============================================//

NTSTATUS DriverEntry(IN PDRIVER_OBJECT pDriver, IN PUNICODE_STRING pRegPath);
VOID UnloadDriver(IN PDRIVER_OBJECT pDriver);

//============================================//
//============= Callback Routine =============//
//============================================//

VOID LoadImageNotifyRoutine(IN PUNICODE_STRING FullImageName, IN HANDLE ProcessId, IN PIMAGE_INFO ImageInfo);
OB_PREOP_CALLBACK_STATUS PreCallback(PVOID RegistrationContext, POB_PRE_OPERATION_INFORMATION pOperationInformation);
VOID PostCallback(PVOID RegistrationContext, POB_POST_OPERATION_INFORMATION pOperationInformation);

//============================================//
//========== User-defined Function  ==========//
//============================================//

VOID TerminateProcess(IN HANDLE pid);
NTSTATUS ObCallbackReg();
BOOLEAN GetOffset(PEPROCESS Process);
BOOLEAN GetPebOffset();


//============================================//
//=========== Undocumented API ===============//
//============================================//

typedef NTSTATUS(*NtQueryInformationProcess_t)(
	IN    HANDLE              ProcessHandle,
	IN    PROCESSINFOCLASS    ProcessInformationClass,
	OUT   PVOID               ProcessInformation,
	IN    ULONG               ProcessInformationLength,
	OUT   PULONG              ReturnLength
	);

typedef PVOID(*PsGetProcessDebugPort_t)(
	IN	PEPROCESS Process
	);

//============================================//
//======= Structure & Global Variable ========//
//============================================//

typedef struct _IMPORT_OFFSET
{
	int			UniqueProcessid_off;
	int			ActiveProcessLinks_off;
	int			ImageFileName_off;
	int			PEB_off;
}IMPORT_OFFSET;

PVOID hRegistration = NULL;	// ObUnRegisterCallbacks 전용
IMPORT_OFFSET iOffset;
PsGetProcessDebugPort_t PsGetProcessDebugPort;
BOOLEAN bOnOff = FALSE;
const char szSystem[] = "System";
const wchar_t szNtQueryInformationProcess[] = L"NtQueryInformationProcess";
const char szTarget[] = "notepad.exe";