using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Text;
using Microsoft.Win32;

namespace VoodooNetClasses
{
    public class HookList
    {
        List<VoodooHook> m_Hooks;

        public HookList()
        {
            m_Hooks = new List<VoodooHook>();

            RegistryKey hookRoot = Registry.CurrentUser.OpenSubKey("Software\\VoodooShader\\Hooks");
            if (hookRoot != null)
            {
                String[] hookKeyNames = hookRoot.GetSubKeyNames();
                foreach (String hookKeyName in hookKeyNames)
                {
                    RegistryKey hookKey = hookRoot.OpenSubKey(hookKeyName);
                    bool active = Convert.ToBoolean(hookKey.GetValue("Active") as String);
                    String name = hookKey.GetValue("Name") as String;
                    String target = hookKey.GetValue("Target") as String;
                    String config = hookKey.GetValue("Config") as String;
                    m_Hooks.Add(new VoodooHook(active, name, target, config));
                }
            }
        }

        public void Commit()
        {
            RegistryKey hookRoot = Registry.CurrentUser.OpenSubKey("Software\\VoodooShader\\Hooks", RegistryKeyPermissionCheck.ReadWriteSubTree);

            foreach (String subkey in hookRoot.GetSubKeyNames())
            {
                hookRoot.DeleteSubKey(subkey);
            }

            int i = 1;
            foreach (VoodooHook hook in m_Hooks)
            {
                RegistryKey hookKey = hookRoot.CreateSubKey(i++.ToString());

                hookKey.SetValue("Active", hook.Active);
                hookKey.SetValue("Name", hook.Name);
                hookKey.SetValue("Target", hook.Target);
                hookKey.SetValue("Config", hook.Config);
            }
        }

        public List<VoodooHook> List
        {
            get { return m_Hooks; }
        }
    }
}
