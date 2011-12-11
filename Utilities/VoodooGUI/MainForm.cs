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
using System.Text;
using System.Runtime.InteropServices;
using System.Windows.Forms;
using Microsoft.Win32;
using VoodooSharp;

namespace VoodooGUI
{
    public partial class MainForm : Form
    {
        [DllImport("kernel32.dll")]
        private static extern IntPtr LoadLibrary(String dllname);

        [DllImport("kernel32.dll")]
        private static extern IntPtr GetProcAddress(IntPtr hModule, String procname);

        [DllImport("kernel32.dll")]
        private static extern bool FreeLibrary(IntPtr hModule);

        private delegate IntPtr InstallGlobalHook();
        private delegate void RemoveGlobalHook(IntPtr hHook);

        private IntPtr m_NativeModule;
        private IntPtr m_GlobalHook;

        private InstallGlobalHook m_InstallFunc;
        private RemoveGlobalHook m_RemoveFunc;

        private List<Hook> m_Hooks;
        bool m_DetailsDirty = false, m_DoNotUpdate = false;
        int m_LastEnter = -1;

        public MainForm()
        {
            InitializeComponent();

            // Do the installation check
            LoadHooks();
            cHook_Table.AutoGenerateColumns = false;
            cHook_Table.DataSource = m_Hooks;

            m_NativeModule = LoadLibrary("Voodoo_HookStub.dll");
            if (m_NativeModule != IntPtr.Zero)
            {
                m_InstallFunc = (InstallGlobalHook)Marshal.GetDelegateForFunctionPointer(GetProcAddress(m_NativeModule, "InstallGlobalHook"), typeof(InstallGlobalHook));
                m_RemoveFunc = (RemoveGlobalHook)Marshal.GetDelegateForFunctionPointer(GetProcAddress(m_NativeModule, "RemoveGlobalHook"), typeof(RemoveGlobalHook));
            }
            Menu_Hook_Enable(null, null);
        }

        /// <summary>
        /// Retrieve a supported apps list and search for any installed.
        /// </summary>
        /// <param name="sender"></param>
        /// <param name="e"></param>
        private void SearchSupported(object sender, EventArgs e)
        {
            // Verify they want to do this/security check
            if (MessageBox.Show(
                "Using this function will scan your hard drive for installed programs.\n"+
                "This uses file names only and will only scan folders you can open; file contents will not be accessed and no data will be sent to the server.\n"+
                "This may take some time if you have many files and may not be entirely accurate. A list of probable results will be shown when the scan is done.\n"+
                "Are you sure you want to run this scan?",
                "Confirm Scan", MessageBoxButtons.YesNo, MessageBoxIcon.Information) == DialogResult.Yes)
            {
                Form scanWindow = new ScanForm();
                scanWindow.ShowDialog();
            }
        }

        private void Menu_Hook_Add(object sender, EventArgs e)
        {
            Hook hook = new Hook();
            hook.Active = false;
            hook.Name = hook.Target = hook.Config = String.Empty;
            
            cHook_Table.EndEdit();
            this.BindingContext[cHook_Table.DataSource].SuspendBinding();
            m_Hooks.Add(hook);
            cHook_Table.DataSource = null;
            cHook_Table.DataSource = m_Hooks;
            this.BindingContext[cHook_Table.DataSource].ResumeBinding();
        }

        private void Menu_Hook_Remove(object sender, EventArgs e)
        {
            if (cHook_Table.SelectedRows.Count > 0)
            {
                cHook_Table.EndEdit();
                this.BindingContext[cHook_Table.DataSource].SuspendBinding();
                m_Hooks.RemoveAt(cHook_Table.SelectedRows[0].Index);
                cHook_Table.DataSource = null;
                cHook_Table.DataSource = m_Hooks;
                this.BindingContext[cHook_Table.DataSource].ResumeBinding();
            }
        }

        private void Form_OnResize(object sender, EventArgs e)
        {
            if (WindowState == FormWindowState.Minimized)
            {
                Hide();

                cTrayIcon.BalloonTipTitle = "Voodoo UI Hidden";
                cTrayIcon.BalloonTipText = "Voodoo hooks will remain active while the UI is hidden.";
                cTrayIcon.ShowBalloonTip(2500);
            }
        }

        private void Notify_OnClick(object sender, EventArgs e)
        {
            if (!this.Visible)
            {
                Show();
                WindowState = FormWindowState.Normal;
            }
            else
            {
                this.Activate();
            }
        }

        private void Menu_Hook_Enable(object sender, EventArgs e)
        {
            if (m_InstallFunc != null)
            {
                m_GlobalHook = m_InstallFunc();
                if (m_GlobalHook != IntPtr.Zero)
                {
                    cMenu_Hook_Off.Visible = false;
                    cMenu_Hook_On.Visible = true;
                }
            }
            else
            {
                cMenu_Hook_Off.Visible = true;
                cMenu_Hook_On.Visible = false;
            }
        }

        private void Menu_Hook_Disable(object sender, EventArgs e)
        {
            if (m_RemoveFunc != null)
            {
                m_RemoveFunc(m_GlobalHook);
                m_GlobalHook = IntPtr.Zero;
            }

            cMenu_Hook_Off.Visible = true;
            cMenu_Hook_On.Visible = false;
        }

