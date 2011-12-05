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

namespace VoodooNetClasses
{
    public class VoodooManifestCache
    {
        public delegate void FetchManifest(String name, String uri);

        public String Path { get; set; }
        public List<RemoteManifest> RemoteManifests { get; set; }
        public List<PackageManifest> PackageManifests { get; set; }
        public event FetchManifest OnFetchManifest;

        public VoodooManifestCache(String iPath)
        {
            RemoteManifests = new List<RemoteManifest>();
            PackageManifests = new List<PackageManifest>();

            Path = iPath;

            if (!Directory.Exists(Path))
            {
                Directory.CreateDirectory(Path);
            }
        }

        public void Sync(Remote remote)
        {            
            try
            {
                WebClient client = new WebClient();
                String remotefile = Path + "\\remote_" + VoodooHash.Hash(remote.Uri) + ".xml";

                if (OnFetchManifest != null) OnFetchManifest.Invoke(remote.Name, remote.Uri);
                client.DownloadFile(remote.Uri, remotefile);

                RemoteManifest remotemanifest = (RemoteManifest)VoodooXml.ValidateObject(remotefile, typeof(RemoteManifest));
                RemoteManifests.Add(remotemanifest);

                foreach (Package package in remotemanifest.Packages)
                {
                    try
                    {
                        String packagefile = Path + "\\package_" + package.PackId.ToString() + ".xml";

                        if (OnFetchManifest != null) OnFetchManifest.Invoke(package.Name, package.ManifestUri);
                        client.DownloadFile(package.ManifestUri, packagefile);

                        PackageManifest packagemanifest = (PackageManifest)VoodooXml.ValidateObject(packagefile, typeof(PackageManifest));
                        PackageManifests.Add(packagemanifest);
                    }
                    catch (System.Exception exc)
                    {
                        throw new Exception(String.Format("Error syncing manifest for package '{0}'.", package.PackId), exc);                   	
                    }
                }
            }
            catch (Exception exc)
            {
                throw new Exception("Error syncing manifests.", exc);
            }
        }

        public void Sync(IEnumerable<Remote> remotes)
        {
            foreach (Remote remote in remotes)
            {
                Sync(remote);
            }
        }

        public void Sync()
        {
            // Sync user remotes
            RegistryKey remotesKey = Registry.CurrentUser.OpenSubKey(@"Software\VoodooShader\Remotes");
            if (remotesKey != null)
            {
                List<Remote> remotes = VoodooRegistry.ReadRemotes(remotesKey);
                Sync(remotes);
            }

            // Sync root remote
            Remote testRemote = new Remote();
            testRemote.Name = "Voodoo Remote";
            testRemote.Uri = "https://www.voodooshader.com/manifests/Remote.xml";
            Sync(testRemote);
        }
    }
}
