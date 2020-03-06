#pragma once
#include <ntifs.h>

//============================================//
//========= LoadImageNotify Routine ==========//
//============================================//

VOID LoadImageNotifyRoutine(IN PUNICODE_STRING FullImageName, IN HANDLE ProcessId, IN PIMAGE_INFO ImageInfo);