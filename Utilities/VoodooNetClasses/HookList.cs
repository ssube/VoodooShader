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

        String m_Name, m_Target, m_Config;

        public VoodooHook(String n, String t, String c)
        {
            m_Name = n; 
            m_Target = t;
            m_Config = c;
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

    class HookList
    {
        List<VoodooHook> m_Hooks;

        public HookList()
        {
            RegistryKey hookRoot = Registry.CurrentUser.OpenSubKey("Software\\VoodooShader\\Hooks");
            if (hookRoot != null)
            {
                String[] hookKeyNames = hookRoot.GetSubKeyNames();
                foreach (String hookKeyName in hookKeyNames)
                {
                    RegistryKey hookKey = hookRoot.OpenSubKey(hookKeyName);
                    String name = hookKey.GetValue("Name") as String;
                    String target = hookKey.GetValue("Target") as String;
                    String config = hookKey.GetValue("Config") as String;
                }
            }
        }
    }
}
