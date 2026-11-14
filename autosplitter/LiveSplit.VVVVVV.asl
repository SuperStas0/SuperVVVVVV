state("VVVVVV", "unknown") {
	// Default state
}

state("VVVVVV", "split") {
	// Modded build: values are read from its SplitData struct in update
}

state("VVVVVV", "v2.4.4") {
	// Game time variables
	int gametimeFrames : "VVVVVV.exe", 0x410BA0; // game.frames
	int gametimeSeconds : "VVVVVV.exe", 0x410BA4; // game.seconds
	int gametimeMinutes : "VVVVVV.exe", 0x410BA8; // game.minutes
	int gametimeHours : "VVVVVV.exe", 0x410BAC; // game.hours

	// Variables for starting the timer
	bool fadetomode : "VVVVVV.exe", 0x225A34; // see README at https://github.com/KSSBrawl/vvvvvv-ida-includes
	int gotomode : "VVVVVV.exe", 0x225A38; // see above
	int timetrialcountdown : "VVVVVV.exe", 0x410D18; // game.timetrialcountdown

	// Variables for stopping the timer on Game Complete
	int saveFrames : "VVVVVV.exe", 0x410BD0; // game.saveframes
	int saveSeconds : "VVVVVV.exe", 0x410BD4; // game.saveseconds

	// Variables for splitting
	bool finalStretch : "VVVVVV.exe", 0x4136C9; // map.finalstretch
	int gamestate : "VVVVVV.exe", 0x410B50; // game.state
	string255 firstTextLineSmall : "VVVVVV.exe", 0x233980, 0x0; // script.txt.Myfirst_ (script = scriptclass)
	string255 firstTextLineLarge : "VVVVVV.exe", 0x233980, 0x0, 0x0; // script.txt.Myfirst_
	int teleport_to_x : "VVVVVV.exe", 0x410C00; // game.teleport_to_x
	int teleport_to_y : "VVVVVV.exe", 0x410C04; // game.teleport_to_y
	byte100 collect : "VVVVVV.exe", 0x221790; // obj.collect (obj = entityclass)
	bool noDeathMode : "VVVVVV.exe", 0x410CDC; // game.nodeathmode
	int swnRank : "VVVVVV.exe", 0x410CC4; // game.swnrank

	// Time trial variables for calculating V rank
	// Why not just read the timetrialrank variable? In short, because of race conditions.
	// We can't reliably read from any variables which are affected by gamestate 82 while also listening out for gamestate 82.
	int deathCount : "VVVVVV.exe", 0x410B94; // game.deathcount
	int timeTrialShinyTarget : "VVVVVV.exe", 0x410D1C; // game.timetrialshinytarget
	int timeTrialPar : "VVVVVV.exe", 0x410D24; // game.timetrialpar

	// Variables for resetting
	int menustate : "VVVVVV.exe", 0x410B5C; // game.gamestate
	bool ingame_titlemode : "VVVVVV.exe", 0x411566; // game.ingame_titlemode
}

state("VVVVVV", "v2.4.3") {
	// Game time variables
	int gametimeFrames : "VVVVVV.exe", 0x40FBA0;
	int gametimeSeconds : "VVVVVV.exe", 0x40FBA4;
	int gametimeMinutes : "VVVVVV.exe", 0x40FBA8;
	int gametimeHours : "VVVVVV.exe", 0x40FBAC;

	// Variables for starting the timer
	bool fadetomode : "VVVVVV.exe", 0x224A30;
	int gotomode : "VVVVVV.exe", 0x224A38;
	int timetrialcountdown : "VVVVVV.exe", 0x40FD18;

	// Variables for splitting
	bool finalStretch : "VVVVVV.exe", 0x4126C9;
	int gamestate : "VVVVVV.exe", 0x40FB50; // actually called state in source
	string255 firstTextLineSmall : "VVVVVV.exe", 0x232980, 0x0;
	string255 firstTextLineLarge : "VVVVVV.exe", 0x232980, 0x0, 0x0;
	int teleport_to_x : "VVVVVV.exe", 0x40FC00;
	int teleport_to_y : "VVVVVV.exe", 0x40FC04;
	byte100 collect : "VVVVVV.exe", 0x220790;

	// Variables for resetting
	int menustate : "VVVVVV.exe", 0x40FB5C; // actually called gamestate in source
	bool ingame_titlemode : "VVVVVV.exe", 0x410566;
}

state("VVVVVV", "v2.4.2") {
	// Game time variables
	int gametimeFrames : "VVVVVV.exe", 0x40FB58;
	int gametimeSeconds : "VVVVVV.exe", 0x40FB5C;
	int gametimeMinutes : "VVVVVV.exe", 0x40FB60;
	int gametimeHours : "VVVVVV.exe", 0x40FB64;

	// Variables for starting the timer
	bool fadetomode : "VVVVVV.exe", 0x2249F0;
	int gotomode : "VVVVVV.exe", 0x2249F8;
	int timetrialcountdown : "VVVVVV.exe", 0x40FCCC;

	// Variables for splitting
	bool finalStretch : "VVVVVV.exe", 0x412619;
	int gamestate : "VVVVVV.exe", 0x40FB08; // actually called state in source
	string255 firstTextLineSmall : "VVVVVV.exe", 0x232938, 0x0;
	string255 firstTextLineLarge : "VVVVVV.exe", 0x232938, 0x0, 0x0;
	int teleport_to_x : "VVVVVV.exe", 0x40FBB8;
	int teleport_to_y : "VVVVVV.exe", 0x40FBBC;
	byte100 collect : "VVVVVV.exe", 0x220760;

	// Variables for resetting
	int menustate : "VVVVVV.exe", 0x40FB14; // actually called gamestate in source
	bool ingame_titlemode : "VVVVVV.exe", 0x41051A;
}

state("VVVVVV", "v2.4.1") {
	// Game time variables
	int gametimeFrames : "VVVVVV.exe", 0x41A928;
	int gametimeSeconds : "VVVVVV.exe", 0x41A92C;
	int gametimeMinutes : "VVVVVV.exe", 0x41A930;
	int gametimeHours : "VVVVVV.exe", 0x41A934;

	// Variables for starting the timer
	bool fadetomode : "VVVVVV.exe", 0x22F7BC;
	int gotomode : "VVVVVV.exe", 0x22F7C4;
	int timetrialcountdown : "VVVVVV.exe", 0x41AA9C;

	// Variables for splitting
	bool finalStretch : "VVVVVV.exe", 0x41D449;
	int gamestate : "VVVVVV.exe", 0x41A8D8; // actually called state in source
	string255 firstTextLineSmall : "VVVVVV.exe", 0x23D708, 0x0;
	string255 firstTextLineLarge : "VVVVVV.exe", 0x23D708, 0x0, 0x0;
	int teleport_to_x : "VVVVVV.exe", 0x41A988;
	int teleport_to_y : "VVVVVV.exe", 0x41A98C;
	byte100 collect : "VVVVVV.exe", 0x22B760;

	// Variables for resetting
	int menustate : "VVVVVV.exe", 0x41A8E4; // actually called gamestate in source
	bool ingame_titlemode : "VVVVVV.exe", 0x41B2EA;
}

