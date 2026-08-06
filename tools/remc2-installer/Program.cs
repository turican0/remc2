using System;
using System.Windows.Forms;
using WixSharp;
using WixSharp.Forms;
using WixSharpSetup;

namespace remc2_installer
{
    internal class Program
    {
        static void Main()
        {
			var version = new Version(0, 0, 0, 0);
#if WIN64
            var project = new ManagedProject("Magic Carpet 2 HD x64",
#else
			var project = new ManagedProject("Magic Carpet 2 HD x86",
#endif
				new Dir(new Id("SHORTCUTDIR"), @"%ProgramMenu%\ReMC\Magic Carpet 2 HD"),
					new Dir(new Id("INSTALLDIR"), @"%ProgramFiles%\ReMC\Magic Carpet 2 HD",
						new DirPermission("Everyone", GenericPermission.All),
#if WIN64
							new WixSharp.File(new Id("PRODUCT_EXE"), @"..\x64\Release\remc2.exe", 
								new FileShortcut(new Id("PRODUCT_EXE_SHORTCUT"), "Magic Carpet 2 HD", "SHORTCUTDIR")
                                {
									IconFile = "Resources/app.ico"
                                })
#else
							new WixSharp.File(new Id("PRODUCT_EXE"), @"..\Release\remc2.exe", 
								new FileShortcut(new Id("PRODUCT_EXE_SHORTCUT"), "Magic Carpet 2 HD", "SHORTCUTDIR")
                                {
									IconFile = "Resources/app.ico"
                                })
#endif
								{
									Permissions = new[] {
										new FilePermission("Everyone", GenericPermission.All) { Execute = true },
										new FilePermission("Users", GenericPermission.All) { Execute = true },
										new FilePermission("AuthenticatedUser", GenericPermission.All) { Execute = true },
										new FilePermission("CREATOR OWNER", GenericPermission.All)  { Execute = true },
									}
                                },
#if WIN64
							new WixSharp.File(new Id("EDITOR_EXE"), @"..\x64\Release\remc2-editor.exe", 
								new FileShortcut(new Id("EDITOR_EXE_SHORTCUT"), "Editor", "SHORTCUTDIR")
								{
									IconFile = "Resources/editor.ico"
								})
#else
							new WixSharp.File(new Id("EDITOR_EXE"), @"..\Release\remc2-editor.exe", 
								new FileShortcut(new Id("EDITOR_EXE_SHORTCUT"), "Editor", "SHORTCUTDIR")
								{
									IconFile = "Resources/editor.ico"
								})
#endif
								{
									Permissions = new[] {
										new FilePermission("Everyone", GenericPermission.All) { Execute = true },
										new FilePermission("Users", GenericPermission.All) { Execute = true },
										new FilePermission("AuthenticatedUser", GenericPermission.All) { Execute = true },
										new FilePermission("CREATOR OWNER", GenericPermission.All)  { Execute = true },
									}
								},
#if WIN64
							new WixSharp.File(new Id("CONFIG_EXE"), @"..\x64\Release\remc2-configurator.exe",
								new FileShortcut(new Id("CONFIG_EXE_SHORTCUT"), "Configure", "SHORTCUTDIR")
								{
									IconFile = "Resources/config.ico"
								})
#else
							new WixSharp.File(new Id("CONFIG_EXE"), @"..\Release\remc2-configurator.exe", 
								new FileShortcut(new Id("CONFIG_EXE_SHORTCUT"), "Configure", "SHORTCUTDIR")
								{
									IconFile = "Resources/config.ico"
								})
#endif
							{
								Permissions = new[] {
										new FilePermission("Everyone", GenericPermission.All) { Execute = true },
										new FilePermission("Users", GenericPermission.All) { Execute = true },
										new FilePermission("AuthenticatedUser", GenericPermission.All) { Execute = true },
										new FilePermission("CREATOR OWNER", GenericPermission.All)  { Execute = true },
									}
							},
#if WIN64
								new File(new Id("CONFIG_INI"), @"..\x64\Release\config.json")
								{
									Permissions = new[] {
										new FilePermission("Everyone", GenericPermission.All) { ChangePermission = true },
										new FilePermission("Users", GenericPermission.All) { ChangePermission = true },
										new FilePermission("AuthenticatedUser", GenericPermission.All) { ChangePermission = true },
										new FilePermission("CREATOR OWNER", GenericPermission.All)  { ChangePermission = true },
									}
								},
#else
								new File(new Id("CONFIG_INI"), @"..\Release\config.json")
								{
									Permissions = new[] {
										new FilePermission("Everyone", GenericPermission.All) { ChangePermission = true },
										new FilePermission("Users", GenericPermission.All) { ChangePermission = true },
										new FilePermission("AuthenticatedUser", GenericPermission.All) { ChangePermission = true },
										new FilePermission("CREATOR OWNER", GenericPermission.All)  { ChangePermission = true },
									}
								},
#endif
#if WIN64
								new File(new Id("EDITOR_CONFIG_INI"), @"..\x64\Release\editor-config.json")
								{
									Permissions = new[] {
										new FilePermission("Everyone", GenericPermission.All) { ChangePermission = true },
										new FilePermission("Users", GenericPermission.All) { ChangePermission = true },
										new FilePermission("AuthenticatedUser", GenericPermission.All) { ChangePermission = true },
										new FilePermission("CREATOR OWNER", GenericPermission.All)  { ChangePermission = true },
									}
								},
#else
								new File(new Id("EDITOR_CONFIG_INI"), @"..\Release\editor-config.json")
								{
									Permissions = new[] {
										new FilePermission("Everyone", GenericPermission.All) { ChangePermission = true },
										new FilePermission("Users", GenericPermission.All) { ChangePermission = true },
										new FilePermission("AuthenticatedUser", GenericPermission.All) { ChangePermission = true },
										new FilePermission("CREATOR OWNER", GenericPermission.All)  { ChangePermission = true },
									}
								},
#endif
#if WIN64
								new File(new Id("BROTLICOMMON_DLL"), @"..\x64\Release\brotlicommon.dll"),
								new File(new Id("BROTLIDEC_DLL"), @"..\x64\Release\brotlidec.dll"),
								new File(new Id("BZ2_DLL"), @"..\x64\Release\bz2.dll"),
								new File(new Id("FREETYPE_DLL"), @"..\x64\Release\freetype.dll"),
								new File(new Id("LIBPNG16_DLL"), @"..\x64\Release\libpng16.dll"),
								new File(new Id("OGG_DLL"), @"..\x64\Release\ogg.dll"),
								new File(new Id("SDL2_DLL"), @"..\x64\Release\SDL2.dll"),
								new File(new Id("SDL2_IMAGE_DLL"), @"..\x64\Release\SDL2_image.dll"),
								new File(new Id("SDL2_MIXER_DLL"), @"..\x64\Release\SDL2_mixer.dll"),
								new File(new Id("SDL2_TTF_DLL"), @"..\x64\Release\SDL2_ttf.dll"),
								new File(new Id("VORBIS_DLL"), @"..\x64\Release\vorbis.dll"),
								new File(new Id("VORBIXFILE_DLL"), @"..\x64\Release\vorbisfile.dll"),
								new File(new Id("WAVPACKDLL_DLL"), @"..\x64\Release\wavpackdll.dll"),
								new File(new Id("ZLIB1_DLL"), @"..\x64\Release\zlib1.dll"),
								new File(new Id("WXBASE32_VC_CUSTOM_DLL"), @"..\x64\Release\wxbase32u_vc_x64_custom.dll"),
								new File(new Id("WXMSW32UD_CORE_VC_CUSTOM_DLL"), @"..\x64\Release\wxmsw32u_core_vc_x64_custom.dll"),
								new File(new Id("JPEG62_DLL"), @"..\x64\Release\jpeg62.dll"),
								new File(new Id("LIBLZMA_DLL"), @"..\x64\Release\liblzma.dll"),
								new File(new Id("PCRE2_16_DLL"), @"..\x64\Release\pcre2-16.dll"),
								new File(new Id("TIFF_DLL"), @"..\x64\Release\tiff.dll"),
								new File(new Id("MC_HD_LOGO_PNG"), @"..\x64\Release\mc2_hd_logo.png"),
#else
								new File(new Id("BROTLICOMMON_DLL"), @"..\Release\brotlicommon.dll"),
								new File(new Id("BROTLIDEC_DLL"), @"..\Release\brotlidec.dll"),
								new File(new Id("BZ2_DLL"), @"..\Release\bz2.dll"),
								new File(new Id("FREETYPE_DLL"), @"..\Release\freetype.dll"),
								new File(new Id("LIBPNG16_DLL"), @"..\Release\libpng16.dll"),
								new File(new Id("OGG_DLL"), @"..\Release\ogg.dll"),
								new File(new Id("SDL2_DLL"), @"..\Release\SDL2.dll"),
								new File(new Id("SDL2_IMAGE_DLL"), @"..\Release\SDL2_image.dll"),
								new File(new Id("SDL2_MIXER_DLL"), @"..\Release\SDL2_mixer.dll"),
								new File(new Id("SDL2_TTF_DLL"), @"..\Release\SDL2_ttf.dll"),
								new File(new Id("VORBIS_DLL"), @"..\Release\vorbis.dll"),
								new File(new Id("VORBIXFILE_DLL"), @"..\Release\vorbisfile.dll"),
								new File(new Id("WAVPACKDLL_DLL"), @"..\Release\wavpackdll.dll"),
								new File(new Id("ZLIB1_DLL"), @"..\Release\zlib1.dll"),
								new File(new Id("WXBASE32_VC_CUSTOM_DLL"), @"..\Release\wxbase32u_vc_custom.dll"),
								new File(new Id("WXMSW32UD_CORE_VC_CUSTOM_DLL"), @"..\Release\wxmsw32u_core_vc_custom.dll"),
								new File(new Id("JPEG62_DLL"), @"..\Release\jpeg62.dll"),
								new File(new Id("LIBLZMA_DLL"), @"..\Release\liblzma.dll"),
								new File(new Id("PCRE2_16_DLL"), @"..\Release\pcre2-16.dll"),
								new File(new Id("TIFF_DLL"), @"..\Release\tiff.dll"),
								new File(new Id("MC_HD_LOGO_PNG"), @"..\Release\mc2_hd_logo.png"),
#endif
								new Dir(new Id("KISS_INSTALLDIR"), @"kiss",
									new Files(@"..\Release\kiss\*.*")),
								new Dir(new Id("FONT_INSTALLDIR"), @"font",
									new Files(@"..\Release\font\*.*")),
								new Dir(new Id("LANGUAGE_INSTALLDIR"), @"CD_FILES\LANGUAGE",
									new Files(@"..\enhancedassets\languages\*.*")),
								new Dir(new Id("HIGHRESGRAPHICS_INSTALLDIR"), @"graphics\high-res",
									new Files(@"..\enhancedassets\graphics\high-res\*.*")
                                    {
                                        ComponentCondition = "HIGHTEX=\"yes\""
                                    }),
								new Dir(new Id("FIXEDMENUGRAPHICS_INSTALLDIR"), @"graphics\fixed\menu",
									new Files(@"..\enhancedassets\graphics\fixed\menu\*.*")
									{
										ComponentCondition = "FIXEDMENUGRAPHICS=\"yes\""
									}),
								new Dir(new Id("EXTENDEDFONTS_INSTALLDIR"), @"graphics\fixed\fonts",
									new Files(@"..\enhancedassets\graphics\fixed\fonts\*.*")
									{
										ComponentCondition = "EXTENDEDFONTS=\"yes\""
									}),
								new Dir(new Id("SPEECH_INSTALLDIR"), @"CD_FILES\SPEECH",
									new Files(@"..\enhancedassets\sound\speech\*.*")
									{
										ComponentCondition = "SPEECH=\"yes\""
									}),
								new Dir(new Id("MUSICOGG_INSTALLDIR"), @"music-ogg",
                                    new Files(@"..\enhancedassets\music-ogg\*.*")),
                                new Dir(new Id("EXTRACT_INSTALLDIR"), @"Extract",
                                    new File(new Id("DOSBOXEXTRACT_CONF"), @"Extract\dosboxExtract-GOG-CD.conf"),
                                    new File(new Id("XXCOPY16_EXE"), @"Extract\XXCOPY16.EXE"),
#if WIN64
									new File(new Id("VC_Redist_EXE"), @"Extract\VC_redist.x64.exe"))),
#else
									new File(new Id("VC_Redist_EXE"), @"Extract\VC_redist.x86.exe"))),
#endif
							new Property(new Id("HIGHTEX_PROPERTY"), "HIGHTEX", "yes"),
							new Property(new Id("FIXEDMENUGRAPHICS_PROPERTY"), "FIXEDMENUGRAPHICS", "yes"),
							new Property(new Id("EXTENDEDFONTS_PROPERTY"), "EXTENDEDFONTS", "yes"),
							new Property(new Id("SPEECH_PROPERTY"), "SPEECH", "yes"),
							new ManagedAction(new Id("MANAGED_ACTION_1"), CustomActions.SetHighResGraphics, Return.check, When.After, Step.InstallFinalize, Condition.NOT_Installed),
							new ManagedAction(new Id("MANAGED_ACTION_2"), CustomActions.SetFixedMenuGraphics, Return.check, When.After, Step.InstallFinalize, Condition.NOT_Installed),
							new ManagedAction(new Id("MANAGED_ACTION_3"), CustomActions.SetExtendedFonts, Return.check, When.After, Step.InstallFinalize, Condition.NOT_Installed));

#if WIN64
            project.Platform = Platform.x64;
#endif
            project.GUID = new Guid("d945f1c4-cbe4-445c-9674-07de64692857");
			project.Version = version;
#if WIN64
			project.DefaultRefAssemblies.Add(@"bin\x64\Release\net48\Newtonsoft.Json.dll");
#else
			project.DefaultRefAssemblies.Add(@"bin\x86\Release\net48\Newtonsoft.Json.dll");
#endif

