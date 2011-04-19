#include "Common.hpp"

namespace VoodooShader
{
    HRESULT WINAPI InstanceFromString(_In_ BSTR lpStr, _In_ REFIID iid, _In_ void ** pp)
    {
        // Error check
        if ( lpStr == NULL || pp == NULL )
        {
            return E_INVALIDARG;
        }

        CLSID clsid;
        // Check string type
        if ( lpStr[0] == '{' )
        {
            // Registry-format string.
            HRESULT getHR = CLSIDFromString(lpStr, &clsid);
            if ( FAILED(getHR) )
            {
                return getHR;
            }
        } else {
            // Assume ProgID
            HRESULT getHR = CLSIDFromProgID(lpStr, &clsid);
            if ( FAILED(getHR) )
            {
                return getHR;
            }
        }

        return CoCreateInstance(clsid, NULL, NULL, iid, pp);
    }
}