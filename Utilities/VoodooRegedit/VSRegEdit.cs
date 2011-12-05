using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Data;
using System.Drawing;
using System.Text;
using System.Windows.Forms;
using Microsoft.Win32;
using VoodooNetClasses;

namespace VoodooRegedit
{
    public partial class VSRegEdit : Form
    {
        GlobalRegistry m_Registry;
        public TreeNode RootNode;

        public VSRegEdit()
        {
            InitializeComponent();
        }

        public void ImportRegistry(object sender, EventArgs e)
        {
            m_Registry = VoodooRegistry.Read();
        }

        public void ExportRegistry(object sender, EventArgs e)
        {
            // Actually export to the registry
            VoodooRegistry.Write(m_Registry);

            ImportRegistry(null, null);
        }

        private void SelectNode(object sender, TreeViewCancelEventArgs e)
        {
            m_KeyGrid.AutoGenerateColumns = false;

            if (e.Node.Name == "Node_Root")
            {
                m_KeyGrid.DataSource = m_Registry;
            } else if (e.Node.Name == "Node_Remotes")
            {
                m_KeyGrid.Columns.Clear();

                DataGridViewTextBoxColumn col_Uri = new DataGridViewTextBoxColumn();
                col_Uri.HeaderText = col_Uri.DataPropertyName = "Uri";
                col_Uri.AutoSizeMode = DataGridViewAutoSizeColumnMode.Fill;

                m_KeyGrid.Columns.Add(col_Uri);

                m_KeyGrid.DataSource = m_Registry.Remotes;
            } else if (e.Node.Name == "Node_Packages")
            {
                m_KeyGrid.Columns.Clear();

                DataGridViewTextBoxColumn col_PackID = new DataGridViewTextBoxColumn();
                col_PackID.HeaderText = col_PackID.DataPropertyName = "PackID";
                col_PackID.AutoSizeMode = DataGridViewAutoSizeColumnMode.AllCells; col_PackID.FillWeight = 120;

                DataGridViewTextBoxColumn col_Name = new DataGridViewTextBoxColumn();
                col_Name.HeaderText = col_Name.DataPropertyName = "Name";
                col_Name.AutoSizeMode = DataGridViewAutoSizeColumnMode.AllCells; col_Name.FillWeight = 110;

                DataGridViewTextBoxColumn col_Version = new DataGridViewTextBoxColumn();
                col_Version.HeaderText = col_Version.DataPropertyName = "Version";
                col_Version.AutoSizeMode = DataGridViewAutoSizeColumnMode.AllCells; col_Version.FillWeight = 100;

                DataGridViewTextBoxColumn col_Date = new DataGridViewTextBoxColumn();
                col_Date.HeaderText = col_Date.DataPropertyName = "Date";
                col_Date.AutoSizeMode = DataGridViewAutoSizeColumnMode.AllCells; col_Date.FillWeight = 90;

                DataGridViewTextBoxColumn col_Props = new DataGridViewTextBoxColumn();
                col_Props.HeaderText = col_Props.DataPropertyName = "Props";
                col_Props.AutoSizeMode = DataGridViewAutoSizeColumnMode.Fill; col_Props.FillWeight = 80;

                m_KeyGrid.Columns.Add(col_PackID);
                m_KeyGrid.Columns.Add(col_Name);
                m_KeyGrid.Columns.Add(col_Version);
                m_KeyGrid.Columns.Add(col_Date);
                m_KeyGrid.Columns.Add(col_Props);

                m_KeyGrid.DataSource = m_Registry.Packages;
            } else if (e.Node.Name == "Node_Modules")
            {
                m_KeyGrid.Columns.Clear();

                DataGridViewTextBoxColumn col_LibID = new DataGridViewTextBoxColumn();
                col_LibID.HeaderText = col_LibID.DataPropertyName = "LibID";
                col_LibID.AutoSizeMode = DataGridViewAutoSizeColumnMode.AllCells; col_LibID.FillWeight = 120;

                DataGridViewTextBoxColumn col_Name = new DataGridViewTextBoxColumn();
                col_Name.HeaderText = col_Name.DataPropertyName = "Name";
                col_Name.AutoSizeMode = DataGridViewAutoSizeColumnMode.AllCells; col_Name.FillWeight = 110;

                DataGridViewTextBoxColumn col_PackID = new DataGridViewTextBoxColumn();
                col_PackID.HeaderText = col_PackID.DataPropertyName = "PackID";
                col_PackID.AutoSizeMode = DataGridViewAutoSizeColumnMode.AllCells; col_PackID.FillWeight = 100;

                DataGridViewTextBoxColumn col_Props = new DataGridViewTextBoxColumn();
                col_Props.HeaderText = col_Props.DataPropertyName = "Props";
                col_Props.AutoSizeMode = DataGridViewAutoSizeColumnMode.Fill; col_Props.FillWeight = 90;

                m_KeyGrid.Columns.Add(col_PackID);
                m_KeyGrid.Columns.Add(col_Name);
                m_KeyGrid.Columns.Add(col_LibID);
                m_KeyGrid.Columns.Add(col_Props);

                m_KeyGrid.DataSource = m_Registry.Modules;
            } else if (e.Node.Name == "Node_Classes")
            {
                m_KeyGrid.Columns.Clear();

                DataGridViewTextBoxColumn col_ClassID = new DataGridViewTextBoxColumn();
                col_ClassID.HeaderText = col_ClassID.DataPropertyName = "ClassID";
                col_ClassID.AutoSizeMode = DataGridViewAutoSizeColumnMode.AllCells; col_ClassID.FillWeight = 120;

                DataGridViewTextBoxColumn col_Name = new DataGridViewTextBoxColumn();
                col_Name.HeaderText = col_Name.DataPropertyName = "Name";
                col_Name.AutoSizeMode = DataGridViewAutoSizeColumnMode.AllCells; col_Name.FillWeight = 110;

                DataGridViewTextBoxColumn col_LibID = new DataGridViewTextBoxColumn();
                col_LibID.HeaderText = col_LibID.DataPropertyName = "LibID";
                col_LibID.AutoSizeMode = DataGridViewAutoSizeColumnMode.AllCells; col_LibID.FillWeight = 100;

                DataGridViewTextBoxColumn col_Props = new DataGridViewTextBoxColumn();
                col_Props.HeaderText = col_Props.DataPropertyName = "Props";
                col_Props.AutoSizeMode = DataGridViewAutoSizeColumnMode.Fill; col_Props.FillWeight = 90;

                m_KeyGrid.Columns.Add(col_ClassID);
                m_KeyGrid.Columns.Add(col_Name);
                m_KeyGrid.Columns.Add(col_LibID);
                m_KeyGrid.Columns.Add(col_Props);

                m_KeyGrid.DataSource = m_Registry.Classes;
            } else if (e.Node.Name == "Node_Hooks")
            {
                m_KeyGrid.Columns.Clear();

                DataGridViewCheckBoxColumn col_Active = new DataGridViewCheckBoxColumn();
                col_Active.HeaderText = col_Active.DataPropertyName = "Active";
                col_Active.AutoSizeMode = DataGridViewAutoSizeColumnMode.AllCells; col_Active.FillWeight = 120;

                DataGridViewTextBoxColumn col_Name = new DataGridViewTextBoxColumn();
                col_Name.HeaderText = col_Name.DataPropertyName = "Name";
                col_Name.AutoSizeMode = DataGridViewAutoSizeColumnMode.AllCells; col_Name.FillWeight = 110;

                DataGridViewTextBoxColumn col_Target = new DataGridViewTextBoxColumn();
                col_Target.HeaderText = col_Target.DataPropertyName = "Target";
                col_Target.AutoSizeMode = DataGridViewAutoSizeColumnMode.Fill; col_Target.FillWeight = 100;

                DataGridViewTextBoxColumn col_Config = new DataGridViewTextBoxColumn();
                col_Config.HeaderText = col_Config.DataPropertyName = "Config";
                col_Config.AutoSizeMode = DataGridViewAutoSizeColumnMode.Fill; col_Config.FillWeight = 90;

                m_KeyGrid.Columns.Add(col_Active);
                m_KeyGrid.Columns.Add(col_Name);
                m_KeyGrid.Columns.Add(col_Target);
                m_KeyGrid.Columns.Add(col_Config);

                m_KeyGrid.DataSource = m_Registry.Hooks;
            }
            else if (e.Node.Name == "Node_Defaults")
            {
                m_KeyGrid.Columns.Clear();

                DataGridViewTextBoxColumn col_DefID = new DataGridViewTextBoxColumn();
                col_DefID.HeaderText = col_DefID.DataPropertyName = "DefID";
                col_DefID.AutoSizeMode = DataGridViewAutoSizeColumnMode.AllCells; col_DefID.FillWeight = 120;

                DataGridViewTextBoxColumn col_Name = new DataGridViewTextBoxColumn();
                col_Name.HeaderText = col_Name.DataPropertyName = "Name";
                col_Name.AutoSizeMode = DataGridViewAutoSizeColumnMode.AllCells; col_Name.FillWeight = 110;

                DataGridViewTextBoxColumn col_Target = new DataGridViewTextBoxColumn();
                col_Target.HeaderText = col_Target.DataPropertyName = "Target";
                col_Target.AutoSizeMode = DataGridViewAutoSizeColumnMode.Fill; col_Target.FillWeight = 100;

                DataGridViewTextBoxColumn col_Config = new DataGridViewTextBoxColumn();
                col_Config.HeaderText = col_Config.DataPropertyName = "Config";
                col_Config.AutoSizeMode = DataGridViewAutoSizeColumnMode.Fill; col_Config.FillWeight = 90;

                m_KeyGrid.Columns.Add(col_DefID);
                m_KeyGrid.Columns.Add(col_Name);
                m_KeyGrid.Columns.Add(col_Target);
                m_KeyGrid.Columns.Add(col_Config);

                m_KeyGrid.DataSource = m_Registry.Defaults;
            }
        }

