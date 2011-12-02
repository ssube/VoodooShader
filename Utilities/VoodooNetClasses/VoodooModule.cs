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
using System.ComponentModel;
using System.Runtime.Serialization;
using System.Security.Permissions;
using Microsoft.Win32;

namespace VoodooNetClasses
{
    [Serializable]
    public class VoodooModule : INotifyPropertyChanged, ISerializable, IVoodooRegistryObject
    {
        public event PropertyChangedEventHandler PropertyChanged;

        Guid m_LibID, m_PackID;
        String m_Name, m_Path, m_Config, m_Props;

        public VoodooModule()
        {

        }

        public VoodooModule(Guid LibID, Guid PackID, String Name, String Path, String Config, String Props)
        {
            m_LibID  = LibID;
            m_PackID = PackID;
            m_Name   = Name;
            m_Path   = Path;
            m_Config = Config;
            m_Props  = Props;
        }

        // ISerializable
        protected VoodooModule(SerializationInfo info, StreamingContext context)
        {
            m_LibID  = new Guid(info.GetString("LibID"));
            m_PackID = new Guid(info.GetString("PackID"));
            m_Name   = info.GetString("Name");
            m_Path   = info.GetString("Path");
            m_Config = info.GetString("Config");
            m_Props  = info.GetString("Props");
        }

        [SecurityPermissionAttribute(SecurityAction.Demand, SerializationFormatter = true)]
        public virtual void GetObjectData(SerializationInfo info, StreamingContext context)
        {
            info.AddValue("LibID",  m_LibID.ToString("D"));
            info.AddValue("PackID", m_PackID.ToString("D"));
            info.AddValue("Name",   m_Name);
            info.AddValue("Path",   m_Path);
            info.AddValue("Config", m_Config);
            info.AddValue("Props",  m_Props);
        }

        // IVoodooRegistryObject
        public String GetID()
        {
            return m_LibID.ToString("D");
        }

        public void FromRegistryKey(RegistryKey key)
        {
            try
            {
                String name = key.Name.Substring(key.Name.LastIndexOf('\\') + 1);
                m_LibID = new Guid(name);
            }
            catch (System.Exception ex)
            {
                m_LibID = Guid.Empty;            	
            }
            try
            {
                m_PackID = new Guid(key.GetValue("PackID") as String);
            }
            catch (System.Exception ex)
            {
                m_PackID = Guid.Empty;
            }
            try
            {
                m_Name = key.GetValue("Name") as String;
            }
            catch (System.Exception ex)
            {
                m_Name = String.Empty;
            }
            try
            {
                m_Path = key.GetValue("Path") as String;
            }
            catch (System.Exception ex)
            {
                m_Path = String.Empty;            	
            }
            try
            {
                m_Config = key.GetValue("Config") as String;
            }
            catch (System.Exception ex)
            {
                m_Config = String.Empty;             	
            }
            try
            {
                m_Props = key.GetValue("Props") as String;
            }
            catch (System.Exception ex)
            {
                m_Props = String.Empty;             	
            }
        }

        public void ToRegistryKey(RegistryKey parent)
        {
            String keyName = m_LibID.ToString("D");

            RegistryKey key = parent.OpenSubKey(keyName);
            if (key != null)
            {
                key.Close();
                parent.DeleteSubKeyTree(keyName);
            }

            key = parent.CreateSubKey(keyName, RegistryKeyPermissionCheck.ReadWriteSubTree);

            key.SetValue("PackID", m_PackID.ToString("D"));
            key.SetValue("Name",   m_Name);
            key.SetValue("Path",   m_Path);
            key.SetValue("Config", m_Config);
            key.SetValue("Props",  m_Props);

            key.Close();
        }

        public Guid LibID
        {
            get { return m_LibID; }
            set { m_LibID = value; this.NotifyPropertyChanged("LibID"); }
        }

        public Guid PackID
        {
            get { return m_PackID; }
            set { m_PackID = value; this.NotifyPropertyChanged("PackID"); }
        }

        public String Name
        {
            get { return m_Name; }
            set { m_Name = value; this.NotifyPropertyChanged("Name"); }
        }

        public String Path
        {
            get { return m_Path; }
            set { m_Path = value; this.NotifyPropertyChanged("Path"); }
        }

        public String Config
        {
            get { return m_Config; }
            set { m_Config = value; this.NotifyPropertyChanged("Config"); }
        }

        public String Props
        {
            get { return m_Props; }
            set { m_Props = value; this.NotifyPropertyChanged("Props"); }
        }

        private void NotifyPropertyChanged(String name)
        {
            if (PropertyChanged != null) { PropertyChanged(this, new PropertyChangedEventArgs(name)); }
        }
    }
}
