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

namespace VoodooGUI
{
    public partial class UpdateManager : Form
    {
        Markdown m_Parser;
        bool m_CancelNav;
        Regex m_ParserRegex;
        ConsoleRedirect m_Redir;

        public UpdateManager()
        {
            InitializeComponent();

            cBrowserDesc.Navigating += new WebBrowserNavigatingEventHandler(webBrowser1_Navigating);

            m_Redir = new ConsoleRedirect(textBox1);
            m_Parser = new Markdown();
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
            else if (e.Node.Tag.GetType() == typeof(VoodooSharp.Version))
            {
                desc = (e.Node.Tag as VoodooSharp.Version).Description;
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
            VoodooManifestCache.Instance.OnFetchManifest += new VoodooManifestCache.FetchManifest(m_Cache_OnFetchManifest);
            VoodooManifestCache.Instance.FetchAll();

            RefreshTree();
        }

        private void InstallSelected(object sender, EventArgs e)
        {
            // Get the version
            PackageManifest pm = null;
            String source = null;
            String target = null;
                        
            TreeNode node = cPackageTree.SelectedNode;
            if (node == null)
            {
                return;
            } else {
                if (node.Tag.GetType() == typeof(VoodooSharp.Version))
                {
                    target = (node.Tag as VoodooSharp.Version).Id;
                    pm = node.Parent.Tag as PackageManifest;
                }
                else if (node.Tag.GetType() == typeof(VoodooSharp.PackageManifest))
                {
                    pm = node.Tag as PackageManifest;
                    target = pm.Package.Version;
                }
            }

            source = VoodooRegistry.Instance.PackageVersion(pm.Package.PackId);
            String msg;
            if (source == null)
            {
                msg = String.Format("Install package {0} to {1}.\nContinue?", pm.Package.Name, target);
            } else {
                msg = String.Format("Update package {0} from {1} to {2}.\nContinue?", pm.Package.Name, source, target);
            }
            
            if (MessageBox.Show(msg, "Confirm Package Update", MessageBoxButtons.OKCancel, MessageBoxIcon.Question) == DialogResult.OK)
            {
                ChangeSetHandler.Update(pm, source, target);
            }
        }

        private void RefreshTree()
        {
            cPackageTree.Nodes.Clear();

            foreach (PackageManifest pm in VoodooManifestCache.Instance.PackageManifests)
            {
                TreeNode packageNode = cPackageTree.Nodes.Add(pm.Package.PackId.ToString(), pm.Package.Name);
                packageNode.Tag = pm;
                foreach (VoodooSharp.Version v in pm.Versions)
                {
                    packageNode.Nodes.Add(v.Id, v.Id).Tag = v;
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
                if (node.Tag.GetType() == typeof(VoodooSharp.Version))
                {
                    pm = node.Parent.Tag as PackageManifest;
                }
                else if (node.Tag.GetType() == typeof(VoodooSharp.PackageManifest))
                {
                    pm = node.Tag as PackageManifest;
                }
            }

            String source = VoodooRegistry.Instance.PackageVersion(pm.Package.PackId);

            if (MessageBox.Show(
                String.Format("Uninstall package {0} from {1}.\nContinue?", pm.Package.Name, source),
                "Confirm Package Update", MessageBoxButtons.OKCancel, MessageBoxIcon.Question) == DialogResult.OK)
            {
                ChangeSetHandler.Update(pm, source, null);
            }
        }
    }
}
