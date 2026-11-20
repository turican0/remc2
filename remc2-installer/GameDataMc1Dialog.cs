using System;
using System.IO;
using System.Windows.Forms;
using remc2_installer;
using WixSharp;
using WixSharp.UI.Forms;

namespace WixSharpSetup
{
	/// <summary>
	/// Installs the Magic Carpet / Hidden Worlds game data (Mc1GameData): from the GOG Magic Carpet Plus (its CD image) or
	/// from a DOS CD, to MC1\CARPET.CD (the installed game) and MC1\CD_Files (the files of the CD).
	/// </summary>
	public partial class GameDataMc1Dialog : ManagedForm
	{
		private const int GogEdition = 0;
		private const int DosEdition = 1;

		private bool _runClicked;

		public GameDataMc1Dialog()
		{
			//NOTE: If this assembly is compiled for v4.0.30319 runtime, it may not be compatible with the MSI hosted CLR.
			//The incompatibility is particularly possible for the Embedded UI scenarios.
			//The safest way to avoid the problem is to compile the assembly for v3.5 Target Framework.remc2_installer
			InitializeComponent();

			this.Shown += GameDataMc1Dialog_Shown;
			this.FormClosing += GameDataMc1Dialog_FormClosing;
		}

		private string InstallDir
		{
			get
			{
				try
				{
					if (Runtime != null)
						return Runtime.InstallDir;
				}
				catch { }
				return "INSTALL DIR";
			}
		}

		private string GetInstructions()
		{
			string gameFolder = Path.Combine(InstallDir, Mc1GameData.GameFolder);
			string cdFolder = Path.Combine(InstallDir, Mc1GameData.CdFolder);
			if (this.cboInstallLocation.SelectedIndex == GogEdition)
			{
				return $"The installer will now copy the CARPET.CD folder of Magic Carpet Plus from: '{this.txtPath.Text}' to: '{gameFolder}'.\n" +
					$"It will then extract the game files from its CD image ({Mc1GameData.GogImageName}) to: '{cdFolder}'.\n\n" +
					"Magic Carpet and Hidden Worlds both use these files.\n\n" +
					"Click [Extract Game Data] to continue";
			}
			return $"The installer will now copy the game files of the CD: '{this.txtPath.Text}' to: '{cdFolder}'.\n" +
				$"It will then set up the install folder: '{gameFolder}' (the tmaps and the levels of the games).\n\n" +
				"Hidden Worlds needs a Magic Carpet Plus CD (the CD with Hidden Worlds).\n\n" +
				"Click [Extract Game Data] to continue";
		}

		private bool InstallFromGog(string path)
		{
			if (Mc1GameData.FindGogCarpetCd(path) == null)
			{
				MessageBox.Show($"No Magic Carpet Plus CD image ({Mc1GameData.GogImageName}) found in '{path}' or its CARPET.CD folder",
					"Invalid Directory", MessageBoxButtons.OK, MessageBoxIcon.Exclamation);
				return false;
			}
			return Run(() => Mc1GameData.InstallFromGog(path, InstallDir));
		}

		private bool InstallFromCd(string path)
		{
			if (Mc1GameData.FindCdGameFolder(path) == null)
			{
				MessageBox.Show($"No Magic Carpet game files (a DATA or CARPET\\DATA folder) found in '{path}'",
					"Invalid Directory", MessageBoxButtons.OK, MessageBoxIcon.Exclamation);
				return false;
			}
			return Run(() => Mc1GameData.InstallFromCd(path, InstallDir));
		}

		// runs the install, false on an error unless the install is to continue anyway
		private bool Run(Func<int> install)
		{
			Cursor previous = Cursor.Current;
			try
			{
				Cursor.Current = Cursors.WaitCursor;
				int files = install();
				if (files == 0)
					throw new InvalidDataException("No game files were found to install");
				treeViewInstallProgress.Nodes["InstallPath"].Nodes["CARPETCD"].Checked = true;
				treeViewInstallProgress.Nodes["InstallPath"].Nodes["CDFiles"].Checked = true;
				return true;
			}
			catch (Exception ex)
			{
				return MessageBox.Show($"Error installing the Magic Carpet game data: {ex.Message}\nWould you like to continue the Installation?",
					"Game Data Error", MessageBoxButtons.YesNo, MessageBoxIcon.Error) == DialogResult.Yes;
			}
			finally
			{
				Cursor.Current = previous;
			}
		}

