using System;
using System.Diagnostics;
using System.IO;
using System.Windows.Forms;
using WixSharp;
using WixSharp.UI.Forms;

namespace WixSharpSetup
{
    public partial class RedistDialog : ManagedForm
    {
		private bool _runClicked;

        public RedistDialog()
        {
            //NOTE: If this assembly is compiled for v4.0.30319 runtime, it may not be compatible with the MSI hosted CLR.
            //The incompatibility is particularly possible for the Embedded UI scenarios.
            //The safest way to avoid the problem is to compile the assembly for v3.5 Target Framework.remc2_installer
            InitializeComponent();

            this.FormClosing += RedistDialog_FormClosing;
        }
   
        private bool InstallRedist()
        {
            try
            {
                if (Directory.Exists(Path.Combine(Runtime.InstallDir, "Extract")))
                {
					string redistPath = Path.Combine(Runtime.InstallDir, @"Extract\VC_redist.x86.exe");

					if (System.IO.File.Exists(Path.Combine(Runtime.InstallDir, @"Extract\VC_redist.x64.exe")))
						redistPath = Path.Combine(Runtime.InstallDir, @"Extract\VC_redist.x64.exe");

					ProcessStartInfo processStartInfo = new ProcessStartInfo(redistPath);
                    processStartInfo.WorkingDirectory = Runtime.InstallDir;
                    processStartInfo.UseShellExecute = false;
                    processStartInfo.ErrorDialog = true;
                    Process.Start(processStartInfo).Wait();
                }
            }
            catch (Exception ex)
            {
                if (MessageBox.Show($"Error installing C++ Redistribution: {ex.Message}\nWould you like to continue the Installation?", "C++ Redistribution Error", MessageBoxButtons.YesNo, MessageBoxIcon.Error) == DialogResult.Yes)
                {
                    return true;
                }
                return false;
            }
            return true;
        }

        private void dialog_Load(object sender, EventArgs e)
        {
            Text = "[ProductName] Setup";
            base.Localize();
        }

        private void btnRun_Click(object sender, EventArgs e)
        {
			bool success = false;
			_runClicked = true;
            btnRun.Enabled = false;
            btnNext.Enabled = false;

			if (InstallRedist())
			{
				MessageBox.Show("C++ Redistribute was successful. Click [Next] to finish", "Success", MessageBoxButtons.OK);
				btnNext.Enabled = true;
			}

			btnRun.Enabled = true;
			btnNext.Enabled = true;
		}

        private void btnNext_Click(object sender, EventArgs e)
        {
			if (!_runClicked)
			{
				if (MessageBox.Show("Are you sure you want to skip installing the C++ Redistributable? You cannot run Magic Carpet 2 HD without it!", "Warning", MessageBoxButtons.YesNo, MessageBoxIcon.Question) == DialogResult.Yes)
					Shell.GoNext();
			}
			else
			{
				Shell.GoNext();
			}
		}

		private void RedistDialog_FormClosing(object sender, FormClosingEventArgs e)
		{
			//Do nothing
		}
	}
}