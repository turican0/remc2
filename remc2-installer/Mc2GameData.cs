using System;
using System.IO;

namespace remc2_installer
{
	/// <summary>
	/// The Magic Carpet 2 game data, in the folders of the config ("paths" MagicCarpet2):
	/// MC2\NETHERW the installed game (gameFolder), MC2\CD_Files the files of the CD (cdFolder).
	/// </summary>
	public static class Mc2GameData
	{
		public static readonly string GameFolder = Path.Combine("MC2", "NETHERW");
		public static readonly string CdFolder = Path.Combine("MC2", "CD_Files");

		// the CD image of the GOG Magic Carpet 2 (game.ins is its cue sheet: the data track, then the audio tracks)
		public const string GogImageName = "game.gog";
		// the installed game of the GOG Magic Carpet 2
		public static readonly string GogGameFolder = Path.Combine("GAME", "NETHERW");

		/// <summary>
		/// The CD image of a GOG Magic Carpet 2 install, null if the folder has none.
		/// </summary>
		public static string FindGogImage(string gogPath)
		{
			if (string.IsNullOrEmpty(gogPath))
				return null;
			string image = Path.Combine(gogPath, GogImageName);
			return File.Exists(image) ? image : null;
		}

		/// <summary>
		/// Copies the installed game (GAME\NETHERW) to MC2\NETHERW and extracts the data track of the CD image to MC2\CD_Files of the
		/// install directory. Files already in MC2\CD_Files that are not on the CD stay (e.g. the SPEECH the installer puts there).
		/// </summary>
		/// <param name="gogPath">The GOG Magic Carpet 2 folder.</param>
		/// <returns>The number of files extracted from the CD image.</returns>
		public static int InstallFromGog(string gogPath, string installDir)
		{
			string image = FindGogImage(gogPath)
				?? throw new FileNotFoundException($"No Magic Carpet 2 CD image ({GogImageName}) found in '{gogPath}'");
			string gogGame = Path.Combine(gogPath, GogGameFolder);
			if (!Directory.Exists(gogGame))
				throw new DirectoryNotFoundException($"No Magic Carpet 2 game folder ({GogGameFolder}) found in '{gogPath}'");

			Utils.CopyDirectory(gogGame, Path.Combine(installDir, GameFolder), true);
			return ExtractGogImage(gogPath, installDir);
		}

		/// <summary>
		/// Extracts the data track of the CD image of a GOG Magic Carpet 2 install to MC2\CD_Files of the install directory.
		/// </summary>
		/// <returns>The number of files extracted.</returns>
		public static int ExtractGogImage(string gogPath, string installDir)
		{
			string image = FindGogImage(gogPath)
				?? throw new FileNotFoundException($"No Magic Carpet 2 CD image ({GogImageName}) found in '{gogPath}'");
			return IsoExtractor.Extract(image, Path.Combine(installDir, CdFolder));
		}
	}
}
