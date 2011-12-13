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
using System.Xml.Serialization;

namespace VoodooSharp
{
    [System.SerializableAttribute()]
    [System.Xml.Serialization.XmlRootAttribute("VoodooConfig", Namespace = "", IsNullable = false)]
    public partial class VSConfig
    {
        private VSConfigGlobal globalField;
        private VSConfigClass[] classesField;

        public VSConfigGlobal Global
        {
            get
            {
                return this.globalField;
            }
            set
            {
                this.globalField = value;
            }
        }

        [System.Xml.Serialization.XmlArrayItemAttribute("Class", IsNullable = false)]
        public VSConfigClass[] Classes
        {
            get
            {
                return this.classesField;
            }
            set
            {
                this.classesField = value;
            }
        }
    }

    [System.SerializableAttribute()]
    [System.Xml.Serialization.XmlTypeAttribute(AnonymousType = true)]
    public partial class VSConfigGlobal
    {
        private NamedString[] variablesField;
        private VSConfigGlobalPlugins pluginsField;
        private VSConfigGlobalClasses classesField;
        private VSConfigGlobalLog logField;

        [System.Xml.Serialization.XmlArrayItemAttribute("Variable", IsNullable = false)]
        public NamedString[] Variables
        {
            get
            {
                return this.variablesField;
            }
            set
            {
                this.variablesField = value;
            }
        }

        public VSConfigGlobalPlugins Plugins
        {
            get
            {
                return this.pluginsField;
            }
            set
            {
                this.pluginsField = value;
            }
        }

        public VSConfigGlobalClasses Classes
        {
            get
            {
                return this.classesField;
            }
            set
            {
                this.classesField = value;
            }
        }

        public VSConfigGlobalLog Log
        {
            get
            {
                return this.logField;
            }
            set
            {
                this.logField = value;
            }
        }
    }

    [System.SerializableAttribute()]
    [System.Xml.Serialization.XmlTypeAttribute(AnonymousType = true)]
    public partial class VSConfigGlobalPlugins
    {
        private string[] fileField;
        private VSConfigGlobalPluginsPath[] pathField;

        [System.Xml.Serialization.XmlElementAttribute("File")]
        public string[] File
        {
            get
            {
                return this.fileField;
            }
            set
            {
                this.fileField = value;
            }
        }

        [System.Xml.Serialization.XmlElementAttribute("Path")]
        public VSConfigGlobalPluginsPath[] Path
        {
            get
            {
                return this.pathField;
            }
            set
            {
                this.pathField = value;
            }
        }
    }

    [System.SerializableAttribute()]
    [System.Xml.Serialization.XmlTypeAttribute(AnonymousType = true)]
    public partial class VSConfigGlobalPluginsPath
    {
        private string filterField;
        private string valueField;

        [System.Xml.Serialization.XmlAttributeAttribute()]
        public string Filter
        {
            get
            {
                return this.filterField;
            }
            set
            {
                this.filterField = value;
            }
        }

        [System.Xml.Serialization.XmlTextAttribute()]
        public string Value
        {
            get
            {
                return this.valueField;
            }
            set
            {
                this.valueField = value;
            }
        }
    }

    [System.SerializableAttribute()]
    [System.Xml.Serialization.XmlTypeAttribute(AnonymousType = true)]
    public partial class VSConfigGlobalClasses
    {
        private string adapterField;
        private string fileSystemField;
        private string hookManagerField;
        private string loggerField;

        public string Adapter
        {
            get
            {
                return this.adapterField;
            }
            set
            {
                this.adapterField = value;
            }
        }

        public string FileSystem
        {
            get
            {
                return this.fileSystemField;
            }
            set
            {
                this.fileSystemField = value;
            }
        }

        public string HookManager
        {
            get
            {
                return this.hookManagerField;
            }
            set
            {
                this.hookManagerField = value;
            }
        }

        public string Logger
        {
            get
            {
                return this.loggerField;
            }
            set
            {
                this.loggerField = value;
            }
        }
    }

    [System.SerializableAttribute()]
    [System.Xml.Serialization.XmlTypeAttribute(AnonymousType = true)]
    public partial class VSConfigGlobalLog
    {
        private string fileField;
        private string levelField;

        public string File
        {
            get
            {
                return this.fileField;
            }
            set
            {
                this.fileField = value;
            }
        }

        public string Level
        {
            get
            {
                return this.levelField;
            }
            set
            {
                this.levelField = value;
            }
        }
    }

    [System.SerializableAttribute()]
    [System.Xml.Serialization.XmlTypeAttribute(AnonymousType = true)]
    public partial class VSConfigClass
    {
        private System.Xml.XmlElement[] anyField;
        private string nameField;

        [System.Xml.Serialization.XmlAnyElementAttribute()]
        public System.Xml.XmlElement[] Any
        {
            get
            {
                return this.anyField;
            }
            set
            {
                this.anyField = value;
            }
        }

        [System.Xml.Serialization.XmlAttributeAttribute()]
        public string Name
        {
            get
            {
                return this.nameField;
            }
            set
            {
                this.nameField = value;
            }
        }
    }
}
