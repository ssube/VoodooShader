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
using System.Runtime.Serialization;
using System.Security.Permissions;

namespace VoodooNetClasses
{
    class VoodooVersion : ISerializable
    {
        String m_Name;
        VoodooRegistry m_Remove, m_Create;

        public VoodooVersion()
        {
            m_Name = String.Empty;
            m_Remove = new VoodooRegistry();
            m_Create = new VoodooRegistry();
        }

        public VoodooVersion(String Name, VoodooRegistry Remove, VoodooRegistry Create)
        {
            m_Name = Name;
            m_Remove  = Remove;
            m_Create  = Create;
        }

        protected VoodooVersion(SerializationInfo info, StreamingContext context)
        {
            m_Name = info.GetString("Name");
            m_Remove = (VoodooRegistry)info.GetValue("Remove", typeof(VoodooRegistry));
            m_Create = (VoodooRegistry)info.GetValue("Create", typeof(VoodooRegistry));
        }

        [SecurityPermissionAttribute(SecurityAction.Demand, SerializationFormatter = true)]
        public virtual void GetObjectData(SerializationInfo info, StreamingContext context)
        {
            info.AddValue("Name", m_Name);
            info.AddValue("Remove", m_Remove);
            info.AddValue("Create", m_Create);
        }
    }
}
