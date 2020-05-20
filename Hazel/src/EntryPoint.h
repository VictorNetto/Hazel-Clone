#ifndef _ENTRY_POINT_H
#define _HAZEL__ENTRY_POINT_HH

extern Hazel::Application * Hazel::CreateApplication();

int main()
{
    auto app = Hazel::CreateApplication();
    app->Run();
    delete app;
}

#endif // _ENTRY_POINT_H