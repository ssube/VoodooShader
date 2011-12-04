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
using System.Security.Permissions;
using System.Xml.Serialization;

namespace VoodooNetClasses
{
    public class VoodooVersionChangeSet : IXmlSerializable
    {
        List<VoodooClass> m_Classes;
        List<String> m_Files;

        [XmlArray("Modules")]
        [XmlArrayElement("Module")]
        List<VoodooModule> Modules { get; set; }

        public VoodooVersionChangeSet()
        {
            Modules = new List<VoodooModule>();
            m_Classes = new List<VoodooClass>();
            m_Files = new List<String>();
        }

        public void AddModules(VoodooModule[] modules)
        {
            Modules.AddRange(modules);
        }

        public void AddClasses(VoodooClass[] classes)
        {
            m_Classes.AddRange(classes);
        }

        public void AddFiles(String[] files)
        {
            m_Files.AddRange(files);
        }

        public void RemoveModules(Predicate<VoodooModule> filter)
        {
            Modules.RemoveAll(filter);
        }

        public void RemoveClasses(Predicate<VoodooClass> filter)
        {
            m_Classes.RemoveAll(filter);
        }

        public void RemoveFiles(Predicate<String> filter)
        {
            m_Files.RemoveAll(filter);
        }
    }

    public class VoodooVersion : IXmlSerializable
    {
        String m_Name;
        VoodooVersionChangeSet m_Remove, m_Create;
        VoodooMessages m_Messages;

        [XmlAttribute("name")]
        public String Name
        {
            get { return m_Name; }
        }

        [XmlElement("Remove")]
        public VoodooVersionChangeSet RemoveSet
        {
            get { return m_Remove; }
        }

        [XmlElement("Create")]
        public VoodooVersionChangeSet CreateSet
        {
            get { return m_Create; }
        }

        [XmlElement("Messages")]
        public VoodooMessages Messages
        {
            get { return m_Messages; }
        }

        public VoodooVersion()
        {
            m_Name = String.Empty;
            m_Remove = new VoodooVersionChangeSet();
            m_Create = new VoodooVersionChangeSet();
            m_Messages = new VoodooMessages();
        }
    }
}
