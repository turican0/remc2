using System;
using System.IO;

namespace remc2_installer
{
	/// <summary>
	/// The Magic Carpet / Hidden Worlds game data, in the folders of the config ("games" MagicCarpet and HiddenWorlds paths):
	/// MC1\CARPET.CD the installed game (gameFolder), MC1\CD_Files the files of the CD (cdFolder).
	/// </summary>
	public static class Mc1GameData
	{
		public static readonly string GameFolder = Path.Combine("MC1", "CARPET.CD");
		public static readonly string CdFolder = Path.Combine("MC1", "CD_Files");

		// the CD image of the GOG Magic Carpet Plus, in its CARPET.CD folder
		public const string GogImageName = "game.gog";
		// the folder of the CD with the game (Magic Carpet and Hidden Worlds); its contents are the CD files of remc1
		public const string CdGameFolder = "CARPET";

		/// <summary>
		/// The CARPET.CD folder of a GOG Magic Carpet Plus install: the folder given, or its CARPET.CD sub folder; null if neither has the CD image.
		/// </summary>
		public static string FindGogCarpetCd(string path)
		{
			if (string.IsNullOrEmpty(path))
				return null;
			if (File.Exists(Path.Combine(path, GogImageName)))
				return path;
			string carpetCd = Path.Combine(path, "CARPET.CD");
			return File.Exists(Path.Combine(carpetCd, GogImageName)) ? carpetCd : null;
		}

		/// <summary>
		/// Copies the installed game (without the CD image) to MC1\CARPET.CD and extracts the CARPET folder of the CD image to MC1\CD_Files of the install directory.
		/// </summary>
		/// <param name="gogPath">The GOG Magic Carpet Plus folder or its CARPET.CD folder.</param>
		/// <returns>The number of files extracted from the CD image.</returns>
		public static int InstallFromGog(string gogPath, string installDir)
		{
			string carpetCd = FindGogCarpetCd(gogPath)
				?? throw new FileNotFoundException($"No Magic Carpet Plus CD image ({GogImageName}) found in '{gogPath}'");

			CopyGameFiles(carpetCd, Path.Combine(installDir, GameFolder));

			string cdFiles = Path.Combine(installDir, CdFolder);
			if (Directory.Exists(cdFiles))
				Directory.Delete(cdFiles, true);
			return IsoExtractor.Extract(Path.Combine(carpetCd, GogImageName), cdFiles, CdGameFolder);
		}

		// copies the folder recursively, leaving out the CD image
		private static void CopyGameFiles(string sourceDir, string destinationDir)
		{
			Directory.CreateDirectory(destinationDir);
			foreach (var file in new DirectoryInfo(sourceDir).GetFiles())
			{
				if (!file.Name.Equals(GogImageName, StringComparison.OrdinalIgnoreCase))
					file.CopyTo(Path.Combine(destinationDir, file.Name), true);
			}
			foreach (var dir in new DirectoryInfo(sourceDir).GetDirectories())
				CopyGameFiles(dir.FullName, Path.Combine(destinationDir, dir.Name));
		}
	}
}
