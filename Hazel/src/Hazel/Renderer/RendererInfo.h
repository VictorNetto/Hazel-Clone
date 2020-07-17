#ifndef _RENDERER_INFO_H
#define _RENDERER_INFO_H

#include <string>

namespace Hazel {

    class RendererInfo {
    public:    
        static const std::string& GetRendererAPI() { return s_RendererAPI; }
        static const std::string& GetVendor() { return s_Vendor; }
        static const std::string& GetRenderer() { return s_Renderer; }
        static const std::string& GetVersion() { return s_Version; }

        static void SetInfo(const std::string& rendererAPI, const std::string& vendor,
            const std::string& renderer, const std::string& version)
        {
            s_RendererAPI = rendererAPI;
            s_Vendor = vendor;
            s_Renderer = renderer;
            s_Version = version;
        }
    private:
        static std::string s_RendererAPI;
        static std::string s_Vendor;
        static std::string s_Renderer;
        static std::string s_Version;
    };

}

#endif // _RENDERER_INFO_H