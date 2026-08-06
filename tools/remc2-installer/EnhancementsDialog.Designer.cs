namespace WixSharpSetup
{
    partial class EnhancedDataDialog
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
            this.label2 = new System.Windows.Forms.Label();
            this.label1 = new System.Windows.Forms.Label();
            this.pnlBottom = new System.Windows.Forms.Panel();
            this.btnBack = new System.Windows.Forms.Button();
            this.btnNext = new System.Windows.Forms.Button();
            this.btnCancel = new System.Windows.Forms.Button();
            this.chkInstallTextures = new System.Windows.Forms.CheckBox();
            this.chkSpeech = new System.Windows.Forms.CheckBox();
            this.chkFixedMenuGraphics = new System.Windows.Forms.CheckBox();
            this.chkExtendedFonts = new System.Windows.Forms.CheckBox();
            this.tblMain = new System.Windows.Forms.TableLayoutPanel();
            this.panel2 = new System.Windows.Forms.Panel();
            this.grpOptions = new System.Windows.Forms.GroupBox();
            this.pnlBottom.SuspendLayout();
            this.tblMain.SuspendLayout();
            this.panel2.SuspendLayout();
            this.grpOptions.SuspendLayout();
            this.SuspendLayout();
            // 
            // label2
            // 
            this.label2.AutoSize = true;
            this.label2.BackColor = System.Drawing.Color.White;
            this.label2.Font = new System.Drawing.Font("Tahoma", 8.25F, System.Drawing.FontStyle.Regular, System.Drawing.GraphicsUnit.Point, ((byte)(0)));
            this.label2.Location = new System.Drawing.Point(30, 31);
            this.label2.Name = "label2";
            this.label2.Size = new System.Drawing.Size(556, 24);
            this.label2.TabIndex = 1;
            this.label2.Text = "Would you like to install enhanced assets for Magic Carpet 2?";
            // 
            // label1
            // 
            this.label1.AutoSize = true;
            this.label1.BackColor = System.Drawing.Color.White;
            this.label1.Font = new System.Drawing.Font("Tahoma", 9F, System.Drawing.FontStyle.Bold, System.Drawing.GraphicsUnit.Point, ((byte)(0)));
            this.label1.Location = new System.Drawing.Point(16, 8);
            this.label1.Name = "label1";
            this.label1.Size = new System.Drawing.Size(261, 25);
            this.label1.TabIndex = 1;
            this.label1.Text = "Install Enhanced Assets";
            // 
            // pnlBottom
            // 
            this.pnlBottom.BackColor = System.Drawing.SystemColors.Control;
            this.pnlBottom.BorderStyle = System.Windows.Forms.BorderStyle.FixedSingle;
            this.pnlBottom.Controls.Add(this.btnBack);
            this.pnlBottom.Controls.Add(this.btnNext);
            this.pnlBottom.Controls.Add(this.btnCancel);
            this.pnlBottom.Dock = System.Windows.Forms.DockStyle.Fill;
            this.pnlBottom.Location = new System.Drawing.Point(3, 279);
            this.pnlBottom.Name = "pnlBottom";
            this.pnlBottom.Size = new System.Drawing.Size(480, 54);
            this.pnlBottom.TabIndex = 9;
            // 
            // btnBack
            // 
            this.btnBack.Anchor = ((System.Windows.Forms.AnchorStyles)((System.Windows.Forms.AnchorStyles.Bottom | System.Windows.Forms.AnchorStyles.Right)));
            this.btnBack.AutoSize = true;
            this.btnBack.Font = new System.Drawing.Font("Tahoma", 8.25F, System.Drawing.FontStyle.Regular, System.Drawing.GraphicsUnit.Point, ((byte)(0)));
            this.btnBack.Location = new System.Drawing.Point(229, 12);
            this.btnBack.Name = "btnBack";
            this.btnBack.Size = new System.Drawing.Size(75, 34);
            this.btnBack.TabIndex = 5;
            this.btnBack.Text = "Back";
            this.btnBack.UseVisualStyleBackColor = true;
            this.btnBack.Click += new System.EventHandler(this.back_Click);
            // 
            // btnNext
            // 
            this.btnNext.Anchor = ((System.Windows.Forms.AnchorStyles)((System.Windows.Forms.AnchorStyles.Bottom | System.Windows.Forms.AnchorStyles.Right)));
            this.btnNext.AutoSize = true;
            this.btnNext.Font = new System.Drawing.Font("Tahoma", 8.25F, System.Drawing.FontStyle.Regular, System.Drawing.GraphicsUnit.Point, ((byte)(0)));
            this.btnNext.Location = new System.Drawing.Point(310, 12);
            this.btnNext.Name = "btnNext";
            this.btnNext.Size = new System.Drawing.Size(75, 34);
            this.btnNext.TabIndex = 4;
            this.btnNext.Text = "Next";
            this.btnNext.UseVisualStyleBackColor = true;
            this.btnNext.Click += new System.EventHandler(this.next_Click);
            // 
            // btnCancel
            // 
            this.btnCancel.Anchor = ((System.Windows.Forms.AnchorStyles)((System.Windows.Forms.AnchorStyles.Bottom | System.Windows.Forms.AnchorStyles.Right)));
            this.btnCancel.AutoSize = true;
            this.btnCancel.DialogResult = System.Windows.Forms.DialogResult.Cancel;
            this.btnCancel.Font = new System.Drawing.Font("Tahoma", 8.25F, System.Drawing.FontStyle.Regular, System.Drawing.GraphicsUnit.Point, ((byte)(0)));
            this.btnCancel.Location = new System.Drawing.Point(391, 12);
            this.btnCancel.Name = "btnCancel";
            this.btnCancel.Size = new System.Drawing.Size(79, 34);
            this.btnCancel.TabIndex = 6;
            this.btnCancel.Text = "Cancel";
            this.btnCancel.UseVisualStyleBackColor = true;
            this.btnCancel.Click += new System.EventHandler(this.cancel_Click);
            // 
            // chkInstallTextures
            // 
            this.chkInstallTextures.AutoSize = true;
            this.chkInstallTextures.Checked = true;
            this.chkInstallTextures.CheckState = System.Windows.Forms.CheckState.Checked;
            this.chkInstallTextures.Location = new System.Drawing.Point(18, 44);
            this.chkInstallTextures.Margin = new System.Windows.Forms.Padding(10);
            this.chkInstallTextures.Name = "chkInstallTextures";
            this.chkInstallTextures.Size = new System.Drawing.Size(267, 28);
            this.chkInstallTextures.TabIndex = 0;
            this.chkInstallTextures.Text = "Install Enhanced Textures";
            this.chkInstallTextures.UseVisualStyleBackColor = true;
            // 
            // chkSpeech
            // 
            this.chkSpeech.AutoSize = true;
            this.chkSpeech.Checked = true;
            this.chkSpeech.CheckState = System.Windows.Forms.CheckState.Checked;
            this.chkSpeech.Location = new System.Drawing.Point(18, 155);
            this.chkSpeech.Margin = new System.Windows.Forms.Padding(10);
            this.chkSpeech.Name = "chkSpeech";
            this.chkSpeech.Size = new System.Drawing.Size(161, 28);
            this.chkSpeech.TabIndex = 3;
            this.chkSpeech.Text = "Install Speech";
            this.chkSpeech.UseVisualStyleBackColor = true;
            // 
            // chkFixedMenuGraphics
            // 
            this.chkFixedMenuGraphics.AutoSize = true;
            this.chkFixedMenuGraphics.Checked = true;
            this.chkFixedMenuGraphics.CheckState = System.Windows.Forms.CheckState.Checked;
            this.chkFixedMenuGraphics.Location = new System.Drawing.Point(18, 81);
            this.chkFixedMenuGraphics.Margin = new System.Windows.Forms.Padding(10);
            this.chkFixedMenuGraphics.Name = "chkFixedMenuGraphics";
            this.chkFixedMenuGraphics.Size = new System.Drawing.Size(280, 28);
            this.chkFixedMenuGraphics.TabIndex = 1;
            this.chkFixedMenuGraphics.Text = "Install Fixed Menu Graphics";
            this.chkFixedMenuGraphics.UseVisualStyleBackColor = true;
            // 
            // chkExtendedFonts
            // 
            this.chkExtendedFonts.AutoSize = true;
            this.chkExtendedFonts.Checked = true;
            this.chkExtendedFonts.CheckState = System.Windows.Forms.CheckState.Checked;
            this.chkExtendedFonts.Location = new System.Drawing.Point(18, 118);
            this.chkExtendedFonts.Margin = new System.Windows.Forms.Padding(10);
            this.chkExtendedFonts.Name = "chkExtendedFonts";
            this.chkExtendedFonts.Size = new System.Drawing.Size(234, 28);
            this.chkExtendedFonts.TabIndex = 2;
            this.chkExtendedFonts.Text = "Install Extended Fonts";
            this.chkExtendedFonts.UseVisualStyleBackColor = true;
            // 
            // tblMain
            // 
            this.tblMain.ColumnCount = 1;
            this.tblMain.ColumnStyles.Add(new System.Windows.Forms.ColumnStyle(System.Windows.Forms.SizeType.Percent, 100F));
            this.tblMain.Controls.Add(this.panel2, 0, 0);
            this.tblMain.Controls.Add(this.pnlBottom, 0, 2);
            this.tblMain.Controls.Add(this.grpOptions, 0, 1);
            this.tblMain.Dock = System.Windows.Forms.DockStyle.Fill;
            this.tblMain.Location = new System.Drawing.Point(0, 0);
            this.tblMain.Name = "tblMain";
            this.tblMain.RowCount = 3;
            this.tblMain.RowStyles.Add(new System.Windows.Forms.RowStyle(System.Windows.Forms.SizeType.Absolute, 70F));
            this.tblMain.RowStyles.Add(new System.Windows.Forms.RowStyle(System.Windows.Forms.SizeType.Percent, 100F));
            this.tblMain.RowStyles.Add(new System.Windows.Forms.RowStyle(System.Windows.Forms.SizeType.Absolute, 60F));
            this.tblMain.Size = new System.Drawing.Size(486, 336);
            this.tblMain.TabIndex = 2;
            // 
            // panel2
            // 
            this.panel2.BackColor = System.Drawing.Color.White;
            this.panel2.BorderStyle = System.Windows.Forms.BorderStyle.FixedSingle;
            this.panel2.Controls.Add(this.label2);
            this.panel2.Controls.Add(this.label1);
            this.panel2.Dock = System.Windows.Forms.DockStyle.Fill;
            this.panel2.Location = new System.Drawing.Point(3, 3);
            this.panel2.Name = "panel2";
            this.panel2.Size = new System.Drawing.Size(480, 64);
            this.panel2.TabIndex = 10;
            // 
            // grpOptions
            // 
            this.grpOptions.Controls.Add(this.chkInstallTextures);
            this.grpOptions.Controls.Add(this.chkExtendedFonts);
            this.grpOptions.Controls.Add(this.chkSpeech);
            this.grpOptions.Controls.Add(this.chkFixedMenuGraphics);
            this.grpOptions.Dock = System.Windows.Forms.DockStyle.Fill;
            this.grpOptions.Location = new System.Drawing.Point(3, 73);
            this.grpOptions.Name = "grpOptions";
            this.grpOptions.Padding = new System.Windows.Forms.Padding(10);
            this.grpOptions.Size = new System.Drawing.Size(480, 200);
            this.grpOptions.TabIndex = 11;
            this.grpOptions.TabStop = false;
            this.grpOptions.Text = "Options";
            // 
            // EnhancedDataDialog
            // 
            this.AcceptButton = this.btnNext;
            this.AutoScaleDimensions = new System.Drawing.SizeF(168F, 168F);
            this.AutoScaleMode = System.Windows.Forms.AutoScaleMode.Dpi;
            this.CancelButton = this.btnCancel;
            this.ClientSize = new System.Drawing.Size(486, 336);
            this.Controls.Add(this.tblMain);
            this.Font = new System.Drawing.Font("Tahoma", 8.25F, System.Drawing.FontStyle.Regular, System.Drawing.GraphicsUnit.Point, ((byte)(0)));
            this.MaximizeBox = false;
            this.MinimumSize = new System.Drawing.Size(510, 400);
            this.Name = "EnhancedDataDialog";
            this.StartPosition = System.Windows.Forms.FormStartPosition.CenterScreen;
            this.Text = "Enhanced Assets";
            this.Load += new System.EventHandler(this.dialog_Load);
            this.pnlBottom.ResumeLayout(false);
            this.pnlBottom.PerformLayout();
            this.tblMain.ResumeLayout(false);
            this.panel2.ResumeLayout(false);
            this.panel2.PerformLayout();
            this.grpOptions.ResumeLayout(false);
            this.grpOptions.PerformLayout();
            this.ResumeLayout(false);

        }

        #endregion
        private System.Windows.Forms.Label label2;
        private System.Windows.Forms.Label label1;
        private System.Windows.Forms.Panel pnlBottom;
        private System.Windows.Forms.Button btnBack;
        private System.Windows.Forms.Button btnNext;
        private System.Windows.Forms.Button btnCancel;
        private System.Windows.Forms.CheckBox chkInstallTextures;
		private System.Windows.Forms.CheckBox chkSpeech;
		private System.Windows.Forms.CheckBox chkFixedMenuGraphics;
		private System.Windows.Forms.CheckBox chkExtendedFonts;
		private System.Windows.Forms.TableLayoutPanel tblMain;
		private System.Windows.Forms.Panel panel2;
		private System.Windows.Forms.GroupBox grpOptions;
	}
}