state("VVVVVV", "v2.4") {
	// Game time variables
	int gametimeFrames : "VVVVVV.exe", 0x416800;
	int gametimeSeconds : "VVVVVV.exe", 0x416804;
	int gametimeMinutes : "VVVVVV.exe", 0x416808;
	int gametimeHours : "VVVVVV.exe", 0x41680C;

	// Variables for starting the timer
	bool fadetomode : "VVVVVV.exe", 0x22B6D4;
	int gotomode : "VVVVVV.exe", 0x22B6DC;
	int timetrialcountdown : "VVVVVV.exe", 0x416964;

	// Variables for splitting
	bool finalStretch : "VVVVVV.exe", 0x419311;
	int gamestate : "VVVVVV.exe", 0x4167B0; // actually called state in source
	string255 firstTextLineSmall : "VVVVVV.exe", 0x239618, 0x0;
	string255 firstTextLineLarge : "VVVVVV.exe", 0x239618, 0x0, 0x0;
	int teleport_to_x : "VVVVVV.exe", 0x416858;
	int teleport_to_y : "VVVVVV.exe", 0x41685C;

	// Variables for resetting
	int menustate : "VVVVVV.exe", 0x4167BC; // actually called gamestate in source
	bool ingame_titlemode : "VVVVVV.exe", 0x4171B2;
}

state("VVVVVV", "v2.3.6") {
	// Game time variables
	int gametimeFrames : "VVVVVV.exe", 0x1C2108;
	int gametimeSeconds : "VVVVVV.exe", 0x1C210C;
	int gametimeMinutes : "VVVVVV.exe", 0x1C2110;
	int gametimeHours : "VVVVVV.exe", 0x1C2114;

	// Variables for starting the timer
	bool fadetomode : "VVVVVV.exe", 0x1C0CC4;
	int gotomode : "VVVVVV.exe", 0x1C0CCC;
	int timetrialcountdown : "VVVVVV.exe", 0x1C2280;

	// Variables for splitting
	bool finalStretch : "VVVVVV.exe", 0x1C4D55;
	int gamestate : "VVVVVV.exe", 0x1C20C0; // actually called state in source
	string255 firstTextLineSmall : "VVVVVV.exe", 0x1C1FE0, 0x0;
	string255 firstTextLineLarge : "VVVVVV.exe", 0x1C1FE0, 0x0, 0x0;
	int teleport_to_x : "VVVVVV.exe", 0x1C217C;
	int teleport_to_y : "VVVVVV.exe", 0x1C2180;

	// Variables for resetting
	int menustate : "VVVVVV.exe", 0x1C20CC; // actually called gamestate in source
	bool ingame_titlemode : "VVVVVV.exe", 0x1C2B7E;
}

state("VVVVVV", "v2.3.4") {
	// Game time variables
	int gametimeFrames : "VVVVVV.exe", 0x1C1108;
	int gametimeSeconds : "VVVVVV.exe", 0x1C110C;
	int gametimeMinutes : "VVVVVV.exe", 0x1C1110;
	int gametimeHours : "VVVVVV.exe", 0x1C1114;

	// Variables for starting the timer
	bool fadetomode : "VVVVVV.exe", 0x1BFCC4;
	int gotomode : "VVVVVV.exe", 0x1BFCCC;
	int timetrialcountdown : "VVVVVV.exe", 0x1C1280;

	// Variables for splitting
	bool finalStretch : "VVVVVV.exe", 0x1C3D55;
	int gamestate : "VVVVVV.exe", 0x1C10C0; // actually called state in source
	string255 firstTextLineSmall : "VVVVVV.exe", 0x1C0FE0, 0x0;
	string255 firstTextLineLarge : "VVVVVV.exe", 0x1C0FE0, 0x0, 0x0;

	// Variables for resetting
	int menustate : "VVVVVV.exe", 0x1C10CC; // actually called gamestate in source
	bool ingame_titlemode : "VVVVVV.exe", 0x1C1B7E;
}

state("VVVVVV", "v2.2") {
	int doneLoading : "VVVVVV.exe", 0x3F8024;
	string255 saveDirectory : "VVVVVV.exe", 0x3F7DD8;
	string255 firstTextLine : "VVVVVV.exe", 0x3F8068, 0x0, 0x0;
	// string255 secondTextLine : "VVVVVV.exe", 0x3F8068, 0x1c, 0x0;
	// string255 thirdTextLine : "VVVVVV.exe", 0x3F8068, 0x38, 0x0;
	// string255 fourthTextLine : "VVVVVV.exe", 0x3F8068, 0x54, 0x0;
}

state("VVVVVV", "v2.0") {
	int doneLoading : "VVVVVV.exe", 0x637054;
	string255 firstTextLine : "VVVVVV.exe", 0x637098, 0x0, 0x0;
}

