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

        public UpdateManager()
        {
            InitializeComponent();

            cBrowserDesc.Navigating += new WebBrowserNavigatingEventHandler(webBrowser1_Navigating);

            m_Parser = new Markdown();
            m_ParserRegex = new Regex("[ ]{2,}");
        }

        void m_Cache_OnFetchManifest(string name, string uri)
        {
            textBox1.Text += String.Format("Fetching {0} from: {1}\n", name, uri);
        }

        void webBrowser1_Navigating(object sender, WebBrowserNavigatingEventArgs e)
        {
            e.Cancel = m_CancelNav;
            m_CancelNav = true;
        }

        private void NodeChanged(object sender, TreeViewEventArgs e)
        {
            String desc = e.Node.Tag as String;
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

            // Load and list packages
            foreach (PackageManifest pm in VoodooManifestCache.Instance.PackageManifests)
            {
                TreeNode packageNode = cPackageTree.Nodes.Add(pm.Package.PackId.ToString(), pm.Package.Name);
                packageNode.Tag = pm.Description;
                foreach (VoodooSharp.Version v in pm.Versions)
                {
                    packageNode.Nodes.Add(v.Id, v.Id).Tag = v.Description;
                }
            }          
        }

        private void InstallSelected(object sender, EventArgs e)
        {
            // Version test
            VoodooSharp.MinimalVersion changes = ChangeSetHandler.CollateChanges(VoodooManifestCache.Instance.PackageManifests[1], null, "0.5.2");
            ChangeSetHandler.ApplyVersion(changes);
        }
    }
}
