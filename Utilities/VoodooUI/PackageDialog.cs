using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Data;
using System.Drawing;
using System.Text;
using System.Windows.Forms;

namespace VoodooUI
{
    public partial class PackageDialog : Form
    {
        public PackageDialog()
        {
            InitializeComponent();
        }

        private void ButtonCancel(object sender, EventArgs e)
        {
            this.DialogResult = DialogResult.Cancel;
            this.Close();
        }

        private void ButtonOK(object sender, EventArgs e)
        {
            this.DialogResult = DialogResult.OK;
            this.Close();
        }

        public String Origin
        {
            get { return cOrigin.Text; }
            set { cOrigin.Text = value; }
        }

        public String Branch
        {
            get { return cBranch.Text; }
            set { cBranch.Text = value; }
        }

        public String Path
        {
            get { return cPath.Text; }
            set { cPath.Text = value; }
        }
    }
}
