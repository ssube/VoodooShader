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
using System.Text;
using System.Web;
using System.Net;

namespace VoodooSharp
{
    [System.SerializableAttribute()]
    public partial class ChangeSet
    {
        [System.Xml.Serialization.XmlElementAttribute("Module")]
        public Module[] Module { get; set; }
        [System.Xml.Serialization.XmlElementAttribute("Default")]
        public Default[] Default { get; set; }
        [System.Xml.Serialization.XmlElementAttribute("File")]
        public string[] File { get; set; }
    }

    public class ChangeSetHandler
    {
        public static bool Update(PackageManifest pm, String sfrom, String sto)
        {
            if (pm == null || sto == null || (sfrom == null && sto == null))
            {
                return false;
            }

            Console.WriteLine("Beginning update procedure.");
            Console.WriteLine("  Package: {0}", pm.Package.Name);

            PackageVersion v_from = sfrom == null ? null : pm.Versions.Find(v => v.Id == sfrom);
            PackageVersion v_to = sto == null ? null : pm.Versions.Find(v => v.Id == sto);

            if (v_from == null && v_to == null)
            {
                return false;
            }

            Stack<PackageVersion> workingSet = new Stack<PackageVersion>();
            PackageVersion check = v_to == null ? v_from : v_to;

            while (check != null && check.Id != sfrom)
            {
                workingSet.Push(check);
                check = pm.Versions.Find(v => v.Id == check.Prev);
            }

            if (v_to == null)
            {
                Registry.Instance.RemovePackage(pm.Package.PackId);
            } else if (workingSet.Count == 0)
            {
                return false;
            }

            Package pack = new Package();
            pack.HomeUri = pm.Package.HomeUri;
            pack.Name = pm.Package.Name;
            pack.PackId = pm.Package.PackId;
            pack.Version = sto;

            Console.WriteLine("{0} updates to be applied.", workingSet.Count);
            while (workingSet.Count > 0)
            {
                PackageVersion wv = workingSet.Pop();
                pack.Version = wv.Id;

                if (v_to == null)
                {
                    if (!UninstallVersion(wv))
                    {
                        Registry.Instance.SetPackage(pack);
                        return false;
                    }
                }
                else
                {
                    if (!InstallVersion(pm.PackageUri, wv))
                    {
                        Registry.Instance.SetPackage(pack);
                        return false;
                    }
                }
            }

            if (v_to == null)
            {
                Registry.Instance.RemovePackage(pm.Package.PackId);
            }
            else
            {
                Registry.Instance.SetPackage(pack);
            }

            Registry.Instance.Write();

            return true;
        }

        public static bool InstallVersion(String root, PackageVersion mv)
        {
            Registry.Instance.Write();
            Registry.Instance.Update();

            if (mv == null) return false;

            Console.WriteLine("Installing version: " + mv.Id);

            if (mv.Remove != null)
            {
                if (mv.Remove.File != null)
                {
                    foreach (String basename in mv.Remove.File)
                    {
                        try
                        {
                            String fullpath = Path.GetFullPath(Path.Combine(Registry.Instance.Path, basename));
                            if (!fullpath.StartsWith(Registry.Instance.Path))
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

                if (mv.Remove.Default != null)
                {
                    foreach (Default def in mv.Remove.Default)
                    {
                        Console.WriteLine("Removing Default: {0}", def.Name);
                        if (Registry.Instance.RemoveDefault(def.DefId) == 0)
                        {
                            Console.WriteLine("  Default not found for removal.", def.DefId);
                        }
                    }
                }

                if (mv.Remove.Module != null)
                {
                    foreach (Module mod in mv.Remove.Module)
                    {
                        Console.WriteLine("Removing Module: {0}", mod.Name);
                        if (Registry.Instance.RemoveModule(mod.LibId) == 0)
                        {
                            Console.WriteLine("  Module not found for removal.", mod.LibId);
                        }
                    }
                }
            }

            if (mv.Create != null)
            {
                if (mv.Create.File != null)
                {
                    WebClient wc = new WebClient();
                    wc.DownloadProgressChanged += new DownloadProgressChangedEventHandler(wc_DownloadProgressChanged);
                    foreach (String basename in mv.Create.File)
                    {
                        try
                        {
                            String localpath = Path.GetFullPath(Path.Combine(Registry.Instance.Path, basename));
                            if (!localpath.StartsWith(Registry.Instance.Path))
                            {
                                Console.WriteLine("Illegal file path, aborting version change.", localpath);
                                Console.WriteLine("This may be a security risk, please contact the package developers and notify the Voodoo Shader developers.");
                                Console.WriteLine("  File: {0}", basename);
                                return false;
                            }

                            String source = root + "/" + mv.VersionUri + "/" + basename;
                            Console.WriteLine("Downloading: {0}", localpath);
                            wc.DownloadFile(source.ToString(), localpath);
                            Console.WriteLine(" done.");
                        }
                        catch (Exception exc)
                        {
                            Console.WriteLine("Error removing file. This file may need to be removed manually.");
                            Console.WriteLine("  File: {0}", basename);
                            Console.WriteLine("  Error: {0}", exc.Message);
                        }
                    }
                }

                if (mv.Create.Default != null)
                {
                    foreach (Default def in mv.Create.Default)
                    {
                        Console.WriteLine("Creating Default: {0}", def.Name);
                        Registry.Instance.SetDefault(def);
                    }
                }

                if (mv.Create.Module != null)
                {
                    foreach (Module mod in mv.Create.Module)
                    {
                        Console.WriteLine("Creating Module: {0}", mod.Name);
                        Registry.Instance.SetModule(mod);
                    }
                }
            }

            Registry.Instance.Write();

            return true;
        }

        public static bool UninstallVersion(PackageVersion mv)
        {
            Registry.Instance.Write();
            Registry.Instance.Update();

            if (mv == null) return false;

            Console.WriteLine("Installing version: " + mv.Id);

            if (mv.Create != null)
            {
                if (mv.Create.File != null)
                {
                    foreach (String basename in mv.Create.File)
                    {
                        try
                        {
                            String fullpath = Path.GetFullPath(Path.Combine(Registry.Instance.Path, basename));
                            if (!fullpath.StartsWith(Registry.Instance.Path))
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

                if (mv.Create.Default != null)
                {
                    foreach (Default def in mv.Remove.Default)
                    {
                        Console.WriteLine("Removing Default: {0}", def.Name);
                        if (Registry.Instance.RemoveDefault(def.DefId) == 0)
                        {
                            Console.WriteLine("  Default not found for removal.", def.DefId);
                        }
                    }
                }

                if (mv.Create.Module != null)
                {
                    foreach (Module mod in mv.Remove.Module)
                    {
                        Console.WriteLine("Removing Module: {0}", mod.Name);
                        if (Registry.Instance.RemoveModule(mod.LibId) == 0)
                        {
                            Console.WriteLine("  Module not found for removal.", mod.LibId);
                        }
                    }
                }
            }

            Registry.Instance.Write();

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
