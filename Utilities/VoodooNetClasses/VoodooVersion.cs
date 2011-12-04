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

namespace VoodooNetClasses
{
    [XmlType("ChangeSet", Namespace = "http://www.voodooshader.com/manifests/Voodoo.xsd")]
    [XmlRoot("ChangeSet", Namespace = "http://www.voodooshader.com/manifests/Voodoo.xsd", IsNullable = false)]
    public class VoodooChangeSet
    {
        [XmlElement("Module")]
        public List<VoodooModule> Modules { get; set; }

        [XmlElement("Default")]
        public List<VoodooDefault> Defaults { get; set; }

        [XmlElement("File")]
        public List<String> Files { get; set; }

        public VoodooChangeSet()
        {
            Modules = new List<VoodooModule>();
            Defaults = new List<VoodooDefault>();
            Files = new List<String>();
        }

        public void AddModules(VoodooModule[] modules)
        {
            Modules.AddRange(modules);
        }

        public void AddClasses(VoodooDefault[] defaults)
        {
            Defaults.AddRange(defaults);
        }

        public void AddFiles(String[] files)
        {
            Files.AddRange(files);
        }

        public void RemoveModules(Predicate<VoodooModule> filter)
        {
            Modules.RemoveAll(filter);
        }

        public void RemoveClasses(Predicate<VoodooDefault> filter)
        {
            Defaults.RemoveAll(filter);
        }

        public void RemoveFiles(Predicate<String> filter)
        {
            Files.RemoveAll(filter);
        }
    }

    [XmlType("Version", Namespace = "http://www.voodooshader.com/manifests/Voodoo.xsd")]
    [XmlRoot("Version", Namespace = "http://www.voodooshader.com/manifests/Voodoo.xsd", IsNullable = false)]
    public class VoodooVersion
    {
        [XmlAttribute("id")]
        public String Name { get; set; }

        [XmlAttribute("parent")]
        public String Parent { get; set; }
        
        [XmlElement("Remove")]
        public VoodooChangeSet Remove { get; set; }
        
        [XmlElement("Create")]
        public VoodooChangeSet Create { get; set; }

        [XmlElement("Messages")]
        public VoodooTranslationSet Messages { get; set; }
        
        public VoodooVersion()
        {
            Name = String.Empty;
            Remove = new VoodooChangeSet();
            Create = new VoodooChangeSet();
            Messages = new VoodooTranslationSet();
        }
    }
}
