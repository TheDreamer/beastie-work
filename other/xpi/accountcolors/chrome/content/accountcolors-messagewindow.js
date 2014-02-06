/************************************************************************/
/*                                                                      */
/*      Account Colors  -  Thunderbird Extension  -  Message Window     */
/*                                                                      */
/*      Javascript for Message Window overlay                           */
/*                                                                      */
/*      Copyright (C) 2008-2013  by  DW-dev                             */
/*                                                                      */
/*      Last Edit  -  19 Mar 2013                                       */
/*                                                                      */
/************************************************************************/

"use strict";

var accountColorsMessage =
{
    accountManager: Components.classes["@mozilla.org/messenger/account-manager;1"].getService(Components.interfaces.nsIMsgAccountManager),
    
    prefs: Components.classes["@mozilla.org/preferences-service;1"].getService(Components.interfaces.nsIPrefService).getBranch("extensions.accountcolors."),
    
    /* Listen for changes to settings */
    
    prefsObserver:
    {
        register: function()
        {
            /* Query nsIPrefBranch2 interface to observe changes to pref values */
            accountColorsMessage.prefs.QueryInterface(Components.interfaces.nsIPrefBranch2);
            
            /* Add the observer */
            accountColorsMessage.prefs.addObserver("",this,false);
        },
        
        observe: function(subject,topic,data)
        {
            var element;
            
            if (topic != "nsPref:changed") return;
            
            /* Update Message Pane & Message Tab */
            
            accountColorsMessage.messageWindow();
        }
    },
    
    /* Message Window */
    
    onLoad: function()
    {
        window.removeEventListener("load",accountColorsMessage.onLoad,false);
        
        /* Register preferences observer */
        
        accountColorsMessage.prefsObserver.register();
        
        /* Add listeners for Message Window */
        
        document.getElementById("messagepane").addEventListener("load",accountColorsMessage.messageWindow,true);
        
        /* Initial call for Message Window */
        
        accountColorsMessage.messageWindow();
    },
    
    messageWindow: function()
    {
        var msgHdr,accountkey,account,accountidkey,folder,server;
        var element,anonelement,fontcolor,bkgdcolor,fontstyle,fontsize;
        
        if (gFolderDisplay != null) msgHdr = gFolderDisplay.selectedMessage;  /* if message already displayed */
        else if ("wrappedJSObject" in window.arguments[0]) msgHdr = window.arguments[0].wrappedJSObject.msgHdr;
        else if (window.arguments[0] instanceof Components.interfaces.nsIMsgDBHdr) msgHdr = window.arguments[0];
        else return;
        
        if (accountColorsMessage.prefs.getBoolPref("message-hdraccount"))  /* color using account in message header */
        {
            accountkey = msgHdr.accountKey;  /* null string if sent message or IMAP received message */
            account = accountColorsMessage.accountManager.getAccount(accountkey);
            
            if (account == null) accountidkey = null;  /* sent message or IMAP received message */
            else if (account.defaultIdentity == null) accountidkey = account.key;
            else accountidkey = account.defaultIdentity.key;
        }
        else  /* color using account in which folder is located */
        {
            folder = msgHdr.folder;
            server = folder.server;
            account = accountColorsMessage.accountManager.FindAccountForServer(server);
            
            if (account.defaultIdentity == null) accountidkey = account.key;
            else accountidkey = account.defaultIdentity.key;
        }
        
        if (accountColorsMessage.prefs.getBoolPref("message-colorfont"))
        {
            fontcolor = accountColorsUtilities.fontColorPref(accountidkey);
            element = document.getElementById("expandedsubjectBox");
            anonelement = document.getAnonymousElementByAttribute(element,"class","headerValue");
            anonelement.style.color = fontcolor;
        }
        else
        {
            element = document.getElementById("expandedsubjectBox");
            anonelement = document.getAnonymousElementByAttribute(element,"class","headerValue");
            anonelement.style.color = "";
        }
        
        if (accountColorsMessage.prefs.getBoolPref("message-colorbkgd"))
        {
            bkgdcolor = accountColorsUtilities.bkgdColorPref(accountidkey);
            element = document.getElementById("expandedHeaderView");
            if (bkgdcolor == "#FFFFFF" && accountColorsMessage.prefs.getBoolPref("message-defaultbkgd")) element.style.backgroundColor = "";
            else element.style.backgroundColor = bkgdcolor;
        }
        else
        {
            element = document.getElementById("expandedHeaderView");
            element.style.backgroundColor = "";
        }
        
        if (accountColorsMessage.prefs.getBoolPref("message-setfontstyle"))
        {
            fontstyle = accountColorsMessage.prefs.getIntPref("message-fontstyle");
            element = document.getElementById("expandedsubjectBox");
            anonelement = document.getAnonymousElementByAttribute(element,"class","headerValue");
            switch (fontstyle)
            { 
                case 0: /* Normal */
                    anonelement.style.fontStyle = "normal";
                    anonelement.style.fontWeight = "normal";
                    break;
                case 1: /* Italic */
                    anonelement.style.fontStyle = "italic";
                    anonelement.style.fontWeight = "normal";
                    break;
                case 2: /* Bold */
                    anonelement.style.fontStyle = "normal";
                    anonelement.style.fontWeight = "bold";
                    break;
                case 3: /* Bold Italic */
                    anonelement.style.fontStyle = "italic";
                    anonelement.style.fontWeight = "bold";
                    break;
            }
        }
        else
        {
            element = document.getElementById("expandedsubjectBox");
            anonelement = document.getAnonymousElementByAttribute(element,"class","headerValue");
            anonelement.style.fontStyle = "";
            anonelement.style.fontWeight = "";
        }
        
        if (accountColorsMessage.prefs.getBoolPref("message-setfontsize"))
        {
            fontsize = accountColorsMessage.prefs.getIntPref("message-fontsize");
            element = document.getElementById("expandedsubjectBox");
            anonelement = document.getAnonymousElementByAttribute(element,"class","headerValue");
            anonelement.style.fontSize = fontsize+"px";
        }
        else
        {
            element = document.getElementById("expandedsubjectBox");
            anonelement = document.getAnonymousElementByAttribute(element,"class","headerValue");
            anonelement.style.fontSize = "";
        }
    }
};

window.addEventListener("load",accountColorsMessage.onLoad,false);   /* first */
//window.addEventListener("focus",accountColorsMessage.messageWindow,false);  /* second */