startup {
	refreshRate = 60;

	vars.startresetParent = "Settings for starting/resetting";
	vars.newgame = "Start timer on new game";
	vars.menuReset = "Reset on exiting to menu";
	vars.ils = "Start/Split/Reset on Time Trials";

	vars.crewmatesParent = "Settings for splitting on rescuing crewmates";
	vars.violet = "Split on Violet's teleporter (SS1)";
	vars.vitellary = "Split on rescuing Vitellary (SS2)";
	vars.vermilion = "Split on rescuing Vermilion (Tower)";
	vars.verdigris = "Split on rescuing Verdigris (Warp Zone)";
	vars.victoria = "Split on rescuing Victoria (Lab)";

	vars.intermissionsParent = "Settings for splitting on completing intermissions";
	vars.int1 = "Split on completing Intermission 1";
	vars.int2 = "Split on completing Intermission 2";

	vars.hundredpercentParent = "Settings pertaining to 100% categories";
	vars.labTelejump = "Split on teleporting to the teleporter under Lab";
	vars.towerTelejump = "Split on teleporting to the teleporter under Tower";

	vars.allAchievementsParent = "Settings pertaining to All Achievements";
	vars.disableTimeTrialTrinkets = "Disable splitting on trinkets when in a Time Trial (v2.4.1+)";
	vars.requireVRank = "Only split on completing time trials when V Rank is achieved (v2.4.4+)";
	vars.motu = "In NDM, split on attaining MotU instead of Game Complete (v2.4.4+)";
	vars.sgrav = "Split on reaching 1 minute in the Super Gravitron (v2.4.4+)";

	vars.trinkets = "Split on collecting trinkets";
	vars.trinketSecretToNobody = "Split on collecting the \"It's a Secret to Nobody\" trinket";
	vars.trinketTrenchWarfare = "Split on collecting the \"Trench Warfare\" trinket";
	vars.trinketWorthTheChallenge = "Split on collecting the \"Young Man, It's Worth the Challenge\" trinket";
	vars.trinketLabMaze = "Split on collecting the unnamed trinket in Lab";
	vars.trinketTantalizing = "Split on collecting the \"The Tantalizing Trinket\" trinket";
	vars.trinketUnobtainium = "Split on collecting the \"Purest Unobtainium\" trinket";
	vars.trinketVictoria = "Split on collecting Victoria's trinket";
	vars.trinketTower1 = "Split on collecting the first Tower trinket";
	vars.trinketTower2 = "Split on collecting the second Tower trinket";
	vars.trinketElephant = "Split on collecting the elephant trinket";
	vars.trinketOneWayRoom = "Split on collecting the \"One Way Room\" trinket";
	vars.trinketKeepComingBack = "Split on collecting the \"You Just Keep Coming Back\" trinket";
	vars.trinketClarionCall = "Split on collecting the \"Clarion Call\" trinket";
	vars.trinketDTTHW = "Split on collecting the \"Doing Things the Hard Way\" trinket";
	vars.trinketPrizeForTheReckless = "Split on collecting the \"Prize for the Reckless\" trinket";
	vars.trinketCave1 = "Split on collecting the unnamed trinket near the ship";
	vars.trinketCave2 = "Split on collecting the unnamed trinket near the Lab";
	vars.trinketCave3 = "Split on collecting the unnamed trinket near the Warp Zone";
	vars.trinketEdgeGames = "Split on collecting the \"Edge Games\" trinket";
	vars.trinketV = "Split on collecting the \"V\" trinket";

	vars.miscellaneousParent = "Miscellaneous settings";
	vars.gameComplete = "Split on game completion";
	vars.gameCompleteAndAllTrinkets = "Only split if all 20 trinkets have been collected (for 100%)";
	vars.dis = "Split on talking to Victoria (for DIS)";
	vars.finalStretch = "Split on Final Level terminal";
	vars.hello = "Split on \"Hello!\" (for Text Storage and Credits Warp)";
	vars.commsRelay = "Split on activating the cutscene in \"Comms Relay\" (v2.4.1+)";

	settings.Add(vars.startresetParent, true);
	settings.CurrentDefaultParent = vars.startresetParent;
	settings.Add(vars.newgame, true);
	settings.Add(vars.menuReset, true);
	settings.Add(vars.ils, false);

	settings.CurrentDefaultParent = null;
	settings.Add(vars.crewmatesParent, true);
	settings.CurrentDefaultParent = vars.crewmatesParent;
	settings.Add(vars.violet, true);
	settings.Add(vars.vitellary, true);
	settings.Add(vars.vermilion, true);
	settings.Add(vars.verdigris, true);
	settings.Add(vars.victoria, true);

	settings.CurrentDefaultParent = null;
	settings.Add(vars.intermissionsParent, true);
	settings.CurrentDefaultParent = vars.intermissionsParent;
	settings.Add(vars.int1, true);
	settings.Add(vars.int2, true);

	settings.CurrentDefaultParent = null;
	settings.Add(vars.hundredpercentParent, true);
	settings.CurrentDefaultParent = vars.hundredpercentParent;
	settings.Add(vars.trinkets, false);
	settings.CurrentDefaultParent = vars.trinkets;
	settings.Add(vars.trinketSecretToNobody, false);
	settings.Add(vars.trinketTrenchWarfare, false);
	settings.Add(vars.trinketWorthTheChallenge, false);
	settings.Add(vars.trinketLabMaze, false);
	settings.Add(vars.trinketTantalizing, false);
	settings.Add(vars.trinketUnobtainium, false);
	settings.Add(vars.trinketVictoria, false);
	settings.Add(vars.trinketTower1, false);
	settings.Add(vars.trinketTower2, false);
	settings.Add(vars.trinketElephant, false);
	settings.Add(vars.trinketOneWayRoom, false);
	settings.Add(vars.trinketKeepComingBack, false);
	settings.Add(vars.trinketClarionCall, false);
	settings.Add(vars.trinketDTTHW, false);
	settings.Add(vars.trinketPrizeForTheReckless, false);
	settings.Add(vars.trinketCave1, false);
	settings.Add(vars.trinketCave2, false);
	settings.Add(vars.trinketCave3, false);
	settings.Add(vars.trinketEdgeGames, false);
	settings.Add(vars.trinketV, false);

	settings.CurrentDefaultParent = vars.hundredpercentParent;
	settings.Add(vars.allAchievementsParent, false);
	settings.CurrentDefaultParent = vars.allAchievementsParent;
	settings.Add(vars.disableTimeTrialTrinkets, false);
	settings.Add(vars.requireVRank, false);
	settings.Add(vars.motu, false);
	settings.Add(vars.sgrav, false);

	settings.CurrentDefaultParent = vars.hundredpercentParent;
	settings.Add(vars.labTelejump, false);
	settings.Add(vars.towerTelejump, false);

	settings.CurrentDefaultParent = null;
	settings.Add(vars.miscellaneousParent, true);
	settings.CurrentDefaultParent = vars.miscellaneousParent;
	settings.Add(vars.gameComplete, true);
	settings.Add(vars.gameCompleteAndAllTrinkets, false, null, vars.gameComplete);
	settings.Add(vars.dis, false);
	settings.Add(vars.finalStretch, false);
	settings.Add(vars.hello, false);
	settings.Add(vars.commsRelay, false);
}

