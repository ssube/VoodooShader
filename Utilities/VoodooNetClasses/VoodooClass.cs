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
    public class VoodooClass : INotifyPropertyChanged, ISerializable
    {
        public event PropertyChangedEventHandler PropertyChanged;

        Guid m_ClassID, m_PackID;
        String m_Name, m_Props;

        public VoodooClass(Guid ClassID, Guid PackID, String Name, String Props)
        {
            m_ClassID = ClassID;
            m_PackID = PackID;
            m_Name = Name;
            m_Props = Props;
        }

        protected VoodooClass(SerializationInfo info, StreamingContext context)
        {
            m_ClassID = new Guid(info.GetString("ClassID"));
            m_PackID = new Guid(info.GetString("PackID"));
            m_Name = info.GetString("Name");
            m_Props = info.GetString("Props");
        }

        [SecurityPermissionAttribute(SecurityAction.Demand, SerializationFormatter = true)]
        public virtual void GetObjectData(SerializationInfo info, StreamingContext context)
        {
            info.AddValue("ClassID", m_ClassID.ToString("D"));
            info.AddValue("PackID", m_PackID.ToString("D"));
            info.AddValue("Name", m_Name);
            info.AddValue("Props", m_Props);
        }

        public Guid ClassID
        {
            get { return m_ClassID; }
            set { m_ClassID = value; this.NotifyPropertyChanged("ClassID"); }
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
