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

namespace VoodooNetClasses
{
    [System.SerializableAttribute()]
    [System.Xml.Serialization.XmlRootAttribute("VoodooRemote", Namespace = "", IsNullable = false)]
    public partial class RemoteManifest
    {
        private Package[] packagesField;
        [System.Xml.Serialization.XmlElementAttribute("Package", IsNullable = false)]
        public Package[] Packages
        {
            get
            {
                return this.packagesField;
            }
            set
            {
                this.packagesField = value;
            }
        }
    }

    [System.SerializableAttribute()]
    public partial class Package
    {
        private Guid packIdField;
        private string nameField;
        private string versionField;
        private string manifestUriField;
        public Guid PackId
        {
            get
            {
                return this.packIdField;
            }
            set
            {
                this.packIdField = value;
            }
        }
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
        public string Version
        {
            get
            {
                return this.versionField;
            }
            set
            {
                this.versionField = value;
            }
        }
        [System.Xml.Serialization.XmlElementAttribute(DataType = "anyURI")]
        public string ManifestUri
        {
            get
            {
                return this.manifestUriField;
            }
            set
            {
                this.manifestUriField = value;
            }
        }
    }

    [System.SerializableAttribute()]
    public partial class NamedString
    {
        private string nameField;
        private string valueField;
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
    public partial class Hook
    {
        private bool activeField;
        private string nameField;
        private string targetField;
        private string configField;
        public bool Active
        {
            get
            {
                return this.activeField;
            }
            set
            {
                this.activeField = value;
            }
        }
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
        public string Target
        {
            get
            {
                return this.targetField;
            }
            set
            {
                this.targetField = value;
            }
        }
        public string Config
        {
            get
            {
                return this.configField;
            }
            set
            {
                this.configField = value;
            }
        }
    }

    [System.SerializableAttribute()]
    public partial class Class
    {
        private Guid classIdField;
        private string nameField;
        private Guid moduleField;
        public Guid ClassId
        {
            get
            {
                return this.classIdField;
            }
            set
            {
                this.classIdField = value;
            }
        }
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
        public Guid Module
        {
            get
            {
                return this.moduleField;
            }
            set
            {
                this.moduleField = value;
            }
        }
    }

    [System.SerializableAttribute()]
    public partial class Remote
    {
        private string nameField;
        private string uriField;
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
        [System.Xml.Serialization.XmlElementAttribute(DataType = "anyURI")]
        public string Uri
        {
            get
            {
                return this.uriField;
            }
            set
            {
                this.uriField = value;
            }
        }
    }

    [System.SerializableAttribute()]
    public partial class Module
    {
        private Guid libIdField;
        private string nameField;
        private string pathField;
        private string configField;
        private Guid packageField;
        public Guid LibId
        {
            get
            {
                return this.libIdField;
            }
            set
            {
                this.libIdField = value;
            }
        }
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
        public string Path
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
        public string Config
        {
            get
            {
                return this.configField;
            }
            set
            {
                this.configField = value;
            }
        }
        public Guid Package
        {
            get
            {
                return this.packageField;
            }
            set
            {
                this.packageField = value;
            }
        }
    }

    [System.SerializableAttribute()]
    public partial class ChangeSet
    {
        private Module[] moduleField;
        private Default[] defaultField;
        private string[] fileField;
        [System.Xml.Serialization.XmlElementAttribute("Module")]
        public Module[] Module
        {
            get
            {
                return this.moduleField;
            }
            set
            {
                this.moduleField = value;
            }
        }
        [System.Xml.Serialization.XmlElementAttribute("Default")]
        public Default[] Default
        {
            get
            {
                return this.defaultField;
            }
            set
            {
                this.defaultField = value;
            }
        }
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
    }

    [System.SerializableAttribute()]
    public partial class Default
    {
        private Guid defIdField;
        private string nameField;
        private string filterField;
        private string configField;
        private Guid[] packagesField;
        public Guid DefId
        {
            get
            {
                return this.defIdField;
            }
            set
            {
                this.defIdField = value;
            }
        }
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
        public string Config
        {
            get
            {
                return this.configField;
            }
            set
            {
                this.configField = value;
            }
        }
        [System.Xml.Serialization.XmlArrayItemAttribute("Package", IsNullable = false)]
        public Guid[] Packages
        {
            get
            {
                return this.packagesField;
            }
            set
            {
                this.packagesField = value;
            }
        }
    }

    [System.SerializableAttribute()]
    public partial class Version
    {
        private MessageSet[] messagesField;
        private ChangeSet removeField;
        private ChangeSet createField;
        private String idField;
        private string parentField;
        [System.Xml.Serialization.XmlArrayItemAttribute("Language", IsNullable = false)]
        public MessageSet[] Messages
        {
            get
            {
                return this.messagesField;
            }
            set
            {
                this.messagesField = value;
            }
        }
        public ChangeSet Remove
        {
            get
            {
                return this.removeField;
            }
            set
            {
                this.removeField = value;
            }
        }
        public ChangeSet Create
        {
            get
            {
                return this.createField;
            }
            set
            {
                this.createField = value;
            }
        }
        [System.Xml.Serialization.XmlAttributeAttribute()]
        public string Id
        {
            get
            {
                return this.idField;
            }
            set
            {
                this.idField = value;
            }
        }
        [System.Xml.Serialization.XmlAttributeAttribute()]
        public string Parent
        {
            get
            {
                return this.parentField;
            }
            set
            {
                this.parentField = value;
            }
        }
    }

