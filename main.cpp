#include <iostream>
#include "PEParser.hpp"
#include "ImportFunctions.hpp"
#include <locale.h>

int main() {
    setlocale(LC_ALL, "Russian");
    const char* filePath = "C:/Users/Дом/loner.exe";

    DWORD fileSize;
    BYTE* fileData = LoadPEFile(filePath, fileSize);
    if (!fileData) {
        return 1;
    }

    PIMAGE_DOS_HEADER dosHeader = GetDOSHeader(fileData);
    PIMAGE_NT_HEADERS ntHeaders = GetNTHeaders(fileData, dosHeader);

    DWORD importDirectoryRVA = ntHeaders->OptionalHeader.DataDirectory[IMAGE_DIRECTORY_ENTRY_IMPORT].VirtualAddress;
    auto sectionHeader = IMAGE_FIRST_SECTION(ntHeaders);

    PIMAGE_SECTION_HEADER importSection = nullptr;
    for (WORD i = 0; i < ntHeaders->FileHeader.NumberOfSections; ++i, ++sectionHeader) {
        if (importDirectoryRVA >= sectionHeader->VirtualAddress &&
            importDirectoryRVA < sectionHeader->VirtualAddress + sectionHeader->Misc.VirtualSize) {
            importSection = sectionHeader;
            break;
        }
    }

    if (importSection) {
        PrintImportFunctions(importDirectoryRVA, importSection, fileData);
    }
    else {
        std::cerr << "Секция импорта не найдена\n";
    }

    HeapFree(GetProcessHeap(), 0, fileData);
    return 0;
}
