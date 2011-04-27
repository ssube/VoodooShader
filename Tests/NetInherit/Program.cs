using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Runtime.InteropServices;
using Voodoo_Core;

namespace NetInherit
{
    [
        ComVisible(true),
        Guid("48ae44ab-aa16-4053-ae10-d90be44049aa"),
        ComSourceInterfaces(typeof(IVoodooHookSystem))
    ]
    public class CSHookSystem : IVoodooHookSystem
    {
        VoodooCore m_Core;
        SortedSet<String> m_Hooks;

        public CSHookSystem()
        {
            m_Hooks = new SortedSet<String>();
        }

        public VoodooCore Core
        {
            get 
            {
                return m_Core;
            }
        }

        public void Add(string pName, FunctionPtr pSource, FunctionPtr pDest)
        {
            m_Hooks.Add(pName);
        }

        public void Remove(string pName)
        {
            m_Hooks.Remove(pName);
        }

        public void RemoveAll()
        {
            m_Hooks.Clear();
        }
    }
}
