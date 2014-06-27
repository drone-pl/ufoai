--!usr/bin/lua

--[[
	MAIN MENU
--]]

-- definitions of named colors (not present yet, names are illustrative)
-- contains entry: COLOR_DARK_YELLOW = [0.0, 0.5, 0.0, 1.0]
-- contains entry: COLOR_WHITE = [1.0, 1.0, 1.0, 1.0]
require ("_colors.ufo")

-- create title control
local lb_title = Label ("_UFO: ALIEN INVASION", 500, 50, 262, 50, "f_title", [0.59, 0.78, 0.56, 1.0], ALIGN_CC)

-- create facebook button
local btn_facebook = Button ("_Facebook", 10, 720, 80, 20, COLOR_DARK_YELLOW)
btn_facebook.tooltip = "_Like us on facebook"
btn_facebook.padding = 0
btn_facebook.selectcolor = [1, 1, 1, 1]

function btn_facebook:onClick ()
	-- todo
end

-- create twitter button
local btn_twitter = Button ("_Twitter", 100, 720, 70, 20, COLOR_DARK_YELLOW)
btn_twitter.tooltip = "_Follow us on twitter"
btn_twitter.padding = 0
btn_twitter.selectcolor = COLOR_WHITE

function btn_twitter:onClick ()
	-- todo
end

-- import a button defined in _assets.ufo.lua (the .lua is added automatically)
require ("_assets.ufo")

-- create button CAMPAIGN
local btn_campain = MainMenuBtn ("_CAMPAIGN")

function btn_campaign:onClick ()
	-- todo
end

-- create buttons panel
local pnl_buttons = Panel ()
pnl_buttons.pos = [312, 250]
pnl_buttons.size = [400, 400]
pnl_buttons.layout = LAYOUT_TOP_DOWN_FLOW
pnl_buttons.layoutMargin = 15

-- create the main window
local main = Window ()
main.background = "ui/main_bg"
main:add (title)
main:add (btn_facebook)
main:add (btn_twitter)
main:add (pnl_buttons)


