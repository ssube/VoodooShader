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
    public partial class VSMessageSet
    {
        private String preField;
        private String postField;
        private String idField;
        public String Pre
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
        public String Post
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
        public String Id
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

}
