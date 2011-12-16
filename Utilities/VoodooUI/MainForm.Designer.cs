namespace VoodooUI
{
    partial class MainForm
    {
        /// <summary>
        /// Required designer variable.
        /// </summary>
        private System.ComponentModel.IContainer components = null;

        /// <summary>
        /// Clean up any resources being used.
        /// </summary>
        /// <param name="disposing">true if managed resources should be disposed; otherwise, false.</param>
        protected override void Dispose(bool disposing)
        {
            if (disposing && (components != null))
            {
                components.Dispose();
            }
            base.Dispose(disposing);
        }

        #region Windows Form Designer generated code

        /// <summary>
        /// Required method for Designer support - do not modify
        /// the contents of this method with the code editor.
        /// </summary>
        private void InitializeComponent()
        {
            this.components = new System.ComponentModel.Container();
            System.ComponentModel.ComponentResourceManager resources = new System.ComponentModel.ComponentResourceManager(typeof(MainForm));
            System.Windows.Forms.DataGridViewCellStyle dataGridViewCellStyle1 = new System.Windows.Forms.DataGridViewCellStyle();
            this.toolStripContainer1 = new System.Windows.Forms.ToolStripContainer();
            this.splitContainer1 = new System.Windows.Forms.SplitContainer();
            this.cHook_Table = new System.Windows.Forms.DataGridView();
            this.tableLayoutPanel1 = new System.Windows.Forms.TableLayoutPanel();
            this.label4 = new System.Windows.Forms.Label();
            this.label3 = new System.Windows.Forms.Label();
            this.button2 = new System.Windows.Forms.Button();
            this.cHook_Config = new System.Windows.Forms.TextBox();
            this.button1 = new System.Windows.Forms.Button();
            this.listBox1 = new System.Windows.Forms.ListBox();
            this.tableLayoutPanel2 = new System.Windows.Forms.TableLayoutPanel();
            this.button3 = new System.Windows.Forms.Button();
            this.button4 = new System.Windows.Forms.Button();
            this.cHook_Name = new System.Windows.Forms.TextBox();
            this.cHook_Target = new System.Windows.Forms.TextBox();
            this.label1 = new System.Windows.Forms.Label();
            this.label2 = new System.Windows.Forms.Label();
            this.cHook_Active = new System.Windows.Forms.CheckBox();
            this.cMenu = new System.Windows.Forms.ToolStrip();
            this.cMenu_Hook_On = new System.Windows.Forms.ToolStripButton();
            this.cMenu_Hook_Off = new System.Windows.Forms.ToolStripButton();
            this.toolStripSeparator3 = new System.Windows.Forms.ToolStripSeparator();
            this.cMenu_Hook_Add = new System.Windows.Forms.ToolStripButton();
            this.cMenu_Hook_Remove = new System.Windows.Forms.ToolStripButton();
            this.toolStripSeparator1 = new System.Windows.Forms.ToolStripSeparator();
            this.cMenu_Hook_Wizard = new System.Windows.Forms.ToolStripButton();
            this.cMenu_FindGames = new System.Windows.Forms.ToolStripButton();
            this.toolStripSeparator2 = new System.Windows.Forms.ToolStripSeparator();
            this.toolStripButton6 = new System.Windows.Forms.ToolStripButton();
            this.toolStripButton5 = new System.Windows.Forms.ToolStripButton();
            this.cMenu_Downloads = new System.Windows.Forms.ToolStripButton();
            this.cTrayIcon = new System.Windows.Forms.NotifyIcon(this.components);
            this.dOpenFile = new System.Windows.Forms.OpenFileDialog();
            this.colActive = new System.Windows.Forms.DataGridViewCheckBoxColumn();
            this.colName = new System.Windows.Forms.DataGridViewTextBoxColumn();
            this.colTarget = new System.Windows.Forms.DataGridViewTextBoxColumn();
            this.colConfig = new System.Windows.Forms.DataGridViewTextBoxColumn();
            this.toolStripContainer1.ContentPanel.SuspendLayout();
            this.toolStripContainer1.TopToolStripPanel.SuspendLayout();
            this.toolStripContainer1.SuspendLayout();
            this.splitContainer1.Panel1.SuspendLayout();
            this.splitContainer1.Panel2.SuspendLayout();
            this.splitContainer1.SuspendLayout();
            ((System.ComponentModel.ISupportInitialize)(this.cHook_Table)).BeginInit();
            this.tableLayoutPanel1.SuspendLayout();
            this.tableLayoutPanel2.SuspendLayout();
            this.cMenu.SuspendLayout();
            this.SuspendLayout();
            // 
            // toolStripContainer1
            // 
            // 
            // toolStripContainer1.ContentPanel
            // 
            this.toolStripContainer1.ContentPanel.Controls.Add(this.splitContainer1);
            resources.ApplyResources(this.toolStripContainer1.ContentPanel, "toolStripContainer1.ContentPanel");
            resources.ApplyResources(this.toolStripContainer1, "toolStripContainer1");
            this.toolStripContainer1.Name = "toolStripContainer1";
            // 
            // toolStripContainer1.TopToolStripPanel
            // 
            this.toolStripContainer1.TopToolStripPanel.Controls.Add(this.cMenu);
            // 
            // splitContainer1
            // 
            resources.ApplyResources(this.splitContainer1, "splitContainer1");
            this.splitContainer1.Name = "splitContainer1";
            // 
            // splitContainer1.Panel1
            // 
            this.splitContainer1.Panel1.Controls.Add(this.cHook_Table);
            // 
            // splitContainer1.Panel2
            // 
            this.splitContainer1.Panel2.Controls.Add(this.tableLayoutPanel1);
            // 
            // cHook_Table
            // 
            this.cHook_Table.AllowUserToAddRows = false;
            this.cHook_Table.AllowUserToDeleteRows = false;
            dataGridViewCellStyle1.BackColor = System.Drawing.SystemColors.ControlLight;
            this.cHook_Table.AlternatingRowsDefaultCellStyle = dataGridViewCellStyle1;
            this.cHook_Table.BackgroundColor = System.Drawing.SystemColors.ControlLight;
            this.cHook_Table.BorderStyle = System.Windows.Forms.BorderStyle.None;
            this.cHook_Table.ColumnHeadersHeightSizeMode = System.Windows.Forms.DataGridViewColumnHeadersHeightSizeMode.AutoSize;
            this.cHook_Table.Columns.AddRange(new System.Windows.Forms.DataGridViewColumn[] {
            this.colActive,
            this.colName,
            this.colTarget,
            this.colConfig});
            resources.ApplyResources(this.cHook_Table, "cHook_Table");
            this.cHook_Table.MultiSelect = false;
            this.cHook_Table.Name = "cHook_Table";
            this.cHook_Table.ReadOnly = true;
            this.cHook_Table.RowHeadersBorderStyle = System.Windows.Forms.DataGridViewHeaderBorderStyle.None;
            this.cHook_Table.RowHeadersVisible = false;
            this.cHook_Table.SelectionMode = System.Windows.Forms.DataGridViewSelectionMode.FullRowSelect;
            this.cHook_Table.RowEnter += new System.Windows.Forms.DataGridViewCellEventHandler(this.RowEnter);
            this.cHook_Table.RowValidating += new System.Windows.Forms.DataGridViewCellCancelEventHandler(this.RowChange);
            // 
            // tableLayoutPanel1
            // 
            resources.ApplyResources(this.tableLayoutPanel1, "tableLayoutPanel1");
            this.tableLayoutPanel1.Controls.Add(this.label4, 0, 2);
            this.tableLayoutPanel1.Controls.Add(this.label3, 0, 1);
            this.tableLayoutPanel1.Controls.Add(this.button2, 2, 2);
            this.tableLayoutPanel1.Controls.Add(this.cHook_Config, 1, 2);
            this.tableLayoutPanel1.Controls.Add(this.button1, 2, 1);
            this.tableLayoutPanel1.Controls.Add(this.listBox1, 0, 4);
            this.tableLayoutPanel1.Controls.Add(this.tableLayoutPanel2, 0, 5);
            this.tableLayoutPanel1.Controls.Add(this.cHook_Name, 1, 0);
            this.tableLayoutPanel1.Controls.Add(this.cHook_Target, 1, 1);
            this.tableLayoutPanel1.Controls.Add(this.label1, 0, 3);
            this.tableLayoutPanel1.Controls.Add(this.label2, 0, 0);
            this.tableLayoutPanel1.Controls.Add(this.cHook_Active, 2, 0);
            this.tableLayoutPanel1.Name = "tableLayoutPanel1";
            // 
            // label4
            // 
            resources.ApplyResources(this.label4, "label4");
            this.label4.Name = "label4";
            // 
            // label3
            // 
            resources.ApplyResources(this.label3, "label3");
            this.label3.Name = "label3";
            // 
            // button2
            // 
            resources.ApplyResources(this.button2, "button2");
            this.button2.Name = "button2";
            this.button2.UseVisualStyleBackColor = true;
            this.button2.Click += new System.EventHandler(this.FindConfig);
            // 
            // cHook_Config
            // 
            resources.ApplyResources(this.cHook_Config, "cHook_Config");
            this.cHook_Config.AutoCompleteMode = System.Windows.Forms.AutoCompleteMode.Suggest;
            this.cHook_Config.AutoCompleteSource = System.Windows.Forms.AutoCompleteSource.FileSystem;
            this.cHook_Config.Name = "cHook_Config";
            this.cHook_Config.TextChanged += new System.EventHandler(this.HookDetailChanged);
            // 
            // button1
            // 
            resources.ApplyResources(this.button1, "button1");
            this.button1.Name = "button1";
            this.button1.UseVisualStyleBackColor = true;
            this.button1.Click += new System.EventHandler(this.FindTarget);
            // 
            // listBox1
            // 
            this.tableLayoutPanel1.SetColumnSpan(this.listBox1, 3);
            resources.ApplyResources(this.listBox1, "listBox1");
            this.listBox1.FormattingEnabled = true;
            this.listBox1.Name = "listBox1";
            // 
            // tableLayoutPanel2
            // 
            resources.ApplyResources(this.tableLayoutPanel2, "tableLayoutPanel2");
            this.tableLayoutPanel1.SetColumnSpan(this.tableLayoutPanel2, 3);
            this.tableLayoutPanel2.Controls.Add(this.button3, 0, 0);
            this.tableLayoutPanel2.Controls.Add(this.button4, 1, 0);
            this.tableLayoutPanel2.Name = "tableLayoutPanel2";
            // 
            // button3
            // 
            resources.ApplyResources(this.button3, "button3");
            this.button3.Name = "button3";
            this.button3.UseVisualStyleBackColor = true;
            this.button3.Click += new System.EventHandler(this.SaveHook);
            // 
            // button4
            // 
            resources.ApplyResources(this.button4, "button4");
            this.button4.Name = "button4";
            this.button4.UseVisualStyleBackColor = true;
            // 
            // cHook_Name
            // 
            resources.ApplyResources(this.cHook_Name, "cHook_Name");
            this.cHook_Name.Name = "cHook_Name";
            this.cHook_Name.TextChanged += new System.EventHandler(this.HookDetailChanged);
            // 
            // cHook_Target
            // 
            resources.ApplyResources(this.cHook_Target, "cHook_Target");
            this.cHook_Target.AutoCompleteMode = System.Windows.Forms.AutoCompleteMode.Suggest;
            this.cHook_Target.AutoCompleteSource = System.Windows.Forms.AutoCompleteSource.FileSystem;
            this.cHook_Target.Name = "cHook_Target";
            this.cHook_Target.TextChanged += new System.EventHandler(this.HookDetailChanged);
            // 
            // label1
            // 
            resources.ApplyResources(this.label1, "label1");
            this.tableLayoutPanel1.SetColumnSpan(this.label1, 3);
            this.label1.Name = "label1";
            // 
            // label2
            // 
            resources.ApplyResources(this.label2, "label2");
            this.label2.Name = "label2";
            // 
            // cHook_Active
            // 
            resources.ApplyResources(this.cHook_Active, "cHook_Active");
            this.cHook_Active.Name = "cHook_Active";
            this.cHook_Active.UseVisualStyleBackColor = true;
            this.cHook_Active.CheckedChanged += new System.EventHandler(this.HookDetailChanged);
            // 
            // cMenu
            // 
            resources.ApplyResources(this.cMenu, "cMenu");
            this.cMenu.Items.AddRange(new System.Windows.Forms.ToolStripItem[] {
            this.cMenu_Hook_On,
            this.cMenu_Hook_Off,
            this.toolStripSeparator3,
            this.cMenu_Hook_Add,
            this.cMenu_Hook_Remove,
            this.toolStripSeparator1,
            this.cMenu_Hook_Wizard,
            this.cMenu_FindGames,
            this.toolStripSeparator2,
            this.toolStripButton6,
            this.toolStripButton5,
            this.cMenu_Downloads});
            this.cMenu.Name = "cMenu";
            this.cMenu.Stretch = true;
            // 
            // cMenu_Hook_On
            // 
            this.cMenu_Hook_On.DisplayStyle = System.Windows.Forms.ToolStripItemDisplayStyle.Image;
            resources.ApplyResources(this.cMenu_Hook_On, "cMenu_Hook_On");
            this.cMenu_Hook_On.Name = "cMenu_Hook_On";
            this.cMenu_Hook_On.Click += new System.EventHandler(this.Menu_Hook_Disable);
            // 
            // cMenu_Hook_Off
            // 
            this.cMenu_Hook_Off.DisplayStyle = System.Windows.Forms.ToolStripItemDisplayStyle.Image;
            resources.ApplyResources(this.cMenu_Hook_Off, "cMenu_Hook_Off");
            this.cMenu_Hook_Off.Name = "cMenu_Hook_Off";
            this.cMenu_Hook_Off.Click += new System.EventHandler(this.Menu_Hook_Enable);
            // 
            // toolStripSeparator3
            // 
            this.toolStripSeparator3.Name = "toolStripSeparator3";
            resources.ApplyResources(this.toolStripSeparator3, "toolStripSeparator3");
            // 
            // cMenu_Hook_Add
            // 
            resources.ApplyResources(this.cMenu_Hook_Add, "cMenu_Hook_Add");
            this.cMenu_Hook_Add.Name = "cMenu_Hook_Add";
            this.cMenu_Hook_Add.Click += new System.EventHandler(this.Menu_Hook_Add);
            // 
            // cMenu_Hook_Remove
            // 
            resources.ApplyResources(this.cMenu_Hook_Remove, "cMenu_Hook_Remove");
            this.cMenu_Hook_Remove.Name = "cMenu_Hook_Remove";
            this.cMenu_Hook_Remove.Click += new System.EventHandler(this.Menu_Hook_Remove);
            // 
            // toolStripSeparator1
            // 
            this.toolStripSeparator1.Name = "toolStripSeparator1";
            resources.ApplyResources(this.toolStripSeparator1, "toolStripSeparator1");
            // 
            // cMenu_Hook_Wizard
            // 
            resources.ApplyResources(this.cMenu_Hook_Wizard, "cMenu_Hook_Wizard");
            this.cMenu_Hook_Wizard.Name = "cMenu_Hook_Wizard";
            // 
            // cMenu_FindGames
            // 
            resources.ApplyResources(this.cMenu_FindGames, "cMenu_FindGames");
            this.cMenu_FindGames.Name = "cMenu_FindGames";
            this.cMenu_FindGames.Click += new System.EventHandler(this.SearchSupported);
            // 
            // toolStripSeparator2
            // 
            this.toolStripSeparator2.Name = "toolStripSeparator2";
            resources.ApplyResources(this.toolStripSeparator2, "toolStripSeparator2");
            // 
            // toolStripButton6
            // 
            this.toolStripButton6.Alignment = System.Windows.Forms.ToolStripItemAlignment.Right;
            this.toolStripButton6.DisplayStyle = System.Windows.Forms.ToolStripItemDisplayStyle.Image;
            resources.ApplyResources(this.toolStripButton6, "toolStripButton6");
            this.toolStripButton6.Name = "toolStripButton6";
            this.toolStripButton6.Click += new System.EventHandler(this.OpenAboutBox);
            // 
            // toolStripButton5
            // 
            this.toolStripButton5.Alignment = System.Windows.Forms.ToolStripItemAlignment.Right;
            this.toolStripButton5.DisplayStyle = System.Windows.Forms.ToolStripItemDisplayStyle.Image;
            resources.ApplyResources(this.toolStripButton5, "toolStripButton5");
            this.toolStripButton5.Name = "toolStripButton5";
            // 
            // cMenu_Downloads
            // 
            resources.ApplyResources(this.cMenu_Downloads, "cMenu_Downloads");
            this.cMenu_Downloads.Name = "cMenu_Downloads";
            this.cMenu_Downloads.Click += new System.EventHandler(this.DownloadWindow);
            // 
            // cTrayIcon
            // 
            this.cTrayIcon.BalloonTipIcon = System.Windows.Forms.ToolTipIcon.Info;
            resources.ApplyResources(this.cTrayIcon, "cTrayIcon");
            this.cTrayIcon.Click += new System.EventHandler(this.Notify_OnClick);
            // 
            // dOpenFile
            // 
            resources.ApplyResources(this.dOpenFile, "dOpenFile");
            this.dOpenFile.RestoreDirectory = true;
            // 
            // colActive
            // 
            this.colActive.AutoSizeMode = System.Windows.Forms.DataGridViewAutoSizeColumnMode.ColumnHeader;
            this.colActive.DataPropertyName = "Active";
            resources.ApplyResources(this.colActive, "colActive");
            this.colActive.Name = "colActive";
            this.colActive.ReadOnly = true;
            // 
            // colName
            // 
            this.colName.AutoSizeMode = System.Windows.Forms.DataGridViewAutoSizeColumnMode.Fill;
            this.colName.DataPropertyName = "Name";
            this.colName.FillWeight = 60F;
            resources.ApplyResources(this.colName, "colName");
            this.colName.Name = "colName";
            this.colName.ReadOnly = true;
            this.colName.SortMode = System.Windows.Forms.DataGridViewColumnSortMode.NotSortable;
            // 
            // colTarget
            // 
            this.colTarget.AutoSizeMode = System.Windows.Forms.DataGridViewAutoSizeColumnMode.Fill;
            this.colTarget.DataPropertyName = "Target";
            resources.ApplyResources(this.colTarget, "colTarget");
            this.colTarget.Name = "colTarget";
            this.colTarget.ReadOnly = true;
            this.colTarget.SortMode = System.Windows.Forms.DataGridViewColumnSortMode.NotSortable;
            // 
            // colConfig
            // 
            this.colConfig.DataPropertyName = "Config";
            resources.ApplyResources(this.colConfig, "colConfig");
            this.colConfig.Name = "colConfig";
            this.colConfig.ReadOnly = true;
            // 
            // MainForm
            // 
            resources.ApplyResources(this, "$this");
            this.AutoScaleMode = System.Windows.Forms.AutoScaleMode.Font;
            this.Controls.Add(this.toolStripContainer1);
            this.Name = "MainForm";
            this.FormClosed += new System.Windows.Forms.FormClosedEventHandler(this.MainForm_Close);
            this.Resize += new System.EventHandler(this.Form_OnResize);
            this.toolStripContainer1.ContentPanel.ResumeLayout(false);
            this.toolStripContainer1.TopToolStripPanel.ResumeLayout(false);
            this.toolStripContainer1.TopToolStripPanel.PerformLayout();
            this.toolStripContainer1.ResumeLayout(false);
            this.toolStripContainer1.PerformLayout();
            this.splitContainer1.Panel1.ResumeLayout(false);
            this.splitContainer1.Panel2.ResumeLayout(false);
            this.splitContainer1.ResumeLayout(false);
            ((System.ComponentModel.ISupportInitialize)(this.cHook_Table)).EndInit();
            this.tableLayoutPanel1.ResumeLayout(false);
            this.tableLayoutPanel1.PerformLayout();
            this.tableLayoutPanel2.ResumeLayout(false);
            this.cMenu.ResumeLayout(false);
            this.cMenu.PerformLayout();
            this.ResumeLayout(false);

        }

        #endregion

        private System.Windows.Forms.ToolStripContainer toolStripContainer1;
        private System.Windows.Forms.ToolStrip cMenu;
        private System.Windows.Forms.ToolStripButton cMenu_Hook_Wizard;
        private System.Windows.Forms.ToolStripButton cMenu_FindGames;
        private System.Windows.Forms.ToolStripSeparator toolStripSeparator2;
        private System.Windows.Forms.ToolStripButton cMenu_Downloads;
        private System.Windows.Forms.ToolStripButton toolStripButton5;
        private System.Windows.Forms.ToolStripButton toolStripButton6;
        private System.Windows.Forms.NotifyIcon cTrayIcon;
        private System.Windows.Forms.ToolStripButton cMenu_Hook_On;
        private System.Windows.Forms.ToolStripButton cMenu_Hook_Off;
        private System.Windows.Forms.ToolStripSeparator toolStripSeparator3;
        private System.Windows.Forms.ToolStripButton cMenu_Hook_Add;
        private System.Windows.Forms.ToolStripButton cMenu_Hook_Remove;
        private System.Windows.Forms.ToolStripSeparator toolStripSeparator1;
        private System.Windows.Forms.OpenFileDialog dOpenFile;
        private System.Windows.Forms.SplitContainer splitContainer1;
        private System.Windows.Forms.DataGridView cHook_Table;
        private System.Windows.Forms.TableLayoutPanel tableLayoutPanel1;
        private System.Windows.Forms.Label label4;
        private System.Windows.Forms.Label label3;
        private System.Windows.Forms.Button button2;
        private System.Windows.Forms.TextBox cHook_Config;
        private System.Windows.Forms.Button button1;
        private System.Windows.Forms.ListBox listBox1;
        private System.Windows.Forms.TableLayoutPanel tableLayoutPanel2;
        private System.Windows.Forms.Button button3;
        private System.Windows.Forms.Button button4;
        private System.Windows.Forms.TextBox cHook_Name;
        private System.Windows.Forms.TextBox cHook_Target;
        private System.Windows.Forms.Label label1;
        private System.Windows.Forms.Label label2;
        private System.Windows.Forms.CheckBox cHook_Active;
        private System.Windows.Forms.DataGridViewCheckBoxColumn colActive;
        private System.Windows.Forms.DataGridViewTextBoxColumn colName;
        private System.Windows.Forms.DataGridViewTextBoxColumn colTarget;
        private System.Windows.Forms.DataGridViewTextBoxColumn colConfig;

    }
}

