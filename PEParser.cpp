#include "PEParser.hpp"
#include <iostream>

BYTE* LoadPEFile(const char* filePath, DWORD& fileSize) {
    HANDLE file = CreateFileA(filePath, GENERIC_READ, FILE_SHARE_READ, NULL, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL);
    if (file == INVALID_HANDLE_VALUE) {
        std::cerr << "Не удалось открыть файл: " << filePath << "\n";
        return nullptr;
    }

    fileSize = GetFileSize(file, NULL);
    BYTE* fileData = reinterpret_cast<BYTE*>(HeapAlloc(GetProcessHeap(), 0, fileSize));
    DWORD bytesRead;
    ReadFile(file, fileData, fileSize, &bytesRead, NULL);
    CloseHandle(file);

    return fileData;
}

PIMAGE_DOS_HEADER GetDOSHeader(BYTE* fileData) {
    return reinterpret_cast<PIMAGE_DOS_HEADER>(fileData);
}

PIMAGE_NT_HEADERS GetNTHeaders(BYTE* fileData, PIMAGE_DOS_HEADER dosHeader) {
    return reinterpret_cast<PIMAGE_NT_HEADERS>(fileData + dosHeader->e_lfanew);
}
