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
using System.Text;
using System.Security.Cryptography;
using System.Xml;
using System.Xml.Schema;
using System.Xml.Serialization;

namespace VoodooNetClasses
{
    public class VoodooDeserialize
    {
        public static object ValidateDeserialize(String filename, Type objtype)
        {
            // Set the validation settings.
            XmlReaderSettings settings = new XmlReaderSettings();
            settings.ValidationType = ValidationType.Schema;
            settings.ValidationFlags |= XmlSchemaValidationFlags.ProcessInlineSchema;
            settings.ValidationFlags |= XmlSchemaValidationFlags.ProcessSchemaLocation;
            settings.ValidationFlags |= XmlSchemaValidationFlags.ReportValidationWarnings;
            settings.ValidationEventHandler += new ValidationEventHandler(ValidationCallBack);

            // Create the XmlReader object.
            XmlReader reader = XmlReader.Create(filename, settings);
            
            XmlSerializer manifestMaker = new XmlSerializer(objtype);

            return manifestMaker.Deserialize(reader);
        }

        // Display any warnings or errors.
        private static void ValidationCallBack(object sender, ValidationEventArgs args)
        {
            if (args.Severity == XmlSeverityType.Warning)
                Console.WriteLine("\tValidation warning: " + args.Message);
            else
                Console.WriteLine("\tValidation error: " + args.Message);

        }
    }

    public class VoodooHash
    {
        public static String Hash(String str)
        {
            byte[] bytes = System.Text.Encoding.Unicode.GetBytes(str);
            return VoodooHash.Hash(bytes);
        }

        public static String Hash(byte[] bytes)
        {
            MD5 hasher = System.Security.Cryptography.MD5.Create();
            byte[] hashBytes = hasher.ComputeHash(bytes);

            StringBuilder sb = new StringBuilder(hashBytes.Length);
            foreach (byte b in hashBytes)
            {
                sb.Append(b.ToString("X2"));
            }

            return sb.ToString();
        }
    }
}
