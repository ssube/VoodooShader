#include "IPass.hpp"

#include "IAdapter.hpp"
#include "Core.hpp"
#include "ILogger.hpp"
#include "ITechnique.hpp"
#include "ITexture.hpp"

namespace VoodooShader
{

 /**
  *
  */
 IPass* IPass::Create(ITechnique* parent, CGpass cgPass)
 {

  IPass* pass;

  new IPass(pass, parent, cgPass);
  pass->Link();
  return pass;
 }

 /**
  *
  */
 IPass::IPass(IPass* &self, ITechnique* parent, CGpass cgPass) :
  m_Technique(parent),
  m_CgPass(cgPass)
 {
  self.reset(this);

  this->m_Core = parent->GetCore();

  const char *passName = cgGetPassName(this->m_CgPass);

  if (passName)
  {
   this->m_Name = passName;
  }
  else
  {

   char nameBuffer[16];

   _itoa_s((int) (&this->m_CgPass), nameBuffer, 16, 16);

   this->m_Name = "pass_";
   this->m_Name += nameBuffer;
  }
 }

 /**
  *
  */
 IPass::~IPass(void)
 {
  m_Target = nullptr;

  // ! @todo Check if passes need to unload themselves from the adapter on dest. m_Core->GetAdapter()->UnloadPass(this);
 }

 /**
  *
  */
 String IPass::ToString(void)
 {

  String name = m_Technique.lock()->GetName();

  name += "::";
  name += m_Name;
  return name;
 };

 /**
  *
  */
 ICore *IPass::GetCore(void)
 {
  return m_Core;
 }

 /**
  *
  */
 ITexture* IPass::GetTarget(void)
 {
  return m_Target;
 }

 /**
  *
  */
 CGprogram IPass::GetProgram(ProgramStage stage)
 {
  switch (stage)
  {
  case PS_Vertex:
   return m_VertexProgram;

  case PS_Fragment:
   return m_FragmentProgram;

  case PS_Geometry:
   return m_GeometryProgram;

  case PS_Domain:
   return m_DomainProgram;

  case PS_Hull:
   return m_HullProgram;

  case PS_Unknown:
  default:
   return nullptr;
  }
 }

 /**
  *
  */
 ITechnique* IPass::GetTechnique(void)
 {
  return m_Technique;
 }

 /**
  *
  */
 CGpass IPass::GetCgPass(void)
 {
  return m_CgPass;
 }

 /**
  *
  */
 void IPass::Link(void)
 {
  this->m_VertexProgram = cgGetPassProgram(this->m_CgPass, CG_VERTEX_DOMAIN);
  this->m_FragmentProgram = cgGetPassProgram(this->m_CgPass, CG_FRAGMENT_DOMAIN);
  this->m_GeometryProgram = cgGetPassProgram(this->m_CgPass, CG_GEOMETRY_DOMAIN);
  this->m_DomainProgram = cgGetPassProgram(this->m_CgPass, CG_TESSELLATION_CONTROL_DOMAIN);
  this->m_HullProgram = cgGetPassProgram(this->m_CgPass, CG_TESSELLATION_EVALUATION_DOMAIN);

  this->m_Target = nullptr;

  CGannotation targetAnnotation = cgGetNamedPassAnnotation(this->m_CgPass, "target");

  if (cgIsAnnotation(targetAnnotation))
  {
   if (cgGetAnnotationType(targetAnnotation) == CG_STRING)
   {

    const char *targetName = cgGetStringAnnotationValue(targetAnnotation);

    this->m_Target = m_Core->GetTexture(targetName);

    if (!this->m_Target.get())
    {
     m_Core->GetLogger()->Log
      (
       LL_Warning,
       VOODOO_CORE_NAME,
       "IPass %s cannot find target %s.",
       this->ToString().c_str(),
       targetName
      );

     this->m_Target = m_Core->GetStageTexture(TS_Pass);
    }
   }
   else
   {
    m_Core->GetLogger()->Log
     (
      LL_Warning,
      VOODOO_CORE_NAME,
      "IPass %s has annotation \"target\" of invalid type.",
      this->ToString().c_str()
     );

    this->m_Target = m_Core->GetStageTexture(TS_Pass);
   }
  }
  else
  {
   m_Core->GetLogger()->Log
    (
     LL_Debug,
     VOODOO_CORE_NAME,
     "IPass %s has no target annotation.",
     this->ToString().c_str()
    );

   this->m_Target = m_Core->GetStageTexture(TS_Pass);
  }


  // Load the programs
  IAdapter* adapter = m_Core->GetAdapter();

  if (!adapter)
  {
   m_Core->GetLogger()->Log
    (
     LL_Warning,
     VOODOO_CORE_NAME,
     "No adapter found, pass %s must be explicitly loaded later.",
     this->ToString().c_str()
    );
  }
  else
  {
   if (!adapter->LoadPass(IPass*(this)))
   {
    m_Core->GetLogger()->Log
     (
      LL_Error,
      VOODOO_CORE_NAME,
      "Failed to load pass %s.",
      this->ToString().c_str()
     );
   }
   else
   {
    m_Core->GetLogger()->Log
     (
      LL_Info,
      VOODOO_CORE_NAME,
      "Successfully loaded pass %s.",
      this->ToString().c_str()
     );
   }
  }
 }
}
