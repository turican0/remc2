namespace WixSharpSetup
{
    partial class GameDataDialog
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
            System.Windows.Forms.TreeNode treeNode4 = new System.Windows.Forms.TreeNode("CD_Files");
            System.Windows.Forms.TreeNode treeNode5 = new System.Windows.Forms.TreeNode("NETHERW");
            System.Windows.Forms.TreeNode treeNode6 = new System.Windows.Forms.TreeNode("C:\\Program Files x86\\Remc2\\Magic Carpet 2 HD", new System.Windows.Forms.TreeNode[] {
            treeNode4,
            treeNode5});
            this.panel2 = new System.Windows.Forms.Panel();
            this.label2 = new System.Windows.Forms.Label();
            this.label1 = new System.Windows.Forms.Label();
            this.panel1 = new System.Windows.Forms.Panel();
            this.btnNext = new System.Windows.Forms.Button();
            this.btnRun = new System.Windows.Forms.Button();
            this.cboInstallLocation = new System.Windows.Forms.ComboBox();
            this.lblInstallLocation = new System.Windows.Forms.Label();
            this.btnBrowse = new System.Windows.Forms.Button();
            this.lblGameData = new System.Windows.Forms.Label();
            this.grpBox = new System.Windows.Forms.GroupBox();
            this.btnInfo = new System.Windows.Forms.Button();
            this.treeViewInstallProgress = new System.Windows.Forms.TreeView();
            this.tblMain = new System.Windows.Forms.TableLayoutPanel();
            this.tableLayoutPanel1 = new System.Windows.Forms.TableLayoutPanel();
            this.txtPath = new System.Windows.Forms.TextBox();
            this.panel2.SuspendLayout();
            this.panel1.SuspendLayout();
            this.grpBox.SuspendLayout();
            this.tblMain.SuspendLayout();
            this.tableLayoutPanel1.SuspendLayout();
            this.SuspendLayout();
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
            // label2
            // 
            this.label2.AutoSize = true;
            this.label2.BackColor = System.Drawing.Color.White;
            this.label2.Font = new System.Drawing.Font("Tahoma", 8.25F, System.Drawing.FontStyle.Regular, System.Drawing.GraphicsUnit.Point, ((byte)(0)));
            this.label2.Location = new System.Drawing.Point(30, 31);
            this.label2.Name = "label2";
            this.label2.Size = new System.Drawing.Size(569, 24);
            this.label2.TabIndex = 1;
            this.label2.Text = "Please select where to get the Magic Carpet 2 game data from";
            // 
            // label1
            // 
            this.label1.AutoSize = true;
            this.label1.BackColor = System.Drawing.Color.White;
            this.label1.Font = new System.Drawing.Font("Tahoma", 9F, System.Drawing.FontStyle.Bold, System.Drawing.GraphicsUnit.Point, ((byte)(0)));
            this.label1.Location = new System.Drawing.Point(16, 8);
            this.label1.Name = "label1";
            this.label1.Size = new System.Drawing.Size(226, 25);
            this.label1.TabIndex = 1;
            this.label1.Text = "Game Data Location";
            // 
            // panel1
            // 
            this.panel1.BackColor = System.Drawing.SystemColors.Control;
            this.panel1.BorderStyle = System.Windows.Forms.BorderStyle.FixedSingle;
            this.panel1.Controls.Add(this.btnNext);
            this.panel1.Controls.Add(this.btnRun);
            this.panel1.Dock = System.Windows.Forms.DockStyle.Fill;
            this.panel1.Location = new System.Drawing.Point(3, 269);
            this.panel1.Name = "panel1";
            this.panel1.Size = new System.Drawing.Size(480, 64);
            this.panel1.TabIndex = 9;
            // 
            // btnNext
            // 
            this.btnNext.Anchor = ((System.Windows.Forms.AnchorStyles)((System.Windows.Forms.AnchorStyles.Bottom | System.Windows.Forms.AnchorStyles.Right)));
            this.btnNext.AutoSize = true;
            this.btnNext.Font = new System.Drawing.Font("Tahoma", 8.25F, System.Drawing.FontStyle.Regular, System.Drawing.GraphicsUnit.Point, ((byte)(0)));
            this.btnNext.Location = new System.Drawing.Point(394, 20);
            this.btnNext.Name = "btnNext";
            this.btnNext.Size = new System.Drawing.Size(75, 34);
            this.btnNext.TabIndex = 1;
            this.btnNext.Text = "Next";
            this.btnNext.UseVisualStyleBackColor = true;
            this.btnNext.Click += new System.EventHandler(this.btnNext_Click);
            // 
            // btnRun
            // 
            this.btnRun.Anchor = ((System.Windows.Forms.AnchorStyles)((System.Windows.Forms.AnchorStyles.Bottom | System.Windows.Forms.AnchorStyles.Right)));
            this.btnRun.AutoSize = true;
            this.btnRun.Font = new System.Drawing.Font("Tahoma", 8.25F, System.Drawing.FontStyle.Regular, System.Drawing.GraphicsUnit.Point, ((byte)(0)));
            this.btnRun.Location = new System.Drawing.Point(199, 20);
            this.btnRun.Name = "btnRun";
            this.btnRun.Size = new System.Drawing.Size(189, 34);
            this.btnRun.TabIndex = 0;
            this.btnRun.Text = "Extract Game Data";
            this.btnRun.UseVisualStyleBackColor = true;
            this.btnRun.Click += new System.EventHandler(this.btnRun_Click);
            // 
            // cboInstallLocation
            // 
            this.cboInstallLocation.Dock = System.Windows.Forms.DockStyle.Fill;
            this.cboInstallLocation.FormattingEnabled = true;
            this.cboInstallLocation.Items.AddRange(new object[] {
            "GOG Edition",
            "DOS Edition"});
            this.cboInstallLocation.Location = new System.Drawing.Point(203, 43);
            this.cboInstallLocation.Name = "cboInstallLocation";
            this.cboInstallLocation.Size = new System.Drawing.Size(254, 31);
            this.cboInstallLocation.TabIndex = 3;
            // 
            // lblInstallLocation
            // 
            this.lblInstallLocation.AutoSize = true;
            this.lblInstallLocation.Location = new System.Drawing.Point(3, 40);
            this.lblInstallLocation.Name = "lblInstallLocation";
            this.lblInstallLocation.Size = new System.Drawing.Size(148, 24);
            this.lblInstallLocation.TabIndex = 2;
            this.lblInstallLocation.Text = "Get Data From:";
            // 
            // btnBrowse
            // 
            this.btnBrowse.AutoSize = true;
            this.btnBrowse.Location = new System.Drawing.Point(3, 83);
            this.btnBrowse.Name = "btnBrowse";
            this.btnBrowse.Size = new System.Drawing.Size(148, 34);
            this.btnBrowse.TabIndex = 4;
            this.btnBrowse.Text = "Browse";
            this.btnBrowse.UseVisualStyleBackColor = true;
            this.btnBrowse.Click += new System.EventHandler(this.btnBrowse_Click);
            // 
            // lblGameData
            // 
            this.lblGameData.AutoSize = true;
            this.tableLayoutPanel1.SetColumnSpan(this.lblGameData, 2);
            this.lblGameData.Location = new System.Drawing.Point(3, 0);
            this.lblGameData.Name = "lblGameData";
            this.lblGameData.Size = new System.Drawing.Size(280, 24);
            this.lblGameData.TabIndex = 15;
            this.lblGameData.Text = "Magic Carpet 2 GOG directory:";
            // 
            // grpBox
            // 
            this.grpBox.Controls.Add(this.tableLayoutPanel1);
            this.grpBox.Dock = System.Windows.Forms.DockStyle.Fill;
            this.grpBox.Location = new System.Drawing.Point(3, 113);
            this.grpBox.Name = "grpBox";
            this.grpBox.Padding = new System.Windows.Forms.Padding(10);
            this.grpBox.Size = new System.Drawing.Size(480, 50);
            this.grpBox.TabIndex = 17;
            this.grpBox.TabStop = false;
            // 
            // btnInfo
            // 
            this.btnInfo.Anchor = ((System.Windows.Forms.AnchorStyles)((System.Windows.Forms.AnchorStyles.Top | System.Windows.Forms.AnchorStyles.Right)));
            this.btnInfo.AutoSize = true;
            this.btnInfo.Location = new System.Drawing.Point(408, 73);
            this.btnInfo.Name = "btnInfo";
            this.btnInfo.Size = new System.Drawing.Size(75, 34);
            this.btnInfo.TabIndex = 8;
            this.btnInfo.Text = "Help";
            this.btnInfo.UseVisualStyleBackColor = true;
            this.btnInfo.Click += new System.EventHandler(this.btnInfo_Click);
            // 
            // treeViewInstallProgress
            // 
            this.treeViewInstallProgress.CheckBoxes = true;
            this.treeViewInstallProgress.Dock = System.Windows.Forms.DockStyle.Fill;
            this.treeViewInstallProgress.Enabled = false;
            this.treeViewInstallProgress.Location = new System.Drawing.Point(3, 176);
            this.treeViewInstallProgress.Margin = new System.Windows.Forms.Padding(3, 10, 3, 3);
            this.treeViewInstallProgress.Name = "treeViewInstallProgress";
            treeNode4.Name = "CDFiles";
            treeNode4.Text = "CD_Files";
            treeNode5.Name = "NETHERW";
            treeNode5.Text = "NETHERW";
            treeNode6.Checked = true;
            treeNode6.Name = "InstallPath";
            treeNode6.Text = "C:\\Program Files x86\\Remc2\\Magic Carpet 2 HD";
            this.treeViewInstallProgress.Nodes.AddRange(new System.Windows.Forms.TreeNode[] {
            treeNode6});
            this.treeViewInstallProgress.ShowPlusMinus = false;
            this.treeViewInstallProgress.ShowRootLines = false;
            this.treeViewInstallProgress.Size = new System.Drawing.Size(480, 87);
            this.treeViewInstallProgress.TabIndex = 19;
            this.treeViewInstallProgress.TabStop = false;
            // 
            // tblMain
            // 
            this.tblMain.ColumnCount = 1;
            this.tblMain.ColumnStyles.Add(new System.Windows.Forms.ColumnStyle(System.Windows.Forms.SizeType.Percent, 100F));
            this.tblMain.Controls.Add(this.panel2, 0, 0);
            this.tblMain.Controls.Add(this.panel1, 0, 4);
            this.tblMain.Controls.Add(this.treeViewInstallProgress, 0, 3);
            this.tblMain.Controls.Add(this.grpBox, 0, 2);
            this.tblMain.Controls.Add(this.btnInfo, 0, 1);
            this.tblMain.Dock = System.Windows.Forms.DockStyle.Fill;
            this.tblMain.Location = new System.Drawing.Point(0, 0);
            this.tblMain.Name = "tblMain";
            this.tblMain.RowCount = 5;
            this.tblMain.RowStyles.Add(new System.Windows.Forms.RowStyle(System.Windows.Forms.SizeType.Absolute, 70F));
            this.tblMain.RowStyles.Add(new System.Windows.Forms.RowStyle(System.Windows.Forms.SizeType.Absolute, 40F));
            this.tblMain.RowStyles.Add(new System.Windows.Forms.RowStyle(System.Windows.Forms.SizeType.Percent, 100F));
            this.tblMain.RowStyles.Add(new System.Windows.Forms.RowStyle(System.Windows.Forms.SizeType.Absolute, 100F));
            this.tblMain.RowStyles.Add(new System.Windows.Forms.RowStyle(System.Windows.Forms.SizeType.Absolute, 70F));
            this.tblMain.Size = new System.Drawing.Size(486, 336);
            this.tblMain.TabIndex = 20;
            // 
            // tableLayoutPanel1
            // 
            this.tableLayoutPanel1.ColumnCount = 2;
            this.tableLayoutPanel1.ColumnStyles.Add(new System.Windows.Forms.ColumnStyle(System.Windows.Forms.SizeType.Absolute, 200F));
            this.tableLayoutPanel1.ColumnStyles.Add(new System.Windows.Forms.ColumnStyle(System.Windows.Forms.SizeType.Percent, 100F));
            this.tableLayoutPanel1.Controls.Add(this.lblInstallLocation, 0, 1);
            this.tableLayoutPanel1.Controls.Add(this.lblGameData, 0, 0);
            this.tableLayoutPanel1.Controls.Add(this.btnBrowse, 0, 2);
            this.tableLayoutPanel1.Controls.Add(this.cboInstallLocation, 1, 1);
            this.tableLayoutPanel1.Controls.Add(this.txtPath, 0, 3);
            this.tableLayoutPanel1.Dock = System.Windows.Forms.DockStyle.Fill;
            this.tableLayoutPanel1.Location = new System.Drawing.Point(10, 34);
            this.tableLayoutPanel1.Name = "tableLayoutPanel1";
            this.tableLayoutPanel1.RowCount = 4;
            this.tableLayoutPanel1.RowStyles.Add(new System.Windows.Forms.RowStyle(System.Windows.Forms.SizeType.Absolute, 40F));
            this.tableLayoutPanel1.RowStyles.Add(new System.Windows.Forms.RowStyle(System.Windows.Forms.SizeType.Absolute, 40F));
            this.tableLayoutPanel1.RowStyles.Add(new System.Windows.Forms.RowStyle(System.Windows.Forms.SizeType.Absolute, 40F));
            this.tableLayoutPanel1.RowStyles.Add(new System.Windows.Forms.RowStyle(System.Windows.Forms.SizeType.Absolute, 40F));
            this.tableLayoutPanel1.Size = new System.Drawing.Size(460, 6);
            this.tableLayoutPanel1.TabIndex = 16;
            // 
            // txtPath
            // 
            this.tableLayoutPanel1.SetColumnSpan(this.txtPath, 2);
            this.txtPath.Dock = System.Windows.Forms.DockStyle.Fill;
            this.txtPath.Location = new System.Drawing.Point(3, 130);
            this.txtPath.Margin = new System.Windows.Forms.Padding(3, 10, 3, 3);
            this.txtPath.Name = "txtPath";
            this.txtPath.Size = new System.Drawing.Size(454, 31);
            this.txtPath.TabIndex = 5;
            this.txtPath.TextChanged += new System.EventHandler(this.Path_TextChanged);
            // 
            // GameDataDialog
            // 
            this.AcceptButton = this.btnRun;
            this.AutoScaleDimensions = new System.Drawing.SizeF(168F, 168F);
            this.AutoScaleMode = System.Windows.Forms.AutoScaleMode.Dpi;
            this.ClientSize = new System.Drawing.Size(486, 336);
            this.Controls.Add(this.tblMain);
            this.Font = new System.Drawing.Font("Tahoma", 8.25F, System.Drawing.FontStyle.Regular, System.Drawing.GraphicsUnit.Point, ((byte)(0)));
            this.MaximizeBox = false;
            this.MinimumSize = new System.Drawing.Size(510, 400);
            this.Name = "GameDataDialog";
            this.SizeGripStyle = System.Windows.Forms.SizeGripStyle.Hide;
            this.StartPosition = System.Windows.Forms.FormStartPosition.CenterScreen;
            this.Text = "Game Data";
            this.Load += new System.EventHandler(this.dialog_Load);
            this.panel2.ResumeLayout(false);
            this.panel2.PerformLayout();
            this.panel1.ResumeLayout(false);
            this.panel1.PerformLayout();
            this.grpBox.ResumeLayout(false);
            this.tblMain.ResumeLayout(false);
            this.tblMain.PerformLayout();
            this.tableLayoutPanel1.ResumeLayout(false);
            this.tableLayoutPanel1.PerformLayout();
            this.ResumeLayout(false);

        }

        #endregion
        private System.Windows.Forms.Panel panel2;
        private System.Windows.Forms.Label label2;
        private System.Windows.Forms.Label label1;
        private System.Windows.Forms.Panel panel1;
        private System.Windows.Forms.Button btnRun;
        private System.Windows.Forms.ComboBox cboInstallLocation;
        private System.Windows.Forms.Label lblInstallLocation;
        private System.Windows.Forms.Button btnBrowse;
        private System.Windows.Forms.Button btnNext;
        private System.Windows.Forms.Label lblGameData;
		private System.Windows.Forms.GroupBox grpBox;
		private System.Windows.Forms.Button btnInfo;
		private System.Windows.Forms.TreeView treeViewInstallProgress;
		private System.Windows.Forms.TableLayoutPanel tblMain;
		private System.Windows.Forms.TableLayoutPanel tableLayoutPanel1;
		private System.Windows.Forms.TextBox txtPath;
	}
}