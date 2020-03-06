#pragma once
#include "notify.h"

//============================================//
//======= DriverEntry & Unload Routine =======//
//============================================//

NTSTATUS DriverEntry(IN PDRIVER_OBJECT pDriver, IN PUNICODE_STRING pRegPath);
VOID UnloadDriver(IN PDRIVER_OBJECT pDriver);

//============================================//
//========== User-defined Function  ==========//
//============================================//

VOID TerminateProcess(IN HANDLE pid);