			//project.ManagedUI = ManagedUI.Empty;    //no standard UI dialogs
			//project.ManagedUI = ManagedUI.Default;  //all standard UI dialogs

			//custom set of standard UI dialogs
			project.ManagedUI = new ManagedUI();
            project.ManagedUI.InstallDialogs.Add(Dialogs.Welcome)
                                            .Add(Dialogs.Licence)
                                            .Add(Dialogs.InstallDir)
                                            .Add<EnhancedDataDialog>()
                                            .Add(Dialogs.Progress)
                                            .Add<GameDataDialog>()
											.Add<RedistDialog>()
											.Add(Dialogs.Exit);

            project.ManagedUI.ModifyDialogs.Add(Dialogs.MaintenanceType)
                                           .Add<EnhancedDataDialog>()
                                           .Add(Dialogs.Progress)
										   .Add<GameDataDialog>()
										   .Add<RedistDialog>()
										   .Add(Dialogs.Exit);

            project.ControlPanelInfo.ProductIcon = @"Resources\app.ico";
            project.ControlPanelInfo.Comments = "Enhanced Edition of Magic Carpet 2 " + version.ToString();
            project.ControlPanelInfo.UrlInfoAbout = "https://github.com/thobbsinteractive/magic-carpet-2-hd";
            project.ControlPanelInfo.Contact = "Magic Carpet 2 HD";
            project.ControlPanelInfo.Manufacturer = "Magic Carpet 2 HD";

