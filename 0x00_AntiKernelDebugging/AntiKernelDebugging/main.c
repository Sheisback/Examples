#include "callbacks.h"

/*//////////////////////////////////////////////////////
# Scenario : ObRegisterCallbacks 와 PsSetLoadImageNotifyRoutine 을 이용한 안티 디버깅
	- ObRegisterCallbacks : PEB 를 이용해서 유저모드 디버깅 중인지 탐지
	- PsSetLoadImageNotifyRoutine : 커널 디버깅 중인지 확인(KdDebuggerEnabeld, KdDebuggerNotPresent 등..)
# File : main.c
# Desc : 드라이버 진입점과 종료 루틴, 사용자 정의 함수
*///////////////////////////////////////////////////////


NTSTATUS DriverEntry(IN PDRIVER_OBJECT pDriver, IN PUNICODE_STRING pRegPath)
{
	UNREFERENCED_PARAMETER(pRegPath);
	UNICODE_STRING PsGetProcessDebugPortString = { 0, };
	pDriver->DriverUnload = UnloadDriver;
	DbgPrintEx(DPFLTR_ACPI_ID, 3, "[INFO] Driver load success\n");

	if (GetOffset(PsGetCurrentProcess()))
	{
		RtlCreateUnicodeString(&PsGetProcessDebugPortString, L"PsGetProcessDebugPort");
		PsGetProcessDebugPort = (PsGetProcessDebugPort_t)MmGetSystemRoutineAddress(&PsGetProcessDebugPortString);

		if (ObCallbackReg() == STATUS_SUCCESS)
		{
			PsSetLoadImageNotifyRoutine(&LoadImageNotifyRoutine);
		}
	}
	return STATUS_SUCCESS;
}

VOID UnloadDriver(IN PDRIVER_OBJECT pDriver)
{
	UNREFERENCED_PARAMETER(pDriver);
	
	PsRemoveLoadImageNotifyRoutine(&LoadImageNotifyRoutine);
	if (hRegistration)	// 콜백 등록에 실패할 경우 예외 처리
	{
		ObUnRegisterCallbacks(hRegistration);
	}
	
	DbgPrintEx(DPFLTR_ACPI_ID, 3, "[INFO] Driver unload success\n");
}