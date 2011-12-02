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
    public class VoodooRemote : VoodooObject, INotifyPropertyChanged, ISerializable, IVoodooRegistryObject
    {
        public event PropertyChangedEventHandler PropertyChanged;

        String m_Uri;

        public VoodooRemote()
        {

        }

        public VoodooRemote(String Uri)
        {
            m_Uri = Uri;
        }

        protected VoodooRemote(SerializationInfo info, StreamingContext context)
        {
            m_Uri = info.GetString("Uri");
        }

        [SecurityPermissionAttribute(SecurityAction.Demand, SerializationFormatter = true)]
        public virtual void GetObjectData(SerializationInfo info, StreamingContext context)
        {
            info.AddValue("Uri", m_Uri);
        }

        public override String GetID()
        {
            return m_Uri;
        }

        public void FromRegistryKey(RegistryKey key)
        {
            m_Uri = key.GetValue("Uri") as String;
        }

        public void ToRegistryKey(RegistryKey parent)
        {
            String keyName = VoodooHash.Hash(m_Uri);

            RegistryKey key = parent.OpenSubKey(keyName);
            if (key != null)
            {
                key.Close();
                parent.DeleteSubKeyTree(keyName);
            }

            key = parent.CreateSubKey(keyName, RegistryKeyPermissionCheck.ReadWriteSubTree);

            key.SetValue("Uri", m_Uri);

            key.Close();
        }

        public String Uri
        {
            get { return m_Uri; }
            set { m_Uri = value; this.NotifyPropertyChanged("Uri"); }
        }

        private void NotifyPropertyChanged(String name)
        {
            if (PropertyChanged != null) { PropertyChanged(this, new PropertyChangedEventArgs(name)); }
        }
    }
}
