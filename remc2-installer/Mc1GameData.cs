using System;
using System.IO;

namespace remc2_installer
{
	/// <summary>
	/// The Magic Carpet / Hidden Worlds game data, in the folders of the config ("paths" MagicCarpet and HiddenWorlds):
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

		/// <summary>
		/// The folder with the game files on a DOS CD (or a copy of it): its CARPET sub folder (Magic Carpet Plus), else the folder
		/// itself when it has the DATA folder (Magic Carpet); null if neither.
		/// </summary>
		public static string FindCdGameFolder(string cdPath)
		{
			if (string.IsNullOrEmpty(cdPath) || !Directory.Exists(cdPath))
				return null;
			string carpet = FindSubdirectory(cdPath, CdGameFolder);
			if (carpet != null && FindSubdirectory(carpet, "DATA") != null)
				return carpet;
			return FindSubdirectory(cdPath, "DATA") != null ? cdPath : null;
		}

		/// <summary>
		/// Copies the game files of a DOS CD to MC1\CD_Files and sets up MC1\CARPET.CD as the DOS install does (its DATA and LEVELS
		/// folders with the tmaps and the levels of Magic Carpet and, when the CD has them, of Hidden Worlds).
		/// </summary>
		/// <param name="cdPath">The CD drive or a copy of the CD.</param>
		/// <returns>The number of files copied from the CD.</returns>
		public static int InstallFromCd(string cdPath, string installDir)
		{
			string cdGame = FindCdGameFolder(cdPath)
				?? throw new DirectoryNotFoundException($"No Magic Carpet game files (DATA folder) found in '{cdPath}'");

			string cdFiles = Path.Combine(installDir, CdFolder);
			if (Directory.Exists(cdFiles))
				Directory.Delete(cdFiles, true);
			int files = CopyDirectory(cdGame, cdFiles);

			SetupInstallFolder(cdFiles, Path.Combine(installDir, GameFolder));
			return files;
		}

		// the files the DOS install puts in C:\CARPET.CD (remc1 SetupInstallFiles_3F1D0_3F510), from the CD files
		private static void SetupInstallFolder(string cdFiles, string gameFolder)
		{
			var installFiles = new[]
			{
				("DATA", "TMAPS0-0"), ("DATA", "TMAPS1-0"),   // Magic Carpet, Hidden Worlds
				("LEVELS", "LEVELS"), ("LEVELS", "DDLEVELS"),
			};
			foreach (var (folder, name) in installFiles)
			{
				string source = FindSubdirectory(cdFiles, folder);
				string target = Path.Combine(gameFolder, folder);
				Directory.CreateDirectory(target);
				if (source == null)
					continue;
				foreach (string extension in new[] { ".DAT", ".TAB" })
				{
					string file = Path.Combine(source, name + extension);
					if (File.Exists(file))
						File.Copy(file, Path.Combine(target, name + extension), true);
				}
			}
		}

		// the sub folder with the name (case insensitive), null if there is none
		private static string FindSubdirectory(string path, string name)
		{
			foreach (var dir in new DirectoryInfo(path).GetDirectories())
				if (dir.Name.Equals(name, StringComparison.OrdinalIgnoreCase))
					return dir.FullName;
			return null;
		}

		// copies the folder recursively (files from a CD are read only: the copies are not), returns the number of files
		private static int CopyDirectory(string sourceDir, string destinationDir)
		{
			Directory.CreateDirectory(destinationDir);
			int files = 0;
			foreach (var file in new DirectoryInfo(sourceDir).GetFiles())
			{
				string target = Path.Combine(destinationDir, file.Name);
				file.CopyTo(target, true);
				File.SetAttributes(target, File.GetAttributes(target) & ~FileAttributes.ReadOnly);
				files++;
			}
			foreach (var dir in new DirectoryInfo(sourceDir).GetDirectories())
				files += CopyDirectory(dir.FullName, Path.Combine(destinationDir, dir.Name));
			return files;
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