init {
	vars.isInTimeTrial = false;
	vars.isGameComplete = false;
	vars.splitAddr = IntPtr.Zero;
	vars.cur = null;
	vars.old = null;
	vars.runInvalid = false;

	if (modules.First().ModuleMemorySize == 0x6D9000) {
		version = "v2.0";
	} else if (modules.First().ModuleMemorySize == 0x456000) {
		version = "v2.2";
	} else if (modules.First().ModuleMemorySize == 0x3E9000) {
		version = "v2.3.4";
	} else if (modules.First().ModuleMemorySize == 0x3EA000) {
		version = "v2.3.6";
	} else if (modules.First().ModuleMemorySize == 0x449000) {
		version = "v2.4";
	} else if (modules.First().ModuleMemorySize == 0x44E000) {
		version = "v2.4.1";
	} else if (modules.First().ModuleMemorySize == 0x442000) {
		if (game.ReadString(IntPtr.Add(modules.First().BaseAddress, 0x19EE5C), 6) == "v2.4.2") {
			version = "v2.4.2";
		} else if (game.ReadString(IntPtr.Add(modules.First().BaseAddress, 0x19E62C), 6) == "v2.4.3") {
			version = "v2.4.3";
		}
	} else if (modules.First().ModuleMemorySize == 0x443000) {
		version = "v2.4.4";
	} else {
		version = "unknown";
	}

	// Not a known vanilla version: look for the SplitData struct of a modded build, located by its marker
	if (version == "unknown") {
		try {
			var splitScanner = new SignatureScanner(game, modules.First().BaseAddress, modules.First().ModuleMemorySize);
			IntPtr splitAddr = splitScanner.Scan(new SigScanTarget(0, "53 55 50 45 52 56 56 56 56 56 56 5F 53 50 4C 49 54")); // SUPERVVVVVV_SPLIT
			if (splitAddr != IntPtr.Zero) {
				vars.splitAddr = splitAddr;
				version = "split";
			}
		} catch (Exception) {
			// Scan failed, stays unknown
		}
	}

	if (version == "v2.3.4" || version == "v2.3.6" || version == "v2.4" || version == "v2.4.1" || version == "v2.4.2" || version == "v2.4.3" || version == "v2.4.4" || version == "split") {
		// No init needed
	} else if (version == "v2.0" || version == "v2.2") {
		// Legacy versions
		vars.gamestate = -1;
		vars.menuID = -1;
		vars.trinketCount = 0;
		vars.finalMode = 0;
		vars.gameTime = new TimeSpan(0, 0, 0, 0);

		vars.timeTrial = -1;

		if (version == "v2.2") {
			var gameObjectAddr = -1;

			// Read the save directory this way, because using state in init is apparently not reliable
			// Specifically, the state doesn't update when you set the version, so using any pointer paths not in the "default" version doesn't work
			var saveDirectory = game.ReadString(IntPtr.Add(modules.First().BaseAddress, 0x3F7DD8), 255);

			// Find the memory page containing the stack
			foreach (MemoryBasicInformation mbi in game.MemoryPages()) {
				// Scan for the music class on the stack
				var scanner = new SignatureScanner(game, mbi.BaseAddress, (int) mbi.RegionSize.ToUInt64());

				// Note: Masks always mask the entire byte, so 0? and ?0 are the same as ??
				IntPtr musicClassPtr = scanner.Scan(new SigScanTarget(0,
					"?? ?? ?? ??",	// int currentsong;
					"CC CC CC CC",	// int musicfade;
					"CC CC CC CC",	// int musicfadein;
					"CC CC CC CC",	// int resumesong;
					"?? ?? ?? ??",	// std::vector<SoundTrack> soundTracks (*begin);
					"?? ?? ?? ??",	// (*end)
					"?? ?? ?? ??",	// (*end_capacity)
					"CC CC CC CC",	// 4 unused bytes
					"?? ?? ?? ??",	// std::vector<MusicTrack> musicTracks (*begin);
					"?? ?? ?? ??",	// (*end)
					"?? ?? ?? ??",	// (*end_capacity)
					"CC CC CC CC",	// 4 unused bytes
					"?? ?? ?? ??",	// SoundSystem soundSystem; bool safeToProcessMusic; 2 unused bytes
					"?? ?? ?? ??",	// int nicechange;
					"?? ?? ?? ??",	// int nicefade;
					"0? CC CC CC",	// bool m_doFadeInVol; 3 unused bytes
					"?? ?0 00 00",	// int FadeVolAmountPerFrame; invariant <= 128;
					"?? ?0 00 00",	// int musicvolume; invariant <= 128;
					"?? ?? ?? ??",	// float volume;
					"00 0? 0? CC"	// bool custompd; bool mmmmmm; bool usingmmmmmm; 1 unused byte
				), 4); // 4 byte aligned

				// First value of game object is a pointer to the save location
				// Use that to verify we have successfully found the right address
				var tempGameObjectAddr = musicClassPtr.ToInt32() + 0x418 - 0x28E8;
				var saveDirAddr = game.ReadValue<int>(new IntPtr(tempGameObjectAddr));
				if (game.ReadString(new IntPtr(saveDirAddr), 255) == saveDirectory) {
					gameObjectAddr = tempGameObjectAddr;
					break;
				}
			}

			// Workaround copied from A Hat In Time autosplitter :)
			if (gameObjectAddr == -1) {
				// Waiting for the game to have booted up. This is a pretty ugly work
				// around, but we don't really know when the game is booted or where the
				// struct will be, so to reduce the amount of searching we are doing, we
				// sleep a bit between every attempt.
				// BUG: freezes livesplit if we can't find the game...
				Thread.Sleep(1000);
				throw new Exception("Could not find music class!");
			}

			// Found the right address, we can find the variables we need
			vars.finalStretchAddr	= gameObjectAddr - 0x53C;
			vars.gamestateAddr		= gameObjectAddr + 0x74	;
			vars.menuIDAddr			= gameObjectAddr + 0x84	;
			vars.gameTimeFrameAddr	= gameObjectAddr + 0xBC	;
			vars.gameTimeSecAddr	= gameObjectAddr + 0xC0	;
			vars.gameTimeMinAddr	= gameObjectAddr + 0xC4	;
			vars.gameTimeHourAddr	= gameObjectAddr + 0xC8	;
			vars.timeTrialAddr		= gameObjectAddr + 0x27C;
			vars.trinketCountAddr	= gameObjectAddr + 0x444;
		} else if (version == "v2.0") {
			// BUG: This pointer path seems to be unreliable
			// Let's wait one second because that's definitely the best solution
			Thread.Sleep(1000);

			var ptr = IntPtr.Add(modules.First().BaseAddress, 0x43330);
			int threadstack0 = game.ReadValue<int>(ptr) + 0xB94;

			vars.gamestateAddr		=	threadstack0 - 0xC50;
			vars.finalStretchAddr	=	vars.gamestateAddr - 0x470;
			vars.menuIDAddr			=	vars.gamestateAddr + 0x8;
			vars.gameTimeFrameAddr	=	vars.gamestateAddr + 0x3c;
			vars.gameTimeSecAddr	=	vars.gamestateAddr + 0x40;
			vars.gameTimeMinAddr	=	vars.gamestateAddr + 0x44;
			vars.gameTimeHourAddr	=	vars.gamestateAddr + 0x48;
			vars.timeTrialAddr		=	vars.gamestateAddr + 0x138;
			vars.trinketCountAddr	=	vars.gamestateAddr + 0x250;
		}
	}
}

