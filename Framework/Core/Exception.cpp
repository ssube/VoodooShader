#include "Exception.hpp"

#include "Core.hpp"
#include "ILogger.hpp"

namespace VoodooShader
{

 /**
  ===================================================================================================================
  *
  ===================================================================================================================
  */
 Exception::Exception
(
 _In_ char *module,
 _In_ char *message,
 _In_ Core *core,
 _In_ char *file,
 _In_ char *function,
 _In_ int line
) :
 std::exception(message),
 m_FmtMsg(NULL),
 m_Module(module),
 m_Message(message),
 m_Core(core),
 m_File(file),
 m_Function(function),
 m_Line(line)
 {
  if (core)
  {

   /*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/
   /*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/
   ILoggerRef logger = core->GetLogger();
   /*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/

   /*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/
   if (logger.get())
   {
    logger->Log(LL_Error, module, "Exception in %s at %s (%d): %s", file, function, line, message);
   }
  }
 }

 /**
  ===================================================================================================================
  *
  ===================================================================================================================
  */
 Exception::Exception
(
 _In_ String module,
 _In_ String message,
 _In_ Core *core,
 _In_ char *file,
 _In_ char *function,
 _In_ int line
) :
 std::exception(message.c_str()),
 m_FmtMsg(NULL),
 m_Module(module),
 m_Message(message),
 m_Core(core),
 m_File(file),
 m_Function(function),
 m_Line(line)
 {
  if (core)
  {

   /*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/
   /*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/
   ILoggerRef logger = core->GetLogger();
   /*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/

   /*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/
   if (logger.get())
   {
    logger->Log
     (
      LL_Error,
      module.c_str(),
      "Exception in %s at %s (%d): %s",
      file,
      function,
      line,
      message.c_str()
     );
   }
  }
 }

 /**
  ===================================================================================================================
  *
  ===================================================================================================================
  */
 Exception::~Exception(void)
 {
  if (m_FmtMsg)
  {
   delete m_FmtMsg;
  }
 }

 /**
  ===================================================================================================================
  *
  ===================================================================================================================
  */
 const char *Exception::what(void)
 {
  if (m_FmtMsg == NULL)
  {
   m_FmtMsg = new char[1024];
   ZeroMemory(m_FmtMsg, 1024);

   sprintf_s
   (
    m_FmtMsg,
    1024,
    "VoodooShader::Exception in module %s, file %s at %s (line %d): %s",
    m_Module.c_str(),
    m_File,
    m_Function,
    m_Line,
    m_Message.c_str()
   );
  }

  return m_FmtMsg;
 }
}
