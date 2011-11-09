using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Data;
using System.Drawing;
using System.Text;
using System.Runtime.InteropServices;
using System.Windows.Forms;

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

        private delegate bool InstallGlobalHook();
        private delegate bool RemoveGlobalHook();

        private delegate bool AddModuleHook([MarshalAs(UnmanagedType.LPWStr)] String name);
        private delegate bool RemoveModuleHook([MarshalAs(UnmanagedType.LPWStr)] String name);
        private delegate bool RemoveAllModuleHooks();

        private IntPtr m_NativeModule;

        private InstallGlobalHook m_InstallFunc;
        private RemoveGlobalHook m_RemoveFunc;

        private AddModuleHook m_AddHook;
        private RemoveModuleHook m_RemoveHook;
        private RemoveAllModuleHooks m_RemoveAllHooks;

        public MainForm()
        {
            InitializeComponent();

            m_NativeModule = LoadLibrary("Voodoo_HookStub.dll");
            if (m_NativeModule != IntPtr.Zero)
            {
                m_InstallFunc = (InstallGlobalHook)Marshal.GetDelegateForFunctionPointer(GetProcAddress(m_NativeModule, "InstallGlobalHook"), typeof(InstallGlobalHook));
                m_RemoveFunc = (RemoveGlobalHook)Marshal.GetDelegateForFunctionPointer(GetProcAddress(m_NativeModule, "RemoveGlobalHook"), typeof(RemoveGlobalHook));

                m_AddHook = (AddModuleHook)Marshal.GetDelegateForFunctionPointer(GetProcAddress(m_NativeModule, "AddModule"), typeof(AddModuleHook));
                m_RemoveHook = (RemoveModuleHook)Marshal.GetDelegateForFunctionPointer(GetProcAddress(m_NativeModule, "RemoveModule"), typeof(RemoveModuleHook));
                m_RemoveAllHooks = (RemoveAllModuleHooks)Marshal.GetDelegateForFunctionPointer(GetProcAddress(m_NativeModule, "RemoveAllModules"), typeof(RemoveAllModuleHooks));
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
                "Using this function will retrieve a list of supported applications from the Voodoo server and scan your hard drive for installed programs.\n"+
                "This uses file names only and will only scan folders you can open; file contents will not be accessed and no file data will be sent to the server.\n"+
                "This may take some time if you have many files. A list of possible programs will be shown when the scan is done.\n"+
                "Are you sure you want to run this scan?", 
                "Confirm Scan", MessageBoxButtons.YesNo, MessageBoxIcon.Information) == DialogResult.Yes)
            {
                Form scanWindow = new ScanForm();
                scanWindow.ShowDialog();
            }
        }

        private void Menu_Hook_Add(object sender, EventArgs e)
        {
            m_InstallFunc();
        }

        private void Menu_Hook_Remove(object sender, EventArgs e)
        {
            m_RemoveFunc();
        }

        private void SyncHookList()
        {
            m_RemoveAllHooks();

            foreach (DataGridViewRow row in dataGridView1.Rows)
            {
                bool active = (bool)row.Cells[0].Value;
                if (active)
                {
                    String target = (String)row.Cells[3].Value;
                    m_AddHook(target);
                }
            }
        }
    }
}
