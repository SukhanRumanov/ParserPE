#pragma once
#include <windows.h>

void PrintImportFunctions(DWORD importDirectoryRVA, PIMAGE_SECTION_HEADER importSection, BYTE* fileData);
