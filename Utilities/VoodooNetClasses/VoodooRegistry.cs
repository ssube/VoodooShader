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
using System.Windows.Forms;
using Microsoft.Win32;

namespace VoodooNetClasses
{
    public class VoodooElement
    {
        protected Dictionary<String, String> m_Props;

        public Dictionary<String, String> Properties
        {
            get { return m_Props; }
            set { m_Props = value; }
        }

        public VoodooElement()
        {
            m_Props = new Dictionary<string, string>();
        }
    }

    public class VoodooModule : VoodooElement
    {
        Guid m_LIBID;

        public VoodooModule(Guid libid, Dictionary<String, String> props)
            : base()
        {
            m_LIBID = libid;
            m_Props = props;
        }

        public Guid LIBID
        {
            get { return m_LIBID; }
            set { m_LIBID = value; } 
        }

        public String Path
        {
            get { return m_Props["modulepath"]; }
            set { m_Props["modulepath"] = value; } 
        }

        public String Config
        {
            get { return m_Props["configpath"]; }
            set { m_Props["configpath"] = value; } 
        }

        public String Remote
        {
            get { return m_Props["remotepath"]; }
            set { m_Props["remotepath"] = value; } 
        }
    }

    public class VoodooClass : VoodooElement
    {
        Guid m_CLSID;

        public VoodooClass(Guid clsid, Dictionary<String, String> props)
            : base()
        {
            m_CLSID = clsid;
            m_Props = props;
        }

        public Guid CLSID
        {
            get { return m_CLSID; }
            set { m_CLSID = value; } 
        }

        public Guid LIBID
        {
            get { return new Guid(m_Props["libid"]); }
            set { m_Props["libid"] = Convert.ToString(value); } 
        }

        public String Name
        {
            get { return m_Props["name"]; }
            set { m_Props["name"] = value; } 
        }
    }

    public class VoodooHook : VoodooElement
    {
        String m_ID;

        public VoodooHook(String id, Dictionary<String, String> props)
            : base()
        {
            m_ID = id;
            m_Props = props;
        }

        public String ID
        {
            get { return m_ID; }
            set { m_ID = value; }
        }

        public String Name 
        { 
            get { return m_Props["name"]; } 
            set { m_Props["name"] = value; } 
        }

        public String Target 
        {
            get { return m_Props["target"]; }
            set { m_Props["target"] = value; } 
        }

        public String Config 
        {
            get { return m_Props["config"]; }
            set { m_Props["config"] = value; } 
        }
    }

    public class VoodooRegistry : VoodooElement
    {
        private Dictionary<Guid, VoodooModule> m_Modules;
        private Dictionary<Guid, VoodooClass> m_Classes;
        private Dictionary<String, VoodooHook> m_Hooks;

        public Dictionary<Guid, VoodooModule> Modules
        {
            get { return m_Modules; }
        }

        public Dictionary<Guid, VoodooClass> Classes
        {
            get { return m_Classes; }
        }

        public Dictionary<String, VoodooHook> Hooks
        {
            get { return m_Hooks; }
        }

        public String Path
        {
            get { return m_Props["path"]; }
            set { m_Props["path"] = value; }
        }

        public VoodooRegistry() : base()
        {
            m_Modules = new Dictionary<Guid, VoodooModule>();
            m_Classes = new Dictionary<Guid, VoodooClass>();
            m_Hooks = new Dictionary<String, VoodooHook>();
        }