--[[

// ==================
// MAIN MENU
// ==================

window main
{
	{
		background		"ui/main_bg"
	}

	string title {
		string			"_UFO: ALIEN INVASION"
		size			"500 50"
		pos				"262 50"
		font			"f_title"
		color			"0.59 0.78 0.56 1"
		contentalign	ALIGN_CC
	}

	/**
	 * @brief Adds cgame games to the main menu (like skirmish and campaign)
	 * @param <1> window id
	 * @param <2> name
	 */
	confunc game_addmode_singleplayer
	{
	}

	button facebook
	{
		pos			"10 720"
		string		"_Facebook"
		tooltip		"_Like us on facebook"
		size		"80 20"
		color		"0 0.5 0 1"
		selectcolor "1 1 1 1"
		padding		0
		onClick		{ cmd "cl_openurl http://www.facebook.com/ufoai/;" }
	}

	button twitter
	{
		pos			"100 720"
		string		"_Twitter"
		tooltip		"_Follow us on twitter"
		size		"70 20"
		color		"0 0.5 0 1"
		selectcolor "1 1 1 1"
		padding		0
		onClick		{ cmd "cl_openurl http://www.twitter.com/ufoai/;" }
	}

	panel buttons {
		{
			pos			"312 250"
			size		"400 400"
			layout		LAYOUT_TOP_DOWN_FLOW
			layoutMargin	15
		}
		MainMenuBtn btn_campaign {
			string		"_CAMPAIGN"
			onClick	{
				cmd "ui_push campaign;"
			}
		}
		MainMenuBtn btn_skirmish {
			string		"_SKIRMISH"
			onClick {
				cmd "ui_push skirmish;"
			}
		}
		MainMenuBtn btn_multiplayer {
			string		"_MULTIPLAYER"
			onClick {
				cmd "ui_push multiplayer;"
			}
		}
		MainMenu2Btn btn_options {
			string		"_OPTIONS"
			onClick {
				cmd "ui_push options;"
			}
		}
		MainMenu2Btn btn_tutorials {
			string	"_TUTORIALS"
			onClick	{
				cmd "ui_push tutorials;"
			}
		}
		MainMenu2Btn btn_credits {
			string	"_DEVELOPERS"
			onClick {
				cmd "seq_start developers;"
			}
		}
		MainMenuExitBtn btn_exit {
			string	"_EXIT"
			onClick {
				cmd "quit;"
			}
		}
	}

	string version {
		string	*cvar:version
		pos		"10 744"
		size	"1004 20"
	}

	// Fuzzy screen effect (see assets.ufo)
	fuzzyScreen overlay { }

	func onWindowOpened
	{
		cmd "music_change main;"
		// TODO: activate this again
		//cmd "ui_push_child popup_tipoftheday main;"
		cmd "check_cvars;"
		if ( *cvar:cl_introshown != 1 ) {
			cmd "ui_push intro;"
			*cvar:cl_introshown = 1
		}
	}
}

// ==================
// CVAR CHECK MENU
// ==================

window checkcvars extends ipopup
{
	{
		pos			"124 192"
		size		"777 368"
		modal		true
		bgcolor		"0 0 0 0.9"
	}

	string title {
		width		"777"
		string		"_Initial settings"
	}

	string name_label
	{
		string		"_User Information"
		pos			"26 48"
		size		"300 30"
		font		"f_normal"
	}

	// ===================
	// NAME
	// ===================
	string current_name
	{
		string		"_Name:"
		pos			"41 88"
		size		"300 20"
		color		"1 1 1 1"
	}
	TextEntry slot_name
	{
		string		*cvar:cl_name
		font		"f_verysmall"
		pos			"170 82"
		size		"185 34"
	}

	// ===================
	// BARS
	// ===================
	string volume_str
	{
		string		"_Volume Control"
		pos			"26 168"
		size		"300 30"
		font		"f_normal"
	}
	string effects_str
	{
		string		"_Effects:"
		pos			"41 198"
		size		"150 20"
	}
	bar effects_bar
	{
		current		*cvar:snd_volume
		pos			"170 208"
		size		"238 6"
		color		"0.582 0.808 0.758 1"
		bordercolor	"0.582 0.808 0.758 1"
		border		"1"
		max			1.0
	}
	string music_str
	{
		string		"_Music:"
		pos			"41 223"
		size		"150 20"
	}
	bar music_bar
	{
		current		*cvar:snd_music_volume
		pos			"170 233"
		size		"238 6"
		color		"0.582 0.808 0.758 1"
		border		"1"
		bordercolor	"0.582 0.808 0.758 1"
		max			128
	}

	// ===================
	// LANGUAGE
	// ===================
	string language_str
	{
		string		"_Language"
		pos			"410 48"
		size		"300 30"
		font		"f_normal"
	}
	string language_str2
	{
		string		"_Language:"
		pos			"425 82"
		size		"108 20"
	}
	optionlist select_language
	{
		pos				"531 78"
		font			"f_language"
		cvar			*cvar:s_language
		background		"ui/panel"
		size			"155 226"
		color			"1 1 1 0.5"
		selectcolor		"1 1 1 0.9"
		padding			6
		contentalign	ALIGN_CC
		dataid			OPTION_LANGUAGES
		onViewChange {
			*node:root.select_language_scroll@current = <viewpos>
			*node:root.select_language_scroll@fullsize = <fullsize>
			*node:root.select_language_scroll@viewsize = <viewsize>
		}
	}

	vscrollbar select_language_scroll {
		image			"ui/scrollbar_v"
		pos				"692 78"
		height			"226"
		current			0
		viewsize		8
		fullsize		21
		onChange {
			*node:root.select_language@viewpos = <current>
		}
	}

	// ===================
	// VALIDATION
	// ===================

	string message
	{
		string		"_You have to set the playername and hit the 'OK' button:"
		pos			"41 338"
		size		"500 40"
		color		"1 1 1 0.5"
	}

	MainMenuBtn bt_ok
	{
		string		"_OK"
		tooltip		"_Save settings"
		pos			"597 338"
		width		"180"
		onClick		{ cmd "ui_pop; saveconfig config.cfg;" }
	}

	fuzzyScreen overlay { }

	// check again - maybe the user has forgotton something
	func onWindowClosed
	{
		cmd "check_cvars;"
	}
}

// ==================
// TIP OF THE DAY
// ==================
window popup_tipoftheday extends ipopup
{
	{
		pos			"38 456"
		size		"496 218"
		closebutton	true
		bgcolor		"0 0 0 0.9"
	}

	string title {
		width		"496"
		string		"_Tip of the day"
	}

	CheckBox activate
	{
		current		*cvar:cl_showTipOfTheDay
		pos			"36 194"
		size		"20 18"
		onChange		{ cmd "check_nexttip;" }
	}
	string activate_string
	{
		string		"_Show tip of the day"
		pos			"58 194"
		size		"200 25"
		font		"f_small"
		onClick		{
			call *node:root.activate@toggle
		}
	}

	text popup_tipoftheday_text
	{
		pos			"26 58"
		size		"444 120"
		lineheight	20
		tabwidth	150
		dataid		TEXT_TIPOFTHEDAY
		rows		6
	}

	MainMenuBtn bt_next
	{
		string		"_Next"
		tooltip		"_Next tip"
		width		"176"
		pos			"320 188"
		font		"f_menu"
		onClick		{
			if ( *cvar:cl_showTipOfTheDay == 1 ) {
				cmd "tipoftheday;"
			} else {
				cmd "ui_close popup_tipoftheday;"
			}
		}
	}

	fuzzyScreen overlay { }

	confunc check_nexttip
	{
		if ( *cvar:cl_showTipOfTheDay == 1 ) {
			*node:root.bt_next@string = "_Next"
			*node:root.bt_next@tooltip = "_Next tip"
		} else {
			*node:root.bt_next@string = "_Close"
			*node:root.bt_next@tooltip = "_Close window"
		}
	}

	func onWindowOpened
	{
		if ( *cvar:cl_showTipOfTheDay == 1 ) {
			cmd "tipoftheday random; check_nexttip;"
		} else {
			cmd "ui_close popup_tipoftheday;"
		}
	}
}
--]]
