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
    [XmlType("Class", Namespace = "http://www.voodooshader.com/manifests/Voodoo.xsd")]
    [XmlRoot("Class", Namespace = "http://www.voodooshader.com/manifests/Voodoo.xsd", IsNullable = false)]
    public class VoodooHook : IVoodooRegistryObject
    {
        [XmlElement("Active", DataType = "xs:boolean")]
        public Boolean Active { get; set; }
        [XmlElement("Name")]
        public String Name { get; set; }
        [XmlElement("Target")]
        public String Target { get; set; }
        [XmlElement("Config")]
        public String Config { get; set; }

        public VoodooHook()
        {
            Active = false;
            Name = Target = Config = String.Empty;
        }

        public VoodooHook(bool iActive, String iName, String iTarget, String iConfig)
        {
            Active = iActive;
            Name   = iName;
            Target = iTarget;
            Config = iConfig;
        }
        
        public void FromRegistryKey(RegistryKey key)
        {
            try
            {
                Active = Convert.ToBoolean(key.GetValue("Active") as String);
            }
            catch (System.Exception)
            {
                Active = false;
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
                Target = key.GetValue("Target") as String;
                if (Target == null)
                {
                    Target = String.Empty;
                }
            }
            catch (System.Exception)
            {
                Target = String.Empty;            	
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
        }

        public void ToRegistryKey(RegistryKey parent)
        {
            String keyName = null;

            List<String> otherNames = new List<String>(parent.GetSubKeyNames());
            int i = 0;
            while (++i < 100)
            {
                if (!otherNames.Contains(i.ToString()))
                {
                    keyName = i.ToString();
                    break;
                }
            }

            if (keyName == null)
            {
                keyName = Guid.NewGuid().ToString("N");
            }

            RegistryKey key = parent.OpenSubKey(keyName);
            if (key != null)
            {
                key.Close();
                parent.DeleteSubKeyTree(keyName);
            }

            key = parent.CreateSubKey(keyName, RegistryKeyPermissionCheck.ReadWriteSubTree);

            if (Active)
            {
                key.SetValue("Active", Active.ToString().ToLower());
            }

            key.SetValue("Name",   Name);
            key.SetValue("Target", Target);
            key.SetValue("Config", Config);

            key.Close();
        }
    }
}
