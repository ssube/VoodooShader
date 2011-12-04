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
using System.Xml.Serialization;
using Microsoft.Win32;

namespace VoodooNetClasses
{
    [XmlType("Module", Namespace = "http://www.voodooshader.com/manifests/Voodoo.xsd")]
    [XmlRoot("Module", Namespace = "http://www.voodooshader.com/manifests/Voodoo.xsd", IsNullable = false)]
    public class VoodooModule : IVoodooRegistryObject
    {
        [XmlElement("LibID", DataType = "Uuid")]
        public Guid LibID { get; set; }

        [XmlElement("PackID", DataType = "Uuid")]
        public Guid PackID { get; set; }

        [XmlElement("Name")]
        public String Name { get; set; }

        [XmlElement("Path")]
        public String Path { get; set; }

        [XmlElement("Config")]
        public String Config { get; set; }

        [XmlElement("Props")]
        public String Props { get; set; }

        public VoodooModule()
        {
            LibID = PackID = Guid.Empty;
            Name = Path = Config = Props = String.Empty;
        }

        public VoodooModule(Guid iLibID, Guid iPackID, String iName, String iPath, String iConfig, String iProps)
        {
            LibID  = iLibID;
            PackID = iPackID;
            Name   = iName;
            Path   = iPath;
            Config = iConfig;
            Props  = iProps;
        }

        // IVoodooRegistryObject
        public void FromRegistryKey(RegistryKey key)
        {
            try
            {
                String name = key.Name.Substring(key.Name.LastIndexOf('\\') + 1);
                LibID = new Guid(name);
                if (LibID == null)
                {
                    LibID = Guid.Empty;
                }
            }
            catch (System.Exception)
            {
                LibID = Guid.Empty;            	
            }
            try
            {
                PackID = new Guid(key.GetValue("PackID") as String);
                if (PackID == null)
                {
                    PackID = Guid.Empty;
                }
            }
            catch (System.Exception)
            {
                PackID = Guid.Empty;
            }
            try
            {
                Name = key.GetValue("Name") as String;
                if (Name == null)
                {
                    Name = String.Empty;
                }
            }
            catch (System.Exception)
            {
                Name = String.Empty;
            }
            try
            {
                Path = key.GetValue("Path") as String;
                if (Path == null)
                {
                    Path = String.Empty;
                }
            }
            catch (System.Exception)
            {
                Path = String.Empty;            	
            }
            try
            {
                Config = key.GetValue("Config") as String;
                if (Config == null)
                {
                    Config = String.Empty;
                }
            }
            catch (System.Exception)
            {
                Config = String.Empty;             	
            }
            try
            {
                Props = key.GetValue("Props") as String;
                if (Props == null)
                {
                    Props = String.Empty;
                }
            }
            catch (System.Exception)
            {
                Props = String.Empty;             	
            }
        }

        public void ToRegistryKey(RegistryKey parent)
        {
            String keyName = LibID.ToString("D");

            RegistryKey key = parent.OpenSubKey(keyName);
            if (key != null)
            {
                key.Close();
                parent.DeleteSubKeyTree(keyName);
            }

            key = parent.CreateSubKey(keyName, RegistryKeyPermissionCheck.ReadWriteSubTree);

            key.SetValue("PackID", PackID.ToString("D"));
            key.SetValue("Name",   Name);
            key.SetValue("Path",   Path);
            key.SetValue("Config", Config);
            key.SetValue("Props",  Props);

            key.Close();
        }

        public List<VoodooClass> GetClasses(String rootpath)
        {
            try
            {
                NativeModule native = new NativeModule(rootpath + "\\" + Path);
                List<VoodooClass> classes = new List<VoodooClass>();
                for (UInt32 i = 0; i < native.Count; ++i)
                {
                    ClassInfo info = native[i];
                    classes.Add(new VoodooClass(info.ClassID, LibID, info.Name, String.Empty));
                }
                return classes;
            }
            catch (Exception)
            {
                return null;
            }
        }
    }
}
