/* Taken from https://github.com/djpohly/dwl/issues/466 */
#define COLOR(hex)    { ((hex >> 24) & 0xFF) / 255.0f, \
                        ((hex >> 16) & 0xFF) / 255.0f, \
                        ((hex >> 8) & 0xFF) / 255.0f, \
                        (hex & 0xFF) / 255.0f }
/* appearance */
static const int sloppyfocus               = 1;  /* focus follows mouse */
static const int bypass_surface_visibility = 0;  /* 1 means idle inhibitors will disable idle tracking even if it's surface isn't visible  */
static const unsigned int borderpx         = 3;  /* border pixel of windows */
static const int draw_minimal_borders      = 1; /* merge adjacent borders */
static const float rootcolor[]             = COLOR(0x222222ff);
static const float bordercolor[]           = COLOR(0x444444ff);
static const float focuscolor[]            = COLOR(0x005577ff);
static const float unfocuseddim[]            = COLOR(0x00000088);
static const float urgentcolor[]           = COLOR(0xff0000ff);
/* This conforms to the xdg-protocol. Set the alpha to zero to restore the old behavior */
static const float fullscreen_bg[]         = {0.1f, 0.1f, 0.1f, 1.0f}; /* You can also use glsl colors */

/* tagging - TAGCOUNT must be no greater than 31 */
#define TAGCOUNT (9)

/* logging */
static int log_level = WLR_ERROR;

/* NOTE: ALWAYS keep a rule declared even if you don't use rules (e.g leave at least one example) */
static const Rule rules[] = {
	/* app_id     title       tags mask     isfloating   neverdim      monitor */
	/* examples:
	{ "Gimp_example",     NULL,       0,            1,           0,            -1 },
	*/
	{ "firefox_example",  NULL,       1 << 8,       0,           1,		   -1 },
};

/* layout(s) */
static const Layout layouts[] = {
	/* symbol     arrange function */
	{ "[]=",      tile },
	{ "><>",      NULL },    /* no layout function means floating behavior */
	{ "[M]",      monocle },
};

/* monitors */
/* (x=-1, y=-1) is reserved as an "autoconfigure" monitor position indicator
 * WARNING: negative values other than (-1, -1) cause problems with Xwayland clients
 * https://gitlab.freedesktop.org/xorg/xserver/-/issues/899
*/
/* NOTE: ALWAYS add a fallback rule, even if you are completely sure it won't be used */
static const MonitorRule monrules[] = {
	/* name       mfact  nmaster scale layout       rotate/reflect                x    y */
	/* example of a HiDPI laptop monitor:
	{ "eDP-1",    0.5f,  1,      2,    &layouts[0], WL_OUTPUT_TRANSFORM_NORMAL,   -1,  -1 },
	*/
	/* defaults */
	{ NULL,       0.55f, 1,      1,    &layouts[0], WL_OUTPUT_TRANSFORM_NORMAL,   -1,  -1 },
};

/* keyboard */
static const struct xkb_rule_names xkb_rules = {
	/* can specify fields: rules, model, layout, variant, options */
	.options = "caps:escape_shifted_capslock",
};

static const int repeat_rate = 25;
static const int repeat_delay = 600;

/* Trackpad */
static const int tap_to_click = 1;
static const int tap_and_drag = 1;
static const int drag_lock = 0;
static const int natural_scrolling = 1;
static const int disable_while_typing = 1;
static const int left_handed = 0;
static const int middle_button_emulation = 0;
/* Scroll sensitivity */
static const double scroll_factor = 0.5f;

/* You can choose between:
LIBINPUT_CONFIG_SCROLL_NO_SCROLL
LIBINPUT_CONFIG_SCROLL_2FG
LIBINPUT_CONFIG_SCROLL_EDGE
LIBINPUT_CONFIG_SCROLL_ON_BUTTON_DOWN
*/
static const enum libinput_config_scroll_method scroll_method = LIBINPUT_CONFIG_SCROLL_2FG;

