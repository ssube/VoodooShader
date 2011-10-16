#include "ITexture.hpp"

namespace VoodooShader
{

 /**
  *
  */
 ITexture::ITexture(String name, void *texture) :
  m_Name(name),
  m_Data(texture)
 { }

 /**
  *
  */
 ITexture::~ITexture(void)
 { }

 /**
  *
  */
 String ITexture::ToString(void)
 {
  return m_Name;
 }

 /**
  *
  */
 ICore *ITexture::GetCore(void)
 {
  return m_Core;
 }

 /**
  *
  */
 void *ITexture::GetData(void)
 {
  return m_Data;
 }

 /**
  *
  */
 TextureDesc ITexture::GetDesc(void)
 {
  return m_Desc;
 }
}
