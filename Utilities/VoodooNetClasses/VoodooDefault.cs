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
using System.ComponentModel;
using System.Runtime.Serialization;
using System.Security.Permissions;
using System.Xml.Serialization;
using Microsoft.Win32;

namespace VoodooNetClasses
{
    [Serializable]
    public class VoodooDefault : INotifyPropertyChanged, ISerializable, IXmlSerializable, IVoodooRegistryObject
    {
        public event PropertyChangedEventHandler PropertyChanged;

        Guid m_DefID;
        String m_Name, m_Target, m_Config;
        List<Guid> m_Packages;

        [XmlElement("DefID", DataType = "Uuid")]
        public Guid DefID
        {
            get { return m_DefID; }
            set { m_DefID = value; this.NotifyPropertyChanged("DefID"); }
        }

        [XmlElement("Name")]
        public String Name
        {
            get { return m_Name; }
            set { m_Name = value; this.NotifyPropertyChanged("Name"); }
        }

        [XmlElement("Target")]
        public String Target
        {
            get { return m_Target; }
            set { m_Target = value; this.NotifyPropertyChanged("Target"); }
        }

        [XmlElement("Config")]
        public String Config
        {
            get { return m_Config; }
            set { m_Config = value; this.NotifyPropertyChanged("Config"); }
        }

        [XmlArray("Packages")]
        [XmlArrayItem("Package")]
        public List<Guid> Packages
        {
            get { return m_Packages; }
            set { m_Packages = value; this.NotifyPropertyChanged("Packages"); }
        }

        public VoodooDefault()
        {
            m_Packages = new List<Guid>();
        }

        public VoodooDefault(Guid DefID, String Name, String Target, String Config)
        {
            m_DefID  = DefID;
            m_Name   = Name;
            m_Target = Target;
            m_Config = Config;
        }

        public void FromRegistryKey(RegistryKey key)
        {
            try
            {
                String name = key.Name.Substring(key.Name.LastIndexOf('\\') + 1);
                m_DefID = new Guid(name);
                if (m_DefID == null)
                {
                    m_DefID = Guid.Empty;
                }
            }
            catch (System.Exception)
            {
                m_DefID = Guid.Empty;
            }
            try
            {
                m_Name = key.GetValue("Name") as String;
                if (m_Name == null)
                {
                    m_Name = String.Empty;
                }
            }
            catch (System.Exception)
            {
                m_Name = String.Empty;
            }
            try
            {
                m_Target = key.GetValue("Target") as String;
                if (m_Target == null)
                {
                    m_Target = String.Empty;
                }
            }
            catch (System.Exception)
            {
                m_Target = String.Empty;
            }
            try
            {
                m_Config = key.GetValue("Config") as String;
                if (m_Config == null)
                {
                    m_Config = String.Empty;
                }
            }
            catch (System.Exception)
            {
                m_Config = String.Empty;
            }
        }

        public void ToRegistryKey(RegistryKey parent)
        {
            String keyName = m_DefID.ToString("D");

            RegistryKey key = parent.OpenSubKey(keyName);
            if (key != null)
            {
                key.Close();
                parent.DeleteSubKeyTree(keyName);
            }

            key = parent.CreateSubKey(keyName, RegistryKeyPermissionCheck.ReadWriteSubTree);

            key.SetValue("Name", m_Name);
            key.SetValue("Target", m_Target);
            key.SetValue("Config", m_Config);

            key.Close();
        }

        private void NotifyPropertyChanged(String name)
        {
            if (PropertyChanged != null) { PropertyChanged(this, new PropertyChangedEventArgs(name)); }
        }
    }
}