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
                m_RegistryTree.Nodes.Add(m_Registry.ToTreeNode());
            }
        }

        public void ExportRegistry(object sender, EventArgs e)
        {
            // Make sure any table changes get committed
            TreeNode node = m_RegistryTree.SelectedNode;
            if (node != null && m_KeyGrid.Rows.Count > 0)
            {
                List<KeyRow> rows = new List<KeyRow>(m_KeyGrid.Rows.Count);
                foreach (DataGridViewRow row in m_KeyGrid.Rows)
                {
                    rows.Add(new KeyRow(row.Cells[0].Value as String, row.Cells[1].Value as String, row.Cells[2].Value as String));
                }

                node.Tag = rows;
            }

            // Actually export to the registry
            TreeNode treeRoot = m_RegistryTree.Nodes[0];

            RegistryKey hive = Registry.CurrentUser;
            if (menu_Button_Hive.Checked)
            {
                hive = Registry.LocalMachine;
            }

            RegistryKey voodooRoot = hive.OpenSubKey(@"SOFTWARE\VoodooShader");
            if (voodooRoot != null)
            {
                hive.DeleteSubKeyTree(@"SOFTWARE\VoodooShader");
            }
            voodooRoot = hive.CreateSubKey(@"SOFTWARE\VoodooShader");

            SaveRegistryKey(voodooRoot, treeRoot);

            ImportRegistry(null, null);
        }

        public void SaveRegistryKey(RegistryKey key, TreeNode node)
        {
            if (node.Tag != null)
            {
                foreach (KeyRow row in node.Tag as List<KeyRow>)
                {
                    RegistryValueKind type = RegistryValueKind.String;
                    try { type = (RegistryValueKind)Enum.Parse(typeof(RegistryValueKind), row.Type as String, true); }
                    catch (Exception) {  }

                    try
                    {
                        key.SetValue(row.Name, row.Value, type);
                    }
                    catch (Exception exc)
                    {
                        MessageBox.Show(String.Format("Error setting value {0} in {1}:\n {2}", row.Name, key.Name, exc.Message));
                    }
                }
            }

            foreach (TreeNode subnode in node.Nodes)
            {
                RegistryKey subkey = key.CreateSubKey(subnode.Text);
                if (subkey != null)
                {
                    subkey = key.CreateSubKey(subnode.Text);
                    if (subkey == null)
                    {
                        MessageBox.Show(String.Format("Unable to create key \"{0}\".", subnode.FullPath), "Error", MessageBoxButtons.OK, MessageBoxIcon.Error);
                    }

                    SaveRegistryKey(subkey, subnode);
                }
            }
        }

        private void SelectNode(object sender, TreeNodeMouseClickEventArgs e)
        {
            TreeNode node = m_RegistryTree.SelectedNode;

            if (node != null && m_KeyGrid.Rows.Count > 0)
            {
                Dictionary<String, String> props = new Dictionary<String, String>(m_KeyGrid.Rows.Count);
                foreach (DataGridViewRow row in m_KeyGrid.Rows)
                {
                    props.Add(row.Cells[0].Value as String, row.Cells[1].Value as String);
                }

                (node.Tag as VoodooElement).Properties = props;
            }

            m_KeyGrid.Rows.Clear();

            if (e.Node.Tag != null)
            {
                foreach (KeyValuePair<String, String> prop in (e.Node.Tag as VoodooElement).Properties)
                {
                    m_KeyGrid.Rows.Add(prop.Key, prop.Value);
                }
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

                        TreeNode classesNode = m_RegistryTree.Nodes[0].Nodes.Find("Classes", false)[0];

                        for (UInt32 i = 0; i < module.Count; ++i)
                        {
                            ClassInfo classinfo = module[i];

                            List<KeyRow> classdata = new List<KeyRow>();

                            classdata.Add(new KeyRow("Module", "String", Convert.ToString(version.LibID)));
                            classdata.Add(new KeyRow("Name", "String", classinfo.Name));
                            classesNode.Tag = classdata;

                            AddNodeTo(classesNode, Convert.ToString(classinfo.ClsID)).Tag = classdata;
                        }

                        // Add module to the list
                        TreeNode modulesNode = m_RegistryTree.Nodes[0].Nodes.Find("Modules", false)[0];

                        List<KeyRow> moduledata = new List<KeyRow>();

                        moduledata.Add(new KeyRow("ModulePath", "String", moduleFile));
                        moduledata.Add(new KeyRow("ConfigPath", "String", String.Empty));
                        moduledata.Add(new KeyRow("RemotePath", "String", String.Empty));
                        moduledata.Add(new KeyRow("Name", "String", version.Name));

                        AddNodeTo(modulesNode, Convert.ToString(version.LibID)).Tag = moduledata;
                    }
                    catch (System.Exception ex)
                    {
                        MessageBox.Show("Unable to load module:\n" + ex.Message, "Error", MessageBoxButtons.OK, MessageBoxIcon.Error);
                    }
                }
            }
        }
    }
}
