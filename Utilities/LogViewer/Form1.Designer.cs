namespace LogViewer
{
 partial class mainwindow
 {
  /// <summary>
  /// Required designer variable.
  /// </summary>
  private System.ComponentModel.IContainer components = nullptr;

  /// <summary>
  /// Clean up any resources being used.
  /// </summary>
  /// <param name="disposing">true if managed resources should be disposed; otherwise, false.</param>
  protected override void Dispose(bool disposing)
  {
   if (disposing && (components != nullptr))
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
            this.toolStripContainer1 = new System.Windows.Forms.ToolStripContainer();
            this.mainBrowser = new System.Windows.Forms.WebBrowser();
            this.toolStrip1 = new System.Windows.Forms.ToolStrip();
            this.toolStripButton1 = new System.Windows.Forms.ToolStripButton();
            this.openFileDialog1 = new System.Windows.Forms.OpenFileDialog();
            this.toolStripButton2 = new System.Windows.Forms.ToolStripButton();
            this.saveFileDialog1 = new System.Windows.Forms.SaveFileDialog();
            this.toolStripContainer1.ContentPanel.SuspendLayout();
            this.toolStripContainer1.TopToolStripPanel.SuspendLayout();
            this.toolStripContainer1.SuspendLayout();
            this.toolStrip1.SuspendLayout();
            this.SuspendLayout();
            //
            // toolStripContainer1
            //
            //
            // toolStripContainer1.ContentPanel
            //
            this.toolStripContainer1.ContentPanel.Controls.Add(this.mainBrowser);
            this.toolStripContainer1.ContentPanel.Size = new System.Drawing.Size(792, 548);
            this.toolStripContainer1.Dock = System.Windows.Forms.DockStyle.Fill;
            this.toolStripContainer1.Location = new System.Drawing.Point(0, 0);
            this.toolStripContainer1.Name = "toolStripContainer1";
            this.toolStripContainer1.Size = new System.Drawing.Size(792, 573);
            this.toolStripContainer1.TabIndex = 0;
            this.toolStripContainer1.Text = "toolStripContainer1";
            //
            // toolStripContainer1.TopToolStripPanel
            //
            this.toolStripContainer1.TopToolStripPanel.Controls.Add(this.toolStrip1);
            //
            // mainBrowser
            //
            this.mainBrowser.AllowWebBrowserDrop = false;
            this.mainBrowser.Dock = System.Windows.Forms.DockStyle.Fill;
            this.mainBrowser.Location = new System.Drawing.Point(0, 0);
            this.mainBrowser.MinimumSize = new System.Drawing.Size(20, 20);
            this.mainBrowser.Name = "mainBrowser";
            this.mainBrowser.Size = new System.Drawing.Size(792, 548);
            this.mainBrowser.TabIndex = 0;
            this.mainBrowser.Url = new System.Uri("", System.UriKind.Relative);
            //
            // toolStrip1
            //
            this.toolStrip1.Dock = System.Windows.Forms.DockStyle.None;
            this.toolStrip1.Items.AddRange(new System.Windows.Forms.ToolStripItem[] {
            this.toolStripButton1,
            this.toolStripButton2});
            this.toolStrip1.Location = new System.Drawing.Point(3, 0);
            this.toolStrip1.Name = "toolStrip1";
            this.toolStrip1.Size = new System.Drawing.Size(188, 25);
            this.toolStrip1.TabIndex = 0;
            //
            // toolStripButton1
            //
            this.toolStripButton1.Image = global::LogViewer.Properties.Resources.OpenFile;
            this.toolStripButton1.ImageTransparentColor = System.Drawing.Color.Magenta;
            this.toolStripButton1.Name = "toolStripButton1";
            this.toolStripButton1.Size = new System.Drawing.Size(84, 22);
            this.toolStripButton1.Text = "Open Log";
            this.toolStripButton1.Click += new System.EventHandler(this.openLog);
            //
            // openFileDialog1
            //
            this.openFileDialog1.DefaultExt = "xmllog";
            this.openFileDialog1.FileName = "openFileDialog1";
            this.openFileDialog1.Filter = "XML Log File|*.xmllog|XML File|*.xml|All Files|*";
            this.openFileDialog1.Title = "Open Voodoo XML Log";
            //
            // toolStripButton2
            //
            this.toolStripButton2.Image = global::LogViewer.Properties.Resources.saveHS;
            this.toolStripButton2.ImageTransparentColor = System.Drawing.Color.Magenta;
            this.toolStripButton2.Name = "toolStripButton2";
            this.toolStripButton2.Size = new System.Drawing.Size(94, 22);
            this.toolStripButton2.Text = "Save HTML";
            this.toolStripButton2.Click += new System.EventHandler(this.saveHTML);
            //
            // saveFileDialog1
            //
            this.saveFileDialog1.DefaultExt = "html";
            this.saveFileDialog1.Filter = "HTML File|*.html|All Files|*";
            this.saveFileDialog1.Title = "Save HTML Log";
            //
            // mainwindow
            //
            this.AutoScaleDimensions = new System.Drawing.SizeF(6F, 13F);
            this.AutoScaleMode = System.Windows.Forms.AutoScaleMode.Font;
            this.ClientSize = new System.Drawing.Size(792, 573);
            this.Controls.Add(this.toolStripContainer1);
            this.Name = "mainwindow";
            this.ShowIcon = false;
            this.Text = "Voodoo Shader Framework Log Viewer";
            this.FormClosing += new System.Windows.Forms.FormClosingEventHandler(this.closeForm);
            this.toolStripContainer1.ContentPanel.ResumeLayout(false);
            this.toolStripContainer1.TopToolStripPanel.ResumeLayout(false);
            this.toolStripContainer1.TopToolStripPanel.PerformLayout();
            this.toolStripContainer1.ResumeLayout(false);
            this.toolStripContainer1.PerformLayout();
            this.toolStrip1.ResumeLayout(false);
            this.toolStrip1.PerformLayout();
            this.ResumeLayout(false);

  }

  #endregion

        private System.Windows.Forms.ToolStripContainer toolStripContainer1;
  private System.Windows.Forms.ToolStrip toolStrip1;
  private System.Windows.Forms.ToolStripButton toolStripButton1;
        private System.Windows.Forms.OpenFileDialog openFileDialog1;
        private System.Windows.Forms.WebBrowser mainBrowser;
        private System.Windows.Forms.ToolStripButton toolStripButton2;
        private System.Windows.Forms.SaveFileDialog saveFileDialog1;
 }
}

