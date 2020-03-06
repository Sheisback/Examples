#pragma once
#include "offset.h"

/*//////////////////////////////////////////////
# File : callbacks.h
# Desc : 콜백 루틴에 대한 정의와 관련 함수 정의
*///////////////////////////////////////////////


/*
# Name  : LoadImageNotifyRoutine
# Param : PUNICODE_STRING, HANDLE, PIMAGE_INFO
# Desc  : KdDebuggerEnabled 와 KdDebuggerNotPresent 커널 전역 변수를 활용
*/
VOID LoadImageNotifyRoutine(
	IN PUNICODE_STRING FullImageName, 
	IN HANDLE ProcessId, 
	IN PIMAGE_INFO ImageInfo)
{
	PEPROCESS *Process = NULL;
	char szProcName[16] = { 0, };


	if (!ImageInfo->SystemModeImage)
	{
		if (PsLookupProcessByProcessId(ProcessId, &Process) == STATUS_SUCCESS)
		{
			strcpy_s(szProcName, 16, (PVOID*)((PCHAR)Process + iOffset.ImageFileName_off));
			if (!_strnicmp(szProcName, szTarget, 16))
			{
				if (*KdDebuggerNotPresent==FALSE)
				{
					DbgPrintEx(DPFLTR_ACPI_ID, 1, "[WARN] Debugger Present\n");
				}
				else
				{
					if (*KdDebuggerEnabled)
					{
						DbgPrintEx(DPFLTR_ACPI_ID, 1, "[WARN] Kernel Debugger Enabled \n");
					}
				}
			}
		}
	}
}

/*
# Name  : PreCallback
# Param : PVOID, POB_PRE_OPERATION_INFORMATION
# Desc  : PsGetProcessDebugPort 를 이용하여 유저모드 디버깅 방지
*/
OB_PREOP_CALLBACK_STATUS PreCallback(
	PVOID RegistrationContext, 
	POB_PRE_OPERATION_INFORMATION pOperationInformation
)
{
	UNREFERENCED_PARAMETER(RegistrationContext);

	char szProcName[16] = { 0, };
	strcpy_s(szProcName, 16, ((DWORD64)pOperationInformation->Object + iOffset.ImageFileName_off));
	if (!_strnicmp(szProcName, szTarget, 16))
	{
		if (PsGetProcessDebugPort(pOperationInformation->Object))
		{
			if (!bOnOff)
			{
				bOnOff = TRUE;
				TerminateProcess(PsGetProcessId(pOperationInformation->Object));
				bOnOff = FALSE;
			}
		}
	}
}

/*
# Name  : PostCallback
# Param : PVOID, POB_POST_OPERATION_INFORMATION
# Desc  : 사용하지 않을 수 있음
*/
VOID PostCallback(PVOID RegistrationContext, POB_POST_OPERATION_INFORMATION pOperationInformation)
{
	UNREFERENCED_PARAMETER(RegistrationContext);

	// your code

}

/*
# Name  : TerminateProcess
# Param : HANDLE
# Desc  : 프로세스 강제 종료 시 사용
*/
VOID TerminateProcess(IN HANDLE pid)
{
	HANDLE hProcess = NULL;
	OBJECT_ATTRIBUTES obAttr = { 0, };
	CLIENT_ID cid = { 0, };

	obAttr.Length = sizeof(obAttr);
	obAttr.Attributes = OBJ_CASE_INSENSITIVE | OBJ_KERNEL_HANDLE;
	cid.UniqueProcess = pid;

	if (ZwOpenProcess(&hProcess, PROCESS_ALL_ACCESS, &obAttr, &cid) 
		== STATUS_SUCCESS)	// Get process handle
	{
		if (ZwTerminateProcess(hProcess, STATUS_ACCESS_DENIED) 
			== STATUS_SUCCESS)	// Terminate process
		{
			DbgPrintEx(DPFLTR_ACPI_ID, 3,
				"[INFO] Success terminate process\n");
		}
		else
		{
			DbgPrintEx(DPFLTR_ACPI_ID, 0,
				"[ERR] Failed terminate process\n");
		}
	}
	else
	{
		DbgPrintEx(DPFLTR_ACPI_ID, 0,
			"[ERR] Failed open process\n");
	}
}

/*
# Name  : ObCallbackReg
# Param : x
# Desc  : ObRegisterCallbacks 호출
*/
NTSTATUS ObCallbackReg()
{
	OB_CALLBACK_REGISTRATION obRegistration = { 0, };
	OB_OPERATION_REGISTRATION opRegistration = { 0, };

	obRegistration.Version = ObGetFilterVersion();	// Get version
	obRegistration.OperationRegistrationCount = 1;	// OB_OPERATION_REGISTRATION count, opRegistration[2] 인 경우 2
	RtlInitUnicodeString(&obRegistration.Altitude, L"300000");	// 임의의 Altitude 지정
	obRegistration.RegistrationContext = NULL;

	opRegistration.ObjectType = PsProcessType;
	opRegistration.Operations = OB_OPERATION_HANDLE_CREATE;	// Create 또는 Open 시 동작
	opRegistration.PreOperation = PreCallback;	// PreOperation 등록
	opRegistration.PostOperation = PostCallback;	// PostOperation 등록

	obRegistration.OperationRegistration = &opRegistration;	// OperationRegistration 등록

	return ObRegisterCallbacks(&obRegistration, &hRegistration);
}