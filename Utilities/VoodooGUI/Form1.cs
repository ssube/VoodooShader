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
    public partial class Form1 : Form
    {
        [DllImport("Voodoo_HookLoader.dll")]
        static extern bool InstallGlobalHook();

        [DllImport("Voodoo_HookLoader.dll")]
        static extern bool RemoveGlobalHook();

        public Form1()
        {
            InitializeComponent();
        }

        private void hook_Install(object sender, EventArgs e)
        {
            InstallGlobalHook();
        }

        private void hook_Remove(object sender, EventArgs e)
        {
            RemoveGlobalHook();
        }

        private void checkedListBox1_SelectedIndexChanged(object sender, EventArgs e)
        {

        }
    }
}
