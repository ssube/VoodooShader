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
using System.Xml.Serialization;
using Microsoft.Win32;

namespace VoodooNetClasses
{
    [XmlType("Class", Namespace = "http://www.voodooshader.com/manifests/Voodoo.xsd")]
    [XmlRoot("Class", Namespace = "http://www.voodooshader.com/manifests/Voodoo.xsd", IsNullable = false)]
    public class VoodooClass : IVoodooRegistryObject
    {
        [XmlElement("ClassID", DataType = "Uuid")]
        public Guid ClassID { get; set; }

        [XmlElement("LibID", DataType = "Uuid")]
        public Guid LibID { get; set; }

        [XmlElement("Name")]
        public String Name { get; set; }

        [XmlElement("Props", IsNullable = true)]
        public String Props { get; set; }

        public VoodooClass()
        {
            ClassID = LibID = Guid.Empty;
            Name = Props = String.Empty;
        }

        public VoodooClass(Guid iClassID, Guid iLibID, String iName, String iProps)
        {
            ClassID = iClassID;
            LibID   = iLibID;
            Name    = iName;
            Props   = iProps;
        }

        public void FromRegistryKey(RegistryKey key)
        {
            try
            {
                String name = key.Name.Substring(key.Name.LastIndexOf('\\') + 1);
                ClassID = new Guid(name);
                if (ClassID == null)
                {
                    ClassID = Guid.Empty;
                }
            }
            catch (System.Exception)
            {
                ClassID = Guid.Empty;
            }
            try
            {
                LibID = new Guid(key.GetValue("LibID") as String);
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
            String keyName = ClassID.ToString("D");

            RegistryKey key = parent.OpenSubKey(keyName);
            if (key != null)
            {
                key.Close();
                parent.DeleteSubKeyTree(keyName);
            }

            key = parent.CreateSubKey(keyName, RegistryKeyPermissionCheck.ReadWriteSubTree);

            key.SetValue("Name",   Name);
            key.SetValue("PackID", LibID.ToString("D"));
            key.SetValue("Props",  Props);

            key.Close();
        }
    }
}
