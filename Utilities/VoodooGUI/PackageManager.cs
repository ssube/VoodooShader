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
using System.ComponentModel;
using System.Data;
using System.Diagnostics;
using System.Drawing;
using System.IO;
using System.Text;
using System.Text.RegularExpressions;
using System.Threading;
using System.Windows.Forms;
using MarkdownSharp;
using VoodooSharp;

namespace VoodooUI
{
    public partial class PackageManager : Form
    {
        Markdown m_Parser;
        bool m_CancelNav;
        Regex m_ParserRegex;

        public PackageManager()
        {
            InitializeComponent();

            cBrowserDesc.Navigating += new WebBrowserNavigatingEventHandler(webBrowser1_Navigating);

            MarkdownOptions mdopt = new MarkdownOptions();
            mdopt.AutoHyperlink = true; mdopt.LinkEmails = true;
            m_Parser = new Markdown(mdopt);

            m_ParserRegex = new Regex("[ ]{2,}");

            RefreshTree();
        }

        void m_Cache_OnFetchManifest(string name, string uri)
        {
            Console.WriteLine("Fetching {0} from: {1}", name, uri);
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

        private void NodeChanged(object sender, TreeViewEventArgs e)
        {
            // Update description
            String desc = null;
            if (e.Node.Tag.GetType() == typeof(PackageManifest))
            {
                desc = (e.Node.Tag as PackageManifest).Description;
            }
            else if (e.Node.Tag.GetType() == typeof(PackageVersion))
            {
                desc = (e.Node.Tag as PackageVersion).Description;
            }

            if (desc != null)
            {
                desc = m_ParserRegex.Replace(desc, " ");
                desc = m_Parser.Transform(desc);
                m_CancelNav = false;
                cBrowserDesc.DocumentText = desc;
            }
        }

        private void FetchRemotes(object sender, EventArgs e)
        {            
            ManifestCache.Instance.OnFetchManifest += new ManifestCache.FetchManifest(m_Cache_OnFetchManifest);
            ManifestCache.Instance.FetchAll();

            RefreshTree();
        }

        private void InstallSelected(object sender, EventArgs e)
        {
            // Get the version
            PackageManifest pm = null;
            String target = null;
                        
            TreeNode node = cPackageTree.SelectedNode;
            if (node == null)
            {
                return;
            } else {
                if (node.Tag.GetType() == typeof(PackageVersion))
                {
                    target = (node.Tag as PackageVersion).Id;
                    pm = node.Parent.Tag as PackageManifest;
                }
                else if (node.Tag.GetType() == typeof(PackageManifest))
                {
                    pm = node.Tag as PackageManifest;
                    target = pm.Package.Version;
                }
            }
            
            if (MessageBox.Show(String.Format("Update package {0} to version {1}.\nContinue?", pm.Package.Name, target), "Confirm Package Update", MessageBoxButtons.OKCancel, MessageBoxIcon.Question) == DialogResult.OK)
            {
                pm.Update(target);
            }
        }

        private void RefreshTree()
        {
            cPackageTree.Nodes.Clear();

            foreach (PackageManifest pm in ManifestCache.Instance.PackageManifests)
            {
                TreeNode packageNode = cPackageTree.Nodes.Add(pm.Package.PackId.ToString(), pm.Package.Name);
                packageNode.Tag = pm;

                Package installedPack = GlobalRegistry.Instance.GetPackage(pm.Package.PackId);
                if (installedPack != null)
                {
                    packageNode.BackColor = System.Drawing.Color.PowderBlue;
                }

                foreach (PackageVersion v in pm.Versions)
                {
                    TreeNode node = packageNode.Nodes.Add(v.Id, v.Id);
                    node.Tag = v;

                    if (installedPack != null && installedPack.Version == v.Id)
                    {
                        node.BackColor = System.Drawing.Color.Honeydew;
                    }
                }
            }  
        }

        private void PackageHome(object sender, EventArgs e)
        {
            TreeNode node = cPackageTree.SelectedNode;
            if (node != null)
            {
                while (node.Parent != null) node = node.Parent;

                Process.Start((node.Tag as PackageManifest).Package.HomeUri);
            }
        }

        private void UninstallSelected(object sender, EventArgs e)
        {
            // Get the version
            PackageManifest pm = null;

            TreeNode node = cPackageTree.SelectedNode;
            if (node == null)
            {
                return;
            }
            else
            {
                if (node.Tag.GetType() == typeof(PackageVersion))
                {
                    pm = node.Parent.Tag as PackageManifest;
                }
                else if (node.Tag.GetType() == typeof(PackageManifest))
                {
                    pm = node.Tag as PackageManifest;
                }
            }

            Package installedPack = GlobalRegistry.Instance.GetPackage(pm.Package.PackId);
            if (installedPack == null)
            {
                return;
            }

            String source = installedPack.Version;

            if (MessageBox.Show(
                String.Format("Uninstall package {0} from {1}.\nContinue?", pm.Package.Name, source),
                "Confirm Package Update", MessageBoxButtons.OKCancel, MessageBoxIcon.Question) == DialogResult.OK)
            {
                pm.Update(null);
            }
        }
    }
}
