namespace VoodooGUI
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
            System.Windows.Forms.DataGridViewCellStyle dataGridViewCellStyle2 = new System.Windows.Forms.DataGridViewCellStyle();
            System.ComponentModel.ComponentResourceManager resources = new System.ComponentModel.ComponentResourceManager(typeof(MainForm));
            this.toolStripContainer1 = new System.Windows.Forms.ToolStripContainer();
            this.cHook_Table = new System.Windows.Forms.DataGridView();
            this.colActive = new System.Windows.Forms.DataGridViewCheckBoxColumn();
            this.colName = new System.Windows.Forms.DataGridViewTextBoxColumn();
            this.colTarget = new System.Windows.Forms.DataGridViewTextBoxColumn();
            this.col_TargetFind = new System.Windows.Forms.DataGridViewButtonColumn();
            this.colConfig = new System.Windows.Forms.DataGridViewTextBoxColumn();
            this.col_ConfigFind = new System.Windows.Forms.DataGridViewButtonColumn();
            this.col_ConfigEdit = new System.Windows.Forms.DataGridViewButtonColumn();
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
            this.toolStripContainer1.ContentPanel.SuspendLayout();
            this.toolStripContainer1.TopToolStripPanel.SuspendLayout();
            this.toolStripContainer1.SuspendLayout();
            ((System.ComponentModel.ISupportInitialize)(this.cHook_Table)).BeginInit();
            this.cMenu.SuspendLayout();
            this.SuspendLayout();
            // 
            // toolStripContainer1
            // 
            // 
            // toolStripContainer1.ContentPanel
            // 
            this.toolStripContainer1.ContentPanel.Controls.Add(this.cHook_Table);
            resources.ApplyResources(this.toolStripContainer1.ContentPanel, "toolStripContainer1.ContentPanel");
            resources.ApplyResources(this.toolStripContainer1, "toolStripContainer1");
            this.toolStripContainer1.Name = "toolStripContainer1";
            // 
            // toolStripContainer1.TopToolStripPanel
            // 
            this.toolStripContainer1.TopToolStripPanel.Controls.Add(this.cMenu);
            // 
            // cHook_Table
            // 
            this.cHook_Table.AllowUserToAddRows = false;
            this.cHook_Table.AllowUserToDeleteRows = false;
            dataGridViewCellStyle2.BackColor = System.Drawing.SystemColors.ControlLight;
            this.cHook_Table.AlternatingRowsDefaultCellStyle = dataGridViewCellStyle2;
            this.cHook_Table.BackgroundColor = System.Drawing.SystemColors.ControlLight;
            this.cHook_Table.BorderStyle = System.Windows.Forms.BorderStyle.None;
            this.cHook_Table.ColumnHeadersHeightSizeMode = System.Windows.Forms.DataGridViewColumnHeadersHeightSizeMode.AutoSize;
            this.cHook_Table.Columns.AddRange(new System.Windows.Forms.DataGridViewColumn[] {
            this.colActive,
            this.colName,
            this.colTarget,
            this.col_TargetFind,
            this.colConfig,
            this.col_ConfigFind,
            this.col_ConfigEdit});
            resources.ApplyResources(this.cHook_Table, "cHook_Table");
            this.cHook_Table.MultiSelect = false;
            this.cHook_Table.Name = "cHook_Table";
            this.cHook_Table.RowHeadersBorderStyle = System.Windows.Forms.DataGridViewHeaderBorderStyle.None;
            this.cHook_Table.RowHeadersVisible = false;
            this.cHook_Table.SelectionMode = System.Windows.Forms.DataGridViewSelectionMode.FullRowSelect;
            this.cHook_Table.CellContentClick += new System.Windows.Forms.DataGridViewCellEventHandler(this.CellClick);
            this.cHook_Table.CellEndEdit += new System.Windows.Forms.DataGridViewCellEventHandler(this.CellChanged);
            // 
            // colActive
            // 
            this.colActive.AutoSizeMode = System.Windows.Forms.DataGridViewAutoSizeColumnMode.ColumnHeader;
            this.colActive.DataPropertyName = "Active";
            resources.ApplyResources(this.colActive, "colActive");
            this.colActive.Name = "colActive";
            // 
            // colName
            // 
            this.colName.AutoSizeMode = System.Windows.Forms.DataGridViewAutoSizeColumnMode.Fill;
            this.colName.DataPropertyName = "Name";
            resources.ApplyResources(this.colName, "colName");
            this.colName.Name = "colName";
            // 
            // colTarget
            // 
            this.colTarget.AutoSizeMode = System.Windows.Forms.DataGridViewAutoSizeColumnMode.Fill;
            this.colTarget.DataPropertyName = "Target";
            this.colTarget.FillWeight = 140F;
            resources.ApplyResources(this.colTarget, "colTarget");
            this.colTarget.Name = "colTarget";
            // 
            // col_TargetFind
            // 
            this.col_TargetFind.AutoSizeMode = System.Windows.Forms.DataGridViewAutoSizeColumnMode.AllCells;
            resources.ApplyResources(this.col_TargetFind, "col_TargetFind");
            this.col_TargetFind.Name = "col_TargetFind";
            this.col_TargetFind.Text = "...";
            this.col_TargetFind.UseColumnTextForButtonValue = true;
            // 
            // colConfig
            // 
            this.colConfig.AutoSizeMode = System.Windows.Forms.DataGridViewAutoSizeColumnMode.Fill;
            this.colConfig.DataPropertyName = "Config";
            resources.ApplyResources(this.colConfig, "colConfig");
            this.colConfig.Name = "colConfig";
            // 
            // col_ConfigFind
            // 
            this.col_ConfigFind.AutoSizeMode = System.Windows.Forms.DataGridViewAutoSizeColumnMode.AllCells;
            resources.ApplyResources(this.col_ConfigFind, "col_ConfigFind");
            this.col_ConfigFind.Name = "col_ConfigFind";
            this.col_ConfigFind.Text = "...";
            this.col_ConfigFind.UseColumnTextForButtonValue = true;
            // 
            // col_ConfigEdit
            // 
            this.col_ConfigEdit.AutoSizeMode = System.Windows.Forms.DataGridViewAutoSizeColumnMode.AllCells;
            resources.ApplyResources(this.col_ConfigEdit, "col_ConfigEdit");
            this.col_ConfigEdit.Name = "col_ConfigEdit";
            this.col_ConfigEdit.Text = "Edit";
            this.col_ConfigEdit.UseColumnTextForButtonValue = true;
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
            ((System.ComponentModel.ISupportInitialize)(this.cHook_Table)).EndInit();
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
        private System.Windows.Forms.DataGridView cHook_Table;
        private System.Windows.Forms.DataGridViewCheckBoxColumn colActive;
        private System.Windows.Forms.DataGridViewTextBoxColumn colName;
        private System.Windows.Forms.DataGridViewTextBoxColumn colTarget;
        private System.Windows.Forms.DataGridViewButtonColumn col_TargetFind;
        private System.Windows.Forms.DataGridViewTextBoxColumn colConfig;
        private System.Windows.Forms.DataGridViewButtonColumn col_ConfigFind;
        private System.Windows.Forms.DataGridViewButtonColumn col_ConfigEdit;

    }
}