start {
	var cur = version == "split" ? vars.cur : current;
	var prev = version == "split" ? vars.old : old;

	// Modded build: an invalid run never starts the timer
	if (version == "split" && cur.invalidRun) {
		return false;
	}

	if (version == "v2.3.4" || version == "v2.3.6" || version == "v2.4" || version == "v2.4.1" || version == "v2.4.2" || version == "v2.4.3" || version == "v2.4.4" || version == "split") {
		// Triggers when fade to new mode completes
		if (!cur.fadetomode && prev.fadetomode) {
			vars.isInTimeTrial = false;
			vars.isGameComplete = false;

			if (cur.gotomode == 0) {
				// New game
				return settings[vars.newgame];
			} else if (cur.gotomode >= 1 && cur.gotomode <= 2) {
				// Load save (teleporter or quicksave)
				return false;
			} else if (cur.gotomode >= 3 && cur.gotomode <= 8) {
				// Time trials
				// Time trial starting behaviour is done elsewhere
				return false;
			} else if (cur.gotomode >= 9 && cur.gotomode <= 10) {
				// No death mode (with or without cutscenes)
				return settings[vars.newgame];
			} else if (cur.gotomode == 11) {
				// Secret Lab
				return false;
			} else if (cur.gotomode >= 12 && cur.gotomode <= 15) {
				// Intermission 1 (with various crewmates)
				return false;
			} else if (cur.gotomode >= 16 && cur.gotomode <= 19) {
				// Intermission 2 (with various crewmates)
				return false;
			} else if (cur.gotomode == 20) {
				// Level editor
				return false;
			} else if (cur.gotomode == 21) {
				// Play level (in editor)
				return false;
			} else if (cur.gotomode == 22) {
				// Play level (in game)
				return false;
			} else if (cur.gotomode == 23) {
				// Continue custom level
				return false;
			} else if (cur.gotomode == 100) {
				// Exit the game
				return false;
			}
		}

		if (cur.timetrialcountdown != prev.timetrialcountdown) {
			if (cur.timetrialcountdown <= 30 && prev.timetrialcountdown > 30) {
				// Start when time trial countdown ends
				vars.isInTimeTrial = true; // if you're doing All Achievements, you really shouldn't be starting your run from a time trial, but we'll set the variable here anyway
				return settings[vars.ils];
			}
		}

		return false;
	} else if (version == "v2.0" || version == "v2.2") {
		// Legacy versions

		// Start if main menu closes and IGT resets
		if (vars.menuIDOld == 1 && vars.menuID == 0 && vars.gameTime.TotalMilliseconds < 100) {
			if (vars.timeTrial == 0) {
				// BUG: starts on selecting intermissions and secret lab from main menu
				return settings[vars.newgame];
			}
		}

		if (vars.gameTimeOld.TotalMilliseconds > vars.gameTime.TotalMilliseconds) {
			if (vars.timeTrial != 0 && vars.timeTrialOld == vars.timeTrial) {
				if (vars.gameTimeOld.TotalMilliseconds >= 4000 && vars.gameTimeOld.TotalMilliseconds <= 4066)
				// Start the timer if: game time resets between 4.000 or 4.066 seconds, the player is in a time trial and the time trial ID didn't change last frame
				return settings[vars.ils];
			}
		}

		return false;
	}
}

