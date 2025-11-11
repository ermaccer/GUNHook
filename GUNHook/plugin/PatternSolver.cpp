#include "PatternSolver.h"
#include "..\utils\Patterns.h"
#include "..\gui\log.h"
#include <chrono>

uintptr_t PatternSolver::ms_patterns[PATID_Total_Patterns];


uintptr_t PatternSolver::GetPattern(const char* szPattern, int offset)
{
    uintptr_t addr = 0;
    try {
        addr = (uintptr_t)hook::txn::get_pattern(szPattern, offset);
    }
    TXN_CATCH();

    return addr;
}

void PatternSolver::Initialize()
{
    eLog::Message(__FUNCTION__, "Starting pattern search");

    for (int i = 0; i < PATID_Total_Patterns; i++)
        ms_patterns[i] = 0;

    auto begin = std::chrono::high_resolution_clock::now();

    ms_patterns[PATID_CameraConstructor] = GetPattern("56 68 E4 00 00 00 E8 ? ? ? ? 8B F0", 0);
    ms_patterns[PATID_CameraConstructor_Hook] = GetPattern("6A 00 68 ? ? ? ? 68 A5 A7 F5 FD", 3);
    ms_patterns[PATID_SetCursorPos_Hook] = GetPattern("A1 ? ? ? ? 99 2B C2 D1 F8 50 FF 15", 13);
    ms_patterns[PATID_SetGameSpeed] = GetPattern("B8 04 00 00 00 03 D0 A1 ? ? ? ? 89 14 24", 8);
    ms_patterns[PATID_SetFOVMult] = GetPattern("DD D8 D9 C9 D9 C9 DE F9 D9 1D", 10);
    ms_patterns[PATID_FindPlayer] = GetPattern("83 EC 44 56 8B F1 8B 0D", 8);
    ms_patterns[PATID_RunScript] = GetPattern("55 8B 6C 24 08 56 55 E8 ? ? ? ? 8B F0", 0);
    ms_patterns[PATID_Hash] = GetPattern("8B 54 24 04 85 D2 75 03 33 C0 C3", 0);
    ms_patterns[PATID_UpdateInput] = GetPattern("83 EC 0C 55 56 57 8B 7C 24 1C 8B 47 18 F7 D8 1B C0 25", 0);
    ms_patterns[PATID_UpdateCamera_Hook] = GetPattern("C7 44 24 50 00 00 80 3F 89 08 8B 54", 8);
    ms_patterns[PATID_UpdateInput_Call1] = GetPattern("E8 ? ? ? ? 5F 5B 5E C2 04 00 8D 4E 20 E8", 0);
    ms_patterns[PATID_UpdateInput_Call2] = GetPattern("E8 ? ? ? ? 5B 5E C2 04 00 8D 8E ? ? ? ? E8", 0);
    ms_patterns[PATID_UpdateInput_Call3] = GetPattern("E8 ? ? ? ? 8B 4E 1C E8 ? ? ? ? 84 C0 0F 84 ? ? ? ? 8B 46 1C", 0);
    ms_patterns[PATID_DInput_Level] = GetPattern("6A 05 51 50 FF 52 34 8B 44 24 14", 0);
    ms_patterns[PATID_DInput_Level2] = GetPattern("6A 03 8D 4C 24 1C 51 68 ? ? ? ? 89 6C 24 24", 0);
    ms_patterns[PATID_LoadAppearance] = GetPattern("83 EC 3C 53 55 56 57 6A 01 6A 08 E8 ? ? ? ? 33 F6 83 C4 08 3B C6", 0);
    ms_patterns[PATID_LoadPlayerModel] = GetPattern("81 EC ? ? ? ? A1 ? ? ? ? 53 55 56 57 8B BC 24 ? ? ? ? 57 B9 ? ? ? ? 89 84 24 ? ? ? ? C6 44 24 ? ? E8 ? ? ? ? 8B F0 8B 6E 04 8B 1E 89 35 ? ? ? ? E8 ? ? ? ? 50 A1", 0);
    ms_patterns[PATID_Script_FindGlobalObject] = GetPattern("8B 44 24 04 50 E8 ? ? ? ? 83 C4 04 85 C0 75 01 C3 8B 40 0C C3", 0);
    ms_patterns[PATID_Script_CreateParams] = GetPattern("A1 ? ? ? ? 8B 0C 85 ? ? ? ? 85 C9 74 47 8B 54 24 08 56 52 E8 ? ? ? ? 8B F0 85 F6 75 32 8B 0D", 0);
    ms_patterns[PATID_Script_DestroyParams1] = GetPattern("51 56 8B 71 04 85 F6 89 4C 24 04 74 7C 53 57 90 8B 5E 0C 56 E8 ? ? ? ? A1 ? ? ? ? 8B 3C 85 ? ? ? ? 83 C4 04 85 FF 74 1B 56", 0);
    ms_patterns[PATID_Script_DestroyParams2] = GetPattern("A1 ? ? ? ? 56 8B 34 85 ? ? ? ? 85 F6 57 8B 7C 24 0C 74 1C 57 8B CE E8 ? ? ? ? 84 C0 74 0B 57 8B CE E8 ? ? ? ? 5F 5E C3 A1", 0);
    ms_patterns[PATID_Script_PushString] = GetPattern("56 57 6A 01 6A 10 8B F9 E8 ? ? ? ? 83 C4 08 85 C0 74 0B 8B C8 E8 ? ? ? ? 8B F0 EB 02 33 F6 8A 4E 01 8B 54 24 10 8B 44 24 0C", 0);
    ms_patterns[PATID_LoadAppearance_Hook] = GetPattern("E8 ? ? ? ? 83 C4 04 8B F0 6A 01 56 8B CB", 0);
    ms_patterns[PATID_LoadPlayerModel_Hook] = GetPattern("68 0E AD 55 D7 E8 ? ? ? ? 8B 0C 24 51 E8 ? ? ? ? 83 C4 08 C3", 14);
    ms_patterns[PATID_Camera_UpdateMatrix_Hook] = GetPattern("E8 ? ? ? ? 8B 96 ? ? ? ? 8D 4C 24 5C 51", 0);


    auto end = std::chrono::high_resolution_clock::now();

    auto time = std::chrono::duration_cast<std::chrono::milliseconds>(end - begin);
    auto timeSeconds = std::chrono::duration_cast<std::chrono::seconds>(end - begin);
    eLog::Message(__FUNCTION__, "Checked %d patterns in %dms (%ds)", PATID_Total_Patterns, time.count(), timeSeconds.count());

}

