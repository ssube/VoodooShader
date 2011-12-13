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
using System.Text;
using Microsoft.Win32;

namespace VoodooSharp
{
    public class ManifestCache
    {
        public delegate void FetchManifest(String name, String uri);

        public String Path { get; set; }

        public List<RemoteManifest> RemoteManifests { get; set; }
        public List<PackageManifest> PackageManifests { get; set; }

        public event FetchManifest OnFetchManifest;

        private static ManifestCache instance;

        public static ManifestCache Instance
        {
            get
            {
                if (instance == null)
                {
                    instance = new ManifestCache();
                }
                return instance;
            }
        }

        private ManifestCache()
        {
            RemoteManifests = new List<RemoteManifest>();
            PackageManifests = new List<PackageManifest>();

            Path = GlobalRegistry.Instance.Path + @"\manifests\";

            if (!Directory.Exists(Path))
            {
                Directory.CreateDirectory(Path);
            }
            else
            {
                foreach (String file in Directory.GetFiles(Path, "package_*.xml"))
                {
                    if (OnFetchManifest != null) OnFetchManifest.Invoke("Local manifest", file);

                    XmlValidator xv = new XmlValidator();
                    PackageManifest pm = xv.ValidateObject<PackageManifest>(file);
                    if (pm != null && !xv.Errors)
                    {
                        PackageManifests.Add(pm);
                    }
                }
            }
        }

        public void Fetch(Remote remote)
        {
            if (remote == null) return;

            WebClient client = new WebClient();

            String remoteUri = remote.Uri + "/remote.xml";
            String remoteFile = Path + "\\remote_" + VoodooHash.Hash(remoteUri) + ".xml";

            try
            {
                if (OnFetchManifest != null) OnFetchManifest.Invoke(remote.Name, remoteUri);
                client.DownloadFile(remoteUri, remoteFile);

                XmlValidator xv = new XmlValidator();
                RemoteManifest rm = xv.ValidateObject<RemoteManifest>(remoteFile);
                if (rm != null && !xv.Errors)
                {
                    RemoteManifests.Add(rm);
                }

                for (int i = 0; i < rm.Packages.Length; ++i)
                {
                    String packageUri = remote.Uri + "/" + rm.Packages[i];
                    String packageFile = Path + "\\package_" + VoodooHash.Hash(packageUri) + ".xml";

                    try
                    {
                        if (OnFetchManifest != null) OnFetchManifest.Invoke(String.Format("{0}; package {1}", remote.Name, i), packageUri);
                        client.DownloadFile(packageUri, packageFile);

                        PackageManifest pm = xv.ValidateObject<PackageManifest>(packageFile);
                        if (pm != null && !xv.Errors)
                        {
                            PackageManifests.Add(pm);
                        }
                    }
                    catch (Exception exc)
                    {
                        throw new Exception(String.Format("Error syncing manifest for package '{0}'.", packageUri), exc);                   	
                    }
                }
            }
            catch (Exception exc)
            {
                Console.WriteLine("Error syncing manifests.");
                Console.WriteLine("  Remote: {0}", remoteUri);
                Console.WriteLine("  Error: {0}", exc.Message);
            }
        }

        public void Fetch(IEnumerable<Remote> remotes)
        {
            foreach (Remote remote in remotes)
            {
                Fetch(remote);
            }
        }

        public void FetchAll()
        {
            Clear();

            // Sync user remotes
            Fetch(GlobalRegistry.Instance.Remotes);

            // Sync root remote
            Remote testRemote = new Remote();
            testRemote.Name = "Voodoo Remote";
            testRemote.Uri = "https://www.voodooshader.com/manifests";
            Fetch(testRemote);
        }

        public void Clear()
        {
            // Reset the cache
            foreach (String file in Directory.GetFiles(Path, "package_*.xml"))
            {
                File.Delete(file);
            }

            foreach (String file in Directory.GetFiles(Path, "remote_*.xml"))
            {
                File.Delete(file);
            }

            RemoteManifests.Clear();
            PackageManifests.Clear();
        }
    }
}
