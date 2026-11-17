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
