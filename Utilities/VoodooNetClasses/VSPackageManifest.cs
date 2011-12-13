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
    [System.Xml.Serialization.XmlRootAttribute("VoodooPackage", Namespace = "", IsNullable = false)]
    public partial class PackageManifest
    {
        private Package packageField;
        private string dscriptionField;
        private string packageUriField;
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
        public string PackageUri
        {
            get
            {
                return this.packageUriField;
            }
            set
            {
                this.packageUriField = value;
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
}
