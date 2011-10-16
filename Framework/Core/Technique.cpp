

#include "ITechnique.hpp"

#include "IAdapter.hpp"
#include "Core.hpp"
#include "Exception.hpp"
#include "ILogger.hpp"
#include "IPass.hpp"
#include "IShader.hpp"
#include "ITexture.hpp"

namespace VoodooShader
{

 /**
  *
  */
 ITechnique* ITechnique::Create(IShader* parent, CGtechnique cgTech)
 {

  ITechnique* tech;

  new ITechnique(tech, parent, cgTech);
  tech->Link(tech);
  return tech;
 }

 /**
  *
  */
 ITechnique::ITechnique(ITechnique* &self, IShader* parent, CGtechnique cgTech) :
  m_Shader(parent),
  m_Core(parent->GetCore()),
  m_CgTechnique(cgTech)
 {
  self.reset(this);

  const char *techName = cgGetTechniqueName(this->m_CgTechnique);

  if (techName)
  {
   this->m_Name = techName;
  }
  else
  {

   char nameBuffer[16];

   sprintf_s(nameBuffer, "tech_%p", m_CgTechnique);
   this->m_Name = nameBuffer;
  }
 }

 /**
  *
  */
 ITechnique::~ITechnique(void)
 {
  m_Target = nullptr;
  m_Passes.clear();
 }

 /**
  *
  */
 ICore *ITechnique::GetCore(void)
 {
  return m_Core;
 }

 /**
  *
  */
 String ITechnique::ToString(void)
 {

  String name = m_Shader.lock()->GetName();

  name += "::";
  name += m_Name;
  return name;
 };

 /**
  *
  */
 IPass* ITechnique::GetPass(size_t index)
 {
  if (index < this->m_Passes.size())
  {
   return this->m_Passes[index];
  }
  else
  {
   Throw(VOODOO_CORE_NAME, "Voodoo ICore: Invalid pass index (> pass count).", m_Core);
  }
 }

 /**
  *
  */
 ITexture* ITechnique::GetTarget(void)
 {
  return m_Target;
 }

 /**
  *
  */
 size_t ITechnique::GetPassCount(void)
 {
  return this->m_Passes.size();
 }

 /**
  *
  */
 IShader* ITechnique::GetShader(void)
 {
  return m_Shader;
 }

 /**
  *
  */
 CGtechnique ITechnique::GetCgTechnique(void)
 {
  return m_CgTechnique;
 }

 /**
  *
  */
 void ITechnique::Link(ITechnique* self)
 {
  this->m_Target = ITexture*();


  // Process the technique's target annotation
  CGannotation targetAnnotation = cgGetNamedTechniqueAnnotation(this->m_CgTechnique, "target");

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

     this->m_Target = m_Core->GetStageTexture(TS_Shader);
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

    this->m_Target = m_Core->GetStageTexture(TS_Shader);
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

   this->m_Target = m_Core->GetStageTexture(TS_Shader);
  }

  this->m_Passes.clear();

  CGpass cPass = cgGetFirstPass(m_CgTechnique);

  while (cgIsPass(cPass))
  {


   // Insert the pass into the vector
   IPass* pass(IPass::Create(self, cPass));

   m_Passes.push_back(pass);

   cPass = cgGetNextPass(cPass);
  }
 }
}
