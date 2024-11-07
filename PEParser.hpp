#pragma once
#include <windows.h>

BYTE* LoadPEFile(const char* filePath, DWORD& fileSize);
PIMAGE_DOS_HEADER GetDOSHeader(BYTE* fileData);
PIMAGE_NT_HEADERS GetNTHeaders(BYTE* fileData, PIMAGE_DOS_HEADER dosHeader);