        private void MainForm_Close(object sender, FormClosedEventArgs e)
        {
            HookSaveCheck();

            if (m_GlobalHook != IntPtr.Zero)
            {
                Menu_Hook_Disable(null, null);
            }
        }

        private void LoadHooks()
        {
            m_DetailsDirty = false;

            try
            {
                RegistryKey hookRoot = Registry.CurrentUser.OpenSubKey(@"SOFTWARE\VoodooShader\Hooks", RegistryKeyPermissionCheck.ReadSubTree);
                if (hookRoot != null)
                {
                    m_Hooks = VoodooRegistry.ReadHooks(hookRoot);
                    hookRoot.Close();
                }
                else
                {
                    m_Hooks = new List<Hook>();
                }
            }
            catch (System.Exception exc)
            {
                System.Windows.Forms.MessageBox.Show(exc.Message, "Error Reading Hooks", MessageBoxButtons.OK, MessageBoxIcon.Error);
                m_Hooks = new List<Hook>();
            }
        }

        private void CommitHooks()
        {
            try
            {
                RegistryKey voodooRoot = Registry.CurrentUser.OpenSubKey(@"SOFTWARE\VoodooShader", RegistryKeyPermissionCheck.ReadWriteSubTree);
                if (voodooRoot != null)
                {
                    if (voodooRoot.OpenSubKey("Hooks") != null)
                    {
                        voodooRoot.DeleteSubKeyTree("Hooks");
                    }
                } else {
                    voodooRoot = Registry.CurrentUser.CreateSubKey(@"SOFTWARE\VoodooShader");
                }
                
                VoodooRegistry.Write(m_Hooks, voodooRoot);

                voodooRoot.Close();
            }
            catch (System.Exception exc)
            {
                System.Windows.Forms.MessageBox.Show(exc.Message, "Error Writing Hooks", MessageBoxButtons.OK, MessageBoxIcon.Error);
            }
        }

        private void SaveHook(object sender, EventArgs e)
        {
            if (cHook_Table.SelectedRows.Count == 0) return;

            cHook_Table.SelectedRows[0].Cells["colActive"].Value = cHook_Active.Checked;
            cHook_Table.SelectedRows[0].Cells["colName"].Value = cHook_Name.Text;
            cHook_Table.SelectedRows[0].Cells["colTarget"].Value = cHook_Target.Text;
            cHook_Table.SelectedRows[0].Cells["colConfig"].Value = cHook_Config.Text;

            m_DetailsDirty = false;

            CommitHooks();
        }

        private void RowEnter(object sender, DataGridViewCellEventArgs e)
        {
            if (m_DoNotUpdate || m_LastEnter == e.RowIndex || e.RowIndex < 0 || e.RowIndex > m_Hooks.Count) return;

            m_DetailsDirty = false;
            m_DoNotUpdate = true;

            cHook_Active.Checked = m_Hooks[e.RowIndex].Active;
            cHook_Name.Text = m_Hooks[e.RowIndex].Name;
            cHook_Target.Text = m_Hooks[e.RowIndex].Target;
            cHook_Config.Text = m_Hooks[e.RowIndex].Config;

            m_DoNotUpdate = false;

            m_LastEnter = e.RowIndex;
        }

        private void FindTarget(object sender, EventArgs e)
        {
            dOpenFile.FilterIndex = 1;
            dOpenFile.Title = "Find Target Program";

            try
            {
                dOpenFile.InitialDirectory = System.IO.Path.GetDirectoryName(cHook_Target.Text);
            }
            catch (Exception) { }

            if (dOpenFile.ShowDialog() == DialogResult.OK)
            {
                cHook_Target.Text = dOpenFile.FileName;
            }
        }

        private void FindConfig(object sender, EventArgs e)
        {
            dOpenFile.FilterIndex = 2;
            dOpenFile.Title = "Find Config File";
            try
            {
                dOpenFile.InitialDirectory = System.IO.Path.GetDirectoryName(cHook_Config.Text);
            }
            catch (Exception) { }

            if (dOpenFile.ShowDialog() == DialogResult.OK)
            {
                cHook_Config.Text = dOpenFile.FileName;
            }
        }

        private void HookDetailChanged(object sender, EventArgs e)
        {
            if (!m_DoNotUpdate) m_DetailsDirty = true;
        }

        private bool HookSaveCheck()
        {
            if (m_DetailsDirty)
            {
                DialogResult result = MessageBox.Show("The hook details have been modified. Would you like to save them?", "Save Hook", MessageBoxButtons.YesNoCancel, MessageBoxIcon.Question);
                if (result == DialogResult.Cancel)
                {
                    return false;
                } else if (result == DialogResult.Yes)
                {
                    SaveHook(null, null);
                }

                m_DetailsDirty = false;
            }

            return true;
        }

        private void RowChange(object sender, DataGridViewCellCancelEventArgs e)
        {
            m_DoNotUpdate = true;
            if (cHook_Table.ContainsFocus && !HookSaveCheck())
            {
                e.Cancel = true;
            }
            m_DoNotUpdate = false;
        }

        private void OpenAboutBox(object sender, EventArgs e)
        {
            new AboutVoodoo().ShowDialog();
        }

        private void DownloadWindow(object sender, EventArgs e)
        {
            new UpdateManager().ShowDialog();
        }
    }
}
