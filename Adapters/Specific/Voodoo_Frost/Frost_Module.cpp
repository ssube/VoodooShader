
#include "Frost_Module.hpp"
#include "Frost_Adapter.hpp"

VoodooShader::Core * VoodooCore = NULL;
VoodooShader::ILoggerRef VoodooLogger = NULL;
VoodooShader::Frost::Adapter * VoodooFrost = NULL;
VoodooShader::ShaderRef TestShader;

GLint gDepthTexture = 0;
GLint gThisFrame = 0;
GLint gLastFrame = 0;
GLint gLastShader = 0;
GLint gLastPass = 0;
GLsizei gViewWidth = 0;
GLsizei gViewHeight = 0;

namespace VoodooShader
{
    namespace Frost
    {
        Version API_ModuleVersion()
        {
            Version moduleVersion = VOODOO_META_VERSION_STRUCT(FROST);
            return moduleVersion;
        }

        int API_ClassCount()
        {
            return 1;
        }

        const char * API_ClassInfo
        (
            _In_ int number
        )
        {
            if (number == 0)
            {
                return "Frost_Adapter";
            }
            else
            {
                return NULL;
            }
        }

        IObject * API_ClassCreate
        (
            _In_ int number,
            _In_ Core * core
        )
        {
            if (number == 0)
            {
                return new Frost::Adapter(core);
            }
            else
            {
                return NULL;
            }
        }
    }
}