/* You can choose between:
LIBINPUT_CONFIG_CLICK_METHOD_NONE
LIBINPUT_CONFIG_CLICK_METHOD_BUTTON_AREAS
LIBINPUT_CONFIG_CLICK_METHOD_CLICKFINGER
*/
static const enum libinput_config_click_method click_method = LIBINPUT_CONFIG_CLICK_METHOD_BUTTON_AREAS;

/* You can choose between:
LIBINPUT_CONFIG_SEND_EVENTS_ENABLED
LIBINPUT_CONFIG_SEND_EVENTS_DISABLED
LIBINPUT_CONFIG_SEND_EVENTS_DISABLED_ON_EXTERNAL_MOUSE
*/
static const uint32_t send_events_mode = LIBINPUT_CONFIG_SEND_EVENTS_ENABLED;

/* You can choose between:
LIBINPUT_CONFIG_ACCEL_PROFILE_FLAT
LIBINPUT_CONFIG_ACCEL_PROFILE_ADAPTIVE
*/
static const enum libinput_config_accel_profile accel_profile = LIBINPUT_CONFIG_ACCEL_PROFILE_ADAPTIVE;
static const double accel_speed = 0.0;

/* You can choose between:
LIBINPUT_CONFIG_TAP_MAP_LRM -- 1/2/3 finger tap maps to left/right/middle
LIBINPUT_CONFIG_TAP_MAP_LMR -- 1/2/3 finger tap maps to left/middle/right
*/
static const enum libinput_config_tap_button_map button_map = LIBINPUT_CONFIG_TAP_MAP_LRM;

/* If you want to use the windows key for MODKEY, use WLR_MODIFIER_LOGO */
#define MODKEY WLR_MODIFIER_LOGO

/* swapped the key to skey, BUT U MIGHT WANT TO SWAP THE ARGUMENT INSTEAD */
#define TAGKEYS(KEY,TAG) \
	{ MODKEY,                    KEY,            view,            {.ui = 1 << TAG} }, \
	{ MODKEY|WLR_MODIFIER_CTRL,  KEY,            toggleview,      {.ui = 1 << TAG} }, \
	{ MODKEY|WLR_MODIFIER_SHIFT, KEY,           tag,             {.ui = 1 << TAG} }, \
	{ MODKEY|WLR_MODIFIER_CTRL|WLR_MODIFIER_SHIFT,KEY,toggletag, {.ui = 1 << TAG} }

/* helper for spawning shell commands in the pre dwm-5.0 fashion */
#define SHCMD(cmd) { .v = (const char*[]){ "/bin/sh", "-c", cmd, NULL } }

/* commands */
static const char *termcmd[] = { "wezterm", NULL };
static const char *menucmd[] = { "rofi","-show", NULL };

