using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Data;
using System.Drawing;
using System.Text;
using System.Runtime.InteropServices;
using System.Windows.Forms;
using Microsoft.Win32;
using VoodooNetClasses;

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

        private List<VoodooHook> m_Hooks;

        public MainForm()
        {
            InitializeComponent();

            LoadHooks();
            cHook_Table.AutoGenerateColumns = false;
            cHook_Table.DataSource = m_Hooks;

            m_NativeModule = LoadLibrary("Voodoo_HookStub.dll");
            if (m_NativeModule != IntPtr.Zero)
            {
                m_InstallFunc = (InstallGlobalHook)Marshal.GetDelegateForFunctionPointer(GetProcAddress(m_NativeModule, "InstallGlobalHook"), typeof(InstallGlobalHook));
                m_RemoveFunc = (RemoveGlobalHook)Marshal.GetDelegateForFunctionPointer(GetProcAddress(m_NativeModule, "RemoveGlobalHook"), typeof(RemoveGlobalHook));
                Menu_Hook_Enable(null, null);
            }
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
            m_Hooks.Add(new VoodooHook(false, String.Empty, String.Empty, String.Empty));
            cHook_Table.Refresh();
        }

        private void Menu_Hook_Remove(object sender, EventArgs e)
        {
            m_Hooks.RemoveAt(cHook_Table.SelectedRows[0].Index);
            cHook_Table.Refresh();
        }

        private void SyncHookList()
        {
        }

        private void Form_OnResize(object sender, EventArgs e)
        {
            if (WindowState == FormWindowState.Minimized)
            {
                Hide();

                cTrayIcon.BalloonTipTitle = "Voodoo UI Hidden";
                cTrayIcon.BalloonTipText = "Voodoo will remain active and will be loaded by hooks while the UI is hidden.";
                cTrayIcon.ShowBalloonTip(5000);
            }
        }

        private void Notify_OnDblClick(object sender, EventArgs e)
        {
            Show();
            WindowState = FormWindowState.Normal;
        }

        private void Notify_OnClick(object sender, EventArgs e)
        {
            Show();
            WindowState = FormWindowState.Normal;
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
        }

        private void Menu_Hook_Disable(object sender, EventArgs e)
        {
            if (m_RemoveFunc != null)
            {
                m_RemoveFunc(m_GlobalHook);
                m_GlobalHook = IntPtr.Zero;
                cMenu_Hook_Off.Visible = true;
                cMenu_Hook_On.Visible = false;
            }
        }

        private void MainForm_Close(object sender, FormClosedEventArgs e)
        {
            if (m_GlobalHook != IntPtr.Zero)
            {
                Menu_Hook_Disable(null, null);
            }
        }

        private void HookTable_Selection(object sender, EventArgs e)
        {
            UpdateHookDetails();
        }

        private void UpdateHookDetails()
        {

        }

        private void RowChange(object sender, DataGridViewCellEventArgs e)
        {
            cHook_Active.Checked = (bool)cHook_Table.Rows[e.RowIndex].Cells["colActive"].Value;
            cHook_Name.Text   = cHook_Table.Rows[e.RowIndex].Cells["colName"].Value as String;
            cHook_Target.Text = cHook_Table.Rows[e.RowIndex].Cells["colTarget"].Value as String;
            cHook_Config.Text = cHook_Table.Rows[e.RowIndex].Cells["colConfig"].Value as String;
        }

        private void SaveHookDetails(object sender, EventArgs e)
        {
            cHook_Table.SelectedRows[0].Cells["colActive"].Value = cHook_Active.Checked;
            cHook_Table.SelectedRows[0].Cells["colName"].Value = cHook_Name.Text;
            cHook_Table.SelectedRows[0].Cells["colTarget"].Value = cHook_Target.Text;
            cHook_Table.SelectedRows[0].Cells["colConfig"].Value = cHook_Config.Text;

            CommitHooks();
        }

        private void LoadHooks()
        {
            m_Hooks = new List<VoodooHook>();

            RegistryKey hookRoot = Registry.CurrentUser.OpenSubKey(@"SOFTWARE\VoodooShader\Hooks", RegistryKeyPermissionCheck.ReadSubTree);
            foreach (String hooKeyName in hookRoot.GetSubKeyNames())
            {
                RegistryKey hookKey = hookRoot.OpenSubKey(hooKeyName);
                VoodooHook hook = new VoodooHook();
                hook.FromRegistryKey(hookKey);
                m_Hooks.Add(hook);
            }
        }

        private void CommitHooks()
        {
            RegistryKey voodooRoot = Registry.CurrentUser.OpenSubKey(@"SOFTWARE\VoodooShader", RegistryKeyPermissionCheck.ReadWriteSubTree);
            if (voodooRoot.OpenSubKey("Hooks") != null)
            {
                voodooRoot.DeleteSubKeyTree("Hooks");
            }
            RegistryKey hookRoot = voodooRoot.CreateSubKey("Hooks");
            foreach (VoodooHook hook in m_Hooks)
            {
                hook.ToRegistryKey(hookRoot);
            }
        }
    }
}