split {
	var cur = version == "split" ? vars.cur : current;
	var prev = version == "split" ? vars.old : old;

	// Modded build: once the run was invalid, the timer is never split automatically
	if (version == "split" && vars.runInvalid) {
		return false;
	}

	if (version == "v2.4.1" || version == "v2.4.2" || version == "v2.4.3" || version == "v2.4.4" || version == "split") {
		// In order to fix the bug where the timer often stopped a frame late, I forced the autosplitter to wait an extra cycle before actually stopping
		// the timer. This is so that the gameTime method can be run one more time before the timer stops, in which it will switch over to displaying
		// the save file's saved time (set in gamestate 3502) instead of the in-game time.
		if (vars.isGameComplete) {
			vars.isGameComplete = false;
			return true;
		}

		// Trinket splits
		if (settings[vars.trinkets]) {
			if (!settings[vars.disableTimeTrialTrinkets] || settings[vars.disableTimeTrialTrinkets] && !vars.isInTimeTrial) {
				if (cur.collect[0] == 1 && prev.collect[0] == 0) {
					// Trinket - It's a Secret to Nobody
					return settings[vars.trinketSecretToNobody];
				} else if (cur.collect[1] == 1 && prev.collect[1] == 0) {
					// Trinket - Trench Warfare
					return settings[vars.trinketTrenchWarfare];
				} else if (cur.collect[9] == 1 && prev.collect[9] == 0) {
					// Trinket - Young Man, It's Worth the Challenge
					return settings[vars.trinketWorthTheChallenge];
				} else if (cur.collect[14] == 1 && prev.collect[14] == 0) {
					// Trinket - Lab Maze
					return settings[vars.trinketLabMaze];
				} else if (cur.collect[10] == 1 && prev.collect[10] == 0) {
					// Trinket - Lab Maze
					return settings[vars.trinketTantalizing];
				} else if (cur.collect[11] == 1 && prev.collect[11] == 0) {
					// Trinket - Purest Unobtainium
					return settings[vars.trinketUnobtainium];
				} else if (cur.collect[18] == 1 && prev.collect[18] == 0) {
					// Trinket - Victoria
					return settings[vars.trinketVictoria];
				} else if (cur.collect[7] == 1 && prev.collect[7] == 0) {
					// Trinket - Tower 1
					return settings[vars.trinketTower1];
				} else if (cur.collect[8] == 1 && prev.collect[8] == 0) {
					// Trinket - Lab Maze
					return settings[vars.trinketTower2];
				} else if (cur.collect[17] == 1 && prev.collect[17] == 0) {
					// Trinket - Elephant
					return settings[vars.trinketElephant];
				} else if (cur.collect[2] == 1 && prev.collect[2] == 0) {
					// Trinket - One Way Room
					return settings[vars.trinketOneWayRoom];
				} else if (cur.collect[3] == 1 && prev.collect[3] == 0) {
					// Trinket - You Just Keep Coming Back
					return settings[vars.trinketKeepComingBack];
				} else if (cur.collect[4] == 1 && prev.collect[4] == 0) {
					// Trinket - Clarion Call
					return settings[vars.trinketClarionCall];
				} else if (cur.collect[5] == 1 && prev.collect[5] == 0) {
					// Trinket - Doing Things the Hard Way
					return settings[vars.trinketDTTHW];
				} else if (cur.collect[6] == 1 && prev.collect[6] == 0) {
					// Trinket - Prize for the Reckless
					return settings[vars.trinketPrizeForTheReckless];
				} else if (cur.collect[15] == 1 && prev.collect[15] == 0) {
					// Trinket - Cave 1
					return settings[vars.trinketCave1];
				} else if (cur.collect[16] == 1 && prev.collect[16] == 0) {
					// Trinket - Cave 2
					return settings[vars.trinketCave2];
				} else if (cur.collect[13] == 1 && prev.collect[13] == 0) {
					// Trinket - Cave 3
					return settings[vars.trinketCave3];
				} else if (cur.collect[12] == 1 && prev.collect[12] == 0) {
					// Trinket - Edge Games
					return settings[vars.trinketEdgeGames];
				} else if (cur.collect[19] == 1 && prev.collect[19] == 0) {
					// Trinket - V
					return settings[vars.trinketV];
				}
			}
		}

		// Gamestate splits
		// Make sure to only split once - current gamestate in range, old gamestate out of range
		if (cur.gamestate != prev.gamestate) {
			// Level endings
			if (cur.gamestate >= 3006 && cur.gamestate <= 3011) {
				if (prev.gamestate < 3006 || prev.gamestate > 3011) {
					// Warp Zone / Verdigris
					return settings[vars.verdigris];
				}
			} else if (cur.gamestate >= 3020 && cur.gamestate <= 3025) {
				if (prev.gamestate < 3020 || prev.gamestate > 3025) {
					// Space Station 2 / Vitellary
					return settings[vars.vitellary];
				}
			} else if (cur.gamestate >= 3040 && cur.gamestate <= 3045) {
				if (prev.gamestate < 3040 || prev.gamestate > 3045) {
					// Laboratory / Victoria
					return settings[vars.victoria];
				}
			} else if (cur.gamestate >= 3050 && cur.gamestate <= 3056) {
				if (prev.gamestate < 3050 || prev.gamestate > 3056) {
					// Space Station 1 / Violet
					// This split is activated elsewhere
				}
			} else if (cur.gamestate >= 3060 && cur.gamestate <= 3065) {
				if (prev.gamestate < 3060 || prev.gamestate > 3065) {
					// Tower / Vermilion
					return settings[vars.vermilion];
				}
			} else if (cur.gamestate >= 3080 && cur.gamestate <= 3082) {
				if (prev.gamestate < 3080 || prev.gamestate > 3082) {
					// Intermission 2 / Gravitron
					return settings[vars.int2];
				}
			} else if (cur.gamestate >= 3085 && cur.gamestate <= 3087) {
				if (prev.gamestate < 3085 || prev.gamestate > 3087) {
					// Intermission 1
					return settings[vars.int1];
				}
			}

			// Other gamestate splits
			if (cur.gamestate >= 4091 && cur.gamestate <= 4099) {
				if (prev.gamestate < 4091 || prev.gamestate > 4099) {
					// Split on Violet's teleporter
					return settings[vars.violet];
				}
			} else if (cur.gamestate >= 3503 && cur.gamestate <= 3509) {
				if (prev.gamestate < 3503 || prev.gamestate > 3509) {
					// Game completion (When "All crew members rescued!" appears on screen)
					// This is when IGT stops counting, which is why we don't split on "Game complete!" appearing

					// Ugly hack to allow only splitting if all 20 trinkets have been collected as well
					var allTrinketsKludge = true;

					if (settings[vars.gameCompleteAndAllTrinkets]) {
						for (var i = 0; i < 20; i++) {
							if (cur.collect[i] == 0) {
								allTrinketsKludge = false;
								break;
							}
						}
					}

					if (allTrinketsKludge && (!settings[vars.motu] || settings[vars.motu] && !cur.noDeathMode)) {
						vars.isGameComplete = settings[vars.gameComplete];
					}
				}
			} else if (cur.gamestate == 33) {
				if (prev.gamestate != 33) {
					// Split on talking to Victoria
					// Note: This might be unreliable on laggy computers
					return settings[vars.dis];
				}
			} else if (cur.gamestate >= 1000 && cur.gamestate <= 1003) {
				if (prev.gamestate < 1000 || prev.gamestate > 1003) {
					// Split on collecting trinkets
					// Note: This might be unreliable on laggy computers (when doing perfect text box skips)
					return settings[vars.trinkets];
				}
			} else if (cur.gamestate >= 82 && cur.gamestate <= 84) {
				if (prev.gamestate < 82 || prev.gamestate > 84) {
					// Split on completing time trials
					vars.isInTimeTrial = false;

					if (!settings[vars.requireVRank]) {
						return settings[vars.ils];
					} else {
						// We have to calculate whether the player has gotten V rank ourselves here, because there's a chance
						// that at the time the autosplitter notices gamestate == 82, the game hasn't yet itself processed the
						// new gamestate and finished calculating the player's rank. Yes, I'm aware this is pretty scuffed.
						// I won't be happy if this works.
						bool isUnderParTime = (cur.gametimeHours * 3600 + cur.gametimeMinutes * 60 + cur.gametimeSeconds <= cur.timeTrialPar);
						bool isDeathless = (cur.deathCount == 0);

						int collectedTrinkets = 0;

						for (int i = 0; i < 20; i++) {
							if (cur.collect[i] != 0) {
								collectedTrinkets++;
							}
						}

						bool hasEnoughTrinkets = (collectedTrinkets >= cur.timeTrialShinyTarget);

						if (isUnderParTime && isDeathless && hasEnoughTrinkets) {
							return settings[vars.ils];
						}

						// After testing, I can confirm this works (splits on V rank and doesn't on anything else), and it splits at the proper time.
						// (My first idea split a frame late.) As predicted, I'm not happy about this; it's even more scuffed than the 100% check on game complete.
					}
				}
			} else if (cur.gamestate == 4050 && cur.teleport_to_x == 8 && cur.teleport_to_y == 11) {
				// Split on teleporting to the teleporter under Tower
				return settings[vars.towerTelejump];
			} else if (cur.gamestate == 4020 && cur.teleport_to_x == 0 && cur.teleport_to_y == 0) {
				// Split on teleporting to the teleporter under Lab
				return settings[vars.labTelejump];
			} else if (cur.gamestate == 31) {
				if (prev.gamestate != 31) { // Comms Relay gamestate
					// Split on activating the cutscene in Comms Relay
					return settings[vars.commsRelay];
				}
			} else if (cur.gamestate == 81) { // quit to menu gamestate
				if (prev.gamestate != 81) {
					// whether or not we were previously in a time trial, we definitely aren't now!
					vars.isInTimeTrial = false;
				}
			} else if (cur.gamestate == 3510) { // trophies awarded
				if (prev.gamestate != 3510) {
					return settings[vars.motu] && cur.noDeathMode;
				}
			}
		}

		if (cur.finalStretch && !prev.finalStretch) {
			// Split on activating final stretch
			return settings[vars.finalStretch];
		}

		if (cur.firstTextLineSmall == "Hello!" || cur.firstTextLineLarge == "Hello!") {
			if (prev.firstTextLineSmall != "Hello!" && prev.firstTextLineLarge != "Hello!") {
				// Split on "Hello!" appearing
				return settings[vars.hello];
			}
		}

		if (cur.timetrialcountdown != prev.timetrialcountdown) {
			if (cur.timetrialcountdown <= 30 && prev.timetrialcountdown > 30) {
				// Test if the time trial countdown has ended
				// Use this variable to disable splitting on trinkets in time trials, if the option to do so has been selected
				vars.isInTimeTrial = true;
			}
		}

		if (cur.swnRank == 6 && prev.swnRank != 6) { // rank 6 is 60 seconds
			return settings[vars.sgrav];
		}

		return false;
	}

	if (version == "v2.3.4" || version == "v2.3.6" || version == "v2.4") {
		// Gamestate splits
		// Make sure to only split once - current gamestate in range, old gamestate out of range
		if (cur.gamestate != prev.gamestate) {
			// Level endings
			if (cur.gamestate >= 3006 && cur.gamestate <= 3011) {
				if (prev.gamestate < 3006 || prev.gamestate > 3011) {
					// Warp Zone / Verdigris
					return settings[vars.verdigris];
				}
			} else if (cur.gamestate >= 3020 && cur.gamestate <= 3025) {
				if (prev.gamestate < 3020 || prev.gamestate > 3025) {
					// Space Station 2 / Vitellary
					return settings[vars.vitellary];
				}
			} else if (cur.gamestate >= 3040 && cur.gamestate <= 3045) {
				if (prev.gamestate < 3040 || prev.gamestate > 3045) {
					// Laboratory / Victoria
					return settings[vars.victoria];
				}
			} else if (cur.gamestate >= 3050 && cur.gamestate <= 3056) {
				if (prev.gamestate < 3050 || prev.gamestate > 3056) {
					// Space Station 1 / Violet
					// This split is activated elsewhere
				}
			} else if (cur.gamestate >= 3060 && cur.gamestate <= 3065) {
				if (prev.gamestate < 3060 || prev.gamestate > 3065) {
					// Tower / Vermilion
					return settings[vars.vermilion];
				}
			} else if (cur.gamestate >= 3080 && cur.gamestate <= 3082) {
				if (prev.gamestate < 3080 || prev.gamestate > 3082) {
					// Intermission 2 / Gravitron
					return settings[vars.int2];
				}
			} else if (cur.gamestate >= 3085 && cur.gamestate <= 3087) {
				if (prev.gamestate < 3085 || prev.gamestate > 3087) {
					// Intermission 1
					return settings[vars.int1];
				}
			}

			// Other gamestate splits
			if (cur.gamestate >= 4091 && cur.gamestate <= 4099) {
				if (prev.gamestate < 4091 || prev.gamestate > 4099) {
					// Split on Violet's teleporter
					return settings[vars.violet];
				}
			} else if (cur.gamestate >= 3503 && cur.gamestate <= 3509) {
				if (prev.gamestate < 3503 || prev.gamestate > 3509) {
					// Game completion (When "All crew members rescued!" appears on screen)
					// This is when IGT stops counting, which is why we don't split on "Game complete!" appearing
					return settings[vars.gameComplete];
				}
			} else if (cur.gamestate == 33) {
				if (prev.gamestate != 33) {
					// Split on talking to Victoria
					// Note: This might be unreliable on laggy computers
					return settings[vars.dis];
				}
			} else if (cur.gamestate >= 82 && cur.gamestate <= 84) {
				if (prev.gamestate < 82 || prev.gamestate > 84) {
					// Split on completing time trials
					return settings[vars.ils];
				}
			} else if (cur.gamestate == 4050 && cur.teleport_to_x == 8 && cur.teleport_to_y == 11) {
				// Split on teleporting to the teleporter under Tower
				return settings[vars.towerTelejump];
			} else if (cur.gamestate == 4020 && cur.teleport_to_x == 0 && cur.teleport_to_y == 0) {
				// Split on teleporting to the teleporter under Lab
				return settings[vars.labTelejump];
			}
		}

		if (cur.finalStretch && !prev.finalStretch) {
			// Split on activating final stretch
			return settings[vars.finalStretch];
		}

		if (cur.firstTextLineSmall == "Hello!" || cur.firstTextLineLarge == "Hello!") {
			if (prev.firstTextLineSmall != "Hello!" && prev.firstTextLineLarge != "Hello!") {
				// Split on "Hello!" appearing
				return settings[vars.hello];
			}
		}

		return false;
	} else if (version == "v2.0" || version == "v2.2") {
		// Legacy versions

		// split on crewmate rescue and intermissions
		if (vars.gamestateOld == 3005 || vars.gamestateOld == 3004) {
			if (vars.gamestate == 3006 || vars.gamestate == 3007) {
				// Verdigris
				return settings[vars.verdigris];
			} else if (vars.gamestate == 3020 || vars.gamestate == 3021) {
				// Vitellary
				return settings[vars.vitellary];
			} else if (vars.gamestate == 3040 || vars.gamestate == 3041) {
				// Victoria
				return settings[vars.victoria];
			} else if (vars.gamestate == 3060 || vars.gamestate == 3061) {
				// Vermilion
				return settings[vars.vermilion];
			} else if (vars.gamestate == 3080 || vars.gamestate == 3081) {
				// Intermission 2
				return settings[vars.int2];
			} else if (vars.gamestate == 3085 || vars.gamestate == 3086) {
				// Intermission 1
				return settings[vars.int1];
			}
		} else if (vars.gamestateOld == 4091 && vars.gamestate != vars.gamestateOld) {
			// split on Violet's teleporter
			return settings[vars.violet];
		} else if (vars.gamestateOld == 3502 && vars.gamestate == 3503) {
			// split on game completion (When "All crew members rescued!" appears on screen)
			// This is when IGT stops counting, which is why we don't split on "Game complete!" appearing, which is one gamestate increment earlier
			return settings[vars.gameComplete];
		} else if (vars.finalMode == 1 && vars.finalModeOld == 0) {
			// split on activating final mode
			return settings[vars.finalStretch];
		} else if (vars.gamestate == 33 && vars.gamestateOld != 33) {
			// split on talking to Victoria
			return settings[vars.dis];
		} else if (cur.firstTextLine == "Hello!" && prev.firstTextLine != cur.firstTextLine) {
			// split on "Hello!" appearing on screen
			return settings[vars.hello];
		} else if (vars.trinketCount == vars.trinketCountOld + 1) {
			// split when collecting trinkets
			return settings[vars.trinkets];
		} else if (vars.gamestate == 83 && vars.gamestateOld != 83 && vars.timeTrial != 0) {
			// split when ending time trial
			return settings[vars.ils];
		}

		return false;
	}
}

