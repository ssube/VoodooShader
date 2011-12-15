namespace VoodooUI
{
    partial class ProgressDialog
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
            this.cProgressLog = new System.Windows.Forms.ListBox();
            this.SuspendLayout();
            // 
            // cProgressLog
            // 
            this.cProgressLog.Dock = System.Windows.Forms.DockStyle.Fill;
            this.cProgressLog.FormattingEnabled = true;
            this.cProgressLog.HorizontalScrollbar = true;
            this.cProgressLog.Location = new System.Drawing.Point(0, 0);
            this.cProgressLog.Name = "cProgressLog";
            this.cProgressLog.ScrollAlwaysVisible = true;
            this.cProgressLog.Size = new System.Drawing.Size(624, 442);
            this.cProgressLog.TabIndex = 0;
            // 
            // ProgressDialog
            // 
            this.AutoScaleDimensions = new System.Drawing.SizeF(6F, 13F);
            this.AutoScaleMode = System.Windows.Forms.AutoScaleMode.Font;
            this.ClientSize = new System.Drawing.Size(624, 442);
            this.ControlBox = false;
            this.Controls.Add(this.cProgressLog);
            this.MaximizeBox = false;
            this.MinimizeBox = false;
            this.Name = "ProgressDialog";
            this.Text = "Progress";
            this.ResumeLayout(false);

        }

        #endregion

        private System.Windows.Forms.ListBox cProgressLog;

    }
}