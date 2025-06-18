#pragma once

enum KeyboardKey : uint16_t
{
    keyboard_key_unknown = 0,

    // USB Keyboard
    keyboard_key_a = 4,
    keyboard_key_b = 5,
    keyboard_key_c = 6,
    keyboard_key_d = 7,
    keyboard_key_e = 8,
    keyboard_key_f = 9,
    keyboard_key_g = 10,
    keyboard_key_h = 11,
    keyboard_key_i = 12,
    keyboard_key_j = 13,
    keyboard_key_k = 14,
    keyboard_key_l = 15,
    keyboard_key_m = 16,
    keyboard_key_n = 17,
    keyboard_key_o = 18,
    keyboard_key_p = 19,
    keyboard_key_q = 20,
    keyboard_key_r = 21,
    keyboard_key_s = 22,
    keyboard_key_t = 23,
    keyboard_key_u = 24,
    keyboard_key_v = 25,
    keyboard_key_w = 26,
    keyboard_key_x = 27,
    keyboard_key_y = 28,
    keyboard_key_z = 29,

    keyboard_key_1 = 30,
    keyboard_key_2 = 31,
    keyboard_key_3 = 32,
    keyboard_key_4 = 33,
    keyboard_key_5 = 34,
    keyboard_key_6 = 35,
    keyboard_key_7 = 36,
    keyboard_key_8 = 37,
    keyboard_key_9 = 38,
    keyboard_key_0 = 39,

    keyboard_key_return = 40,
    keyboard_key_escape = 41,
    keyboard_key_backspace = 42,
    keyboard_key_tab = 43,
    keyboard_key_space = 44,

    keyboard_key_minus = 45,
    keyboard_key_equals = 46,
    keyboard_key_leftbracket = 47,
    keyboard_key_rightbracket = 48,
    keyboard_key_backslash = 49, 
    keyboard_key_nonushash = 50, 
    keyboard_key_semicolon = 51,
    keyboard_key_apostrophe = 52,
    keyboard_key_grave = 53, 
    keyboard_key_comma = 54,
    keyboard_key_period = 55,
    keyboard_key_slash = 56,

    keyboard_key_capslock = 57,

    keyboard_key_f1 = 58,
    keyboard_key_f2 = 59,
    keyboard_key_f3 = 60,
    keyboard_key_f4 = 61,
    keyboard_key_f5 = 62,
    keyboard_key_f6 = 63,
    keyboard_key_f7 = 64,
    keyboard_key_f8 = 65,
    keyboard_key_f9 = 66,
    keyboard_key_f10 = 67,
    keyboard_key_f11 = 68,
    keyboard_key_f12 = 69,

    keyboard_key_printscreen = 70,
    keyboard_key_scrolllock = 71,
    keyboard_key_pause = 72,
    keyboard_key_insert = 73, 
    keyboard_key_home = 74,
    keyboard_key_pageup = 75,
    keyboard_key_delete = 76,
    keyboard_key_end = 77,
    keyboard_key_pagedown = 78,
    keyboard_key_right = 79,
    keyboard_key_left = 80,
    keyboard_key_down = 81,
    keyboard_key_up = 82,

    keyboard_key_numlock = 83,

    keyboard_key_divide = 84,
    keyboard_key_kp_multiply = 85,
    keyboard_key_kp_minus = 86,
    keyboard_key_kp_plus = 87,
    keyboard_key_kp_enter = 88,
    keyboard_key_kp_1 = 89,
    keyboard_key_kp_2 = 90,
    keyboard_key_kp_3 = 91,
    keyboard_key_kp_4 = 92,
    keyboard_key_kp_5 = 93,
	keyboard_key_kp_6 = 94,
	keyboard_key_kp_7 = 95,
    keyboard_key_kp_8 = 96,
    keyboard_key_kp_9 = 97,
    keyboard_key_kp_0 = 98,
    keyboard_key_kp_period = 99,

    keyboard_key_nonusbackslash = 100,
    keyboard_key_application = 101, // Windows contextual menu
    keyboard_key_power = 102, 
    keyboard_key_kp_equals = 103,
    keyboard_key_f13 = 104,
    keyboard_key_f14 = 105,
    keyboard_key_f15 = 106,
    keyboard_key_f16 = 107,
    keyboard_key_f17 = 108,
    keyboard_key_f18 = 109,
    keyboard_key_f19 = 110,
    keyboard_key_f20 = 111,
    keyboard_key_f21 = 112,
    keyboard_key_f22 = 113,
    keyboard_key_f23 = 114,
    keyboard_key_f24 = 115,
    keyboard_key_execute = 116,
    keyboard_key_help = 117,    // AL Integrated Help Center
    keyboard_key_menu = 118,    // Menu (show menu)
    keyboard_key_select = 119,
    keyboard_key_stop = 120,    // AC Stop
    keyboard_key_again = 121,   // AC Redo/Repeat
    keyboard_key_undo = 122,    // AC Undo
    keyboard_key_cut = 123,     // AC Cut
    keyboard_key_copy = 124,    // AC Copy
    keyboard_key_paste = 125,   // AC Paste
    keyboard_key_find = 126,    // AC Find
    keyboard_key_mute = 127,
    keyboard_key_volumeup = 128,
    keyboard_key_volumedown = 129,
    keyboard_key_lockingcapslock = 130,
    keyboard_key_lockingnumlock = 131,
    keyboard_key_lockingscrolllock = 132,
    keyboard_key_kp_comma = 133,
    keyboard_key_kp_equalsas400 = 134,