        private void Menu_Key_Add(object sender, EventArgs e)
        {
            object source = m_KeyGrid.DataSource;
            if (source.GetType() == typeof(List<VoodooRemote>))
            {
                ((List<VoodooRemote>)source).Add(new VoodooRemote());
            }
            else if (source.GetType() == typeof(List<VoodooPackage>))
            {
                ((List<VoodooPackage>)source).Add(new VoodooPackage());
            }
            else if (source.GetType() == typeof(List<VoodooModule>))
            {
                ((List<VoodooModule>)source).Add(new VoodooModule());
            }
            else if (source.GetType() == typeof(List<VoodooClass>))
            {
                ((List<VoodooClass>)source).Add(new VoodooClass());
            }
            else if (source.GetType() == typeof(List<VoodooHook>))
            {
                ((List<VoodooHook>)source).Add(new VoodooHook());
            }
            else if (source.GetType() == typeof(List<VoodooDefault>))
            {
                ((List<VoodooDefault>)source).Add(new VoodooDefault());
            }
            m_KeyGrid.Refresh();
        }

        private void Menu_Key_Delete(object sender, EventArgs e)
        {
            if (m_KeyGrid.SelectedRows == null || m_KeyGrid.SelectedRows.Count == 0)
            {
                return;
            }

            object source = m_KeyGrid.DataSource;
            if (source.GetType() == typeof(List<VoodooRemote>))
            {
                ((List<VoodooRemote>)source).RemoveAt(m_KeyGrid.SelectedRows[0].Index);
            }
            else if (source.GetType() == typeof(List<VoodooPackage>))
            {
                ((List<VoodooPackage>)source).RemoveAt(m_KeyGrid.SelectedRows[0].Index);
            }
            else if (source.GetType() == typeof(List<VoodooModule>))
            {
                ((List<VoodooModule>)source).RemoveAt(m_KeyGrid.SelectedRows[0].Index);
            }
            else if (source.GetType() == typeof(List<VoodooClass>))
            {
                ((List<VoodooClass>)source).RemoveAt(m_KeyGrid.SelectedRows[0].Index);
            }
            else if (source.GetType() == typeof(List<VoodooHook>))
            {
                ((List<VoodooHook>)source).RemoveAt(m_KeyGrid.SelectedRows[0].Index);
            }
            else if (source.GetType() == typeof(List<VoodooDefault>))
            {
                ((List<VoodooDefault>)source).RemoveAt(m_KeyGrid.SelectedRows[0].Index);
            }
            m_KeyGrid.Refresh();
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

                            m_Registry.Classes.Add(new VoodooClass(classinfo.ClassID, version.LibID, classinfo.Name, String.Empty));
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
