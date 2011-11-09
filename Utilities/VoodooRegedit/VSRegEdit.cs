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
            m_RegistryTree.Nodes.Clear();

            RootNode = new TreeNode("Voodoo Registry Root");

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
                m_Registry = VoodooRegistry.FromRegistryKey(voodooRoot);
                RootNode = m_Registry.ToTreeNode();
                m_RegistryTree.Nodes.Add(RootNode);
            }
        }

        public void ExportRegistry(object sender, EventArgs e)
        {
            SyncRows();

            // Actually export to the registry
            m_Registry = VoodooRegistry.FromTreeNode(RootNode);

            RegistryKey voodooRoot;
            if (menu_Button_Hive.Checked)
            {
                voodooRoot = Registry.LocalMachine.OpenSubKey(@"SOFTWARE\VoodooShader", true);
            } else {
                voodooRoot = Registry.CurrentUser.OpenSubKey(@"SOFTWARE\VoodooShader", true);
            }

            m_Registry.ToRegistryKey(voodooRoot);

            ImportRegistry(null, null);
        }

        private void SelectNode(object sender, TreeViewCancelEventArgs e)
        {
            SyncRows();

            m_KeyGrid.Rows.Clear();

            if (e.Node.Tag != null)
            {
                foreach (KeyValuePair<String, String> prop in e.Node.Tag as Dictionary<String, String>)
                {
                    m_KeyGrid.Rows.Add(prop.Key, prop.Value);
                }
            }
        }

        private void SyncRows()
        {
            if (m_KeyGrid.IsCurrentCellDirty)
            {
                m_KeyGrid.CommitEdit(DataGridViewDataErrorContexts.Commit);
            }

            TreeNode node = m_RegistryTree.SelectedNode;

            if (node != null)
            {
                Dictionary<String, String> props = new Dictionary<String, String>(m_KeyGrid.Rows.Count);

                foreach (DataGridViewRow row in m_KeyGrid.Rows)
                {
                    props.Add(row.Cells[0].Value as String, row.Cells[1].Value as String);
                }

                node.Tag = props;
            }
        }

        private void Menu_Node_Edit(object sender, EventArgs e)
        {
            KeyEdit dialog = new KeyEdit();
            dialog.Value = m_RegistryTree.SelectedNode.Text;

            if (dialog.ShowDialog() == DialogResult.OK)
            {
                m_RegistryTree.SelectedNode.Text = dialog.Value;
            }
        }

        private void Menu_Node_AddChild(object sender, EventArgs e)
        {
            TreeNode node = m_RegistryTree.SelectedNode;
            if (node != null)
            {
                AddNodeTo(node);
            }
        }

        private void Menu_Node_AddSibling(object sender, EventArgs e)
        {
            TreeNode node = m_RegistryTree.SelectedNode;
            if (node != null && node != RootNode)
            {
                node = node.Parent;
                if (node != null)
                {
                    AddNodeTo(node);
                }
            }
        }

        private TreeNode AddNodeTo(TreeNode parent, String text = null)
        {
            if (text == null)
            {
                KeyEdit dialog = new KeyEdit();

                if (dialog.ShowDialog() == DialogResult.OK)
                {
                    text = dialog.Value;
                }
            }

            return parent.Nodes.Add(text, text);
        }

        private void Menu_Node_Remove(object sender, EventArgs e)
        {
            TreeNode node = m_RegistryTree.SelectedNode;
            if (node != null && node != RootNode)
            {
                node.Parent.Nodes.Remove(node);
            }
        }

        private void Menu_Key_Add(object sender, EventArgs e)
        {
            m_KeyGrid.Rows.Add();
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
                        TreeNode modulesNode = m_RegistryTree.Nodes[0].Nodes["modules"];

                        Dictionary<String, String> moduledata = new Dictionary<String, String>();

                        moduledata.Add("modulePath", moduleFile);
                        moduledata.Add("configPath", String.Empty);
                        moduledata.Add("remotePath", String.Empty);
                        moduledata.Add("name", version.Name);

                        AddNodeTo(modulesNode, libid).Tag = moduledata;

                        // Add classes
                        TreeNode classesNode = m_RegistryTree.Nodes[0].Nodes["classes"];

                        for (UInt32 i = 0; i < module.Count; ++i)
                        {
                            ClassInfo classinfo = module[i];

                            Dictionary<String, String> classdata = new Dictionary<String, String>();

                            classdata.Add("module", libid);
                            classdata.Add("name", classinfo.Name);

                            AddNodeTo(classesNode, Convert.ToString(classinfo.ClsID)).Tag = classdata;
                        }
                    }
                    catch (System.Exception ex)
                    {
                        MessageBox.Show("Unable to load module:\n" + ex.Message, "Error", MessageBoxButtons.OK, MessageBoxIcon.Error);
                    }
                }

                m_Registry = VoodooRegistry.FromTreeNode(RootNode);
            }
        }
    }
}
