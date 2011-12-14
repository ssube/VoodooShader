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
using System.IO;
using System.Net;
using System.Web;
using System.Xml.Serialization;

namespace VoodooSharp
{
    [System.SerializableAttribute()]
    public partial class PackageVersion
    {
        [System.Xml.Serialization.XmlArrayItemAttribute("Language", IsNullable = false)]
        public List<MessageSet> Messages { get; set; }

        public string Description { get; set; }
        public string VersionUri { get; set; }
        public ChangeSet Remove { get; set; }
        public ChangeSet Create { get; set; }

        [System.Xml.Serialization.XmlAttributeAttribute()]
        public string Id { get; set; }

        [System.Xml.Serialization.XmlAttributeAttribute()]
        public string Prev { get; set; }
        
        public bool Install(String root)
        {
            GlobalRegistry.Instance.Write();
            GlobalRegistry.Instance.Update();

            Console.WriteLine("Installing version: " + Id);

            if (Remove != null)
            {
                if (Remove.File != null)
                {
                    foreach (CheckedFile basename in Remove.File)
                    {
                        try
                        {
                            String fullpath = Path.GetFullPath(Path.Combine(GlobalRegistry.Instance.Path, basename.Filename));
                            if (!fullpath.StartsWith(GlobalRegistry.Instance.Path))
                            {
                                Console.WriteLine("Illegal file path, aborting version change.", fullpath);
                                Console.WriteLine("This may be a security risk, please contact the package developers and notify the Voodoo Shader developers.");
                                Console.WriteLine("  File: {0}", basename);
                                return false;
                            }
                            File.Delete(fullpath);
                        }
                        catch (Exception exc)
                        {
                            Console.WriteLine("Error removing file. This file may need to be removed manually.");
                            Console.WriteLine("  File: {0}", basename);
                            Console.WriteLine("  Error: {0}", exc.Message);
                        }
                    }
                }

                if (Remove.Default != null)
                {
                    foreach (Default def in Remove.Default)
                    {
                        Console.WriteLine("Removing Default: {0}", def.Name);
                        if (GlobalRegistry.Instance.RemoveDefault(def.DefId) == 0)
                        {
                            Console.WriteLine("  Default not found for removal.", def.DefId);
                        }
                    }
                }

                if (Remove.Module != null)
                {
                    foreach (Module mod in Remove.Module)
                    {
                        Console.WriteLine("Removing Module: {0}", mod.Name);
                        if (GlobalRegistry.Instance.RemoveModule(mod.LibId) == 0)
                        {
                            Console.WriteLine("  Module not found for removal.", mod.LibId);
                        }
                    }
                }
            }

            if (Create != null)
            {
                if (Create.File != null)
                {
                    WebClient wc = new WebClient();
                    wc.DownloadProgressChanged += new DownloadProgressChangedEventHandler(wc_DownloadProgressChanged);
                    foreach (CheckedFile file in Create.File)
                    {
                        try
                        {
                            String localpath = Path.GetFullPath(Path.Combine(GlobalRegistry.Instance.Path, file.Filename));

                            if (!localpath.StartsWith(GlobalRegistry.Instance.Path))
                            {
                                Console.WriteLine("Illegal file path, aborting version change.", localpath);
                                Console.WriteLine("This may be a security risk, please contact the package developers and notify the Voodoo Shader developers.");
                                Console.WriteLine("  File: {0}", file);
                                return false;
                            }

                            String path = Path.GetDirectoryName(localpath);
                            Directory.CreateDirectory(path);

                            String source = root + "/" + VersionUri + "/" + file.Filename;
                            Console.WriteLine("Downloading: {0}", localpath);
                            wc.DownloadFile(source.ToString(), localpath);
                            Console.Write(" done. Verifying... ");
                            String actualSum = VoodooHash.HashFile(localpath);
                            if (actualSum.ToUpper() == file.Checksum.ToUpper())
                            {
                                Console.WriteLine("checksum succeeded.");
                            }
                            else
                            {
                                Console.WriteLine("checksum failed!");
                            }
                        }
                        catch (Exception exc)
                        {
                            Console.WriteLine("Error removing file. This file may need to be removed manually.");
                            Console.WriteLine("  File: {0}", file);
                            Console.WriteLine("  Error: {0}", exc.Message);
                        }
                    }
                }

                if (Create.Default != null)
                {
                    foreach (Default def in Create.Default)
                    {
                        Console.WriteLine("Creating Default: {0}", def.Name);
                        GlobalRegistry.Instance.SetDefault(def);
                    }
                }

                if (Create.Module != null)
                {
                    foreach (Module mod in Create.Module)
                    {
                        Console.WriteLine("Creating Module: {0}", mod.Name);
                        GlobalRegistry.Instance.SetModule(mod);
                    }
                }
            }

            GlobalRegistry.Instance.Write();

            return true;
        }

        public bool Uninstall()
        {
            GlobalRegistry.Instance.Write();
            GlobalRegistry.Instance.Update();

            Console.WriteLine("Installing version: " + Id);

            if (Create != null)
            {
                if (Create.File != null)
                {
                    foreach (CheckedFile file in Create.File)
                    {
                        try
                        {
                            String fullpath = Path.GetFullPath(Path.Combine(GlobalRegistry.Instance.Path, file.Filename));
                            if (!fullpath.StartsWith(GlobalRegistry.Instance.Path))
                            {
                                Console.WriteLine("Illegal file path, aborting version change.", fullpath);
                                Console.WriteLine("This may be a security risk, please contact the package developers and notify the Voodoo Shader developers.");
                                Console.WriteLine("  File: {0}", file);
                                return false;
                            }
                            File.Delete(fullpath);
                        }
                        catch (Exception exc)
                        {
                            Console.WriteLine("Error removing file. This file may need to be removed manually.");
                            Console.WriteLine("  File: {0}", file);
                            Console.WriteLine("  Error: {0}", exc.Message);
                        }
                    }
                }

                if (Create.Default != null)
                {
                    foreach (Default def in Remove.Default)
                    {
                        Console.WriteLine("Removing Default: {0}", def.Name);
                        if (GlobalRegistry.Instance.RemoveDefault(def.DefId) == 0)
                        {
                            Console.WriteLine("  Default not found for removal.", def.DefId);
                        }
                    }
                }

                if (Create.Module != null)
                {
                    foreach (Module mod in Remove.Module)
                    {
                        Console.WriteLine("Removing Module: {0}", mod.Name);
                        if (GlobalRegistry.Instance.RemoveModule(mod.LibId) == 0)
                        {
                            Console.WriteLine("  Module not found for removal.", mod.LibId);
                        }
                    }
                }
            }

            GlobalRegistry.Instance.Write();

            return true;
        }

        static void wc_DownloadProgressChanged(object sender, DownloadProgressChangedEventArgs e)
        {
            if (e.ProgressPercentage % 10 == 0)
            {
                Console.Write(".");
            }
        }
    }
}
