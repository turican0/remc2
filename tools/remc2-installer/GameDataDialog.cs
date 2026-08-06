using System;
using System.Diagnostics;
using System.IO;
using System.Windows.Forms;
using WixSharp;
using WixSharp.UI.Forms;

namespace WixSharpSetup
{
    public partial class GameDataDialog : ManagedForm
    {
		private bool _runClicked;

        public GameDataDialog()
        {
            //NOTE: If this assembly is compiled for v4.0.30319 runtime, it may not be compatible with the MSI hosted CLR.
            //The incompatibility is particularly possible for the Embedded UI scenarios.
            //The safest way to avoid the problem is to compile the assembly for v3.5 Target Framework.remc2_installer
            InitializeComponent();

            this.Shown += GameDataDialog_Shown;
            this.FormClosing += GameDataDialog_FormClosing;
        }

        private string GetGoGInstructions()
        {
            string installPath = "INSTALL DIR";
            try
            {
                if (Runtime != null)
                    installPath = Runtime.InstallDir;
            }
            catch { };

            return $"The installer will now atempt to copy the NETHERW directory from: 'this.txtPath.Text' to: '{Path.Combine(installPath, "NETHERW")}'.\n" +
                $"It will then extract the CD Files to: '{Path.Combine(this.txtPath.Text, "CD_Files")}' and copy them to: '{Path.Combine(installPath, "CD_Files")}'.\n\n" +
                $"If this extract fails you can run '{Path.Combine(this.txtPath.Text, "Extract.bat")}' to extract the files and manually copy them to '{Path.Combine(installPath, "CD_Files")}'.\n\n" +
                "Click [Extract Game Data] to continue";
        }

        private string GetDosInstructions()
        {
            string installPath = "INSTALL DIR";
            try
            {
                if (Runtime != null)
                    installPath = Runtime.InstallDir;
            }
            catch { };

            return $"The installer will now attempt to install the game from the CD: '{this.txtPath.Text}' to: '{Path.Combine(installPath, @"NETHERW")}'.\n" +
                $"It will then attempt to extract the CD Files from: '{this.txtPath.Text}' and copy them to: '{Path.Combine(installPath, "CD_Files")}'.\n\n" +
				"Click [Extract Game Data] to continue";
        }

        private bool ValidateGoGGameDataLocation(string path)
        {
            bool valid = false;

            if (Directory.Exists(path))
            {
                string[] files = Directory.GetFiles(path);

                bool foundGameIns = false;

                foreach (string file in files)
                {
                    if (file.PathGetFileName().Equals("game.ins", StringComparison.InvariantCultureIgnoreCase))
                    {
                        foundGameIns = true;
                        break;
                    }
                }

                bool foundGameData = false;
                bool foundNetherw = false;

                string[] directories = Directory.GetDirectories(path);

                if (HasGameDir(directories))
                {
                    foreach (string directory in directories)
                    {
                        if (IsGameDir(directory))
                        {
                            foundGameData = true;
                            string[] gameDirectories = Directory.GetDirectories(directory);
                            if (HasNetherwDir(gameDirectories))
                            {
                                foreach (string gameDirectory in gameDirectories)
                                {
                                    if (IsNetherwDir(gameDirectory))
                                    {
                                        foundNetherw = ValidateNetherwContent(gameDirectory);
                                        break;
                                    }
                                }
                            }
                        }
                    }
                }

                valid = foundGameIns && foundGameData && foundNetherw;

                if (!foundGameIns)
                    MessageBox.Show($"game.ins not found", "No game.ins", MessageBoxButtons.OK, MessageBoxIcon.Exclamation);
                else if (!foundGameData)
                    MessageBox.Show($"GAME data directory not found", "Invalid Directory", MessageBoxButtons.OK, MessageBoxIcon.Exclamation);
            }
            else
            {
                MessageBox.Show($"This {path} directory does not exist", "Invalid Directory", MessageBoxButtons.OK, MessageBoxIcon.Exclamation);
            }

            return valid;
        }

        public bool ValidateNetherwContent(string gameDirectory)
        {
            string[] netherwDirectories = Directory.GetDirectories(gameDirectory);
            bool foundCData = HasCDataDir(netherwDirectories);
            bool foundCLevels = HasCLevelsDir(netherwDirectories);
            bool foundSound = HasSoundDir(netherwDirectories);

            if (!foundCData)
                MessageBox.Show($"CDATA directory not found", "No CDATA", MessageBoxButtons.OK, MessageBoxIcon.Exclamation);
            else if (!foundCLevels)
                MessageBox.Show($"CLEVELS directory not found", "No CLEVELS", MessageBoxButtons.OK, MessageBoxIcon.Exclamation);
            else if (!foundSound)
                MessageBox.Show($"SOUND directory not found", "No SOUND", MessageBoxButtons.OK, MessageBoxIcon.Exclamation);

            return foundCData && foundCLevels && foundSound;
        }