		private void SetGoGInstall()
		{
			this.txtPath.Text = @"C:\Program Files (x86)\GOG Galaxy\Games\Magic Carpet Plus";
			this.lblGameData.Text = "Magic Carpet Plus GOG directory:";
		}

		private void SetDosInstall()
		{
			this.txtPath.Text = @"D:\";
			this.lblGameData.Text = "Magic Carpet CD:";
		}

		private void UpdateTree()
		{
			this.treeViewInstallProgress.Nodes["InstallPath"].Text = InstallDir;
		}

		private void GameDataMc1Dialog_Shown(object sender, EventArgs e)
		{
			this.cboInstallLocation.SelectedIndexChanged += CboInstallLocation_SelectedIndexChanged;
			this.cboInstallLocation.SelectedIndex = GogEdition;
			UpdateTree();
			treeViewInstallProgress.ExpandAll();
		}

		private void GameDataMc1Dialog_FormClosing(object sender, FormClosingEventArgs e)
		{
			this.Shown -= GameDataMc1Dialog_Shown;
		}

		private void dialog_Load(object sender, EventArgs e)
		{
			Text = "[ProductName] Setup";
			base.Localize();
		}

		private void CboInstallLocation_SelectedIndexChanged(object sender, EventArgs e)
		{
			if (this.cboInstallLocation.SelectedIndex == GogEdition)
				SetGoGInstall();
			else if (this.cboInstallLocation.SelectedIndex == DosEdition)
				SetDosInstall();
		}

		private void btnRun_Click(object sender, EventArgs e)
		{
			_runClicked = true;
			SetControlsEnabled(false);

			bool success = false;
			if (this.cboInstallLocation.SelectedIndex == GogEdition)
				success = InstallFromGog(this.txtPath.Text);
			else if (this.cboInstallLocation.SelectedIndex == DosEdition)
				success = InstallFromCd(this.txtPath.Text);

			SetControlsEnabled(true);
			if (success)
				MessageBox.Show("Extraction was successful. Click [Next] to continue", "Success", MessageBoxButtons.OK);
		}

		private void SetControlsEnabled(bool enabled)
		{
			btnInfo.Enabled = enabled;
			btnBrowse.Enabled = enabled;
			btnRun.Enabled = enabled;
			btnNext.Enabled = enabled;
			txtPath.Enabled = enabled;
			cboInstallLocation.Enabled = enabled;
		}

		private void btnBrowse_Click(object sender, EventArgs e)
		{
			using (var fbd = new System.Windows.Forms.FolderBrowserDialog())
			{
				fbd.SelectedPath = this.txtPath.Text;
				if (fbd.ShowDialog() == DialogResult.OK && !string.IsNullOrWhiteSpace(fbd.SelectedPath))
					this.txtPath.Text = fbd.SelectedPath;
			}
		}

		private void btnNext_Click(object sender, EventArgs e)
		{
			// the game data of Magic Carpet is optional: Magic Carpet 2 runs without it
			if (_runClicked ||
				MessageBox.Show("Are you sure you want to skip Extracting the Magic Carpet Game Data? You cannot run Magic Carpet or Hidden Worlds without it.",
					"Warning", MessageBoxButtons.YesNo, MessageBoxIcon.Question) == DialogResult.Yes)
			{
				Shell.GoNext();
			}
		}

		private void Path_TextChanged(object sender, EventArgs e)
		{
			UpdateTree();
		}

		private void btnInfo_Click(object sender, EventArgs e)
		{
			MessageBox.Show(GetInstructions(), "Infomation", MessageBoxButtons.OK, MessageBoxIcon.Information);
		}
	}
}
