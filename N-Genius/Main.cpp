#include "PCH.h"

#if _DEBUG
// ReSharper disable once CppUnusedIncludeDirective
#include <vld.h>
#endif

#include "SDL.h"
#include "Core.h"

int main(int, char*[]) {
	ngenius::Core core;
	core.Run();
    return 0;
}