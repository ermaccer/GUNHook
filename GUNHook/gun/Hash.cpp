#include "Hash.h"
#include "../plugin/PatternHelper.h"

unsigned int _hash(const char* input)
{
    static unsigned int hash_pat = _pat("8B 54 24 04 85 D2 75 03 33 C0 C3", 0, __FUNCTION__);
    return ((unsigned int (_cdecl*)(const char*))hash_pat)(input);
}
