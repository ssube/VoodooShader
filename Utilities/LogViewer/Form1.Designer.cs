namespace LogViewer
{
	partial class mainwindow
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
			System.ComponentModel.ComponentResourceManager resources = new System.ComponentModel.ComponentResourceManager(typeof(mainwindow));
			this.toolStripContainer1 = new System.Windows.Forms.ToolStripContainer();
			this.toolStrip1 = new System.Windows.Forms.ToolStrip();
			this.toolStripButton1 = new System.Windows.Forms.ToolStripButton();
			this.openFileDialog1 = new System.Windows.Forms.OpenFileDialog();
			this.tableLayoutPanel1 = new System.Windows.Forms.TableLayoutPanel();
			this.textSummary = new System.Windows.Forms.Label();
			this.gridModules = new System.Windows.Forms.DataGridView();
			this.name = new System.Windows.Forms.DataGridViewTextBoxColumn();
			this.major = new System.Windows.Forms.DataGridViewTextBoxColumn();
			this.minor = new System.Windows.Forms.DataGridViewTextBoxColumn();
			this.patch = new System.Windows.Forms.DataGridViewTextBoxColumn();
			this.rev = new System.Windows.Forms.DataGridViewTextBoxColumn();
			this.debug = new System.Windows.Forms.DataGridViewTextBoxColumn();
			this.dataGridView1 = new System.Windows.Forms.DataGridView();
			this.eventnum = new System.Windows.Forms.DataGridViewTextBoxColumn();
			this.severity = new System.Windows.Forms.DataGridViewTextBoxColumn();
			this.ticks = new System.Windows.Forms.DataGridViewTextBoxColumn();
			this.source = new System.Windows.Forms.DataGridViewTextBoxColumn();
			this.body = new System.Windows.Forms.DataGridViewTextBoxColumn();
			this.toolStripContainer1.ContentPanel.SuspendLayout();
			this.toolStripContainer1.TopToolStripPanel.SuspendLayout();
			this.toolStripContainer1.SuspendLayout();
			this.toolStrip1.SuspendLayout();
			this.tableLayoutPanel1.SuspendLayout();
			((System.ComponentModel.ISupportInitialize)(this.gridModules)).BeginInit();
			((System.ComponentModel.ISupportInitialize)(this.dataGridView1)).BeginInit();
			this.SuspendLayout();
			// 
			// toolStripContainer1
			// 
			// 
			// toolStripContainer1.ContentPanel
			// 
			this.toolStripContainer1.ContentPanel.Controls.Add(this.tableLayoutPanel1);
			this.toolStripContainer1.ContentPanel.Size = new System.Drawing.Size(632, 428);
			this.toolStripContainer1.Dock = System.Windows.Forms.DockStyle.Fill;
			this.toolStripContainer1.Location = new System.Drawing.Point(0, 0);
			this.toolStripContainer1.Name = "toolStripContainer1";
			this.toolStripContainer1.Size = new System.Drawing.Size(632, 453);
			this.toolStripContainer1.TabIndex = 0;
			this.toolStripContainer1.Text = "toolStripContainer1";
			// 
			// toolStripContainer1.TopToolStripPanel
			// 
			this.toolStripContainer1.TopToolStripPanel.Controls.Add(this.toolStrip1);
			// 
			// toolStrip1
			// 
			this.toolStrip1.Dock = System.Windows.Forms.DockStyle.None;
			this.toolStrip1.Items.AddRange(new System.Windows.Forms.ToolStripItem[] {
            this.toolStripButton1});
			this.toolStrip1.Location = new System.Drawing.Point(3, 0);
			this.toolStrip1.Name = "toolStrip1";
			this.toolStrip1.Size = new System.Drawing.Size(33, 25);
			this.toolStrip1.TabIndex = 0;
			// 
			// toolStripButton1
			// 
			this.toolStripButton1.DisplayStyle = System.Windows.Forms.ToolStripItemDisplayStyle.Image;
			this.toolStripButton1.Image = ((System.Drawing.Image)(resources.GetObject("toolStripButton1.Image")));
			this.toolStripButton1.ImageTransparentColor = System.Drawing.Color.Magenta;
			this.toolStripButton1.Name = "toolStripButton1";
			this.toolStripButton1.Size = new System.Drawing.Size(23, 22);
			this.toolStripButton1.Text = "toolStripButton1";
			// 
			// openFileDialog1
			// 
			this.openFileDialog1.FileName = "openFileDialog1";
			// 
			// tableLayoutPanel1
			// 
			this.tableLayoutPanel1.ColumnCount = 1;
			this.tableLayoutPanel1.ColumnStyles.Add(new System.Windows.Forms.ColumnStyle(System.Windows.Forms.SizeType.Percent, 50F));
			this.tableLayoutPanel1.ColumnStyles.Add(new System.Windows.Forms.ColumnStyle(System.Windows.Forms.SizeType.Percent, 50F));
			this.tableLayoutPanel1.Controls.Add(this.textSummary, 0, 0);
			this.tableLayoutPanel1.Controls.Add(this.gridModules, 0, 1);
			this.tableLayoutPanel1.Controls.Add(this.dataGridView1, 0, 2);
			this.tableLayoutPanel1.Dock = System.Windows.Forms.DockStyle.Fill;
			this.tableLayoutPanel1.GrowStyle = System.Windows.Forms.TableLayoutPanelGrowStyle.FixedSize;
			this.tableLayoutPanel1.Location = new System.Drawing.Point(0, 0);
			this.tableLayoutPanel1.Name = "tableLayoutPanel1";
			this.tableLayoutPanel1.RowCount = 3;
			this.tableLayoutPanel1.RowStyles.Add(new System.Windows.Forms.RowStyle(System.Windows.Forms.SizeType.Percent, 16.70762F));
			this.tableLayoutPanel1.RowStyles.Add(new System.Windows.Forms.RowStyle(System.Windows.Forms.SizeType.Percent, 83.29238F));
			this.tableLayoutPanel1.RowStyles.Add(new System.Windows.Forms.RowStyle(System.Windows.Forms.SizeType.Absolute, 252F));
			this.tableLayoutPanel1.Size = new System.Drawing.Size(632, 428);
			this.tableLayoutPanel1.TabIndex = 0;
			// 
			// textSummary
			// 
			this.textSummary.AutoSize = true;
			this.textSummary.Dock = System.Windows.Forms.DockStyle.Fill;
			this.textSummary.Location = new System.Drawing.Point(3, 0);
			this.textSummary.Name = "textSummary";
			this.textSummary.Size = new System.Drawing.Size(626, 29);
			this.textSummary.TabIndex = 0;
			this.textSummary.Text = "label1";
			// 
			// gridModules
			// 
			this.gridModules.AllowUserToAddRows = false;
			this.gridModules.AllowUserToDeleteRows = false;
			this.gridModules.ColumnHeadersHeightSizeMode = System.Windows.Forms.DataGridViewColumnHeadersHeightSizeMode.AutoSize;
			this.gridModules.Columns.AddRange(new System.Windows.Forms.DataGridViewColumn[] {
            this.name,
            this.major,
            this.minor,
            this.patch,
            this.rev,
            this.debug});
			this.gridModules.Dock = System.Windows.Forms.DockStyle.Fill;
			this.gridModules.Location = new System.Drawing.Point(3, 32);
			this.gridModules.Name = "gridModules";
			this.gridModules.ReadOnly = true;
			this.gridModules.Size = new System.Drawing.Size(626, 140);
			this.gridModules.TabIndex = 1;
			// 
			// name
			// 
			this.name.HeaderText = "Name";
			this.name.Name = "name";
			this.name.ReadOnly = true;
			// 
			// major
			// 
			this.major.HeaderText = "Major";
			this.major.Name = "major";
			this.major.ReadOnly = true;
			// 
			// minor
			// 
			this.minor.HeaderText = "Minor";
			this.minor.Name = "minor";
			this.minor.ReadOnly = true;
			// 
			// patch
			// 
			this.patch.HeaderText = "Patch";
			this.patch.Name = "patch";
			this.patch.ReadOnly = true;
			// 
			// rev
			// 
			this.rev.HeaderText = "Revision";
			this.rev.Name = "rev";
			this.rev.ReadOnly = true;
			// 
			// debug
			// 
			this.debug.HeaderText = "Debug";
			this.debug.Name = "debug";
			this.debug.ReadOnly = true;
			// 
			// dataGridView1
			// 
			this.dataGridView1.AllowUserToAddRows = false;
			this.dataGridView1.AllowUserToDeleteRows = false;
			this.dataGridView1.ColumnHeadersHeightSizeMode = System.Windows.Forms.DataGridViewColumnHeadersHeightSizeMode.AutoSize;
			this.dataGridView1.Columns.AddRange(new System.Windows.Forms.DataGridViewColumn[] {
            this.eventnum,
            this.severity,
            this.ticks,
            this.source,
            this.body});
			this.dataGridView1.Dock = System.Windows.Forms.DockStyle.Fill;
			this.dataGridView1.Location = new System.Drawing.Point(3, 178);
			this.dataGridView1.Name = "dataGridView1";
			this.dataGridView1.ReadOnly = true;
			this.dataGridView1.Size = new System.Drawing.Size(626, 247);
			this.dataGridView1.TabIndex = 2;
			// 
			// eventnum
			// 
			this.eventnum.HeaderText = "Event";
			this.eventnum.Name = "eventnum";
			this.eventnum.ReadOnly = true;
			// 
			// severity
			// 
			this.severity.HeaderText = "Severity";
			this.severity.Name = "severity";
			this.severity.ReadOnly = true;
			// 
			// ticks
			// 
			this.ticks.HeaderText = "Ticks";
			this.ticks.Name = "ticks";
			this.ticks.ReadOnly = true;
			// 
			// source
			// 
			this.source.HeaderText = "Source";
			this.source.Name = "source";
			this.source.ReadOnly = true;
			// 
			// body
			// 
			this.body.HeaderText = "Body";
			this.body.Name = "body";
			this.body.ReadOnly = true;
			// 
			// mainwindow
			// 
			this.AutoScaleDimensions = new System.Drawing.SizeF(6F, 13F);
			this.AutoScaleMode = System.Windows.Forms.AutoScaleMode.Font;
			this.ClientSize = new System.Drawing.Size(632, 453);
			this.Controls.Add(this.toolStripContainer1);
			this.Name = "mainwindow";
			this.Text = "Voodoo Shader Framework Log Viewer";
			this.toolStripContainer1.ContentPanel.ResumeLayout(false);
			this.toolStripContainer1.TopToolStripPanel.ResumeLayout(false);
			this.toolStripContainer1.TopToolStripPanel.PerformLayout();
			this.toolStripContainer1.ResumeLayout(false);
			this.toolStripContainer1.PerformLayout();
			this.toolStrip1.ResumeLayout(false);
			this.toolStrip1.PerformLayout();
			this.tableLayoutPanel1.ResumeLayout(false);
			this.tableLayoutPanel1.PerformLayout();
			((System.ComponentModel.ISupportInitialize)(this.gridModules)).EndInit();
			((System.ComponentModel.ISupportInitialize)(this.dataGridView1)).EndInit();
			this.ResumeLayout(false);

		}

		#endregion

		private System.Windows.Forms.ToolStripContainer toolStripContainer1;
		private System.Windows.Forms.TableLayoutPanel tableLayoutPanel1;
		private System.Windows.Forms.Label textSummary;
		private System.Windows.Forms.DataGridView gridModules;
		private System.Windows.Forms.DataGridViewTextBoxColumn name;
		private System.Windows.Forms.DataGridViewTextBoxColumn major;
		private System.Windows.Forms.DataGridViewTextBoxColumn minor;
		private System.Windows.Forms.DataGridViewTextBoxColumn patch;
		private System.Windows.Forms.DataGridViewTextBoxColumn rev;
		private System.Windows.Forms.DataGridViewTextBoxColumn debug;
		private System.Windows.Forms.DataGridView dataGridView1;
		private System.Windows.Forms.DataGridViewTextBoxColumn eventnum;
		private System.Windows.Forms.DataGridViewTextBoxColumn severity;
		private System.Windows.Forms.DataGridViewTextBoxColumn ticks;
		private System.Windows.Forms.DataGridViewTextBoxColumn source;
		private System.Windows.Forms.DataGridViewTextBoxColumn body;
		private System.Windows.Forms.ToolStrip toolStrip1;
		private System.Windows.Forms.ToolStripButton toolStripButton1;
		private System.Windows.Forms.OpenFileDialog openFileDialog1;
	}
}