    keyboard_key_international1 = 135,
    keyboard_key_international2 = 136,
    keyboard_key_international3 = 137,
    keyboard_key_international4 = 138,
    keyboard_key_international5 = 139,
    keyboard_key_international6 = 140,
    keyboard_key_international7 = 141,
    keyboard_key_international8 = 142,
    keyboard_key_international9 = 143,
    keyboard_key_lang1 = 144, // Hangul/English toggle 
    keyboard_key_lang2 = 145, // Hanja conversion
    keyboard_key_lang3 = 146, // Katakana
    keyboard_key_lang4 = 147, // Hiragana
    keyboard_key_lang5 = 148, // Zenkaku/Hankaku
    keyboard_key_lang6 = 149, // Reserved
	keyboard_key_lang7 = 150, // Reserved
	keyboard_key_lang8 = 151, // Reserved
	keyboard_key_lang9 = 152, // Reserved

    keyboard_key_alterase = 153, 
    keyboard_key_sysreq = 154,
    keyboard_key_cancel = 155, 
    keyboard_key_clear = 156,
    keyboard_key_prior = 157,
    keyboard_key_return2 = 158,
    keyboard_key_separator = 159,
    keyboard_key_out = 160,
    keyboard_key_oper = 161,
    keyboard_key_clearagain = 162,
    keyboard_key_crsel = 163,
    keyboard_key_exsel = 164,

    keyboard_key_kp_00 = 176,
    keyboard_key_kp_000 = 177,
    keyboard_key_thousandsseparator = 178,
    keyboard_key_decimalseparator = 179,
    keyboard_key_currencyunit = 180,
    keyboard_key_currencysubunit = 181,
    keyboard_key_kp_leftparen = 182,
    keyboard_key_kp_rightparen = 183,
    keyboard_key_kp_leftbrace = 184,
    keyboard_key_kp_rightbrace = 185,
    keyboard_key_kp_tab = 186,
    keyboard_key_kp_backspace = 187,
    keyboard_key_kp_a = 188,
    keyboard_key_kp_b = 189,
    keyboard_key_kp_c = 190,
    keyboard_key_kp_d = 191,
    keyboard_key_kp_e = 192,
    keyboard_key_kp_f = 193,
    keyboard_key_kp_xor = 194,
    keyboard_key_kp_power = 195,
    keyboard_key_kp_percent = 196,
    keyboard_key_kp_less = 197,
    keyboard_key_kp_greater = 198,
    keyboard_key_kp_ampersand = 199,
    keyboard_key_kp_db_lampersand = 200,
    keyboard_key_kp_vericalbar = 201,
    keyboard_key_kp_dblverticalbar = 202,
    keyboard_key_kp_colon = 203,
    keyboard_key_kp_hash = 204,
    keyboard_key_kp_space = 205,
    keyboard_key_kp_at = 206,
    keyboard_key_kp_exclam = 207,
    keyboard_key_kp_memstore = 208,
    keyboard_key_kp_memrecall = 209,
    keyboard_key_kp_memclear = 210,
    keyboard_key_kp_memadd = 211,
    keyboard_key_kp_memsubtract = 212,
    keyboard_key_kp_memmultiply = 213,
    keyboard_key_kp_memdivide = 214,
    keyboard_key_kp_plusminus = 215,
    keyboard_key_kp_clear = 216,
    keyboard_key_kp_clearentry = 217,
    keyboard_key_kp_binary = 218,
    keyboard_key_kp_octal = 219,
    keyboard_key_kp_decimal = 220,
    keyboard_key_kp_hexadecimal = 221,

    keyboard_key_lctrl = 224,
    keyboard_key_lshift = 225,
    keyboard_key_lalt = 226,
    keyboard_key_lgui = 227,
    keyboard_key_rctrl = 228,
    keyboard_key_rshift = 229,
    keyboard_key_ralt = 230, // Alt gr
    keyboard_key_rgui = 231, // Windows key

    keyboard_di_count = 256,
    keyboard_key_mode = 257,  

    // Usage page 0x07
    keyboard_key_sleep = 258,                   
    keyboard_key_wake = 259,                    
    keyboard_key_channel_increment = 260,  
    keyboard_key_channel_decrement = 261,  

    keyboard_key_media_play = 262,
	keyboard_key_media_pause = 263,         
    keyboard_key_media_record = 264,   
    keyboard_key_media_fast_forward = 265,
    keyboard_key_media_rewind = 266,      
    keyboard_key_media_next_track = 267,  
    keyboard_key_media_previous_track = 268,
    keyboard_key_media_stop = 269,
	keyboard_key_media_eject = 270,         
    keyboard_key_media_play_pause = 271,    
    keyboard_key_media_select = 272,        

    keyboard_key_ac_new = 273,            
    keyboard_key_ac_open = 274,          
    keyboard_key_ac_close = 275,       
    keyboard_key_ac_exit = 276,            
    keyboard_key_ac_save = 277,           
    keyboard_key_ac_print = 278,     
    keyboard_key_ac_properties = 279,   

    keyboard_key_ac_search = 280,  
    keyboard_key_ac_home = 281,           
    keyboard_key_ac_back = 282,         
    keyboard_key_ac_forward = 283,        
    keyboard_key_ac_stop = 284,            
    keyboard_key_ac_refresh = 285,         
    keyboard_key_ac_bookmarks = 286,        

    // Add any other keys here

    keyboard_key_reserved = 400,    // 400-500 reserved for dynamic keycodes

    keyboard_key_count = 512
};

struct KeyboardState
{
	bool BtnState[keyboard_key_count] = { false };
};