        public bool ValidateCDContent(string cdDirectory)
        {
            string[] cdDirectories = Directory.GetDirectories(cdDirectory);
            bool foundData = HasDataDir(cdDirectories);
            bool foundIntro = HasIntroDir(cdDirectories);
            bool foundLanguage = HasLanguageDir(cdDirectories);
            bool foundLevels = HasLevelsDir(cdDirectories);
            bool foundSound = HasSoundDir(cdDirectories);

            if (!foundData)
                MessageBox.Show($"DATA directory not found", "No DATA", MessageBoxButtons.OK, MessageBoxIcon.Exclamation);
            else if (!foundIntro)
                MessageBox.Show($"Intro directory not found", "No INTRO", MessageBoxButtons.OK, MessageBoxIcon.Exclamation);
            else if (!foundLanguage)
                MessageBox.Show($"Language directory not found", "No LANGUAGE", MessageBoxButtons.OK, MessageBoxIcon.Exclamation);
            else if (!foundLevels)
                MessageBox.Show($"Levels directory not found", "No LEVELS", MessageBoxButtons.OK, MessageBoxIcon.Exclamation);
            else if (!foundSound)
                MessageBox.Show($"SOUND directory not found", "No SOUND", MessageBoxButtons.OK, MessageBoxIcon.Exclamation);

            return foundData && foundIntro && foundLanguage && foundLevels && foundSound;
        }

        public bool CopyExtractBatchFile(string gamePath)
        {
            if (System.IO.File.Exists(Path.Combine(Runtime.InstallDir, @"Extract.bat")))
            {
                System.IO.File.Copy(Path.Combine(Runtime.InstallDir, @"Extract.bat"), Path.Combine(gamePath, @"Extract.bat"), true);
            }
            return true;
        }

        public bool MoveGameData(string gamePath)
        {
            try
            {
                if (Directory.Exists(gamePath))
                {
                    Utils.CopyDirectory(gamePath, Path.Combine(Runtime.InstallDir, @"NETHERW"), true);
                }
                else
                {
                    if (MessageBox.Show($"Error finding directory {gamePath}\nWould you like to continue the Installation?", "CD File Extraction Error", MessageBoxButtons.YesNo, MessageBoxIcon.Error) == DialogResult.Yes)
                    {
                        return true;
                    }
                    return false;
                }
				treeViewInstallProgress.Nodes["InstallPath"].Nodes["NETHERW"].Checked = true;
			}
            catch (Exception ex)
            {
                MessageBox.Show($"Error moving game data files: {ex.Message}");
                return false;
            }
            return true;
        }

		public bool InstallFromCD(string cdPath)
		{
			try
			{
				if (Directory.Exists(cdPath))
				{
					// Create the destination directory
					if (!Directory.Exists(Path.Combine(Runtime.InstallDir, @"NETHERW")))
						Directory.CreateDirectory(Path.Combine(Runtime.InstallDir, @"NETHERW"));

					if (!Directory.Exists(Path.Combine(Runtime.InstallDir, @"NETHERW/SOUND")))
						Directory.CreateDirectory(Path.Combine(Runtime.InstallDir, @"NETHERW/SOUND"));

					Utils.CopyDirectory(Path.Combine(cdPath, "SOUND"), Path.Combine(Runtime.InstallDir, @"NETHERW/SOUND"), true);
				}
				else
				{
					if (MessageBox.Show($"Error finding directory {cdPath}\nWould you like to continue the Installation?", "CD File Extraction Error", MessageBoxButtons.YesNo, MessageBoxIcon.Error) == DialogResult.Yes)
					{
						return true;
					}
					return false;
				}
				treeViewInstallProgress.Nodes["InstallPath"].Nodes["NETHERW"].Checked = true;
			}
			catch (Exception ex)
			{
				MessageBox.Show($"Error installing from CD: {ex.Message}");
				return false;
			}
			return true;
		}

        public bool CopyExtractFolder(string gamePath)
        {
            string path = Runtime.InstallDir;

            try
            {
                if (Directory.Exists(Path.Combine(path, "Extract")))
                {
					Utils.CopyDirectory(Path.Combine(path, "Extract"), Path.Combine(gamePath, "Extract"), true);
                }
                else
                {
                    MessageBox.Show($"Error copying extract folder not present in: {Path.Combine(path, "Extract")}");
                    return false;
                }
            }
            catch (Exception ex)
            {
                MessageBox.Show($"Error copying extract folder: {ex.Message}");
                return false;
            }

            return true;
        }

