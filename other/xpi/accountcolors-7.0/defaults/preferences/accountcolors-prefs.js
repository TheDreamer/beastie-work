/************************************************************************/
/*                                                                      */
/*      Account Colors  -  Thunderbird Extension  -  Preferences        */
/*                                                                      */
/*      Javascript for Preferences defaults                             */
/*                                                                      */
/*      Copyright (C) 2008-2014  by  DW-dev                             */
/*                                                                      */
/*      Last Edit  -  19 Feb 2014                                       */
/*                                                                      */
/************************************************************************/

/* defaults for accountidkey-fontcolor are handled in accountcolors-utilities.js */
/* defaults for accountidkey-bkgdcolor are handled in accountcolors-utilities.js */

/* Color Picker */

pref("extensions.accountcolors.picker-autobkgd",false);

/* Folder Pane */

pref("extensions.accountcolors.folder-setfontstyle",false);
pref("extensions.accountcolors.folder-fontstyle",2);
pref("extensions.accountcolors.folder-setfontsize",false);
pref("extensions.accountcolors.folder-fontsize",12);
pref("extensions.accountcolors.folder-colorfont",true);
pref("extensions.accountcolors.folder-colorbkgd",true);
pref("extensions.accountcolors.folder-colorfldfont",true);
pref("extensions.accountcolors.folder-colorfldbkgd",true);
pref("extensions.accountcolors.folder-boldnewmail",false);
pref("extensions.accountcolors.folder-undernewmail",false);
pref("extensions.accountcolors.folder-noboldunread",false);
pref("extensions.accountcolors.folder-showlines",false);
pref("extensions.accountcolors.folder-darkerbar",false);
pref("extensions.accountcolors.folder-incspacing",false);
pref("extensions.accountcolors.folder-defaultbkgd",false);

/* Thread Pane */

pref("extensions.accountcolors.thread-setfontstyle",false);
pref("extensions.accountcolors.thread-fontstyle",0);
pref("extensions.accountcolors.thread-setfontsize",false);
pref("extensions.accountcolors.thread-fontsize",12);
pref("extensions.accountcolors.thread-setfromstyle",false);
pref("extensions.accountcolors.thread-fromstyle",0);
pref("extensions.accountcolors.thread-setfromsize",false);
pref("extensions.accountcolors.thread-fromsize",12);
pref("extensions.accountcolors.thread-colorfont",false);
pref("extensions.accountcolors.thread-colorbkgd",false);
pref("extensions.accountcolors.thread-colorfrom",false);
pref("extensions.accountcolors.thread-colorother",false);
pref("extensions.accountcolors.thread-boldsubject",false);
pref("extensions.accountcolors.thread-boldfrom",false);
pref("extensions.accountcolors.thread-showstripes",false);
pref("extensions.accountcolors.thread-darkerbar",false);
pref("extensions.accountcolors.thread-incspacing",false);
pref("extensions.accountcolors.thread-hdraccount",false);

/* Message Pane/Tab/Window */

pref("extensions.accountcolors.message-setfontstyle",false);
pref("extensions.accountcolors.message-fontstyle",2);
pref("extensions.accountcolors.message-setfontsize",false);
pref("extensions.accountcolors.message-fontsize",12);
pref("extensions.accountcolors.message-colorfont",false);
pref("extensions.accountcolors.message-colorbkgd",false);
pref("extensions.accountcolors.message-hdraccount",false);
pref("extensions.accountcolors.message-defaultbkgd",false);

/* Compose Window */

pref("extensions.accountcolors.compose-setfontstyle",false);
pref("extensions.accountcolors.compose-fontstyle",0);
pref("extensions.accountcolors.compose-setfontsize",false);
pref("extensions.accountcolors.compose-fontsize",12);
pref("extensions.accountcolors.compose-setidfontstyle",false);
pref("extensions.accountcolors.compose-idfontstyle",0);
pref("extensions.accountcolors.compose-setidfontsize",false);
pref("extensions.accountcolors.compose-idfontsize",12);
pref("extensions.accountcolors.compose-colorfont",false);
pref("extensions.accountcolors.compose-colorbkgd",false);
pref("extensions.accountcolors.compose-colorfieldbkgd",false);
pref("extensions.accountcolors.compose-colorfromfont",false);
pref("extensions.accountcolors.compose-colorfrombkgd",false);
pref("extensions.accountcolors.compose-defaultbkgd",false);

/* For migration to Version 4.0 */

pref("extensions.accountcolors.install-migrated",false);

/* For migration to Version 7.0 */

pref("extensions.accountcolors.install-migrated70",false);
pref("extensions.accountcolors.compose-colorfrom",false);
