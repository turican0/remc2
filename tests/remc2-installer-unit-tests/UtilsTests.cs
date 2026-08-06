using NUnit.Framework;
using System.IO;

namespace remc2_installer_unit_tests
{
	public class Tests
	{
		[SetUp]
		public void Setup()
		{
		}

		[TestCase(@"C:\Temp")]
		public void DeleteFilesTest(string testDir)
		{
			Utils.DeleteFiles(new System.IO.DirectoryInfo(testDir), null, new[] { "SAVE" });

			Assert.Pass();
		}

		[TestCase(@"Resources\config.json", @"Resources\config-modified.json")]
		public void EditConfigFileTest(string beforePath, string afterPath)
		{
			if (!File.Exists(beforePath) || !File.Exists(afterPath))
				Assert.Fail();

			var expectedJson = File.ReadAllText(afterPath);

			var testOutputPath = @"Resources\test-output.json";
			Utils.SetHighResGraphics(beforePath, testOutputPath, true);
			Utils.SetFixedMenuGraphics(testOutputPath, testOutputPath, true);
			Utils.SetExtendedFonts(testOutputPath, testOutputPath, true);

			var actualJson = File.ReadAllText(testOutputPath);

			if (expectedJson.Length != actualJson.Length)
				Assert.Fail();

			for (int i = 0; i < actualJson.Length; i++)
			{
				if(expectedJson[i] != actualJson[i])
					Assert.Fail();
			}

			Assert.Pass();
		}
	}


}