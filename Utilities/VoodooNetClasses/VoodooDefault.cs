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
using System.Xml.Serialization;
using Microsoft.Win32;

namespace VoodooNetClasses
{
    [XmlType("Class", Namespace = "http://www.voodooshader.com/manifests/Voodoo.xsd")]
    [XmlRoot("Class", Namespace = "http://www.voodooshader.com/manifests/Voodoo.xsd", IsNullable = false)]
    public class VoodooDefault : IVoodooRegistryObject
    {
        [XmlElement("DefID", DataType = "Uuid")]
        public Guid DefID { get; set; }

        [XmlElement("Name")]
        public String Name { get; set; }

        [XmlElement("Filter")]
        public String Filter { get; set; }

        [XmlElement("Config")]
        public String Config { get; set; }

        [XmlArray("Packages")]
        [XmlArrayItem("Package")]
        public List<Guid> Packages { get; set; }
        
        public VoodooDefault()
        {
            DefID = Guid.Empty;
            Name = Filter = Config = String.Empty;
            Packages = new List<Guid>();
        }

        public VoodooDefault(Guid iDefID, String iName, String iFilter, String iConfig, Guid[] iPackages = null)
        {
            DefID  = iDefID;
            Name   = iName;
            Filter = iFilter;
            Config = iConfig;

            Packages = new List<Guid>();
            if (iPackages != null)
            {
                Packages.AddRange(iPackages);
            }
        }

        public void FromRegistryKey(RegistryKey key)
        {
            Packages = new List<Guid>();

            try
            {
                DefID = new Guid(key.GetValue("DefID") as String);
                if (DefID == null)
                {
                    DefID = Guid.Empty;
                }
            }
            catch (Exception)
            {
                DefID = Guid.Empty;
            }
            try
            {
                Name = key.GetValue("Name") as String;
                if (Name == null)
                {
                    Name = String.Empty;
                }
            }
            catch (Exception)
            {
                Name = String.Empty;
            }
            try
            {
                Filter = key.GetValue("Filter") as String;
                if (Filter == null)
                {
                    Filter = String.Empty;
                }
            }
            catch (Exception)
            {
                Filter = String.Empty;
            }
            try
            {
                Config = key.GetValue("Config") as String;
                if (Config == null)
                {
                    Config = String.Empty;
                }
            }
            catch (Exception)
            {
                Config = String.Empty;
            }
            try
            {
                String packListCat = key.GetValue("Packages") as String;
                String[] packages = packListCat.Split(';');
                foreach (String package in packages)
                {
                    Packages.Add(new Guid(package));
                }
            }
            catch (Exception)
            {
                Config = String.Empty;
            }
        }

        public void ToRegistryKey(RegistryKey parent)
        {
            String keyName = DefID.ToString("D");

            RegistryKey key = parent.OpenSubKey(keyName);
            if (key != null)
            {
                key.Close();
                parent.DeleteSubKeyTree(keyName);
            }

            key = parent.CreateSubKey(keyName, RegistryKeyPermissionCheck.ReadWriteSubTree);

            key.SetValue("Name", Name);
            key.SetValue("Filter", Filter);
            key.SetValue("Config", Config);

            StringBuilder sb = new StringBuilder();
            Packages.ForEach(t => sb.Append(t.ToString("D") + ";"));
            key.SetValue("Packages", sb.ToString());

            key.Close();
        }
    }
}