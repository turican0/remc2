using NUnit.Framework;
using remc2_installer;
using System;
using System.IO;

namespace remc2_installer_unit_tests
{
	public class Mc1GameDataTests
	{
		// the default GOG Galaxy location of Magic Carpet Plus; the tests are skipped without it
		private const string GogPath = @"C:\Program Files (x86)\GOG Galaxy\Games\Magic Carpet Plus";

		private string m_TempDir;

		[SetUp]
		public void Setup()
		{
			m_TempDir = Path.Combine(Path.GetTempPath(), "remc2-mc1-test-" + Guid.NewGuid().ToString("N"));
		}

		[TearDown]
		public void TearDown()
		{
			if (Directory.Exists(m_TempDir))
				Directory.Delete(m_TempDir, true);
		}

		[Test]
		public void InstallFromGogTest()
		{
			if (Mc1GameData.FindGogCarpetCd(GogPath) == null)
				Assert.Ignore($"Magic Carpet Plus not installed in {GogPath}");

			int files = Mc1GameData.InstallFromGog(GogPath, m_TempDir);

			Assert.That(files, Is.GreaterThan(0));
			// the CD files the game loads, e.g. the fonts (remc1 Mc1Files.cpp)
			string cdFiles = Path.Combine(m_TempDir, Mc1GameData.CdFolder);
			Assert.That(File.Exists(Path.Combine(cdFiles, "DATA", "FONT0.DAT")), Is.True);
			Assert.That(File.Exists(Path.Combine(cdFiles, "DATA", "FONT0.TAB")), Is.True);
			// Hidden Worlds data on the same CD
			Assert.That(File.Exists(Path.Combine(cdFiles, "DATA", "BUILD1-0.DAT")), Is.True);
			// the installed game, without the CD image
			string gameFiles = Path.Combine(m_TempDir, Mc1GameData.GameFolder);
			Assert.That(Directory.Exists(Path.Combine(gameFiles, "DATA")), Is.True);
			Assert.That(File.Exists(Path.Combine(gameFiles, Mc1GameData.GogImageName)), Is.False);
		}

		[TestCase(true)]   // Magic Carpet Plus: the game in the CARPET folder of the CD
		[TestCase(false)]  // Magic Carpet: the game in the root of the CD
		public void InstallFromCdTest(bool carpetFolder)
		{
			// a small CD: the data of both games, levels and intro, read only as on a CD
			string cd = Path.Combine(m_TempDir, "cd");
			string cdGame = carpetFolder ? Path.Combine(cd, "CARPET") : cd;
			foreach (string file in new[] { @"DATA\FONT0.DAT", @"DATA\TMAPS0-0.DAT", @"DATA\TMAPS0-0.TAB", @"DATA\TMAPS1-0.DAT", @"DATA\TMAPS1-0.TAB",
				@"LEVELS\LEVELS.DAT", @"LEVELS\LEVELS.TAB", @"LEVELS\DDLEVELS.DAT", @"LEVELS\DDLEVELS.TAB", @"INTRO\INTRO.DAT" })
			{
				string path = Path.Combine(cdGame, file);
				Directory.CreateDirectory(Path.GetDirectoryName(path));
				File.WriteAllText(path, file);
				File.SetAttributes(path, FileAttributes.ReadOnly);
			}
			string install = Path.Combine(m_TempDir, "install");

			Assert.That(Mc1GameData.FindCdGameFolder(cd), Is.EqualTo(cdGame));
			int files = Mc1GameData.InstallFromCd(cd, install);

			Assert.That(files, Is.EqualTo(10));
			string cdFiles = Path.Combine(install, Mc1GameData.CdFolder);
			Assert.That(File.Exists(Path.Combine(cdFiles, "DATA", "FONT0.DAT")), Is.True);
			Assert.That(File.GetAttributes(Path.Combine(cdFiles, "DATA", "FONT0.DAT")).HasFlag(FileAttributes.ReadOnly), Is.False);
			// the install folder as the DOS install sets it up: the tmaps and the levels of both games
			string gameFolder = Path.Combine(install, Mc1GameData.GameFolder);
			foreach (string file in new[] { @"DATA\TMAPS0-0.DAT", @"DATA\TMAPS1-0.TAB", @"LEVELS\LEVELS.DAT", @"LEVELS\DDLEVELS.TAB" })
				Assert.That(File.Exists(Path.Combine(gameFolder, file)), Is.True, file);
			Assert.That(File.Exists(Path.Combine(gameFolder, "DATA", "FONT0.DAT")), Is.False);

			// the read only files of the test CD, for the TearDown
			foreach (string file in Directory.GetFiles(cd, "*", SearchOption.AllDirectories))
				File.SetAttributes(file, FileAttributes.Normal);
		}

		[Test]
		public void InstallFromCdRejectsFolderWithoutGameTest()
		{
			Directory.CreateDirectory(Path.Combine(m_TempDir, "empty"));
			Assert.That(Mc1GameData.FindCdGameFolder(Path.Combine(m_TempDir, "empty")), Is.Null);
			Assert.Throws<DirectoryNotFoundException>(() => Mc1GameData.InstallFromCd(Path.Combine(m_TempDir, "empty"), Path.Combine(m_TempDir, "install")));
		}

		[Test]
		public void ExtractRejectsNonIsoTest()
		{
			Directory.CreateDirectory(m_TempDir);
			string notIso = Path.Combine(m_TempDir, "not.iso");
			File.WriteAllBytes(notIso, new byte[40000]);

			Assert.Throws<InvalidDataException>(() => IsoExtractor.Extract(notIso, Path.Combine(m_TempDir, "out")));
		}
	}
}
