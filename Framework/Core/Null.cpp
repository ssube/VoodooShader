#include "Null.hpp"

#include "Pass.hpp"
#include "Texture.hpp"

namespace VoodooShader
{
    namespace VoodooNull
    {
        NullLogger::NullLogger(Core * core)
                : m_Core(core)
        { }

        NullLogger::~NullLogger() { }

        Core * NullLogger::GetCore()
        {
            return m_Core;
        }
        String NullLogger::GetName()
        {
            return "NullLogger";
        }

        bool NullLogger::Open(_In_ String Filename, _In_ bool Append)
        {
            return false;
        }
        void NullLogger::Close() {}
        void NullLogger::Flush() {}
        void NullLogger::SetLogLevel(_In_ LogLevel level) {}
        LogLevel NullLogger::GetLogLevel()
        {
            return LL_Unknown;
        }
        void NullLogger::LogModule(_In_ Version module) {}
        void NullLogger::Log(_In_ LogLevel level, _In_ const char * module,  _In_ _Printf_format_string_ const char * msg, ...) {}
        void NullLogger::SetFlags(_In_ LogFlags flush) {}
        LogFlags NullLogger::GetFlags()
        {
            return LF_Unknown;
        }

        NullAdapter::NullAdapter(Core * core)
                : m_Core(core)
        {}

        NullAdapter::~NullAdapter()
        {}

        Core * NullAdapter::GetCore()
        {
            return m_Core;
        }
        String NullAdapter::GetName()
        {
            return "NullAdapter";
        }
        bool NullAdapter::LoadPass(_In_ PassRef pass)
        {
            return false;
        }
        bool NullAdapter::UnloadPass(_In_ PassRef pass)
        {
            return false;
        }
        void NullAdapter::SetPass(_In_ PassRef pass) { }
        PassRef NullAdapter::GetPass()
        {
            return nullptr;
        }
        void NullAdapter::SetTarget(_In_ TextureRef target) { }
        TextureRef NullAdapter::GetTarget()
        {
            return nullptr;
        }
        TextureRef NullAdapter::CreateTexture(_In_ String Name, _In_ TextureDesc Desc)
        {
            return nullptr;
        }
        TextureRef NullAdapter::LoadTexture(_In_ String Name, TextureRegion Region)
        {
            return nullptr;
        }
        void NullAdapter::DrawGeometry(int Vertexes, VertexStruct * pVertexData) { }
        void NullAdapter::DrawShader(_In_ ShaderRef shader) { }
        void NullAdapter::ApplyParameter(_In_ ParameterRef param) { }
        void NullAdapter::SetProperty(String Property, String Value) { }
        String NullAdapter::GetProperty(String Property)
        {
            return String();
        }
        bool NullAdapter::ConnectTexture(_In_ ParameterRef param, _In_ TextureRef texture)
        {
            return false;
        }
        void NullAdapter::HandleError(_In_ CGcontext context, _In_ int error) { }
    }
}
