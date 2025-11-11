#pragma once
#include "..\framework.h"

enum EPatternID {
	PATID_CameraConstructor,
	PATID_CameraConstructor_Hook,
	PATID_SetCursorPos_Hook,
	PATID_SetGameSpeed,
	PATID_SetFOVMult,
	PATID_FindPlayer,
	PATID_RunScript,
	PATID_Hash,
	PATID_UpdateInput,
	PATID_UpdateCamera_Hook,
	PATID_UpdateInput_Call1,
	PATID_UpdateInput_Call2,
	PATID_UpdateInput_Call3,
	PATID_DInput_Level,
	PATID_DInput_Level2,
	PATID_LoadAppearance,
	PATID_LoadPlayerModel,
	PATID_Script_FindGlobalObject,
	PATID_Script_CreateParams,
	PATID_Script_DestroyParams1,
	PATID_Script_DestroyParams2,
	PATID_Script_PushString,

	PATID_LoadAppearance_Hook,
	PATID_LoadPlayerModel_Hook,

	PATID_Camera_UpdateMatrix_Hook,


	PATID_Total_Patterns
};


class PatternSolver {
public:
	static uintptr_t ms_patterns[PATID_Total_Patterns];

	static uintptr_t GetPattern(const char* szPattern, int offset);

	static void			Initialize();
	static int			GetNumPatternsOK();
	static bool			CheckMissingPatterns();
	static const char*	GetPatternName(int id);

};


uintptr_t _pattern(EPatternID id);