#include "keys.h"
static const Key keys[] = {
	/* Note that Shift changes certain key codes: c -> C, 2 -> at, etc. */
	/* modifier                  key                 function        argument */
	{ 0,                         Key_XF86AudioRaiseVolume, spawn, SHCMD("/home/seva/.local/bin/volume up")},
	{ 0,                         Key_XF86AudioLowerVolume, spawn, SHCMD("/home/seva/.local/bin/volume down")},
	{ 0,                         Key_XF86AudioMute, spawn, SHCMD("/home/seva/.local/bin/volume mute")},
	{ 0,                         Key_XF86MonBrightnessUp, spawn, SHCMD("/home/seva/.local/bin/brightness up")},
	{ 0,                         Key_XF86MonBrightnessDown, spawn, SHCMD("/home/seva/.local/bin/brightness down")},
	{ MODKEY,                    Key_space,      spawn,          {.v = menucmd} },
	{ MODKEY|WLR_MODIFIER_SHIFT, Key_Return,     spawn,          {.v = termcmd} },
	{ MODKEY,                    Key_j,          focusstack,     {.i = +1} },
	{ MODKEY,                    Key_k,          focusstack,     {.i = -1} },
	{ MODKEY|WLR_MODIFIER_SHIFT, Key_j,          movestack,      {.i = +1} },
	{ MODKEY|WLR_MODIFIER_SHIFT, Key_k,          movestack,      {.i = -1} },
	{ MODKEY,                    Key_i,          incnmaster,     {.i = +1} },
	{ MODKEY,                    Key_d,          incnmaster,     {.i = -1} },
	{ MODKEY,                    Key_h,          setmfact,       {.f = -0.05f} },
	{ MODKEY,                    Key_l,          setmfact,       {.f = +0.05f} },
	{ MODKEY,                    Key_Return,     zoom,           {0} },
	{ MODKEY,                    Key_Tab,        view,           {0} },
	{ MODKEY|WLR_MODIFIER_SHIFT, Key_q,          killclient,     {0} },
	{ MODKEY,                    Key_t,          setlayout,      {.v = &layouts[0]} },
	{ MODKEY,                    Key_f,          setlayout,      {.v = &layouts[1]} },
	{ MODKEY,                    Key_m,          setlayout,      {.v = &layouts[2]} },
	/*{ MODKEY,                    XKB_KEY_space,      setlayout,      {0} },*/
	{ MODKEY,           		     Key_apostrophe, toggledimming,  {0} },
	{ MODKEY,           		     Key_b,          spawn,  SHCMD("/home/seva/.local/bin/systatus") },
	{ MODKEY|WLR_MODIFIER_SHIFT, Key_space,      togglefloating, {0} },
	{ MODKEY,                    Key_e,          togglefullscreen, {0} },
	{ MODKEY,                    Key_0,          view,           {.ui = ~0} },
	{ MODKEY|WLR_MODIFIER_SHIFT, Key_parenright, tag,            {.ui = ~0} },
	{ MODKEY,                    Key_comma,      focusmon,       {.i = WLR_DIRECTION_LEFT} },
	{ MODKEY,                    Key_period,     focusmon,       {.i = WLR_DIRECTION_RIGHT} },
	{ MODKEY|WLR_MODIFIER_SHIFT, Key_less,       tagmon,         {.i = WLR_DIRECTION_LEFT} },
	{ MODKEY|WLR_MODIFIER_SHIFT, Key_greater,    tagmon,         {.i = WLR_DIRECTION_RIGHT} },
	TAGKEYS(          Key_1,                      0),
	TAGKEYS(          Key_2,                          1),
	TAGKEYS(          Key_3,                  2),
	TAGKEYS(          Key_4,                      3),
	TAGKEYS(          Key_5,                     4),
	TAGKEYS(          Key_6,                 5),
	TAGKEYS(          Key_7,                   6),
	TAGKEYS(          Key_8,                    7),
	TAGKEYS(          Key_9,                   8),
	{ MODKEY|WLR_MODIFIER_SHIFT, Key_e,          quit,           {0} },

	/* Ctrl-Alt-Backspace and Ctrl-Alt-Fx used to be handled by X server */
	{ WLR_MODIFIER_CTRL|WLR_MODIFIER_ALT,Key_BackSpace, quit, {0} },
	/* Ctrl-Alt-Fx is used to switch to another VT, if you don't know what a VT is
	 * do not remove them.
	 */
#define CHVT(KEY,n) { WLR_MODIFIER_CTRL|WLR_MODIFIER_ALT,KEY, chvt, {.ui = (n)} }
	CHVT(Key_F1, 1), CHVT(Key_F2,  2),  CHVT(Key_F3,  3),  CHVT(Key_F4,  4),
	CHVT(Key_F5, 5), CHVT(Key_F6,  6),  CHVT(Key_F7,  7),  CHVT(Key_F8,  8),
	CHVT(Key_F9, 9), CHVT(Key_F10, 10), CHVT(Key_F11, 11), CHVT(Key_F12, 12),
};

static const Button buttons[] = {
	{ MODKEY, BTN_LEFT,   moveresize,     {.ui = CurMove} },
	{ MODKEY, BTN_MIDDLE, togglefloating, {0} },
	{ MODKEY, BTN_RIGHT,  moveresize,     {.ui = CurResize} },
};
