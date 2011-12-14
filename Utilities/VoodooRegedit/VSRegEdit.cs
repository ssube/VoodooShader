using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Data;
using System.Drawing;
using System.Text;
using System.Windows.Forms;
using Microsoft.Win32;
using VoodooSharp;

namespace VoodooRegedit
{
    public partial class VSRegEdit : Form
    {
        public TreeNode RootNode;

        public VSRegEdit()
        {
            InitializeComponent();
        }

        public void ImportRegistry(object sender, EventArgs e)
        {
            GlobalRegistry.Instance.Update();
        }

        public void ExportRegistry(object sender, EventArgs e)
        {
            // Actually export to the registry
            GlobalRegistry.Instance.Write();

            ImportRegistry(null, null);
        }

        private void SelectNode(object sender, TreeViewCancelEventArgs e)
        {
            m_KeyGrid.AutoGenerateColumns = false;

            if (e.Node.Name == "Node_Root")
            {
                m_KeyGrid.DataSource = GlobalRegistry.Instance;
            } else if (e.Node.Name == "Node_Remotes")
            {
                m_KeyGrid.Columns.Clear();

                DataGridViewTextBoxColumn col_Uri = new DataGridViewTextBoxColumn();
                col_Uri.HeaderText = col_Uri.DataPropertyName = "Uri";
                col_Uri.AutoSizeMode = DataGridViewAutoSizeColumnMode.Fill;

                m_KeyGrid.Columns.Add(col_Uri);

                m_KeyGrid.DataSource = GlobalRegistry.Instance.Remotes;
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

                m_KeyGrid.DataSource = GlobalRegistry.Instance.Packages;
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

                m_KeyGrid.DataSource = GlobalRegistry.Instance.Modules;
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

                m_KeyGrid.DataSource = GlobalRegistry.Instance.Classes;
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

                m_KeyGrid.DataSource = GlobalRegistry.Instance.Hooks;
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

                m_KeyGrid.DataSource = GlobalRegistry.Instance.Defaults;
            }
        }

        private void Menu_Key_Add(object sender, EventArgs e)
        {
            object source = m_KeyGrid.DataSource;
            if (source.GetType() == typeof(List<Remote>))
            {
                ((List<Remote>)source).Add(new Remote());
            }
            else if (source.GetType() == typeof(List<Package>))
            {
                ((List<Package>)source).Add(new Package());
            }
            else if (source.GetType() == typeof(List<Module>))
            {
                ((List<Module>)source).Add(new Module());
            }
            else if (source.GetType() == typeof(List<Class>))
            {
                ((List<Class>)source).Add(new Class());
            }
            else if (source.GetType() == typeof(List<Hook>))
            {
                ((List<Hook>)source).Add(new Hook());
            }
            else if (source.GetType() == typeof(List<Default>))
            {
                ((List<Default>)source).Add(new Default());
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
            if (source.GetType() == typeof(List<Remote>))
            {
                ((List<Remote>)source).RemoveAt(m_KeyGrid.SelectedRows[0].Index);
            }
            else if (source.GetType() == typeof(List<Package>))
            {
                ((List<Package>)source).RemoveAt(m_KeyGrid.SelectedRows[0].Index);
            }
            else if (source.GetType() == typeof(List<Module>))
            {
                ((List<Module>)source).RemoveAt(m_KeyGrid.SelectedRows[0].Index);
            }
            else if (source.GetType() == typeof(List<Class>))
            {
                ((List<Class>)source).RemoveAt(m_KeyGrid.SelectedRows[0].Index);
            }
            else if (source.GetType() == typeof(List<Hook>))
            {
                ((List<Hook>)source).RemoveAt(m_KeyGrid.SelectedRows[0].Index);
            }
            else if (source.GetType() == typeof(List<Default>))
            {
                ((List<Default>)source).RemoveAt(m_KeyGrid.SelectedRows[0].Index);
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
                        Module mod = new Module();
                        mod.LibId = version.LibID;
                        mod.Name = version.Name;
                        mod.Path = moduleFile;
                        GlobalRegistry.Instance.Modules.Add(mod);

                        // Add classes
                        for (UInt32 i = 0; i < module.Count; ++i)
                        {
                            ClassInfo classinfo = module[i];

                            Class cls = new Class();
                            cls.ClassId = classinfo.ClassID;
                            cls.Module = version.LibID;
                            cls.Name = classinfo.Name;
                            GlobalRegistry.Instance.Classes.Add(cls);
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
