using System.Collections.Generic;

namespace remc2_installer
{
	public class Setting
	{
		public string name { get; set; }
		public string version { get; set; }
		public bool isActive { get; set; }
		public string launchArguments { get; set; }
		public Paths paths { get; set; }
		public Multiplayer multiplayer { get; set; }
		public Sound sound { get; set; }
		public Graphics graphics { get; set; }
		public Game game { get; set; }
		public Controls controls { get; set; }
	}

	public class Multiplayer
	{
		public int serverPort { get; set; }
		public int serverClientPort { get; set; }
		public int clientPort { get; set; }
		public int clientServerPort { get; set; }
		public string clientServerIp { get; set; }
	}

	public class Paths
	{
		public string[] description { get; set; }
		public string gameFolder { get; set; }
		public string cdFolder { get; set; }
	}

	public class Sound
	{
		public string[] description { get; set; }
		public bool hqSound { get; set; }
		public bool oggMusic { get; set; }
		public string oggFolder { get; set; }
		public bool oggMusicAlternative { get; set; }
		public bool fixSpeedSound { get; set; }
		public int maxSimultaniousSounds { get; set; }
		public string speechFolder { get; set; }
		public bool autoShowObjectivesForForeignLanguages { get; set; }
	}

	public class Graphics
	{
		public string[] description { get; set; }
		public int displayIndex { get; set; }
		public int windowResWidth { get; set; }
		public int windowResHeight { get; set; }
		public bool maintainAspectRatio { get; set; }
		public bool startWindowed { get; set; }
		public GameDetail gameDetail { get; set; }
		public Threading threading { get; set; }
	}

	public class GameDetail
	{
		public string[] description { get; set; }
		public int gameResWidth { get; set; }
		public int gameResHeight { get; set; }
		public int gameUiScale { get; set; }
		public bool useHighResGraphics { get; set; }
		public string highResGraphicsFolder { get; set; }
		public bool useFixedMenuGraphics { get; set; }
		public string fixedMenuGraphicsFolder { get; set; }
		public bool useExtendedFonts { get; set; }
		public string extendedFontsFolder { get; set; }
		public bool sky { get; set; }
		public bool reflections { get; set; }
		public bool dynamicLighting { get; set; }
		public int viewDistanceScale { get; set; }
	}

	public class Threading
	{
		public string[] description { get; set; }
		public bool isActive { get; set; }
		public float sizePercentToThreadRender { get; set; }
		public int numberOfRenderThreads { get; set; }
		public bool assignToSpecificCores { get; set; }
	}

	public class Game
	{
		public string[] description { get; set; }
		public int maxGameFps { get; set; }
	}

	public class Controls
	{
		public List<Mouse> mouse { get; set; }
		public List<Keyboard> keyboard { get; set; }
		public List<GamePad> gamePad { get; set; }
	}

	public class Mouse
	{
		public string[] description { get; set; }
		public string name { get; set; }
		public bool isActive { get; set; }
		public bool invertYAxis { get; set; }
		public bool invertXAxis { get; set; }
		public float mouseScaleX { get; set; }
		public float mouseScaleY { get; set; }
		public bool disableLRButtonsMenuOpen {  get; set; }
		public int spellLeft { get; set; }
		public int spellRight { get; set; }
		public int map { get; set; }
		public int spellMenu { get; set; }
		public int spellMenuMark { get; set; }
	}

	public class Keyboard
	{
		public string[] description { get; set; }
		public string name { get; set; }
		public bool isActive { get; set; }
		public string forward { get; set; }
		public string backwards { get; set; }
		public string left { get; set; }
		public string right { get; set; }
		public string map { get; set; }
		public string spellMenu { get; set; }
		public string spellMenuMark { get; set; }
	}

	public class GamePad
	{
		public string name { get; set; }
		public bool isActive { get; set; }
		public int controllerId { get; set; }
		public int buttonMiniMap { get; set; }
		public int buttonSpell { get; set; }
		public int buttonPauseMenu { get; set; }
		public int buttonEsc { get; set; }
		public int buttonFireL { get; set; }
		public int buttonFireR { get; set; }
		public int buttonMenuSelect { get; set; }
		public int triggerDeadZone { get; set; }
		public bool hapticEnabled { get; set; }
		public int hapticMaxGain { get; set; }
		public int hatNav { get; set; }
		public int hatMov { get; set; }
		public bool hatNavInv { get; set; }
		public bool hatMovInv { get; set; }
		public int axisLong { get; set; }
		public int axisLongDeadZone { get; set; }
		public int axisLongNavDeadZone { get; set; }
		public bool axisLongInv { get; set; }
		public int axisTrans { get; set; }
		public bool axisTransInv { get; set; }
		public int axisTransDeadZone { get; set; }
		public int axisTransNavDeadZone { get; set; }
		public int axisNavNs { get; set; }
		public bool axisNavNsInv { get; set; }
		public int axisNavEw { get; set; }
		public bool axisNavEwInv { get; set; }
		public int axisFireR { get; set; }
		public int axisFireL { get; set; }
		public int axisYaw { get; set; }
		public bool axisYawInv { get; set; }
		public int axisYawDeadZone { get; set; }
		public AxisSensitivity axisYawSensitivity { get; set; }
		public int axisPitch { get; set; }
		public bool axisPitchInv { get; set; }
		public int axisPitchDeadZone { get; set; }
		public AxisSensitivity axisPitchSensitivity { get; set; }
	}

	public class AxisSensitivity
	{
		public List<Zone> zones { get; set; }
	}

	public class Zone
	{
		public int start { get; set; }
		public int end { get; set; }
		public double factor { get; set; }
	}

	public class Config
	{
		public List<Setting> settings { get; set; }
	}
};