            project.LicenceFile = @"Resources\MagicCarpet2HD.licence.rtf";
            project.BackgroundImage = @"Resources\MagicCarpet2HD.dialog_bmp.png";
            project.BannerImage = @"Resources\MagicCarpet2HD.dialog_banner.png";
			ValidateAssemblyCompatibility();
            project.AfterInstall += OnAfterInstall;
            project.BuildMsi();
        }

        static void ValidateAssemblyCompatibility()
        {
            var assembly = System.Reflection.Assembly.GetExecutingAssembly();

            if (!assembly.ImageRuntimeVersion.StartsWith("v2."))
            {
                Console.WriteLine("Warning: assembly '{0}' is compiled for {1} runtime, which may not be compatible with the CLR version hosted by MSI. " +
                                  "The incompatibility is particularly possible for the EmbeddedUI scenarios. " +
                                   "The safest way to solve the problem is to compile the assembly for v3.5 Target Framework.",
                                   assembly.GetName().Name, assembly.ImageRuntimeVersion);
            }
        }

        public static void OnAfterInstall(SetupEventArgs e)
        {
            if (e.IsUninstalling)
            {
				if (MessageBox.Show("Would you like to keep your Save Game data?", "Keep Save files?", MessageBoxButtons.YesNo, MessageBoxIcon.Question, MessageBoxDefaultButton.Button1) == DialogResult.Yes)
				{
					var path = e.InstallDir;
					if (path.IsDirectory())
					{
						Utils.DeleteFiles(new System.IO.DirectoryInfo(path), null, new[] { "SAVE" });
					}
				}
				else
				{
					e.InstallDir.DeleteIfExists();
				}
            }
        }
    }
}