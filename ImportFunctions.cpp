#include "ImportFunctions.hpp"
#include <iostream>

void PrintImportFunctions(DWORD importDirectoryRVA, PIMAGE_SECTION_HEADER importSection, BYTE* fileData) {
    if (!importSection || !fileData) {
        std::cerr << "Неверный указатель на секцию или данные файла.\n";
        return;
    }

    size_t rawOffset = reinterpret_cast<size_t>(fileData) + importSection->PointerToRawData;
    auto importDescriptor = reinterpret_cast<PIMAGE_IMPORT_DESCRIPTOR>(rawOffset + (importDirectoryRVA - importSection->VirtualAddress));

    while (importDescriptor->Name != 0) {
        auto namePtr = reinterpret_cast<char*>(rawOffset + (importDescriptor->Name - importSection->VirtualAddress));
        if (!IsBadReadPtr(namePtr, 1)) {
            std::cout << "Импорт из DLL: " << namePtr << "\n";
        }
        else {
            std::cerr << "Ошибка чтения имени DLL.\n";
            break;
        }

        DWORD thunk = importDescriptor->OriginalFirstThunk ? importDescriptor->OriginalFirstThunk : importDescriptor->FirstThunk;
        auto thunkData = reinterpret_cast<PIMAGE_THUNK_DATA>(rawOffset + (thunk - importSection->VirtualAddress));

        while (thunkData->u1.AddressOfData != 0) {
            if (thunkData->u1.AddressOfData < 0x80000000) {
                auto funcName = reinterpret_cast<char*>(rawOffset + (thunkData->u1.AddressOfData - importSection->VirtualAddress + 2));
                if (!IsBadReadPtr(funcName, 1)) {
                    std::cout << "\tФункция: " << funcName << "\n";
                }
                else {
                    std::cerr << "\tОшибка чтения имени функции.\n";
                    break;
                }
            }
            ++thunkData;
        }
        ++importDescriptor;
    }
}
