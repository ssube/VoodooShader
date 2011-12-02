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
    namespace VoodooRegistry
    {
        public class VoodooElement
        {
            protected List<VoodooProperty> m_Props;

            public List<VoodooProperty> Properties
            {
                get { return m_Props; }
                set { m_Props = value; }
            }

            public VoodooElement()
            {
                m_Props = new List<VoodooProperty>();
            }
        }

        public class VoodooModule : VoodooElement
        {
            Guid m_LIBID;

            public VoodooModule(Guid libid, List<VoodooProperty> props)
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
        }

        public class VoodooClass : VoodooElement
        {
            Guid m_CLSID;

            public VoodooClass(Guid clsid, List<VoodooProperty> props)
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
        }

        public class VoodooHook : VoodooElement
        {
            String m_ID;

            public VoodooHook(String id, List<VoodooProperty> props)
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
        }

        public class VoodooRegistry : VoodooElement
        {
            private List<String> m_Remotes;
            private Dictionary<Guid, VoodooPackage> m_Packages;
            private Dictionary<Guid, VoodooModule> m_Modules;
            private Dictionary<Guid, VoodooClass> m_Classes;
            private Dictionary<String, VoodooHook> m_Hooks;

            public List<String> Remotes
            {
                get { return m_Remotes; }
            }

            public Dictionary<Guid, VoodooPackage> Packages
            {
                get { return m_Packages; }
            }

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

            public VoodooRegistry()
                : base()
            {
                m_Remotes = new List<string>();
                m_Packages = new Dictionary<Guid, VoodooPackage>();
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
                    vreg.m_Props.Add(new VoodooProperty(propName.ToLower(), Convert.ToString(root.GetValue(propName))));
                }

                RegistryKey remotesKey = root.OpenSubKey("remotes");
                if (remotesKey != null)
                {
                    foreach (String remoteID in remotesKey.GetValueNames())
                    {
                        vreg.m_Remotes.Add(Convert.ToString(remotesKey.GetValue(remoteID)));
                    }
                }

                RegistryKey packagesKey = root.OpenSubKey("packages");
                if (packagesKey != null)
                {
                    foreach (String packageKeyName in packagesKey.GetSubKeyNames())
                    {
                        RegistryKey packageKey = packagesKey.OpenSubKey(packageKeyName);
                        if (packageKey != null)
                        {
                            Guid libid = new Guid(packageKeyName);

                            List<VoodooProperty> props = new List<VoodooProperty>(packageKey.ValueCount);

                            foreach (String propName in packageKey.GetValueNames())
                            {
                                props.Add(new VoodooProperty(propName.ToLower(), Convert.ToString(packageKey.GetValue(propName))));
                            }

                            vreg.m_Packages.Add(libid, new VoodooPackage(libid, props));
                        }
                    }
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

                            List<VoodooProperty> props = new List<VoodooProperty>(moduleKey.ValueCount);

                            foreach (String propName in moduleKey.GetValueNames())
                            {
                                props.Add(new VoodooProperty(propName.ToLower(), Convert.ToString(moduleKey.GetValue(propName))));
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

                            List<VoodooProperty> props = new List<VoodooProperty>(classKey.ValueCount);

                            foreach (String propName in classKey.GetValueNames())
                            {
                                props.Add(new VoodooProperty(propName.ToLower(), Convert.ToString(classKey.GetValue(propName))));
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
                            List<VoodooProperty> props = new List<VoodooProperty>(hookKey.ValueCount);

                            foreach (String propName in hookKey.GetValueNames())
                            {
                                props.Add(new VoodooProperty(propName.ToLower(), Convert.ToString(hookKey.GetValue(propName))));
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

                vreg.m_Props = node.Tag as List<VoodooProperty>;

                TreeNode remotesNode = node.Nodes["remotes"];
                if (remotesNode != null)
                {
                    vreg.m_Remotes = remotesNode.Tag as List<String>;
                }

                TreeNode packagesNode = node.Nodes["packages"];
                if (packagesNode != null)
                {
                    foreach (TreeNode packageNode in packagesNode.Nodes)
                    {
                        Guid libid = new Guid(packageNode.Name);
                        if (!vreg.m_Packages.ContainsKey(libid))
                        {
                            vreg.m_Packages.Add(libid, new VoodooPackage(libid, packageNode.Tag as List<VoodooProperty>));
                        }
                    }
                }

                TreeNode modulesNode = node.Nodes["modules"];
                if (modulesNode != null)
                {
                    foreach (TreeNode moduleNode in modulesNode.Nodes)
                    {
                        Guid libid = new Guid(moduleNode.Name);
                        if (!vreg.m_Modules.ContainsKey(libid))
                        {
                            vreg.m_Modules.Add(libid, new VoodooModule(libid, moduleNode.Tag as List<VoodooProperty>));
                        }
                    }
                }

                TreeNode classesNode = node.Nodes["classes"];
                if (classesNode != null)
                {
                    foreach (TreeNode classNode in classesNode.Nodes)
                    {
                        Guid clsid = new Guid(classNode.Name);
                        if (!vreg.m_Classes.ContainsKey(clsid))
                        {
                            vreg.m_Classes.Add(clsid, new VoodooClass(clsid, classNode.Tag as List<VoodooProperty>));
                        }
                    }
                }

                TreeNode hooksNode = node.Nodes["hooks"];
                if (hooksNode != null)
                {
                    foreach (TreeNode hookNode in hooksNode.Nodes)
                    {
                        if (!vreg.m_Hooks.ContainsKey(hookNode.Name))
                        {
                            vreg.m_Hooks.Add(hookNode.Name, new VoodooHook(hookNode.Name, hookNode.Tag as List<VoodooProperty>));
                        }
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

                foreach (VoodooProperty prop in m_Props)
                {
                    root.SetValue(prop.Name, prop.Value, RegistryValueKind.String);
                }

                RegistryKey remotesKey = root.CreateSubKey("remotes");
                int counter = 0;
                foreach (String remote in m_Remotes)
                {
                    remotesKey.SetValue(Convert.ToString(counter++), remote, RegistryValueKind.String);
                }

                RegistryKey packagesKey = root.CreateSubKey("packages");
                foreach (KeyValuePair<Guid, VoodooPackage> packagePair in m_Packages)
                {
                    RegistryKey packageKey = packagesKey.CreateSubKey(Convert.ToString(packagePair.Key));

                    foreach (VoodooProperty prop in packagePair.Value.Properties)
                    {
                        packageKey.SetValue(prop.Name, prop.Value, RegistryValueKind.String);
                    }
                }

                RegistryKey modulesKey = root.CreateSubKey("modules");
                foreach (KeyValuePair<Guid, VoodooModule> modulePair in m_Modules)
                {
                    RegistryKey moduleKey = modulesKey.CreateSubKey(Convert.ToString(modulePair.Key));

                    foreach (VoodooProperty prop in modulePair.Value.Properties)
                    {
                        moduleKey.SetValue(prop.Name, prop.Value, RegistryValueKind.String);
                    }
                }

                RegistryKey classesKey = root.CreateSubKey("classes");
                foreach (KeyValuePair<Guid, VoodooClass> classPair in m_Classes)
                {
                    RegistryKey classKey = classesKey.CreateSubKey(Convert.ToString(classPair.Key));

                    foreach (VoodooProperty prop in classPair.Value.Properties)
                    {
                        classKey.SetValue(prop.Name, prop.Value, RegistryValueKind.String);
                    }
                }

                RegistryKey hooksKey = root.CreateSubKey("hooks");
                foreach (KeyValuePair<String, VoodooHook> hookPair in m_Hooks)
                {
                    RegistryKey hookKey = hooksKey.CreateSubKey(hookPair.Key);

                    foreach (VoodooProperty prop in hookPair.Value.Properties)
                    {
                        hookKey.SetValue(prop.Name, prop.Value, RegistryValueKind.String);
                    }
                }
            }

            public TreeNode ToTreeNode()
            {
                TreeNode node = new TreeNode("Voodoo Registry");
                node.Tag = m_Props;

                TreeNode remotesNode = node.Nodes.Add("remotes", "Remotes");
                remotesNode.Tag = m_Remotes;

                TreeNode packagesNode = node.Nodes.Add("packages", "Packages");
                foreach (KeyValuePair<Guid, VoodooPackage> vpackage in m_Packages)
                {
                    String name = Convert.ToString(vpackage.Key);
                    packagesNode.Nodes.Add(name, name).Tag = vpackage.Value.Properties;
                }

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
                if (!m_Hooks.ContainsKey(vhook.ID))
                {
                    m_Hooks.Add(vhook.ID, vhook);
                    return true;
                }
                else
                {
                    return false;
                }
            }
        }
    }
}
