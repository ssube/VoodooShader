/*
 * This file is part of the Voodoo Shader Framework.
 *
 * Copyright (c) 2010-2012 by Sean Sube
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
using System.Text;

namespace VoodooSharp
{
    public class MessageSet
    {
        public String Description { get; set; }
        [System.Xml.Serialization.XmlAttributeAttribute("Id", DataType = "language")]
        public String Language { get; set; }
    }

    public class Default
    {
        public String Name { get; set; }
        public String Filter { get; set; }
        public String Config { get; set; }
    }

    [System.SerializableAttribute()]
    [System.Xml.Serialization.XmlRootAttribute("VoodooPackage", Namespace = "", IsNullable = false)]
    public class Package
    {
        [System.Xml.Serialization.XmlArrayAttribute("Languages")]
        [System.Xml.Serialization.XmlArrayItemAttribute("Language")]
        public List<MessageSet> Languages { get; set; }

        [System.Xml.Serialization.XmlArrayAttribute("Defaults")]
        [System.Xml.Serialization.XmlArrayItemAttribute("Default")]
        public List<Default> Defaults { get; set; }
    }
}