    [System.SerializableAttribute()]
    public partial class MessageSet
    {
        private string preField;
        private string postField;
        private string idField;
        public string Pre
        {
            get
            {
                return this.preField;
            }
            set
            {
                this.preField = value;
            }
        }
        public string Post
        {
            get
            {
                return this.postField;
            }
            set
            {
                this.postField = value;
            }
        }
        [System.Xml.Serialization.XmlAttributeAttribute(DataType = "language")]
        public string Id
        {
            get
            {
                return this.idField;
            }
            set
            {
                this.idField = value;
            }
        }
    }

    [System.SerializableAttribute()]
    [System.Xml.Serialization.XmlRootAttribute("VoodooPackage", Namespace = "", IsNullable = false)]
    public partial class PackageManifest
    {
        private Package packageField;
        private string dscriptionField;
        private MessageSet[] messagesField;
        private Version[] versionsField;
        public Package Package
        {
            get
            {
                return this.packageField;
            }
            set
            {
                this.packageField = value;
            }
        }
        public string Description
        {
            get
            {
                return this.dscriptionField;
            }
            set
            {
                this.dscriptionField = value;
            }
        }
        [System.Xml.Serialization.XmlArrayItemAttribute("Language", IsNullable = false)]
        public MessageSet[] Messages
        {
            get
            {
                return this.messagesField;
            }
            set
            {
                this.messagesField = value;
            }
        }
        [System.Xml.Serialization.XmlArrayItemAttribute(IsNullable = false)]
        public Version[] Versions
        {
            get
            {
                return this.versionsField;
            }
            set
            {
                this.versionsField = value;
            }
        }
    }

    [System.SerializableAttribute()]
    [System.Xml.Serialization.XmlRootAttribute("VoodooRegistry", Namespace = "", IsNullable = false)]
    public partial class GlobalRegistry
    {
        private Remote[] remotesField;
        private Package[] packagesField;
        private Module[] modulesField;
        private Class[] classesField;
        private Hook[] hooksField;
        private Default[] defaultsField;
        private string pathField;
        private string languageField;
        private string binPrefixField;
        [System.Xml.Serialization.XmlArrayItemAttribute(IsNullable = false)]
        public Remote[] Remotes
        {
            get
            {
                return this.remotesField;
            }
            set
            {
                this.remotesField = value;
            }
        }
        [System.Xml.Serialization.XmlArrayItemAttribute(IsNullable = false)]
        public Package[] Packages
        {
            get
            {
                return this.packagesField;
            }
            set
            {
                this.packagesField = value;
            }
        }
        [System.Xml.Serialization.XmlArrayItemAttribute(IsNullable = false)]
        public Module[] Modules
        {
            get
            {
                return this.modulesField;
            }
            set
            {
                this.modulesField = value;
            }
        }
        [System.Xml.Serialization.XmlArrayItemAttribute(IsNullable = false)]
        public Class[] Classes
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
        [System.Xml.Serialization.XmlArrayItemAttribute(IsNullable = false)]
        public Hook[] Hooks
        {
            get
            {
                return this.hooksField;
            }
            set
            {
                this.hooksField = value;
            }
        }
        [System.Xml.Serialization.XmlArrayItemAttribute(IsNullable = false)]
        public Default[] Defaults
        {
            get
            {
                return this.defaultsField;
            }
            set
            {
                this.defaultsField = value;
            }
        }
        [System.Xml.Serialization.XmlAttributeAttribute()]
        public string Path
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
        [System.Xml.Serialization.XmlAttributeAttribute(DataType = "language")]
        public string Language
        {
            get
            {
                return this.languageField;
            }
            set
            {
                this.languageField = value;
            }
        }
        [System.Xml.Serialization.XmlAttributeAttribute()]
        public string BinPrefix
        {
            get
            {
                return this.binPrefixField;
            }
            set
            {
                this.binPrefixField = value;
            }
        }
    }
    [System.SerializableAttribute()]
    [System.Xml.Serialization.XmlRootAttribute("VoodooConfig", Namespace = "", IsNullable = false)]
    public partial class ConfigFile
    {
        private ConfigFileGlobal globalField;
        private ConfigFileClass[] classesField;
        public ConfigFileGlobal Global
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
        public ConfigFileClass[] Classes
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
    public partial class ConfigFileGlobal
    {
        private NamedString[] variablesField;
        private ConfigFileGlobalPlugins pluginsField;
        private ConfigFileGlobalClasses classesField;
        private ConfigFileGlobalLog logField;
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
        public ConfigFileGlobalPlugins Plugins
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
        public ConfigFileGlobalClasses Classes
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
        public ConfigFileGlobalLog Log
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
    public partial class ConfigFileGlobalPlugins
    {
        private string[] fileField;
        private ConfigFileGlobalPluginsPath[] pathField;
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
        public ConfigFileGlobalPluginsPath[] Path
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
    public partial class ConfigFileGlobalPluginsPath
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
    public partial class ConfigFileGlobalClasses
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
    public partial class ConfigFileGlobalLog
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
    public partial class ConfigFileClass
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
