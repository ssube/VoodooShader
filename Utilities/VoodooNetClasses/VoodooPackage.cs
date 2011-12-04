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
using System.Net;
using System.Xml.Serialization;
using Microsoft.Win32;
using System.IO;

namespace VoodooNetClasses
{
    [XmlType("Package", Namespace = "http://www.voodooshader.com/manifests/Voodoo.xsd")]
    [XmlRoot("Package", Namespace = "http://www.voodooshader.com/manifests/Voodoo.xsd", IsNullable = false)]
    public class VoodooPackage : IVoodooRegistryObject
    {
        [XmlElement("PackID", DataType = "Uuid")]
        public Guid PackID { get; set; }

        [XmlElement("Name")]
        public String Name { get; set; }

        [XmlElement("Version")]
        public String Version { get; set; }

        [XmlElement("Date", DataType = "xs:date")]
        public DateTime Date { get; set; }

        [XmlElement("ManifestUri")]
        public String ManifestUri { get; set; }

        [XmlElement("Props")]
        public String Props { get; set; }

        public VoodooPackage()
        {
            PackID = Guid.Empty;
            Name = Version = ManifestUri = Props = String.Empty;
            Date = DateTime.MinValue;
        }

        public VoodooPackage(Guid iPackID, String iName, String iVersion, DateTime iDate, String iManifestUri, String iProps)
        {
            PackID  = iPackID;
            Name    = iName;
            Version = iVersion;
            Date    = iDate;
            ManifestUri = iManifestUri;
            Props   = iProps;
        }

        public void FromRegistryKey(RegistryKey key)
        {
            try
            {
                String name = key.Name.Substring(key.Name.LastIndexOf('\\') + 1);
                PackID = new Guid(name);
                if (PackID == null)
                {
                    PackID = Guid.Empty;
                }
            }
            catch (System.Exception)
            {
                PackID = Guid.Empty;
            }
            try
            {
                Name = key.GetValue("Name") as String;
                if (Name == null)
                {
                    Name = String.Empty;
                }
            }
            catch (System.Exception)
            {
                Name = String.Empty;
            }
            try
            {
                Version = key.GetValue("Version") as String;
                if (Version == null)
                {
                    Version = String.Empty;
                }
            }
            catch (System.Exception)
            {
                Version = String.Empty;
            }
            try
            {
                Date = Convert.ToDateTime(key.GetValue("Date") as String);
                if (Date == null)
                {
                    Date = DateTime.MinValue;
                }
            }
            catch (System.Exception)
            {
                Date = DateTime.MinValue;
            }
            try
            {
                ManifestUri = key.GetValue("ManifestUri") as String;
                if (ManifestUri == null)
                {
                    ManifestUri = String.Empty;
                }
            }
            catch (System.Exception)
            {
                ManifestUri = String.Empty;
            }
            try
            {
                Props = key.GetValue("Props") as String;
                if (Props == null)
                {
                    Props = String.Empty;
                }
            }
            catch (System.Exception)
            {
                Props = String.Empty;            	
            }
        }

        public void ToRegistryKey(RegistryKey parent)
        {
            String keyName = PackID.ToString("D");

            RegistryKey key = parent.OpenSubKey(keyName);
            if (key != null)
            {
                key.Close();
                parent.DeleteSubKeyTree(keyName);
            }

            key = parent.CreateSubKey(keyName, RegistryKeyPermissionCheck.ReadWriteSubTree);

            key.SetValue("Name",    Name);
            key.SetValue("Version", Version);
            key.SetValue("Date", Date);
            key.SetValue("ManifestUri", ManifestUri);
            key.SetValue("Props",   Props);

            key.Close();
        }

        public VoodooPackageManifest GetManifest(String path)
        {
            String filename = path + "\\packages\\" + PackID.ToString("D") + ".xml";

            WebClient client = new WebClient();
            client.DownloadFile(ManifestUri, filename);

            XmlSerializer manifestMaker = new XmlSerializer(typeof(VoodooPackageManifest));

            return (VoodooPackageManifest)manifestMaker.Deserialize(new StringReader(filename));
        }
    }
}
