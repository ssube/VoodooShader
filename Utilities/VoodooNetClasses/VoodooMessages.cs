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
    [XmlType("MessageSet", Namespace = "http://www.voodooshader.com/manifests/Voodoo.xsd")]
    [XmlRoot("MessageSet", Namespace = "http://www.voodooshader.com/manifests/Voodoo.xsd", IsNullable = false)]
    public class VoodooMessageSet
    {
        [XmlAttribute("id", DataType = "language")]
        public String Language { get; set; }

        [XmlElement("Pre")]
        public String Pre { get; set; }
        
        [XmlElement("Post")]
        public String Post { get; set; }

        public VoodooMessageSet()
        {
            Language = Pre = Post = String.Empty;
        }

        public VoodooMessageSet(String iLanguage, String iPre, String iPost)
        {
            Language = iLanguage;
            Pre = iPre;
            Post = iPost;
        }
    }

    [XmlType("TranslationSet", Namespace = "http://www.voodooshader.com/manifests/Voodoo.xsd")]
    [XmlRoot("TranslationSet", Namespace = "http://www.voodooshader.com/manifests/Voodoo.xsd", IsNullable = false)]
    public class VoodooTranslationSet
    {
        [XmlElement("Language")]
        public List<VoodooMessageSet> Languages { get; set; }

        public VoodooMessageSet this[String id]
        {
            get { return Languages.Find(t => t.Language == id); }
        }
    }
}
