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
using System.IO;
using System.Net;
using System.Xml.Serialization;
using Microsoft.Win32;

namespace VoodooNetClasses
{
    [XmlType("Remote", Namespace = "http://www.voodooshader.com/manifests/Voodoo.xsd")]
    [XmlRoot("Remote", Namespace = "http://www.voodooshader.com/manifests/Voodoo.xsd", IsNullable = false)]
    public class VoodooRemote : IVoodooRegistryObject
    {
        [XmlElement("Name")]
        public String Name { get; set; }

        [XmlElement("Uri")]
        public String Uri { get; set; }

        public VoodooRemote()
        {
            Uri = String.Empty;
        }

        public VoodooRemote(String iUri)
        {
            Uri = Uri;
        }

        public void FromRegistryKey(RegistryKey key)
        {
            try
            {
                Uri = key.GetValue("Uri") as String;
                if (Uri == null)
                {
                    Uri = String.Empty;
                }
            }
            catch (Exception)
            {
                Uri = String.Empty;
            }
        }

        public void ToRegistryKey(RegistryKey parent)
        {
            String keyName = VoodooHash.Hash(Uri);

            parent.SetValue(keyName, Uri);
        }

        public VoodooRemoteManifest GetManifest(String path)
        {
            String filename = path + "\\" + VoodooHash.Hash(Uri) + ".xml";

            WebClient client = new WebClient();
            client.DownloadFile(Uri, filename);

            XmlSerializer manifestMaker = new XmlSerializer(typeof(VoodooRemoteManifest));

            return (VoodooRemoteManifest)manifestMaker.Deserialize(new StringReader(filename));
        }
    }
}
