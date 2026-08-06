using System;
using System.IO;
using WixToolset.Dtf.WindowsInstaller;

public class CustomActions
{
	[CustomAction]
    public static ActionResult SetHighResGraphics(Session session)
    {
		bool enhancedTextures = false;

		try
		{
			if (!string.IsNullOrWhiteSpace(session["HIGHTEX"]) && session["HIGHTEX"].Equals("yes", System.StringComparison.InvariantCultureIgnoreCase))
			{
				enhancedTextures = true;
			}

			session.Log($"Setting Enhanced Textures to: {enhancedTextures}");
			string path = session["INSTALLDIR"];
			string configFilePath = Path.Combine(path, "config.json");
			session.Log($"Setting config.json file: {configFilePath}");

			if (File.Exists(configFilePath))
			{
				session.Log($"Updating config File: {configFilePath}");

				if (Utils.SetHighResGraphics(configFilePath, configFilePath, enhancedTextures))
				{
					session.Log($"Success updating config File: {configFilePath}");
					return ActionResult.Success;
				}

				return ActionResult.Failure;
			}
			else
			{
				return ActionResult.Failure;
			}
		}
		catch (Exception ex)
		{
			session.Log($"Error Setting enhanced textures: {ex.Message}");
			return ActionResult.Failure;
		}
    }

	[CustomAction]
	public static ActionResult SetFixedMenuGraphics(Session session)
	{
		bool fixedMenuGraphics = false;

		try
		{
			if (!string.IsNullOrWhiteSpace(session["FIXEDMENUGRAPHICS"]) && session["FIXEDMENUGRAPHICS"].Equals("yes", System.StringComparison.InvariantCultureIgnoreCase))
			{
				fixedMenuGraphics = true;
			}

			session.Log($"Setting fixed menu graphics to: {fixedMenuGraphics}");
			string path = session["INSTALLDIR"];
			string configFilePath = Path.Combine(path, "config.json");
			session.Log($"Setting config.json file: {configFilePath}");

			if (File.Exists(configFilePath))
			{
				session.Log($"Updating config File: {configFilePath}");

				if (Utils.SetFixedMenuGraphics(configFilePath, configFilePath, fixedMenuGraphics))
				{
					session.Log($"Success updating config File: {configFilePath}");
					return ActionResult.Success;
				}

				return ActionResult.Failure;
			}
			else
			{
				return ActionResult.Failure;
			}
		}
		catch (Exception ex)
		{
			session.Log($"Error Setting fixed menu graphics: {ex.Message}");
			return ActionResult.Failure;
		}
	}

	[CustomAction]
	public static ActionResult SetExtendedFonts(Session session)
	{
		bool extendedFonts = false;

		try
		{
			if (!string.IsNullOrWhiteSpace(session["EXTENDEDFONTS"]) && session["EXTENDEDFONTS"].Equals("yes", System.StringComparison.InvariantCultureIgnoreCase))
			{
				extendedFonts = true;
			}

			session.Log($"Setting extended fonts to: {extendedFonts}");
			string path = session["INSTALLDIR"];
			string configFilePath = Path.Combine(path, "config.json");
			session.Log($"Setting config.json file: {configFilePath}");

			if (File.Exists(configFilePath))
			{
				session.Log($"Updating config File: {configFilePath}");

				if (Utils.SetExtendedFonts(configFilePath, configFilePath, extendedFonts))
				{
					session.Log($"Success updating config File: {configFilePath}");
					return ActionResult.Success;
				}

				return ActionResult.Failure;
			}
			else
			{
				return ActionResult.Failure;
			}
		}
		catch (Exception ex)
		{
			session.Log($"Error Setting extended fonts: {ex.Message}");
			return ActionResult.Failure;
		}
	}
}

