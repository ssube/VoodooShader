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
    public partial class Version
    {
        private VSMessageSet[] messagesField;
        private ChangeSet removeField;
        private ChangeSet createField;
        private String idField;
        private string parentField;
        private String descriptionField;
        private String versionuriField;
        public string Description
        {
            get
            {
                return this.descriptionField;
            }
            set
            {
                this.descriptionField = value;
            }
        }
        public string VersionUri
        {
            get
            {
                return this.versionuriField;
            }
            set
            {
                this.versionuriField = value;
            }
        }
        [System.Xml.Serialization.XmlArrayItemAttribute("Language", IsNullable = false)]
        public VSMessageSet[] Messages
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
}
