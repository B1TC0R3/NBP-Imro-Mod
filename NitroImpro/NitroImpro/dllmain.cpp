// dllmain.cpp : Definiert den Einstiegspunkt für die DLL-Anwendung.
#include "pch.h"

import AddressOperator;

//IMPORTANT: EJECTING CRASHES THE GAME!

DWORD WINAPI HackThread(HMODULE hModule) {
    bool run = true;

    AllocConsole();
    FILE* f;
    freopen_s(&f, "CONOUT$", "w", stdout);

    uintptr_t baseAddr = (uintptr_t)GetModuleHandle(NULL);

    uintptr_t* meter = (uintptr_t*)(baseAddr + 0x10D7BE8);
    uintptr_t* assist_a = (uintptr_t*)(baseAddr + 0x10D7C18);
    uintptr_t* assist_b = (uintptr_t*)(baseAddr + 0x10D7C30);

    std::vector<int> meter_options = { 0, 18000 };
    std::vector<int> assista_options = { 0, 999999 };
    std::vector<int> assistb_options = { 0, 999999 };

    AddressOperator<int> meterOp = AddressOperator<int>(meter, meter_options, "Meter   ");
    AddressOperator<int> assistaOp = AddressOperator<int>(assist_a, assista_options, "Assist A");
    AddressOperator<int> assistbOp = AddressOperator<int>(assist_b, assistb_options, "Assist B");

    assistaOp.print();

    while (run) {
        meterOp.execute();
        assistaOp.execute();
        assistbOp.execute();

        if (GetAsyncKeyState(VK_SHIFT) & 1) {
            break;

        }

        if (GetAsyncKeyState(0x45) & 1) {
            assistaOp.changeSelection(true);
            system("cls");
            assistaOp.print();

        }

        Sleep(5);

    }

    fclose(f);
    FreeConsole();
    FreeLibraryAndExitThread(hModule, 0);

    return 0;

}

BOOL APIENTRY DllMain( HMODULE hModule,
                       DWORD  ul_reason_for_call,
                       LPVOID lpReserved
                     )
{
    switch (ul_reason_for_call)
    {
    case DLL_PROCESS_ATTACH:
    case DLL_THREAD_ATTACH:
        CloseHandle(CreateThread(nullptr, NULL, (LPTHREAD_START_ROUTINE)HackThread, hModule, NULL, nullptr));
    case DLL_THREAD_DETACH:
    case DLL_PROCESS_DETACH:
        break;
    }
    return TRUE;
}

