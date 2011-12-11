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
    public class MinimalVersion
    {
        public PackageManifest Source;
        public String Id, VersionUri;
        public List<String> CreateFile, RemoveFile;
        public List<Module> CreateModule, RemoveModule;
        public List<Default> CreateDefault, RemoveDefault;

        public MinimalVersion(PackageManifest s)
        {
            Source = s;
            CreateFile = new List<String>(); RemoveFile = new List<String>();
            CreateModule = new List<Module>(); RemoveModule = new List<Module>();
            CreateDefault = new List<Default>(); RemoveDefault = new List<Default>();
        }

        public MinimalVersion(PackageManifest s, Version v)
        {
            Source = s;
            Id = v.Id;
            VersionUri = v.VersionUri;
            if (v.Create != null)
            {
                CreateFile = new List<String>(v.Create.File);
                CreateModule = new List<Module>(v.Create.Module);
                CreateDefault = new List<Default>(v.Create.Default);
            }
            if (v.Remove != null)
            {
                RemoveFile = new List<String>(v.Remove.File);
                RemoveModule = new List<Module>(v.Remove.Module);
                RemoveDefault = new List<Default>(v.Remove.Default);
            }
        }
    }

    public class ChangeSetHandler
    {
        public static MinimalVersion CollateChanges(PackageManifest package, String id_from, String id_to)
        {
            if (package == null || package.Versions == null) return null;

            List<Version> versions = new List<Version>(package.Versions);

            Version v_from = versions.Find(v => v.Id == id_from);
            Version v_to = versions.Find(v => v.Id == id_to);

            // If from is the direct parent of to, we can simply return to.
            if (v_from != null && v_to != null && v_to.Parent == v_from.Id)
            {
                return new MinimalVersion(package, v_to);
            }

            Stack<Version> changechain = new Stack<Version>();

            Version check = null;
            if (v_to == null)
            {
                check = v_from;
            }
            else
            {
                check = v_to;
            }

            // Otherwise, we're installing something, so walk up.
            //! @todo This assumes a linear chain and is not optimal for branches.
            while (check != null)
            {
                changechain.Push(check);
                if ((v_from != null && check.Parent == v_from.Id) || check.Parent == null)
                {
                    break;
                }
                else
                {
                    check = versions.Find(v => v.Id == check.Parent);
                }
            }

            // Compile the stack into a final changeset:
            MinimalVersion finalmv = new MinimalVersion(package);

            String sourceID = v_from == null ? "(null)" : v_from.Id;
            String destID = v_to == null ? "(null)" : v_to.Id;
            finalmv.Id = String.Format("__{0} to {1}__", sourceID, destID);

            List<MinimalVersion> workingSet = new List<MinimalVersion>(changechain.Count);

            while (changechain.Count > 0)
            {
                Version c_change = changechain.Pop();

                MinimalVersion mv = new MinimalVersion(null);
                mv.Id = c_change.Id;
                mv.VersionUri = c_change.VersionUri;

                if (c_change.Remove != null)
                {
                    if (c_change.Remove.File != null)
                    {
                        foreach (String removal in c_change.Remove.File)
                        {
                            if (workingSet.Find(v => v.CreateFile.RemoveAll(s => s == removal) > 0) == null)
                            {
                                finalmv.RemoveFile.Add(removal);
                            }
                        }
                    }
                    
                    if (c_change.Remove.Module != null)
                    {
                        foreach (Module removal in c_change.Remove.Module)
                        {
                            if (finalmv.CreateModule.RemoveAll(s => s.LibId == removal.LibId) > 0)
                            {
                                finalmv.RemoveModule.Add(removal);
                            }
                        }
                    }

                    if (c_change.Remove.Default != null)
                    {
                        foreach (Default removal in c_change.Remove.Default)
                        {
                            if (finalmv.CreateDefault.RemoveAll(s => s.DefId == removal.DefId) > 0)
                            {
                                finalmv.RemoveDefault.Add(removal);
                            }
                        }
                    }
                }

                if (c_change.Create != null)
                {
                    if (c_change.Create.File != null) mv.CreateFile.AddRange(c_change.Create.File);
                    if (c_change.Create.Module != null) finalmv.CreateModule.AddRange(c_change.Create.Module);
                    if (c_change.Create.Default != null) finalmv.CreateDefault.AddRange(c_change.Create.Default);
                }

                workingSet.Add(mv);
            }

            foreach (MinimalVersion mv in workingSet)
            {
                if (mv.CreateFile != null) mv.CreateFile.ForEach(cf => finalmv.CreateFile.Add(mv.VersionUri + "/" + cf));
            }

            if (v_to == null)
            {
                finalmv.RemoveFile = finalmv.CreateFile;
                finalmv.CreateFile = null;
                finalmv.RemoveModule = finalmv.CreateModule;
                finalmv.CreateModule = null;
                finalmv.RemoveDefault = finalmv.CreateDefault;
                finalmv.CreateDefault = null;
            }

            return finalmv;
        }

        public static void ApplyVersion(MinimalVersion mv)
        {
            VoodooRegistry.Instance.Write();
            VoodooRegistry.Instance.Update();
            
            if (mv.RemoveFile != null)
            {
                foreach (String basename in mv.RemoveFile)
                {
                    try
                    {
                        String fullpath = Path.GetFullPath(Path.Combine(VoodooRegistry.Instance.Path, basename));
                        if (!fullpath.StartsWith(VoodooRegistry.Instance.Path))
                        {
                            Console.WriteLine("Illegal path for file '{0}', aborting version change. This may be a security risk, please contact the package developers and notify the Voodoo Shader developers.", fullpath);
                            return;
                        }
                        File.Delete(fullpath);
                    }
                    catch (Exception exc)
                    {
                        Console.WriteLine("Error removing file. This file may need to be removed manually.\n\tFile: {0}\n\tError: {1}", basename, exc.Message);
                    }
                }
            }

            if (mv.CreateFile != null)
            {
                foreach (String basename in mv.CreateFile)
                {
                    try
                    {
                        String localpath = Path.GetFullPath(Path.Combine(VoodooRegistry.Instance.Path, basename));
                        if (!localpath.StartsWith(VoodooRegistry.Instance.Path))
                        {
                            Console.WriteLine("Illegal path for file '{0}', aborting version change. This may be a security risk, please contact the package developers and notify the Voodoo Shader developers.", localpath);
                            return;
                        }

                        String source = Path.Combine(mv.Source.PackageUri, basename);
                        WebClient wc = new WebClient();
                        wc.DownloadFile(source.ToString(), localpath);
                    }
                    catch (Exception exc)
                    {
                        Console.WriteLine("Error removing file. This file may need to be removed manually.\n\tFile: {0}\n\tError: {1}", basename, exc.Message);
                    }
                }
            }

            if (mv.RemoveDefault != null)
            {
                foreach (Default def in mv.RemoveDefault)
                {
                    if (VoodooRegistry.Instance.Defaults.RemoveAll(pdef => pdef.DefId == def.DefId) == 0)
                    {
                        Console.WriteLine("Default {0} not found for removal.", def.DefId);
                    }
                }
            }

            if (mv.RemoveModule != null)
            {
                foreach (Module mod in mv.RemoveModule)
                {
                    VoodooRegistry.Instance.Modules.RemoveAll(pmod => pmod.LibId == mod.LibId);
                }
            }

            if (mv.CreateDefault != null)
            {
                foreach (Default def in mv.CreateDefault)
                {
                    VoodooRegistry.Instance.Defaults.Add(def);
                }
            }

            if (mv.CreateModule != null)
            {
                foreach (Module mod in mv.CreateModule)
                {
                    VoodooRegistry.Instance.Modules.Add(mod);
                }
            }

            VoodooRegistry.Instance.Write();
        }
    }
}
