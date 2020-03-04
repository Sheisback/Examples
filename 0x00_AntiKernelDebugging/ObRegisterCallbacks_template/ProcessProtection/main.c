#include "callbacks.h"

PVOID hRegistration = NULL;	// 언로드 시, 사용하기 위해 전역변수로 선언

/*
# Name  : ObRegExample
# Param : x
# Desc  : OB_CALLBACK, OPERATION_REGISTRATION 구조체 초기화 및 ObRegisterCallbacks 를 이용해 콜백 루틴 등록
*/
NTSTATUS ObRegExample()
{
	OB_CALLBACK_REGISTRATION obRegistration = { 0, };
	OB_OPERATION_REGISTRATION opRegistration = { 0, };

	obRegistration.Version = ObGetFilterVersion();
	obRegistration.OperationRegistrationCount = 1;
	RtlInitUnicodeString(&obRegistration.Altitude, L"300000");
	obRegistration.RegistrationContext = NULL;

	opRegistration.ObjectType = PsProcessType;
	opRegistration.Operations = OB_OPERATION_HANDLE_CREATE;
	opRegistration.PreOperation = PreCallback;
	opRegistration.PostOperation = PostCallback;

	obRegistration.OperationRegistration = &opRegistration;
	
	DbgPrintEx(DPFLTR_ACPI_ID, 0, "[+] ObRegisterCallbacks Test\n");
	return ObRegisterCallbacks(&obRegistration,&hRegistration);
}

/*
# Name  : DriverEntry
# Param : PDRIVER_OBJECT, PUNICODE_STRING
# Desc  : 드라이버 진입점
*/
NTSTATUS DriverEntry(IN PDRIVER_OBJECT pDriver, IN PUNICODE_STRING pRegPath)
{
	UNREFERENCED_PARAMETER(pRegPath);
	UNREFERENCED_PARAMETER(pDriver);
	NTSTATUS ret = STATUS_SUCCESS;
	DbgPrintEx(DPFLTR_ACPI_ID, 0, "[+] Load Driver\n");
	pDriver->DriverUnload = UnloadDriver;
	ret = ObRegExample();
	if (ret==STATUS_SUCCESS)
	{
		DbgPrintEx(DPFLTR_ACPI_ID, 0, "[+] Success Registeration\n");
	}
	else
	{
		DbgPrintEx(DPFLTR_ACPI_ID, 0, "[+] Failed Registration %IX\n",ret);
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
	if (hRegistration)
	{
		ObUnRegisterCallbacks(hRegistration);
	}
	DbgPrintEx(DPFLTR_ACPI_ID, 0, "[+] Unload Driver\n");
}