reset {
	var cur = version == "split" ? vars.cur : current;
	var prev = version == "split" ? vars.old : old;

	// Modded build: once the run was invalid, only exiting to the menu resets the timer
	var invalid = version == "split" && vars.runInvalid;

	if (version == "v2.4" || version == "v2.4.1" || version == "v2.4.2" || version == "v2.4.3" || version == "v2.4.4" || version == "split") {
		// menustate values:
		// 0: in-game
		// 1: main menu
		// 2: in-game menu or map menu
		// 3: teleporter menu
		// 4: credits
		// 5: credits2
		// 6: editor mode
		// 7: fake loading screen

		// menu states 0, 2 and 3 are allowed, menu state 1 is allowed if ingame_titlemode is true
		if (cur.menustate != 0 && cur.menustate != 2 && cur.menustate != 3 && (cur.menustate != 1 || !cur.ingame_titlemode)) {
			if (prev.menustate == 0 || prev.menustate == 2 || prev.menustate == 3 || (prev.menustate == 1 && prev.ingame_titlemode)) {
				// Reset on exiting to menu
				vars.isInTimeTrial = false;
				return settings[vars.menuReset] || settings[vars.ils] || invalid;
			}
		}

		if (cur.timetrialcountdown > 30 && prev.timetrialcountdown <= 30) {
			return settings[vars.ils] && !invalid;
		}

		return false;
	} else if (version == "v2.3.4" || version == "v2.3.6") {
		// menustate values:
		// 0: in-game
		// 1: main menu
		// 2: click to start?
		// 3: focus mode?
		// 4: in-game menu or map menu
		// 5: teleporter menu
		// 6: credits
		// 7: credits2
		// 8: editor mode
		// 9: fake loading screen

		// menu states 0, 4 and 5 are allowed, menu state 1 is allowed if ingame_titlemode is true
		if (cur.menustate != 0 && cur.menustate != 4 && cur.menustate != 5 && (cur.menustate != 1 || !cur.ingame_titlemode)) {
			if (prev.menustate == 0 || prev.menustate == 4 || prev.menustate == 5 || (prev.menustate == 1 && prev.ingame_titlemode)) {
				// Reset on exiting to menu
				return settings[vars.menuReset] || settings[vars.ils];
			}
		}

		if (cur.timetrialcountdown > 30 && prev.timetrialcountdown <= 30) {
			return settings[vars.ils];
		}

		return false;
	} else if (version == "v2.0" || version == "v2.2") {
		// Legacy versions
		if (vars.gameTimeOld.TotalMilliseconds > vars.gameTime.TotalMilliseconds && vars.gameTime.TotalMilliseconds < 67) {
			// reset if game time resets (on exit to menu)
			// BUG: There are glitches that reset IGT, but they would invalidate
			// the run anyway (except Any%, which uses RTA, but then IGT is
			// irrelevant anyway)
			// BUG: sometimes the timer randomly resets during runs
			// Tried to fix by not resetting if gameTime is > 66
			if (vars.timeTrial == 0) {
				return settings[vars.menuReset] || settings[vars.ils];
			}
		}
		// reset if in main menu (shouldn't ever really happen, but you never know)
		return vars.menuID == 1 && settings[vars.menuReset];
	}
}

