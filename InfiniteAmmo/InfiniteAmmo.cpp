// InfiniteAmmo.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include <iostream>
#include <vector>
#include <Windows.h>
#include "proc.h"

int main()
{
    // Get proc ID of the target process
    DWORD procId = GetProcId(L"ac_client.exe");

    if (!procId) {
        return;
    }

    // Get module base address
    uintptr_t moduleBase = GetModuleBaseAddress(procId, L"ac_client.exe");


    // Get handle to process
    HANDLE hProcess = 0;
    hProcess = OpenProcess(PROCESS_ALL_ACCESS, NULL, procId);


    // Resolve the base address of the pointer chain
    uintptr_t dynamicPtrBaseAddr = moduleBase + 0x10F4F4;

    std::cout << "DynamicPtrBaseAddr = 0x" << std::hex << dynamicPtrBaseAddr <<  std::endl;


    // Resolve ammo pointer chain
    std::vector<unsigned int> ammoOffsets = { 0x374, 0x14, 0x0 };
    uintptr_t ammoAddr = FindDMAAddy(hProcess, dynamicPtrBaseAddr, ammoOffsets);

    std::cout << "AmmoAddr = 0x" << std::hex << ammoAddr << std::endl;


    // Read ammo value
    int ammoValue = 0;

    ReadProcessMemory(hProcess, (BYTE*)ammoAddr, &ammoValue, sizeof(ammoValue), nullptr);
    std::cout << "Current ammo = " << std::dec << ammoValue << std::endl;


    // Write to it
    int newAmmo = 3301; 
    WriteProcessMemory(hProcess, (BYTE*)ammoAddr, &newAmmo, sizeof(newAmmo), nullptr);


    // Read out again
    ReadProcessMemory(hProcess, (BYTE*)ammoAddr, &ammoValue, sizeof(ammoValue), nullptr);
    std::cout << "New ammo = " << std::dec << ammoValue << std::endl;


    // additional health hack with the dynamic pointer that was initially found (it's the pointer to the player object)
    uintptr_t healthAddr = FindDMAAddy(hProcess, dynamicPtrBaseAddr, { 0xf8 });

    int newHealth = 1337;

    WriteProcessMemory(hProcess, (BYTE*)healthAddr, &newHealth, sizeof(newHealth), nullptr);

    getchar();
    


    return 0;
}