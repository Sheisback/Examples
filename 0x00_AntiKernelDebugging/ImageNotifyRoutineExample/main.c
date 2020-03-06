#include "common.h"

/*
# Name  : TerminateProcess
# Param : HANDLE
# Desc  : PID로 프로세스 핸들을 얻은 후, 강제 프로세스 종료
*/
VOID TerminateProcess(IN HANDLE pid)
{
	HANDLE hProcess = NULL;
	OBJECT_ATTRIBUTES obAttr = { 0, };
	CLIENT_ID cid = { 0, };

	obAttr.Length = sizeof(obAttr);
	obAttr.Attributes = OBJ_CASE_INSENSITIVE | OBJ_KERNEL_HANDLE;
	cid.UniqueProcess = pid;

	if (ZwOpenProcess(&hProcess, PROCESS_ALL_ACCESS, &obAttr, &cid) == STATUS_SUCCESS)
	{
		if (ZwTerminateProcess(hProcess, STATUS_ACCESS_DENIED) == STATUS_SUCCESS)
		{
			DbgPrintEx(DPFLTR_ACPI_ID, DPFLTR_WARNING_LEVEL,
				"[INFO] Success terminate process\n");
		}
		else
		{
			DbgPrintEx(DPFLTR_ACPI_ID, DPFLTR_WARNING_LEVEL,
				"[ERROR] Failed terminate process\n");
		}
	}
	else
	{
		DbgPrintEx(DPFLTR_ACPI_ID, DPFLTR_WARNING_LEVEL,
			"[ERROR] Failed open process\n");
	}


}

/*
# Name  : LoadImageNotifyRoutine
# Param : PUNICODE_STRING, HANDLE, PIMAGE_INFO
# Desc  : 블랙 리스트에 등록 된 이미지가 로드 될 때 TerminateProcess 함수를 호출
*/
VOID LoadImageNotifyRoutine(IN PUNICODE_STRING FullImageName, IN HANDLE ProcessId, IN PIMAGE_INFO ImageInfo)
{
	if (!ImageInfo->SystemModeImage)
	{
		for (int i = 0; i < sizeof(szTarget) / sizeof(PVOID); i++)
		{
			if (wcsstr(FullImageName->Buffer, szTarget[i]))
			{
				DbgPrintEx(DPFLTR_ACPI_ID, DPFLTR_WARNING_LEVEL,
					"[WARN] Unauthorized Image Load : \n\t[%.4X] %wZ\n", ProcessId, FullImageName);

				TerminateProcess(ProcessId);
				
			}
		}

	}
}

/*
# Name  : DriverEntry
# Param : PDRIVER_OBJECT, PUNICODE_STRING
# Desc  : 드라이버 진입점
*/
NTSTATUS DriverEntry(IN PDRIVER_OBJECT pDriver, IN PUNICODE_STRING pRegPath)
{
	UNREFERENCED_PARAMETER(pRegPath);

	pDriver->DriverUnload = UnloadDriver;

	DbgPrintEx(DPFLTR_ACPI_ID, DPFLTR_INFO_LEVEL, "[INFO] Load Driver\n");

	if (PsSetLoadImageNotifyRoutine(&LoadImageNotifyRoutine) != STATUS_SUCCESS)
	{
		DbgPrintEx(DPFLTR_ACPI_ID, DPFLTR_ERROR_LEVEL, "[ERROR] Failed register\n");

	}
	else
	{
		DbgPrintEx(DPFLTR_ACPI_ID, DPFLTR_INFO_LEVEL, "[INFO] Success register\n");
	}

	return STATUS_SUCCESS;
}

/*
# Name  : UnloadDriver
# Param : PDRIVER_OBJECT
# Desc  : 드라이버 종료 루틴, 등록된 콜백 루틴을 해제
*/
VOID UnloadDriver(IN PDRIVER_OBJECT pDriver)
{
	UNREFERENCED_PARAMETER(pDriver);
	PsRemoveLoadImageNotifyRoutine(&LoadImageNotifyRoutine);
	DbgPrintEx(DPFLTR_ACPI_ID, DPFLTR_INFO_LEVEL, "[INFO] Unload Driver\n");

}