        public static VoodooRegistry FromRegistryKey(RegistryKey root)
        {
            if (root == null)
            {
                throw new Exception("Invalid registry key.");
            }

            VoodooRegistry vreg = new VoodooRegistry();

            foreach (String propName in root.GetValueNames())
            {
                vreg.m_Props.Add(propName.ToLower(), Convert.ToString(root.GetValue(propName)));
            }

            RegistryKey modulesKey = root.OpenSubKey("modules");
            if (modulesKey != null)
            {
                foreach (String moduleKeyName in modulesKey.GetSubKeyNames())
                {
                    RegistryKey moduleKey = modulesKey.OpenSubKey(moduleKeyName);
                    if (moduleKey != null)
                    {
                        Guid libid = new Guid(moduleKeyName);

                        Dictionary<String, String> props = new Dictionary<String, String>(moduleKey.ValueCount);

                        foreach (String propName in moduleKey.GetValueNames())
                        {
                            props.Add(propName.ToLower(), Convert.ToString(moduleKey.GetValue(propName)));
                        }

                        vreg.m_Modules.Add(libid, new VoodooModule(libid, props));
                    }
                }
            }

            RegistryKey classesKey = root.OpenSubKey("classes");
            if (classesKey != null)
            {
                foreach (String classKeyName in classesKey.GetSubKeyNames())
                {
                    RegistryKey classKey = classesKey.OpenSubKey(classKeyName);
                    if (classKey != null)
                    {
                        Guid clsid = new Guid(classKeyName);

                        Dictionary<String, String> props = new Dictionary<String, String>(classKey.ValueCount);

                        foreach (String propName in classKey.GetValueNames())
                        {
                            props.Add(propName.ToLower(), Convert.ToString(classKey.GetValue(propName)));
                        }

                        vreg.m_Classes.Add(clsid, new VoodooClass(clsid, props));
                    }
                }
            }

            RegistryKey hooksKey = root.OpenSubKey("hooks");
            if (hooksKey != null)
            {
                foreach (String hookKeyName in hooksKey.GetSubKeyNames())
                {
                    RegistryKey hookKey = classesKey.OpenSubKey(hookKeyName);
                    if (hookKey != null)
                    {
                        Dictionary<String, String> props = new Dictionary<String, String>(hookKey.ValueCount);

                        foreach (String propName in hookKey.GetValueNames())
                        {
                            props.Add(propName.ToLower(), Convert.ToString(hookKey.GetValue(propName)));
                        }

                        vreg.m_Hooks.Add(hookKeyName, new VoodooHook(hookKeyName, props));
                    }
                }
            }

            return vreg;
        }

        public static VoodooRegistry FromTreeNode(TreeNode node)
        {
            if (node == null)
            {
                throw new Exception("Invalid tree node.");
            }

            VoodooRegistry vreg = new VoodooRegistry();

            vreg.m_Props = node.Tag as Dictionary<String, String>;

            TreeNode modulesNode = node.Nodes["modules"];
            if (modulesNode != null)
            {
                foreach (TreeNode moduleNode in modulesNode.Nodes)
                {
                    Guid libid = new Guid(moduleNode.Name);
                    vreg.m_Modules.Add(libid, new VoodooModule(libid, moduleNode.Tag as Dictionary<String, String>));
                }
            }

            TreeNode classesNode = node.Nodes["classes"];
            if (classesNode != null)
            {
                foreach (TreeNode classNode in classesNode.Nodes)
                {
                    Guid clsid = new Guid(classNode.Name);
                    vreg.m_Classes.Add(clsid, new VoodooClass(clsid, classNode.Tag as Dictionary<String, String>));
                }
            }

            TreeNode hooksNode = node.Nodes["hooks"];
            if (hooksNode != null)
            {
                foreach (TreeNode hookNode in hooksNode.Nodes)
                {
                    vreg.m_Hooks.Add(hookNode.Name, new VoodooHook(hookNode.Name, hookNode.Tag as Dictionary<String, String>));
                }
            }

            return vreg;
        }

        public static VoodooRegistry FromXmlNode()
        {
            throw new NotImplementedException();
        }

