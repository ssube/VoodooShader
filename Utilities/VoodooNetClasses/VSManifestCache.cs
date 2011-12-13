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
    public class VSManifestCache
    {
        public delegate void FetchManifest(String name, String uri);

        public String Path { get; set; }

        public List<RemoteManifest> RemoteManifests { get; set; }
        public List<PackageManifest> PackageManifests { get; set; }

        public event FetchManifest OnFetchManifest;

        private static VSManifestCache instance;

        public static VSManifestCache Instance
        {
            get
            {
                if (instance == null)
                {
                    instance = new VSManifestCache();
                }
                return instance;
            }
        }

        private VSManifestCache()
        {
            RemoteManifests = new List<RemoteManifest>();
            PackageManifests = new List<PackageManifest>();

            Path = VSRegistry.Instance.Path + @"\manifests\";

            if (!Directory.Exists(Path))
            {
                Directory.CreateDirectory(Path);
            }
            else
            {
                foreach (String file in Directory.GetFiles(Path, "package_*.xml"))
                {
                    if (OnFetchManifest != null) OnFetchManifest.Invoke("Local manifest", file);

                    PackageManifest packagemanifest = VSXml.ValidateObject<PackageManifest>(file);
                    PackageManifests.Add(packagemanifest);
                }
            }
        }

        public void Fetch(Remote remote)
        {
            if (remote == null) return;

            WebClient client = new WebClient();

            String remoteUri = remote.Uri + "/remote.xml";
            String remoteFile = Path + "\\remote_" + VSHash.Hash(remoteUri) + ".xml";

            try
            {
                if (OnFetchManifest != null) OnFetchManifest.Invoke(remote.Name, remoteUri);
                client.DownloadFile(remoteUri, remoteFile);

                RemoteManifest remotemanifest = VSXml.ValidateObject<RemoteManifest>(remoteFile);
                RemoteManifests.Add(remotemanifest);

                for (int i = 0; i < remotemanifest.Packages.Length; ++i)
                {
                    String packageUri = remote.Uri + "/" + remotemanifest.Packages[i];
                    String packageFile = Path + "\\package_" + VSHash.Hash(packageUri) + ".xml";

                    try
                    {
                        if (OnFetchManifest != null) OnFetchManifest.Invoke(String.Format("{0}; package {1}", remote.Name, i), packageUri);
                        client.DownloadFile(packageUri, packageFile);

                        PackageManifest packagemanifest = VSXml.ValidateObject<PackageManifest>(packageFile);
                        PackageManifests.Add(packagemanifest);
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
            Fetch(VSRegistry.Instance.Remotes);

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
