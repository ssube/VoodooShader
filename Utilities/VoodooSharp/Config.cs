/*
 * This file is part of the Voodoo Shader Framework.
 *
 * Copyright (c) 2010-2013 by Sean Sube
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

namespace VoodooSharp
{
    [System.SerializableAttribute()]
    public partial class NamedString
    {
        [XmlAttributeAttribute("Name")]
        public string Name { get; set; }
        [XmlTextAttribute]
        public string Value { get; set; }
    }

    [System.SerializableAttribute()]
    [System.Xml.Serialization.XmlRootAttribute("VoodooConfig", Namespace = "", IsNullable = false)]
    public partial class Config
    {
        public ConfigGlobal Global { get; set; }

        [System.Xml.Serialization.XmlArrayItemAttribute("Class", IsNullable = false)]
        public ConfigClass[] Classes { get; set; }
    }

    [System.SerializableAttribute()]
    [System.Xml.Serialization.XmlTypeAttribute(AnonymousType = true)]
    public partial class ConfigGlobal
    {
        [System.Xml.Serialization.XmlArrayItemAttribute("Variable", IsNullable = false)]
        public NamedString[] Variables { get; set; }
        public ConfigGlobalPlugins Plugins { get; set; }
        public ConfigGlobalLog Log { get; set; }
        public ConfigGlobalClasses Classes { get; set; }
    }

    [System.SerializableAttribute()]
    [System.Xml.Serialization.XmlTypeAttribute(AnonymousType = true)]
    public partial class ConfigGlobalPlugins
    {
        [System.Xml.Serialization.XmlElementAttribute("File")]
        public string[] File { get; set; }

        [System.Xml.Serialization.XmlElementAttribute("Path")]
        public ConfigGlobalPluginsPath[] Path { get; set; }
    }

    [System.SerializableAttribute()]
    [System.Xml.Serialization.XmlTypeAttribute(AnonymousType = true)]
    public partial class ConfigGlobalPluginsPath
    {
        [System.Xml.Serialization.XmlAttributeAttribute()]
        public string Filter { get; set; }

        [System.Xml.Serialization.XmlTextAttribute()]
        public string Value { get; set; }
    }

    [System.SerializableAttribute()]
    [System.Xml.Serialization.XmlTypeAttribute(AnonymousType = true)]
    public partial class ConfigGlobalClasses
    {
        public string Adapter { get; set; }
        public string FileSystem { get; set; }
        public string HookManager { get; set; }
        public string Logger { get; set; }
    }

    [System.SerializableAttribute()]
    [System.Xml.Serialization.XmlTypeAttribute(AnonymousType = true)]
    public partial class ConfigGlobalLog
    {
        public string File { get; set; }
        public string Level { get; set; }
    }

    [System.SerializableAttribute()]
    [System.Xml.Serialization.XmlTypeAttribute(AnonymousType = true)]
    public partial class ConfigClass
    {
        [System.Xml.Serialization.XmlAnyElementAttribute()]
        public System.Xml.XmlElement[] Any { get; set; }

        [System.Xml.Serialization.XmlAttributeAttribute()]
        public string Name { get; set; }
    }
}
