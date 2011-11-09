/*
 * This file is part of the Voodoo Shader Framework.
 *
 * Copyright (c) 2010-2011 by Sean Sube
 *
 * The Voodoo Shader Framework is free software; you can redistribute it and/or modify it under the terms of the GNU Lesser
 * General Public License as published by the Free Software Foundation; either version 3 of the License, or (at your option)
 * any later version.  This program is distributed in the hope that it will be useful, but WITHOUT ANY WARRANTY; without
 * even the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the GNU Lesser General Public
 * License for more details.
 *
 * You should have received a copy of the GNU Lesser General Public License along with this program; if not, write to
 * the Free Software Foundation, Inc., 51 Franklin St, Fifth Floor, Boston, MA 02110-1301 US
 *
 * Support and more information may be found at
 *   http://www.voodooshader.com
 * or by contacting the lead developer at
 *   peachykeen@voodooshader.com
 */

using System;
using System.Collections.Generic;
using System.Text;
using Microsoft.Win32;

namespace VoodooNetClasses
{
    public class VoodooModule
    {
        Guid m_LIBID;
        String m_ModulePath, m_ConfigPath, m_RemotePath;
        Dictionary<String, object> m_Props;

        public VoodooModule(Guid libid, String path, String config, String remote, Dictionary<String, object> props = null)
        {
            m_LIBID = libid;
            m_ModulePath = path;
            m_ConfigPath = config;
            m_RemotePath = remote;
            m_Props = props;
        }

        public VoodooModule(Guid libid, Dictionary<String, object> props)
        {
            m_LIBID = libid;
            m_Props = props;

            object pvalue;
            if (m_Props.TryGetValue("module", out pvalue))
            {
                m_ModulePath = (String)pvalue;
                m_Props.Remove("module");
            }
            if (m_Props.TryGetValue("config", out pvalue))
            {
                m_ConfigPath = (String)pvalue;
                m_Props.Remove("config");
            }
            if (m_Props.TryGetValue("remote", out pvalue))
            {
                m_RemotePath = (String)pvalue;
                m_Props.Remove("remote");
            }
        }

        public Guid LIBID
        {
            get { return m_LIBID; }
        }

        public String Path
        {
            get { return m_ModulePath; }
        }

        public String Config
        {
            get { return m_ConfigPath; }
        }

        public String Remote
        {
            get { return m_RemotePath; }
        }
    }

    public class VoodooClass
    {
        Guid m_CLSID, m_LIBID;
        String m_Name;
        Dictionary<String, object> m_Props;

        public VoodooClass(Guid clsid, Guid libid, String name, Dictionary<String, object> properties)
        {
            m_CLSID = clsid;
            m_LIBID = libid;
            m_Name = name;
            m_Props = properties;
        }

        public VoodooClass(Guid clsid, Dictionary<String, object> props)
        {
            m_CLSID = clsid;
            m_Props = props;

            object pvalue;
            if (m_Props.TryGetValue("libid", out pvalue))
            {
                m_LIBID = (Guid)pvalue;
                m_Props.Remove("libid");
            }
            if (m_Props.TryGetValue("name", out pvalue))
            {
                m_Name = (String)pvalue;
                m_Props.Remove("name");
            }
        }

        public Guid CLSID
        {
            get { return m_CLSID; }
        }

        public Guid LIBID
        {
            get { return m_LIBID; }
        }

        public String Name
        {
            get { return m_Name; }
        }
    }

    public class VoodooHook
    {
        String m_Name, m_Target, m_Config;

        public String Name { get { return m_Name; } }
        public String Target { get { return m_Target; } }
        public String Config { get { return m_Config; } }
    }

    public class VoodooRegistry
    {
        private Dictionary<Guid, VoodooModule> m_Modules;
        private Dictionary<Guid, VoodooClass> m_Classes;
        private Dictionary<String, VoodooHook> m_Hooks;

        public VoodooClass GetClass(Guid dCLSID)
        {
            return m_Classes[dCLSID];
        }

        public VoodooModule GetModule(Guid dLIBID)
        {
            return m_Modules[dLIBID];
        }

        public static VoodooRegistry LoadFromRegistryKey(RegistryKey root)
        {
            if (root == null)
            {
                throw new Exception("Invalid registry key.");
            }

            VoodooRegistry vreg = new VoodooRegistry();

            RegistryKey modulesKey = root.OpenSubKey("modules");
            if (modulesKey != null)
            {
                String[] moduleKeys = modulesKey.GetSubKeyNames();
                foreach (String moduleKeyName in moduleKeys)
                {
                    RegistryKey moduleKey = modulesKey.OpenSubKey(moduleKeyName);

                    Dictionary<String, object> props = new Dictionary<String, object>(moduleKey.ValueCount);

                    foreach (String propName in moduleKey.GetValueNames())
                    {
                        props.Add(propName.ToLower(), moduleKey.GetValue(propName));
                    }

                    Guid libid = new Guid(moduleKeyName);
                    vreg.m_Modules.Add(libid, new VoodooModule(libid, props));
                }
            }

            RegistryKey classesKey = root.OpenSubKey("classes");
            if (classesKey != null)
            {
                String[] classKeys = classesKey.GetSubKeyNames();
                foreach (String classKeyName in classKeys)
                {
                    RegistryKey classKey = classesKey.OpenSubKey(classKeyName);

                    Dictionary<String, object> props = new Dictionary<String, object>(classKey.ValueCount);

                    foreach (String propName in classKey.GetValueNames())
                    {
                        props.Add(propName.ToLower(), classKey.GetValue(propName));
                    }
                    
                    Guid clsid = new Guid(classKeyName);
                    vreg.m_Classes.Add(clsid, new VoodooClass(clsid, props));
                }
            }

            RegistryKey hooksKey = root.OpenSubKey("hooks");

            return vreg;
        }
    }
}
