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
using System.Windows.Forms;
using MarkdownSharp;
using VoodooNetClasses;

namespace VoodooGUI
{
    public partial class UpdateManager : Form
    {
        VoodooManifestCache m_Cache;
        Markdown m_Parser;
        bool m_CancelNav;

        public UpdateManager()
        {
            InitializeComponent();

            webBrowser1.Navigating += new WebBrowserNavigatingEventHandler(webBrowser1_Navigating);

            m_Parser = new Markdown();

            String cachePath = VoodooRegistry.GetRootPath() + @"\manifests\";
            m_Cache = new VoodooManifestCache(cachePath);

            // Sync remotes
            Remote testRemote = new Remote();
            testRemote.Name = "VoodooShader.com";
            testRemote.Uri = "https://www.voodooshader.com/manifests/Remote.xml";
            m_Cache.Sync(testRemote);

            // Load and list packages
            foreach (PackageManifest pm in m_Cache.PackageManifests)
            {
                int index = cPackageGrid.Rows.Add();
                cPackageGrid.Rows[index].Cells[1].Value = pm.Package.Name;
                cPackageGrid.Rows[index].Cells[2].Value = pm.Package.Version;
            }
        }

        void webBrowser1_Navigating(object sender, WebBrowserNavigatingEventArgs e)
        {
            e.Cancel = m_CancelNav;
            m_CancelNav = true;
        }

        private void RowEnter(object sender, DataGridViewCellEventArgs e)
        {
            String desc = m_Cache.PackageManifests[e.RowIndex].Description;
            desc = desc.Replace("  ", " ");
            desc = m_Parser.Transform(desc);
            m_CancelNav = false;
            webBrowser1.DocumentText = desc;
        }

        private void SelectionChanged(object sender, EventArgs e)
        {
        }
    }
}