onStart {
	vars.runInvalid = false;
}

gameTime {
	var cur = version == "split" ? vars.cur : current;

	if (version == "v2.4.4" || version == "split") { // we separate out v2.4.4 because it's the only version so far in which saveSeconds and saveFrames are stored to the state
		if (vars.isGameComplete) { // evil hack to get the timer to show the right time on game complete (after gamestate 3503)
			return new TimeSpan(0, cur.saveSeconds / 3600, cur.saveSeconds % 3600 / 60, cur.saveSeconds % 60, 100 * cur.saveFrames / 3);
		} else {
			return new TimeSpan(0, cur.gametimeHours, cur.gametimeMinutes, cur.gametimeSeconds, 100*cur.gametimeFrames/3);
		}
	} else if (version == "v2.3.4" || version == "v2.3.6" || version == "v2.4" || version == "v2.4.1" || version == "v2.4.2" || version == "v2.4.3") {
		return new TimeSpan(0, cur.gametimeHours, cur.gametimeMinutes, cur.gametimeSeconds, 100*cur.gametimeFrames/3);
	} else if (version == "v2.0" || version == "v2.2") {
		// Legacy versions
		return vars.gameTime;
	}
}

isLoading {
	// Disable game time approximation
	return true;
}

update {
	if (version == "unknown") {
		// Prevents isLoading, gameTime, reset, split, and start from running
		return false;
	}

	if (version == "split") {
		// Modded build: read the whole SplitData struct (desktop_version/src/SplitData.cpp) in one go
		IntPtr splitAddr = (IntPtr) vars.splitAddr;
		byte[] buf = game.ReadBytes(splitAddr, 265);
		if (buf == null || buf.Length < 265 || System.Text.Encoding.ASCII.GetString(buf, 0, 17) != "SUPERVVVVVV_SPLIT") {
			return false;
		}

		var snapshot = new System.Dynamic.ExpandoObject();
		var data = (System.Collections.Generic.IDictionary<string, object>) snapshot;
		data["gametimeFrames"] = System.BitConverter.ToInt32(buf, 32);
		data["gametimeSeconds"] = System.BitConverter.ToInt32(buf, 36);
		data["gametimeMinutes"] = System.BitConverter.ToInt32(buf, 40);
		data["gametimeHours"] = System.BitConverter.ToInt32(buf, 44);
		data["gotomode"] = System.BitConverter.ToInt32(buf, 48);
		data["timetrialcountdown"] = System.BitConverter.ToInt32(buf, 52);
		data["saveFrames"] = System.BitConverter.ToInt32(buf, 56);
		data["saveSeconds"] = System.BitConverter.ToInt32(buf, 60);
		data["gamestate"] = System.BitConverter.ToInt32(buf, 64);
		data["teleport_to_x"] = System.BitConverter.ToInt32(buf, 68);
		data["teleport_to_y"] = System.BitConverter.ToInt32(buf, 72);
		data["swnRank"] = System.BitConverter.ToInt32(buf, 76);
		data["deathCount"] = System.BitConverter.ToInt32(buf, 80);
		data["timeTrialShinyTarget"] = System.BitConverter.ToInt32(buf, 84);
		data["timeTrialPar"] = System.BitConverter.ToInt32(buf, 88);
		data["menustate"] = System.BitConverter.ToInt32(buf, 92);
		data["fadetomode"] = buf[96] != 0;
		data["finalStretch"] = buf[97] != 0;
		data["noDeathMode"] = buf[98] != 0;
		data["ingame_titlemode"] = buf[99] != 0;

		byte[] collect = new byte[100];
		System.Array.Copy(buf, 100, collect, 0, 100);
		data["collect"] = collect;

		int textLength = 0;
		while (textLength < 64 && buf[200 + textLength] != 0) {
			textLength++;
		}
		string text = System.Text.Encoding.UTF8.GetString(buf, 200, textLength);
		data["firstTextLineSmall"] = text;
		data["firstTextLineLarge"] = text;
		data["invalidRun"] = buf[264] != 0;

		if (buf[264] != 0) {
			vars.runInvalid = true;
		}

		if (vars.cur == null) {
			vars.old = snapshot;
		} else {
			vars.old = vars.cur;
		}
		vars.cur = snapshot;
		return true;
	}

	if (version == "v2.3.4" || version == "v2.3.6" || version == "v2.4" || version == "v2.4.1" || version == "v2.4.2" || version == "v2.4.3" || version == "v2.4.4") {
		// No updates needed
		return true;
	} else if (version == "v2.0" || version == "v2.2") {
		// Legacy versions

		// Update variables
		vars.gameTimeOld = vars.gameTime;

		int gameTimeHours = game.ReadValue<int>(new IntPtr(vars.gameTimeHourAddr));
		int gameTimeMinutes = game.ReadValue<int>(new IntPtr(vars.gameTimeMinAddr));
		int gameTimeSeconds = game.ReadValue<int>(new IntPtr(vars.gameTimeSecAddr));
		int gameTimeFrames = game.ReadValue<int>(new IntPtr(vars.gameTimeFrameAddr));

		// Subtract one frame from the timer because the autosplitter splits a frame late
		vars.gameTime = new TimeSpan(0, gameTimeHours, gameTimeMinutes, gameTimeSeconds, 100*(gameTimeFrames-1)/3);

		vars.gamestateOld = vars.gamestate;
		vars.menuIDOld = vars.menuID;
		vars.trinketCountOld = vars.trinketCount;
		vars.timeTrialOld = vars.timeTrial;
		vars.finalModeOld = vars.finalMode;

		vars.gamestate = game.ReadValue<int>(new IntPtr(vars.gamestateAddr));
		vars.menuID = game.ReadValue<int>(new IntPtr(vars.menuIDAddr));
		vars.trinketCount = game.ReadValue<int>(new IntPtr(vars.trinketCountAddr));

		vars.timeTrial = game.ReadValue<int>(new IntPtr(vars.timeTrialAddr));
		vars.finalMode = game.ReadValue<int>(new IntPtr(vars.finalStretchAddr));
		return true;
	}
}