        private bool ExtractCDFiles(string gamePath)
        {
            try
            {
                if (Directory.Exists(Path.Combine(gamePath, "CD_Files")))
                {
                    Directory.Delete(Path.Combine(gamePath, "CD_Files"), true);
                }

                if (Directory.Exists(Path.Combine(gamePath, "Extract")))
                {
                    ProcessStartInfo processStartInfo = new ProcessStartInfo(Path.Combine(gamePath, @"DOSBOX\dosbox.exe"), "-conf \"Extract\\dosboxExtract-GOG-CD.conf\" -c exit");
                    processStartInfo.WorkingDirectory = gamePath;
                    processStartInfo.UseShellExecute = false;
                    processStartInfo.ErrorDialog = true;
                    Process.Start(processStartInfo).Wait();
                }
            }
            catch (Exception ex)
            {
                if (MessageBox.Show($"Error Extracting CD Files: {ex.Message}\nWould you like to continue the Installation?", "CD File Extraction Error", MessageBoxButtons.YesNo, MessageBoxIcon.Error) == DialogResult.Yes)
                {
                    return true;
                }
                return false;
            }
            return true;
        }

        private bool MoveCDFiles(string cdPath)
        {
            try
            {
                if (Directory.Exists(cdPath))
                {
                    Utils.CopyDirectory(cdPath, Path.Combine(Runtime.InstallDir, "CD_Files"), true);
                }
                else
                {
                    if (MessageBox.Show($"Error finding directory {cdPath}\nWould you like to continue the Installation?", "CD File Extraction Error", MessageBoxButtons.YesNo, MessageBoxIcon.Error) == DialogResult.Yes)
                    {
                        return true;
                    }
                    return false;
                }
				treeViewInstallProgress.Nodes["InstallPath"].Nodes["CDFiles"].Checked = true;
			}
            catch (Exception ex)
            {
                if (MessageBox.Show($"Error Moving CD Files: {ex.Message}\nWould you like to continue the Installation?", "CD File Extraction Error", MessageBoxButtons.YesNo, MessageBoxIcon.Error) == DialogResult.Yes)
                {
                    return true;
                }
                return false;
            }
			return true;
        }

        private string GetDirectoryName(string path)
        {
            DirectoryInfo directoryInfo = new DirectoryInfo(path);
            return directoryInfo.Name;
        }

        private bool HasGameDir(string[] directories)
        {
            foreach (string directory in directories)
            {
                if (IsGameDir(directory))
                {
                    return true;
                }
            }
            return false;
        }

        private bool IsGameDir(string directory)
        {
            if (GetDirectoryName(directory).Equals("game", StringComparison.InvariantCultureIgnoreCase))
            {
                return true;
            }
            return false;
        }

        private bool HasNetherwDir(string[] directories)
        {
            foreach (string directory in directories)
            {
                if (IsNetherwDir(directory))
                {
                    return true;
                }
            }
            return false;
        }

        private bool IsNetherwDir(string directory)
        {
            if (GetDirectoryName(directory).Equals("netherw", StringComparison.InvariantCultureIgnoreCase))
            {
                return true;
            }
            return false;
        }

        private bool HasCDataDir(string[] directories)
        {
            foreach (string directory in directories)
            {
                if (GetDirectoryName(directory).Equals("cdata", StringComparison.InvariantCultureIgnoreCase))
                {
                    return true;
                }
            }
            return false;
        }

        private bool HasDataDir(string[] directories)
        {
            foreach (string directory in directories)
            {
                if (GetDirectoryName(directory).Equals("data", StringComparison.InvariantCultureIgnoreCase))
                {
                    return true;
                }
            }
            return false;
        }

        private bool HasIntroDir(string[] directories)
        {
            foreach (string directory in directories)
            {
                if (GetDirectoryName(directory).Equals("intro", StringComparison.InvariantCultureIgnoreCase))
                {
                    return true;
                }
            }
            return false;
        }

        private bool HasLanguageDir(string[] directories)
        {
            foreach (string directory in directories)
            {
                if (GetDirectoryName(directory).Equals("language", StringComparison.InvariantCultureIgnoreCase))
                {
                    return true;
                }
            }
            return false;
        }

        private bool HasCLevelsDir(string[] directories)
        {
            foreach (string directory in directories)
            {
                if (GetDirectoryName(directory).Equals("clevels", StringComparison.InvariantCultureIgnoreCase))
                {
                    return true;
                }
            }
            return false;
        }

        private bool HasLevelsDir(string[] directories)
        {
            foreach (string directory in directories)
            {
                if (GetDirectoryName(directory).Equals("levels", StringComparison.InvariantCultureIgnoreCase))
                {
                    return true;
                }
            }
            return false;
        }

