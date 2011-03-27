using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Data;
using System.Drawing;
using System.Net;
using System.Text;
using System.Windows.Forms;
using System.Xml;
using System.Xml.Linq;

namespace Installer
{
    public partial class Form1 : Form
    {
        private XDocument mVersions;

        public Form1()
        {
            InitializeComponent();

            UpdateAvailable();
        }

        private void UpdateAvailable()
        {
            while ( !UpdateVersions() )
            {
                DialogResult button = System.Windows.Forms.MessageBox.Show
                (
                    "Unable to get latest version information.", "Update Error", MessageBoxButtons.RetryCancel, MessageBoxIcon.Error
                );

                if ( button == DialogResult.Cancel )
                {
                    break;
                }
            }

            gridAvailable.Rows.Clear();
            if (mVersions != null)
            {
                IEnumerable<XElement> modules = mVersions.Elements("Module");
                foreach (XElement module in modules)
                {
                    String name = module.Attribute("name").Value;
                    String version = module.Attribute("version").Value;
                    gridAvailable.Rows.Add(name, version);
                }
            }
        }

        private bool UpdateVersions()
        {
            mVersions = null;

            try
            {
                mVersions = XDocument.Load("http://localhost/voodoo/versions.xml");
                return true;
            }
            catch (Exception exc)
            {
                System.Windows.Forms.MessageBox.Show("Error getting version information:\n" + exc.Message);
                return false;
            }
        } 
    }
}
