#ifndef _ENTRY_POINT_H
#define _ENTRY_POINT_H

extern Hazel::Application * Hazel::CreateApplication();

int main()
{
    Hazel::Log::Init();
    
    HZ_CORE_ERROR("Some Error!");
    int a = 5;
    HZ_CLIENT_INFO("Some fatal error Var={1} VAR={0}", 1, a);

    auto app = Hazel::CreateApplication();
    app->Run();
    delete app;
}

#endif // _ENTRY_POINT_H