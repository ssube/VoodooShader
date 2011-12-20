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
using System.Diagnostics;
using System.IO;
using System.Net;
using System.Security.Cryptography;
using System.Text;
using System.Threading;
using System.Windows.Forms;
using VoodooSharp;

namespace VoodooUI
{
    public partial class PackageManager : Form
    {
        bool m_CancelNav;
        ProgressDialog m_ProgressForm;

        public PackageManager()
        {
            InitializeComponent();

            cBrowserDesc.Navigating += new WebBrowserNavigatingEventHandler(webBrowser1_Navigating);
        }

        void webBrowser1_Navigating(object sender, WebBrowserNavigatingEventArgs e)
        {
            e.Cancel = m_CancelNav;
            if (m_CancelNav)
            {
                System.Diagnostics.Process.Start(e.Url.ToString());
            }
            m_CancelNav = true;
        }

        void RefreshTree()
        {
            List<String> packages = FindPackages(GlobalRegistry.Instance.Path);

            foreach (String package in packages)
            {
                cPackageTree.Nodes.Add(package);
            }
        }

        List<String> FindPackages(String root)
        {
            List<String> packages = new List<String>();
            string[] subdirs = Directory.GetDirectories(root);

            foreach (string subdir in subdirs)
            {
                if (String.Compare(subdir, "user", true) != 0 && Directory.Exists(Path.Combine(subdir, ".git")))
                {
                    packages.Add(Path.GetDirectoryName(subdir));
                }
            }

            return packages;
        }

        void ClonePackage(String source, String branch, String path)
        {
            try
            {
                String fullpath = Path.Combine(GlobalRegistry.Instance.Path, path);

                String command = String.Format("clone {0} {1}", source, fullpath);
                RunGitCommand(command);
            }
            catch (Exception exc)
            {
                MessageBox.Show("Error downloading package:\n" + exc.Message);
            }
        }

        void UpdatePackage(String package)
        {
            try
            {
                String fullpath = Path.Combine(GlobalRegistry.Instance.Path, package);

                RunGitCommand("reset --hard");
                RunGitCommand("pull");
            }
            catch (Exception exc)
            {
                MessageBox.Show("Error updating package:\n" + exc.Message);
            }
        }

        void StartupCheck()
        {
            if (!GlobalRegistry.Exists)
            {
                dFolderBrowser.Description = "Select Voodoo Shader installation path.";
                if (dFolderBrowser.ShowDialog() == DialogResult.OK)
                {
                    GlobalRegistry.Instance.Path = dFolderBrowser.SelectedPath;
                    GlobalRegistry.Instance.Write();
                }
            }

            if (!ExistsInPath("git.exe") && !ExistsInPath("git.cmd"))
            {
                m_ProgressForm = new ProgressDialog();
                m_ProgressForm.WriteLine("Unable to locate git for package management.");

                WebClient client = new WebClient();
                client.DownloadProgressChanged += new DownloadProgressChangedEventHandler(client_DownloadProgressChanged);
                client.DownloadFileCompleted += new System.ComponentModel.AsyncCompletedEventHandler(client_DownloadFileCompleted);

                client.DownloadFileAsync(new Uri("http://msysgit.googlecode.com/files/Git-1.7.8-preview20111206.exe"), "msysgit.exe");
                m_ProgressForm.Show(this);
                this.Enabled = false;
                m_ProgressForm.WriteLine("Downloading git installer...");
            }

            ClonePackage("git://github.com/peachykeen/VoodooShader.git", "master", Path.Combine(GlobalRegistry.Instance.Path, "example"));
        }

        void client_DownloadProgressChanged(object sender, DownloadProgressChangedEventArgs e)
        {
            m_ProgressForm.Progress = e.ProgressPercentage;
        }

        void client_DownloadFileCompleted(object sender, System.ComponentModel.AsyncCompletedEventArgs e)
        {
            m_ProgressForm.WriteLine("Done downloading git installer.");

            SHA1 verify = SHA1.Create();
            byte[] hash = verify.ComputeHash(File.ReadAllBytes("msysgit.exe"));
            byte[] goal = { 0x75, 0x8b, 0xd1, 0xbb, 0xb9, 0x12, 0x73, 0x24, 0x87, 0xd7, 0xb4, 0x24, 0x0f, 0x7d, 0xe3, 0x3b, 0xbb, 0xab, 0x18, 0x48 };
            bool valid = true;
            for (int i = 0; i < hash.Length; ++i)
            {
                if (goal[i] != hash[i]) valid = false;
            }

            if (!valid)
            {
                m_ProgressForm.WriteLine("The git installer appears to be corrupt.");
                m_ProgressForm.WriteLine("You will need to download and install git manually.");
                Process.Start("http://code.google.com/p/msysgit/downloads/detail?name=Git-1.7.8-preview20111206.exe");

                m_ProgressForm.AllowClose = true;
                this.Enabled = true;
            }
            else
            {
                m_ProgressForm.WriteLine("Launching git installer...");
                Process.Start("msysgit.exe").WaitForExit();
                m_ProgressForm.WriteLine("Git installer has completed.");

                m_ProgressForm.AllowClose = true;
                this.Enabled = true;
            }
        }

        public static bool ExistsInPath(string fileName)
        {
            if (GetFullPath(fileName) != null)
                return true;
            return false;
        }

        public static string GetFullPath(string fileName)
        {
            if (File.Exists(fileName))
                return Path.GetFullPath(fileName);

            var values = Environment.GetEnvironmentVariable("PATH");
            foreach (var path in values.Split(';'))
            {
                var fullPath = Path.Combine(path, fileName);
                if (File.Exists(fullPath))
                    return fullPath;
            }
            return null;
        }

        private void FormShown(object sender, EventArgs e)
        {
            StartupCheck();
        }

        public void RunGitCommand(String command)
        {
            m_ProgressForm = new ProgressDialog();
            m_ProgressForm.ShellExecute(this, "git " + command);
        }

        private void PackageSelect(object sender, TreeViewEventArgs e)
        {
            String pack = e.Node.Text;
            String fullpath = Path.Combine(GlobalRegistry.Instance.Path, pack);
            String manifest = Path.Combine(fullpath, "package.xml");
            if (File.Exists(manifest))
            {
                XmlValidator xv = new XmlValidator();
                Package package = xv.ValidateObject<Package>(manifest);

                m_CancelNav = false;
                if (package != null)
                {
                    cBrowserDesc.DocumentText = package.Languages[0].Description;
                }
                else
                {
                    cBrowserDesc.DocumentText = "Unable to load package info.";
                }
            }
        }
    }
}
