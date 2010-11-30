// The following ifdef block is the standard way of creating macros which make exporting 
// from a DLL simpler. All files within this DLL are compiled with the FRAMEWORK_EXPORTS
// symbol defined on the command line. this symbol should not be defined on any project
// that uses this DLL. This way any other project whose source files include this file see 
// FRAMEWORK_API functions as being imported from a DLL, whereas this DLL sees symbols
// defined with this macro as being exported.

//#pragma once

#define VOODOO_IMPORT

#include "Includes.hpp"

#include "Adapter.hpp"
#include "Converter.hpp"
#include "Core.hpp"
#include "Exception.hpp"
#include "FullscreenManager.hpp"
#include "Logger.hpp"
#include "MaterialManager.hpp"
#include "Meta.hpp"
#include "Parameter.hpp"
#include "Reference.hpp"
#include "Shader.hpp"
#include "Texture.hpp"