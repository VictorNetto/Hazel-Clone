#ifndef _ENTRY_POINT_H
#define _ENTRY_POINT_H

#include "Hazel/Debug/Instrumentor.h"

extern Hazel::Application * Hazel::CreateApplication();

int main()
{
    Hazel::Log::Init();

    HZ_PROFILE_BEGIN_SESSION("Startup", "Profiling/HazelProfile-Startup.json");
    auto app = Hazel::CreateApplication();
    HZ_PROFILE_END_SESSION();

    HZ_PROFILE_BEGIN_SESSION("Runtime", "Profiling/HazelProfile-Runtime.json");
    app->Run();
    HZ_PROFILE_END_SESSION();

    HZ_PROFILE_BEGIN_SESSION("Shutdown", "Profiling/HazelProfile-Shutdown.json");
    delete app;
    HZ_PROFILE_END_SESSION();
}

#endif // _ENTRY_POINT_H