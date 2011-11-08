using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Data;
using System.Drawing;
using System.Text;
using System.Windows.Forms;
using Microsoft.Win32;

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
        public TreeNode RootNode;

        public VSRegEdit()
        {
            InitializeComponent();
        }

        public void ImportRegistry(object sender, EventArgs e)
        {
            m_RegistryTree.Nodes.Clear();

            RootNode = new TreeNode("Voodoo Registry Root");

            RegistryKey voodooRoot = Registry.CurrentUser.OpenSubKey(@"SOFTWARE\VoodooShader");
            if (voodooRoot == null)
            {
                voodooRoot = Registry.CurrentUser.CreateSubKey(@"SOFTWARE\VoodooShader");
                if (voodooRoot == null)
                {
                    MessageBox.Show("Unable to open or create Voodoo root key.", "Error", MessageBoxButtons.OK, MessageBoxIcon.Error);
                    return;
                }
            }
            else
            {
                LoadRegistryKey(ref voodooRoot, ref RootNode);
            }

            m_RegistryTree.Nodes.Add(RootNode);
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

            RegistryKey voodooRoot = Registry.CurrentUser.OpenSubKey(@"SOFTWARE\VoodooShader");
            if (voodooRoot != null)
            {
                Registry.CurrentUser.DeleteSubKeyTree(@"SOFTWARE\VoodooShader");
            }
            voodooRoot = Registry.CurrentUser.CreateSubKey(@"SOFTWARE\VoodooShader");

            SaveRegistryKey(voodooRoot, treeRoot);

            ImportRegistry(null, null);
        }

        public void LoadRegistryKey(ref RegistryKey key, ref TreeNode node)
        {
            String[] valnames = key.GetValueNames();
            List<KeyRow> rows = new List<KeyRow>(key.ValueCount);

            for (int i = 0; i < key.ValueCount; ++i)
            {
                String valvalue = Convert.ToString(key.GetValue(valnames[i]));
                String valtype = Convert.ToString(key.GetValueKind(valnames[i]));
                KeyRow row = new KeyRow(valnames[i], valtype, valvalue);
                rows.Add(row);
            }

            node.Tag = rows;

            foreach (String subkeyname in key.GetSubKeyNames())
            {
                TreeNode subnode = new TreeNode(subkeyname);
                RegistryKey subkey = key.OpenSubKey(subkeyname);

                LoadRegistryKey(ref subkey, ref subnode);

                node.Nodes.Add(subnode);
            }
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
                List<KeyRow> rows = new List<KeyRow>(m_KeyGrid.Rows.Count);
                foreach (DataGridViewRow row in m_KeyGrid.Rows)
                {
                    rows.Add(new KeyRow(row.Cells[0].Value as String, row.Cells[1].Value as String, row.Cells[2].Value as String));
                }

                node.Tag = rows;
            }

            m_KeyGrid.Rows.Clear();

            if (e.Node.Tag != null)
            {
                foreach (KeyRow row in e.Node.Tag as List<KeyRow>)
                {
                    m_KeyGrid.Rows.Add(row.Name, row.Type, row.Value);
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

        private void AddNodeTo(TreeNode parent)
        {
            KeyEdit dialog = new KeyEdit();

            if (dialog.ShowDialog() == DialogResult.OK)
            {
                parent.Nodes.Add(dialog.Value);
            }
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
    }
}
