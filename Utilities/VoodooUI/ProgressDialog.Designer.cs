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
            this.cProgressBar = new System.Windows.Forms.ProgressBar();
            this.cDetails = new System.Windows.Forms.RichTextBox();
            this.bOK = new System.Windows.Forms.Button();
            this.bCancel = new System.Windows.Forms.Button();
            this.SuspendLayout();
            // 
            // cProgressBar
            // 
            this.cProgressBar.Anchor = ((System.Windows.Forms.AnchorStyles)((System.Windows.Forms.AnchorStyles.Bottom | System.Windows.Forms.AnchorStyles.Left)));
            this.cProgressBar.Location = new System.Drawing.Point(3, 413);
            this.cProgressBar.Name = "cProgressBar";
            this.cProgressBar.Size = new System.Drawing.Size(188, 26);
            this.cProgressBar.TabIndex = 5;
            // 
            // cDetails
            // 
            this.cDetails.Anchor = ((System.Windows.Forms.AnchorStyles)((((System.Windows.Forms.AnchorStyles.Top | System.Windows.Forms.AnchorStyles.Bottom) 
            | System.Windows.Forms.AnchorStyles.Left) 
            | System.Windows.Forms.AnchorStyles.Right)));
            this.cDetails.Location = new System.Drawing.Point(3, 3);
            this.cDetails.Name = "cDetails";
            this.cDetails.Size = new System.Drawing.Size(618, 404);
            this.cDetails.TabIndex = 6;
            this.cDetails.Text = "";
            // 
            // bOK
            // 
            this.bOK.Anchor = ((System.Windows.Forms.AnchorStyles)((System.Windows.Forms.AnchorStyles.Bottom | System.Windows.Forms.AnchorStyles.Right)));
            this.bOK.DialogResult = System.Windows.Forms.DialogResult.OK;
            this.bOK.Enabled = false;
            this.bOK.Location = new System.Drawing.Point(455, 413);
            this.bOK.Name = "bOK";
            this.bOK.Size = new System.Drawing.Size(80, 26);
            this.bOK.TabIndex = 7;
            this.bOK.Text = "OK";
            this.bOK.UseVisualStyleBackColor = true;
            this.bOK.Click += new System.EventHandler(this.ButtonOK);
            // 
            // bCancel
            // 
            this.bCancel.Anchor = ((System.Windows.Forms.AnchorStyles)((System.Windows.Forms.AnchorStyles.Bottom | System.Windows.Forms.AnchorStyles.Right)));
            this.bCancel.DialogResult = System.Windows.Forms.DialogResult.Cancel;
            this.bCancel.Location = new System.Drawing.Point(541, 413);
            this.bCancel.Name = "bCancel";
            this.bCancel.Size = new System.Drawing.Size(80, 26);
            this.bCancel.TabIndex = 8;
            this.bCancel.Text = "Cancel";
            this.bCancel.UseVisualStyleBackColor = true;
            this.bCancel.Click += new System.EventHandler(this.ButtonCancel);
            // 
            // ProgressDialog
            // 
            this.AutoScaleDimensions = new System.Drawing.SizeF(6F, 13F);
            this.AutoScaleMode = System.Windows.Forms.AutoScaleMode.Font;
            this.CancelButton = this.bCancel;
            this.ClientSize = new System.Drawing.Size(624, 442);
            this.Controls.Add(this.cProgressBar);
            this.Controls.Add(this.cDetails);
            this.Controls.Add(this.bOK);
            this.Controls.Add(this.bCancel);
            this.FormBorderStyle = System.Windows.Forms.FormBorderStyle.FixedDialog;
            this.MaximizeBox = false;
            this.MinimizeBox = false;
            this.Name = "ProgressDialog";
            this.ShowInTaskbar = false;
            this.StartPosition = System.Windows.Forms.FormStartPosition.CenterParent;
            this.Text = "Progress";
            this.ResumeLayout(false);

        }

        #endregion

        private System.Windows.Forms.ProgressBar cProgressBar;
        private System.Windows.Forms.RichTextBox cDetails;
        private System.Windows.Forms.Button bOK;
        private System.Windows.Forms.Button bCancel;



    }
}