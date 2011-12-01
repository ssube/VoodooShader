using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Text;
using Microsoft.Win32;

namespace VoodooNetClasses
{
    public class VoodooHook : INotifyPropertyChanged
    {
        public event PropertyChangedEventHandler PropertyChanged;

        bool m_Active;
        String m_Name, m_Target, m_Config;

        public VoodooHook(bool a, String n, String t, String c)
        {
            m_Active = a;
            m_Name = n; 
            m_Target = t;
            m_Config = c;
        }

        public bool Active
        {
            get { return m_Active; }
            set { m_Active = value; this.NotifyPropertyChanged("Active"); }
        }

        public String Name
        {
            get { return m_Name; }
            set { m_Name = value; this.NotifyPropertyChanged("Name"); }
        }

        public String Target
        {
            get { return m_Target; }
            set { m_Target = value; this.NotifyPropertyChanged("Target"); }
        }

        public String Config
        {
            get { return m_Config; }
            set { m_Config = value; this.NotifyPropertyChanged("Config"); }
        }

        private void NotifyPropertyChanged(String name)
        {
            if (PropertyChanged != null) { PropertyChanged(this, new PropertyChangedEventArgs(name)); }
        }
    }

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
