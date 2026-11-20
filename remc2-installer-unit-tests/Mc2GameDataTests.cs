using NUnit.Framework;
using remc2_installer;
using System;
using System.IO;

namespace remc2_installer_unit_tests
{
	public class Mc2GameDataTests
	{
		// the default GOG Galaxy location of Magic Carpet 2; the tests are skipped without it
		private const string GogPath = @"C:\Program Files (x86)\GOG Galaxy\Games\Magic Carpet 2";

		private string m_TempDir;

		[SetUp]
		public void Setup()
		{
			m_TempDir = Path.Combine(Path.GetTempPath(), "remc2-mc2-test-" + Guid.NewGuid().ToString("N"));
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
			if (Mc2GameData.FindGogImage(GogPath) == null)
				Assert.Ignore($"Magic Carpet 2 not installed in {GogPath}");

			// the speech the installer puts in CD_Files before the extraction stays
			string speech = Path.Combine(m_TempDir, Mc2GameData.CdFolder, "SPEECH", "TRACK01.wav");
			Directory.CreateDirectory(Path.GetDirectoryName(speech));
			File.WriteAllText(speech, "speech");

			int files = Mc2GameData.InstallFromGog(GogPath, m_TempDir);

			Assert.That(files, Is.GreaterThan(0));
			// the CD folders the game reads (remc2-configurator GameFilesCheck)
			string cdFiles = Path.Combine(m_TempDir, Mc2GameData.CdFolder);
			foreach (string folder in new[] { "DATA", "INTRO", "LANGUAGE", "LEVELS", "SOUND" })
				Assert.That(Directory.Exists(Path.Combine(cdFiles, folder)), Is.True, folder);
			Assert.That(File.Exists(Path.Combine(cdFiles, "LEVELS", "LEVELS.DAT")), Is.True);
			Assert.That(File.ReadAllText(speech), Is.EqualTo("speech"));
			// the installed game
			string gameFiles = Path.Combine(m_TempDir, Mc2GameData.GameFolder);
			Assert.That(Directory.Exists(Path.Combine(gameFiles, "CDATA")), Is.True);
		}

		[Test]
		public void InstallFromGogRejectsFolderWithoutImageTest()
		{
			Directory.CreateDirectory(m_TempDir);
			Assert.That(Mc2GameData.FindGogImage(m_TempDir), Is.Null);
			Assert.Throws<FileNotFoundException>(() => Mc2GameData.InstallFromGog(m_TempDir, Path.Combine(m_TempDir, "install")));
		}
	}
}
