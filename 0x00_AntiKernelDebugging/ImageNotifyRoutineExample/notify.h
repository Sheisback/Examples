#pragma once
#include <ntifs.h>

//============================================//
//========= LoadImageNotify Routine ==========//
//============================================//

VOID LoadImageNotifyRoutine(IN PUNICODE_STRING FullImageName, IN HANDLE ProcessId, IN PIMAGE_INFO ImageInfo);

//============================================//
//=============== Black List =================//
//============================================//

const wchar_t *szTarget[2] = { L"notepad.exe" ,L"x64dbg.exe" };