        public void ToRegistryKey(RegistryKey root)
        {
            foreach (String sub in root.GetSubKeyNames())
            {
                root.DeleteSubKeyTree(sub);
            }

            foreach (String val in root.GetValueNames())
            {
                root.DeleteValue(val);
            }

            foreach (KeyValuePair<String, String> prop in m_Props)
            {
                root.SetValue(prop.Key, prop.Value);
            }

            RegistryKey modulesKey = root.CreateSubKey("modules");
            foreach (KeyValuePair<Guid, VoodooModule> modulePair in m_Modules)
            {
                RegistryKey moduleKey = modulesKey.CreateSubKey(Convert.ToString(modulePair.Key));

                foreach (KeyValuePair<String, String> prop in modulePair.Value.Properties)
                {
                    moduleKey.SetValue(prop.Key, prop.Value);
                }
            }

            RegistryKey classesKey = root.CreateSubKey("classes");
            foreach (KeyValuePair<Guid, VoodooClass> classPair in m_Classes)
            {
                RegistryKey classKey = classesKey.CreateSubKey(Convert.ToString(classPair.Key));

                foreach (KeyValuePair<String, String> prop in classPair.Value.Properties)
                {
                    classKey.SetValue(prop.Key, prop.Value);
                }
            }

            RegistryKey hooksKey = root.CreateSubKey("hooks");
            foreach (KeyValuePair<String, VoodooHook> hookPair in m_Hooks)
            {
                RegistryKey hookKey = hooksKey.CreateSubKey(hookPair.Key);

                foreach (KeyValuePair<String, String> prop in hookPair.Value.Properties)
                {
                    hookKey.SetValue(prop.Key, prop.Value);
                }
            }
        }

        public TreeNode ToTreeNode()
        {
            TreeNode node = new TreeNode("Voodoo Registry");
            node.Tag = m_Props;

            TreeNode modulesNode = node.Nodes.Add("modules", "Modules");
            foreach (KeyValuePair<Guid, VoodooModule> vmodule in m_Modules)
            {
                String name = Convert.ToString(vmodule.Key);
                modulesNode.Nodes.Add(name, name).Tag = vmodule.Value.Properties;
            }

            TreeNode classesNode = node.Nodes.Add("classes", "Classes");
            foreach (KeyValuePair<Guid, VoodooClass> vclass in m_Classes)
            {
                String name = Convert.ToString(vclass.Key);
                classesNode.Nodes.Add(name, name).Tag = vclass.Value.Properties;
            }

            TreeNode hooksNode = node.Nodes.Add("hooks", "Hooks");
            foreach (KeyValuePair<String, VoodooHook> vhook in m_Hooks)
            {
                hooksNode.Nodes.Add(vhook.Key, vhook.Key).Tag = vhook.Value.Properties;
            }

            return node;
        }

        /// <summary>
        /// Adds all modules, classes and hooks from the given VoodooRegistry into this one, as if the appropriate add
        /// element function was called for each (and following the same rules). This does not do any processing of
        /// properties or merge elements, names are considered exclusive.
        /// </summary>
        /// <param name="other">Registry to append to this one.</param>
        public void Append(VoodooRegistry other)
        {
            foreach (KeyValuePair<Guid, VoodooModule> vmodule in other.m_Modules)
            {
                this.AddModule(vmodule.Value);
            }

            foreach (KeyValuePair<Guid, VoodooClass> vclass in other.m_Classes)
            {
                this.AddClass(vclass.Value);
            }

            foreach (KeyValuePair<String, VoodooHook> vhook in other.m_Hooks)
            {
                this.AddHook(vhook.Value);
            }
        }

        public bool AddModule(VoodooModule vmodule)
        {
            if (!m_Modules.ContainsKey(vmodule.LIBID))
            {
                m_Modules.Add(vmodule.LIBID, vmodule);
                return true;
            }
            else
            {
                return false;
            }
        }

        public bool AddClass(VoodooClass vclass)
        {
            if (!m_Classes.ContainsKey(vclass.CLSID))
            {
                m_Classes.Add(vclass.CLSID, vclass);
                return true;
            }
            else
            {
                return false;
            }
        }

        public bool AddHook(VoodooHook vhook)
        {
            if (!m_Hooks.ContainsKey(vhook.Name))
            {
                m_Hooks.Add(vhook.Name, vhook);
                return true;
            }
            else
            {
                return false;
            }
        }
    }
}
