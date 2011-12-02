using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Data;
using System.Drawing;
using System.Text;
using System.Windows.Forms;
using Microsoft.Win32;
using System.Security.Principal;
using VoodooNetClasses;

namespace VoodooRegedit
{
    public struct KeyRow
    {
        public String Name, Type, Value;

        public KeyRow(String name, String type, String value)
        {
            Name = name;
            Type = type;
            Value = value;
        }
    }

    public partial class VSRegEdit : Form
    {
        VoodooRegistry m_Registry;
        public TreeNode RootNode;

        public VSRegEdit()
        {
            InitializeComponent();

            if (!IsUserAdministrator())
            {
                menu_Button_Hive.Enabled = false;
            }
        }

        public bool IsUserAdministrator()
        {
            //bool value to hold our return value
            bool isAdmin;
            try
            {
                //get the currently logged in user
                WindowsIdentity user = WindowsIdentity.GetCurrent();
                WindowsPrincipal principal = new WindowsPrincipal(user);
                isAdmin = principal.IsInRole(WindowsBuiltInRole.Administrator);
            }
            catch (UnauthorizedAccessException)
            {
                isAdmin = false;
            }
            catch (Exception)
            {
                isAdmin = false;
            }
            return isAdmin;
        }

        public void ImportRegistry(object sender, EventArgs e)
        {
            RegistryKey hive = Registry.CurrentUser;
            if (menu_Button_Hive.Checked)
            {
                hive = Registry.LocalMachine;
            }

            RegistryKey voodooRoot = hive.OpenSubKey(@"SOFTWARE\VoodooShader");
            if (voodooRoot == null)
            {
                voodooRoot = hive.CreateSubKey(@"SOFTWARE\VoodooShader");
                if (voodooRoot == null)
                {
                    MessageBox.Show("Unable to open or create Voodoo root key.", "Error", MessageBoxButtons.OK, MessageBoxIcon.Error);
                    return;
                }
            }
            else
            {
                m_Registry = new VoodooRegistry();
                m_Registry.FromRegistryKey(voodooRoot);
            }
        }

        public void ExportRegistry(object sender, EventArgs e)
        {
            // Actually export to the registry
            RegistryKey voodooRoot;
            if (menu_Button_Hive.Checked)
            {
                voodooRoot = Registry.LocalMachine.OpenSubKey(@"SOFTWARE", true);
            } else {
                voodooRoot = Registry.CurrentUser.OpenSubKey(@"SOFTWARE", true);
            }

            m_Registry.ToRegistryKey(voodooRoot);

            ImportRegistry(null, null);
        }

        private void SelectNode(object sender, TreeViewCancelEventArgs e)
        {
            m_KeyGrid.AutoGenerateColumns = true;
            if (e.Node.Name == "Node_Root")
            {
                m_KeyGrid.DataSource = m_Registry;
            } else if (e.Node.Name == "Node_Remotes")
            {
                m_KeyGrid.DataSource = m_Registry.Remotes;
            } else if (e.Node.Name == "Node_Packages")
            {
                m_KeyGrid.DataSource = m_Registry.Packages;
            } else if (e.Node.Name == "Node_Modules")
            {
                m_KeyGrid.DataSource = m_Registry.Modules;
            } else if (e.Node.Name == "Node_Classes")
            {
                m_KeyGrid.DataSource = m_Registry.Classes;
            } else if (e.Node.Name == "Node_Hooks")
            {
                m_KeyGrid.DataSource = m_Registry.Hooks;
            }
            else if (e.Node.Name == "Node_Defaults")
            {
                m_KeyGrid.DataSource = m_Registry.Defaults;
            }
        }

        private void Menu_Key_Add(object sender, EventArgs e)
        {
            object source = m_KeyGrid.DataSource;
            Type ot = source.GetType().GetGenericArguments()[0];
            Type lt = typeof(List<ot>);
            ((lt)source).Add(Activator.CreateInstance(ot));
        }

        private void Menu_Key_Delete(object sender, EventArgs e)
        {
            if (m_KeyGrid.SelectedRows != null && m_KeyGrid.SelectedRows.Count > 0)
            {
                m_KeyGrid.Rows.Remove(m_KeyGrid.SelectedRows[0]);
            }
        }

        private void Menu_Module_Import(object sender, EventArgs e)
        {
            if (openFileDialog1.ShowDialog() == DialogResult.OK)
            {
                String[] files = openFileDialog1.FileNames;

                foreach (String moduleFile in files)
                {
                    try
                    {
                        NativeModule module = new NativeModule(moduleFile);
                        ModuleVersion version = module.Version;
                        String libid = Convert.ToString(version.LibID);

                        // Add module
                        m_Registry.Modules.Add(new VoodooModule(version.LibID, Guid.Empty, version.Name, moduleFile, String.Empty, String.Empty));

                        // Add classes
                        for (UInt32 i = 0; i < module.Count; ++i)
                        {
                            ClassInfo classinfo = module[i];

                            m_Registry.Classes.Add(new VoodooClass(classinfo.ClsID, version.LibID, classinfo.Name, String.Empty));
                        }

                        // Export again
                    }
                    catch (System.Exception ex)
                    {
                        MessageBox.Show("Unable to load module:\n" + ex.Message, "Error", MessageBoxButtons.OK, MessageBoxIcon.Error);
                    }
                }
            }
        }

        private void Menu_Xml_Import(object sender, EventArgs e)
        {

        }
    }
}
