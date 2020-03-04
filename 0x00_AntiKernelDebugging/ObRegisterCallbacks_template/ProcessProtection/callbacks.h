#pragma once
#include "common.h"

//============================================//
//======= Pre&Post Callback Functions ========//
//============================================//

OB_PREOP_CALLBACK_STATUS PreCallback(PVOID RegistrationContext, POB_PRE_OPERATION_INFORMATION pOperationInformation)
{
	UNREFERENCED_PARAMETER(RegistrationContext);
	UNREFERENCED_PARAMETER(pOperationInformation);
	
	DbgPrintEx(DPFLTR_ACPI_ID, 0, "[+] Pre Callback Routine");

	return OB_PREOP_SUCCESS;
}

void PostCallback(PVOID RegistrationContext, POB_POST_OPERATION_INFORMATION pOperationInformation)
{
	UNREFERENCED_PARAMETER(RegistrationContext);
	UNREFERENCED_PARAMETER(pOperationInformation);

	DbgPrintEx(DPFLTR_ACPI_ID, 0, "[+] Post Callback Routine\n");
}