int PatternSolver::GetNumPatternsOK()
{
    int patternNum = 0;

    for (int i = 0; i < PATID_Total_Patterns; i++)
        if (ms_patterns[i]) patternNum++;
    return patternNum;
}

bool PatternSolver::CheckMissingPatterns()
{
    int missingPatterns = 0;
    for (int i = 0; i < PATID_Total_Patterns; i++)
        if (ms_patterns[i] == 0)
        {
            missingPatterns++;
            eLog::Message(__FUNCTION__, "ERROR: Could not find %s!", GetPatternName(i));
        }
    return missingPatterns > 0;
}

const char* PatternSolver::GetPatternName(int id)
{
    if (id >= PATID_Total_Patterns)
        return "UNKNOWN";

    static const char* szPatternNames[PATID_Total_Patterns] = {
    "CameraConstructor",
    "CameraConstructor_Hook",
    "SetCursorPos_Hook",
    "SetGameSpeed",
    "SetFOVMult",
    "FindPlayer",
    "RunScript",
    "Hash",
    "UpdateInput",
    "UpdateCamera_Hook",
    "UpdateInput_Call1",
    "UpdateInput_Call2",
    "UpdateInput_Call3",
    "DInput_Level",
    "DInput_Level2",
    "LoadAppearance",
    "LoadPlayerModel",
    "Script_FindGlobalObject",
    "Script_CreateParams",
    "Script_DestroyParams1",
    "Script_DestroyParams2",
    "Script_PushString",
    "LoadAppearance_Hook",
    "LoadPlayerModel_Hook",
    "Camera_UpdateMatrix_Hook",
    };   

    return szPatternNames[id];
}

uintptr_t _pattern(EPatternID id)
{
    if (id >= PATID_Total_Patterns)
        return 0;

    return PatternSolver::ms_patterns[id];
}
