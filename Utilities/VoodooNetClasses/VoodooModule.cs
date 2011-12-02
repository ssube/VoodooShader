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

namespace VoodooNetClasses
{
    [Serializable]
    public class VoodooModule : INotifyPropertyChanged, ISerializable
    {
        public event PropertyChangedEventHandler PropertyChanged;

        Guid m_LibID, m_PackID;
        String m_Name, m_Path, m_Config, m_Props;

        public VoodooModule(Guid LibID, Guid PackID, String Name, String Path, String Config, String Props)
        {
            m_LibID = LibID;
            m_PackID = PackID;
            m_Name = Name;
            m_Path = Path;
            m_Config = Config;
            m_Props = Props;
        }

        protected VoodooModule(SerializationInfo info, StreamingContext context)
        {
            m_LibID = new Guid(info.GetString("LibID"));
            m_PackID = new Guid(info.GetString("PackID"));
            m_Name = info.GetString("Name");
            m_Path = info.GetString("Path");
            m_Config = info.GetString("Config");
            m_Props = info.GetString("Props");
        }

        [SecurityPermissionAttribute(SecurityAction.Demand, SerializationFormatter = true)]
        public virtual void GetObjectData(SerializationInfo info, StreamingContext context)
        {
            info.AddValue("LibID", m_LibID.ToString("D"));
            info.AddValue("PackID", m_PackID.ToString("D"));
            info.AddValue("Name", m_Name);
            info.AddValue("Path", m_Path);
            info.AddValue("Config", m_Config);
            info.AddValue("Props", m_Props);
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