        private bool HasSoundDir(string[] directories)
        {
            foreach (string directory in directories)
            {
                if (GetDirectoryName(directory).Equals("sound", StringComparison.InvariantCultureIgnoreCase))
                {
                    return true;
                }
            }
            return false;
        }

        private void SetGoGInstall()
        {
            this.txtPath.Text = @"C:\Program Files (x86)\GOG Galaxy\Games\Magic Carpet 2";
            this.lblGameData.Text = @"Magic Carpet 2 GOG directory:";
        }

        private void SetDosInstall()
        {
            this.txtPath.Text = @"D:\";
            this.lblGameData.Text = "Magic Carpet 2 CD:";
        }

        private string GetInstructions()
        {
            if (this.cboInstallLocation.SelectedIndex == 0)
            {
                return this.GetGoGInstructions();
            }
            else if (this.cboInstallLocation.SelectedIndex == 1)
            {
                return this.GetDosInstructions();
            }
            return string.Empty;
        }

		private void UpdateTree()
		{
			string installPath = "INSTALL DIR";
			try
			{
				if (Runtime != null)
					installPath = Runtime.InstallDir;
			}
			catch { };

			this.treeViewInstallProgress.Nodes["InstallPath"].Text = installPath;
			
		}

        private void GameDataDialog_Shown(object sender, EventArgs e)
        {
			this.cboInstallLocation.SelectedIndexChanged += CboInstallLocation_SelectedIndexChanged;
			this.cboInstallLocation.SelectedIndex = 0;
			UpdateTree();
			treeViewInstallProgress.ExpandAll();
        }

        private void GameDataDialog_FormClosing(object sender, FormClosingEventArgs e)
        {
            this.Shown -= GameDataDialog_Shown;
        }

        private void dialog_Load(object sender, EventArgs e)
        {
            Text = "[ProductName] Setup";
            base.Localize();


        }

        private void CboInstallLocation_SelectedIndexChanged(object sender, EventArgs e)
        {
            if (this.cboInstallLocation.SelectedIndex == 0)
            {
                SetGoGInstall();
            }
            else if (this.cboInstallLocation.SelectedIndex == 1)
            {
                SetDosInstall();
            }
        }

        private void btnRun_Click(object sender, EventArgs e)
        {
			bool success = false;
			_runClicked = true;

			btnInfo.Enabled = false;
			btnBrowse.Enabled = false;
            btnRun.Enabled = false;
            btnNext.Enabled = false;
            txtPath.Enabled = false;
			cboInstallLocation.Enabled = true;

			if (this.cboInstallLocation.SelectedIndex == 0)
            {
                //GOG
                if (ValidateGoGGameDataLocation(this.txtPath.Text) &&
                CopyExtractBatchFile(this.txtPath.Text) &&
                MoveGameData(Path.Combine(this.txtPath.Text, @"GAME\NETHERW")) &&
                CopyExtractFolder(this.txtPath.Text) &&
                ExtractCDFiles(this.txtPath.Text) &&
                MoveCDFiles(Path.Combine(this.txtPath.Text, "CD_Files")))
                {
					success = true;
				}
            }
            else if (this.cboInstallLocation.SelectedIndex == 1)
            {
                //DOS
                if (ValidateCDContent(this.txtPath.Text) &&
				InstallFromCD(Path.Combine(this.txtPath.Text)) &&
                MoveCDFiles(this.txtPath.Text))
                {
					success = true;
				}
            }

			btnInfo.Enabled = true;

			if (success)
			{
				MessageBox.Show("Extraction was successful. Click [Next] to finish", "Success", MessageBoxButtons.OK);
				btnNext.Enabled = true;
			}

			btnBrowse.Enabled = true;
			btnRun.Enabled = true;
			btnNext.Enabled = true;
			txtPath.Enabled = true;
			cboInstallLocation.Enabled = true;
		}

        private void btnBrowse_Click(object sender, EventArgs e)
        {
            using (var fbd = new System.Windows.Forms.FolderBrowserDialog())
            {
                fbd.SelectedPath = this.txtPath.Text;
                DialogResult result = fbd.ShowDialog();

                if (result == DialogResult.OK && !string.IsNullOrWhiteSpace(fbd.SelectedPath))
                {
                    this.txtPath.Text = fbd.SelectedPath;
                }
            }
        }

        private void btnNext_Click(object sender, EventArgs e)
        {
			if (!_runClicked)
			{
				if (MessageBox.Show("Are you sure you want to skip Extracting the Game Data? You cannot run Magic Carpet 2 HD without it!", "Warning", MessageBoxButtons.YesNo, MessageBoxIcon.Question) == DialogResult.Yes)
					Shell.GoNext();
			}
			else
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