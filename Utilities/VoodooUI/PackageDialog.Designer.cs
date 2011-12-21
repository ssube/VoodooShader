namespace VoodooUI
{
    partial class PackageDialog
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
            this.bOK = new System.Windows.Forms.Button();
            this.bCancel = new System.Windows.Forms.Button();
            this.label1 = new System.Windows.Forms.Label();
            this.label2 = new System.Windows.Forms.Label();
            this.label3 = new System.Windows.Forms.Label();
            this.cOrigin = new System.Windows.Forms.TextBox();
            this.cBranch = new System.Windows.Forms.TextBox();
            this.cPath = new System.Windows.Forms.TextBox();
            this.SuspendLayout();
            // 
            // bOK
            // 
            this.bOK.Anchor = ((System.Windows.Forms.AnchorStyles)((System.Windows.Forms.AnchorStyles.Bottom | System.Windows.Forms.AnchorStyles.Right)));
            this.bOK.DialogResult = System.Windows.Forms.DialogResult.OK;
            this.bOK.Location = new System.Drawing.Point(264, 94);
            this.bOK.Name = "bOK";
            this.bOK.Size = new System.Drawing.Size(80, 26);
            this.bOK.TabIndex = 9;
            this.bOK.Text = "OK";
            this.bOK.UseVisualStyleBackColor = true;
            this.bOK.Click += new System.EventHandler(this.ButtonOK);
            // 
            // bCancel
            // 
            this.bCancel.Anchor = ((System.Windows.Forms.AnchorStyles)((System.Windows.Forms.AnchorStyles.Bottom | System.Windows.Forms.AnchorStyles.Right)));
            this.bCancel.DialogResult = System.Windows.Forms.DialogResult.Cancel;
            this.bCancel.Location = new System.Drawing.Point(350, 94);
            this.bCancel.Name = "bCancel";
            this.bCancel.Size = new System.Drawing.Size(80, 26);
            this.bCancel.TabIndex = 10;
            this.bCancel.Text = "Cancel";
            this.bCancel.UseVisualStyleBackColor = true;
            this.bCancel.Click += new System.EventHandler(this.ButtonCancel);
            // 
            // label1
            // 
            this.label1.AutoSize = true;
            this.label1.Location = new System.Drawing.Point(16, 15);
            this.label1.Name = "label1";
            this.label1.Size = new System.Drawing.Size(37, 13);
            this.label1.TabIndex = 11;
            this.label1.Text = "Origin:";
            // 
            // label2
            // 
            this.label2.AutoSize = true;
            this.label2.Location = new System.Drawing.Point(16, 41);
            this.label2.Name = "label2";
            this.label2.Size = new System.Drawing.Size(44, 13);
            this.label2.TabIndex = 12;
            this.label2.Text = "Branch:";
            // 
            // label3
            // 
            this.label3.AutoSize = true;
            this.label3.Location = new System.Drawing.Point(16, 67);
            this.label3.Name = "label3";
            this.label3.Size = new System.Drawing.Size(32, 13);
            this.label3.TabIndex = 13;
            this.label3.Text = "Path:";
            // 
            // cOrigin
            // 
            this.cOrigin.Location = new System.Drawing.Point(81, 12);
            this.cOrigin.Name = "cOrigin";
            this.cOrigin.Size = new System.Drawing.Size(349, 20);
            this.cOrigin.TabIndex = 14;
            // 
            // cBranch
            // 
            this.cBranch.Location = new System.Drawing.Point(81, 38);
            this.cBranch.Name = "cBranch";
            this.cBranch.Size = new System.Drawing.Size(349, 20);
            this.cBranch.TabIndex = 15;
            // 
            // cPath
            // 
            this.cPath.Location = new System.Drawing.Point(81, 64);
            this.cPath.Name = "cPath";
            this.cPath.Size = new System.Drawing.Size(349, 20);
            this.cPath.TabIndex = 16;
            // 
            // PackageDialog
            // 
            this.AcceptButton = this.bOK;
            this.AutoScaleDimensions = new System.Drawing.SizeF(6F, 13F);
            this.AutoScaleMode = System.Windows.Forms.AutoScaleMode.Font;
            this.CancelButton = this.bCancel;
            this.ClientSize = new System.Drawing.Size(442, 132);
            this.Controls.Add(this.cPath);
            this.Controls.Add(this.cBranch);
            this.Controls.Add(this.cOrigin);
            this.Controls.Add(this.label3);
            this.Controls.Add(this.label2);
            this.Controls.Add(this.label1);
            this.Controls.Add(this.bOK);
            this.Controls.Add(this.bCancel);
            this.FormBorderStyle = System.Windows.Forms.FormBorderStyle.FixedDialog;
            this.MaximizeBox = false;
            this.MinimizeBox = false;
            this.Name = "PackageDialog";
            this.Text = "Package Details";
            this.ResumeLayout(false);
            this.PerformLayout();

        }

        #endregion

        private System.Windows.Forms.Button bOK;
        private System.Windows.Forms.Button bCancel;
        private System.Windows.Forms.Label label1;
        private System.Windows.Forms.Label label2;
        private System.Windows.Forms.Label label3;
        private System.Windows.Forms.TextBox cOrigin;
        private System.Windows.Forms.TextBox cBranch;
        private System.Windows.Forms.TextBox cPath;
    }
}