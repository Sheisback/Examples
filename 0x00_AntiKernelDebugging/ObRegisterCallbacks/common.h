#pragma once
#include "callbacks.h"

//============================================//
//======= DriverEntry & Unload Routine =======//
//============================================//

NTSTATUS DriverEntry(IN PDRIVER_OBJECT pDriver, IN PUNICODE_STRING pRegPath);
VOID UnloadDriver(IN PDRIVER_OBJECT pDriver);


//============================================//
//====== Object Callback Routine Define ======//
//============================================//

OB_PREOP_CALLBACK_STATUS PreCallback(PVOID RegistrationContext, POB_PRE_OPERATION_INFORMATION pOperationInformation);
void PostCallback(PVOID RegistrationContext, POB_POST_OPERATION_INFORMATION pOperationInformation);


//============================================//
//========== User-defined Function  ==========//
//============================================//

NTSTATUS ObRegExample();
BOOLEAN GetOffset(PEPROCESS Process);
BOOLEAN GetPebOffet();