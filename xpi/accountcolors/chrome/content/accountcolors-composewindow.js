/************************************************************************/
/*                                                                      */
/*      Account Colors  -  Thunderbird Extension  -  Compose Window     */
/*                                                                      */
/*      Javascript for Compose Window overlay                           */
/*                                                                      */
/*      Copyright (C) 2008-2013  by  DW-dev                             */
/*                                                                      */
/*      Last Edit  -  25 May 2013                                       */
/*                                                                      */
/************************************************************************/

"use strict";

var accountColorsCompose =
{
    prefs: Components.classes["@mozilla.org/preferences-service;1"].getService(Components.interfaces.nsIPrefService).getBranch("extensions.accountcolors."),
    
    /* Listen for changes to settings */
    
    prefsObserver:
    {
        register: function()
        {
            /* Query nsIPrefBranch2 interface to observe changes to pref values */
            accountColorsCompose.prefs.QueryInterface(Components.interfaces.nsIPrefBranch2);
            
            /* Add the observer */
            accountColorsCompose.prefs.addObserver("",this,false);
        },
        
        observe: function(subject,topic,data)
        {
            if (topic != "nsPref:changed") return;
            
            /* Update Compose Window */
            
            accountColorsCompose.composeWindow();
        }
    },
    
    /* On Load */
    
    onLoad: function()
    {
        window.removeEventListener("load",accountColorsCompose.onLoad,false);
        
        /* Register preferences observer */
        
        accountColorsCompose.prefsObserver.register();
        
        /* Add listeners for Compose Window */
        
        window.addEventListener("compose-window-init",accountColorsCompose.composeWindow,true);
        window.addEventListener("compose-from-changed",accountColorsCompose.composeWindow,true);
        
        /* Initial call for Compose Window */
        
        accountColorsCompose.composeWindow();
    },
    
    /* Compose Window */
    
    composeWindow: function()
    {
        var i,menulist,menupopup,menuitem,accountidkey;
        var element,anonelement,bkgdcolor,fontcolor,fontstyle,fontsize;
        
        menulist = document.getElementById("msgIdentity");
        accountidkey = menulist.selectedItem.value;
        
        if (accountColorsCompose.prefs.getBoolPref("compose-colorbkgd"))
        {
            bkgdcolor = accountColorsUtilities.bkgdColorPref(accountidkey);
            element = document.getElementById("textcol-addressingWidget");
            element.style.backgroundColor = bkgdcolor;
            /* Can only change background-color by first setting -moz-appearance: none   */
            /* Then need to apply additional styling to imitate -moz-appearance: listbox */
            element = document.getElementById("attachmentBucket");
            element.setAttribute("style","-moz-appearance: none;" +
                                         "-moz-border-top-colors: none;" +
                                         "-moz-border-right-colors: none;" +
                                         "-moz-border-bottom-colors: none;" +
                                         "-moz-border-left-colors: none;" +
                                         "background-color: " + bkgdcolor + ";" +
                                         "border: 1px solid #828790;");
            /* Can only change background-color by first setting -moz-appearance: none     */
            /* Then need to apply additional styling to imitate -moz-appearance: textfield */
            element = document.getElementById("msgSubject");
            element.setAttribute("style","-moz-appearance: none;" +
                                         "-moz-border-top-colors: none;" +
                                         "-moz-border-right-colors: none;" +
                                         "-moz-border-bottom-colors: none;" +
                                         "-moz-border-left-colors: none;" +
                                         "background-color: " + bkgdcolor + ";" +
                                         "border-top: 1px solid #ABADB3;" +
                                         "border-right: 1px solid #DBDFE6;" +
                                         "border-bottom: 1px solid #E3E9EF;" +
                                         "border-left: 1px solid #E2E3EA;" +
                                         "-moz-border-radius: 2px;" +
                                         "padding: 1px 3px 2px;");
        }
        else
        {
            element = document.getElementById("typecol-addressingWidget");
            element.style.backgroundColor = "";
            element = document.getElementById("textcol-addressingWidget");
            element.style.backgroundColor = "";
            element = document.getElementById("attachmentBucket");
            element.removeAttribute("style");
            element = document.getElementById("msgSubject");
            element.removeAttribute("style");
        }
        
        /* Must be after -moz-appearance: none which forces font color to black */
        
        if (accountColorsCompose.prefs.getBoolPref("compose-colorfont"))
        {
            element = document.getElementById("msgSubject");
            element.style.color = accountColorsUtilities.fontColorPref(accountidkey);
        }
        else
        {
            element = document.getElementById("msgSubject");
            element.style.color = "";
        }
        
        if (accountColorsCompose.prefs.getBoolPref("compose-setfontstyle"))
        {
            fontstyle = accountColorsCompose.prefs.getIntPref("compose-fontstyle");
            element = document.getElementById("msgSubject");
            switch (fontstyle)
            { 
                case 0: /* Normal */
                    element.style.fontStyle = "normal";
                    element.style.fontWeight = "normal";
                    break;
                case 1: /* Italic */
                    element.style.fontStyle = "italic";
                    element.style.fontWeight = "normal";
                    break;
                case 2: /* Bold */
                    element.style.fontStyle = "normal";
                    element.style.fontWeight = "bold";
                    break;
                case 3: /* Bold Italic */
                    element.style.fontStyle = "italic";
                    element.style.fontWeight = "bold";
                    break;
            }
        }
        else
        {
            element = document.getElementById("msgSubject");
            element.style.fontStyle = "";
            element.style.fontWeight = "";
        }
        
        if (accountColorsCompose.prefs.getBoolPref("compose-setfontsize"))
        {
            fontsize = accountColorsCompose.prefs.getIntPref("compose-fontsize");
            element = document.getElementById("msgSubject");
            element.style.fontSize = fontsize+"px";
        }
        else
        {
            element = document.getElementById("msgSubject");
            element.style.fontSize = "";
        }
        
        if (accountColorsCompose.prefs.getBoolPref("compose-colorfrom"))
        {
            bkgdcolor = accountColorsUtilities.bkgdColorPref(accountidkey);
            element = document.getElementById("msgIdentity");
            anonelement = document.getAnonymousElementByAttribute(element,"class","menulist-label-box");
            anonelement.style.setProperty("color",accountColorsUtilities.fontColorPref(accountidkey),"important");
            if (bkgdcolor == "#FFFFFF" && accountColorsCompose.prefs.getBoolPref("compose-defaultbkgd")) anonelement.style.backgroundColor = "";
            else anonelement.style.setProperty("background-color",bkgdcolor,"important");
        }
        else
        {
            element = document.getElementById("msgIdentity");
            anonelement = document.getAnonymousElementByAttribute(element,"class","menulist-label-box");
            anonelement.style.color = "";
            anonelement.style.backgroundColor = "";
        }
        
        if (accountColorsCompose.prefs.getBoolPref("compose-colormenu"))
        {
            menupopup = document.getElementById("msgIdentityPopup");
            for (i = 0; i < menupopup.childNodes.length; i++)
            {
                menuitem = menupopup.childNodes[i];
                menuitem.style.color = accountColorsUtilities.fontColorPref(menuitem.value);
                menuitem.style.backgroundColor = accountColorsUtilities.bkgdColorPref(menuitem.value);
            }
        }
        else
        {
            menupopup = document.getElementById("msgIdentityPopup");
            for (i = 0; i < menupopup.childNodes.length; i++)
            {
                menuitem = menupopup.childNodes[i];
                menuitem.style.color = "";
                menuitem.style.backgroundColor = "";
            }
        }
        
        if (accountColorsCompose.prefs.getBoolPref("compose-setidfontstyle"))
        {
            fontstyle = accountColorsCompose.prefs.getIntPref("compose-idfontstyle");
            element = document.getElementById("msgIdentity");
            anonelement = document.getAnonymousElementByAttribute(element,"class","menulist-label");
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
            element = document.getElementById("msgIdentity");
            anonelement = document.getAnonymousElementByAttribute(element,"class","menulist-label");
            anonelement.style.fontStyle = "";
            anonelement.style.fontWeight = "";
        }
        
        if (accountColorsCompose.prefs.getBoolPref("compose-setidfontsize"))
        {
            fontsize = accountColorsCompose.prefs.getIntPref("compose-idfontsize");
            element = document.getElementById("msgIdentity");
            anonelement = document.getAnonymousElementByAttribute(element,"class","menulist-label");
            anonelement.style.fontSize = fontsize+"px";
            anonelement = document.getAnonymousElementByAttribute(element,"class","menulist-label menulist-description");
            anonelement.style.fontSize = fontsize+"px";
        }
        else
        {
            element = document.getElementById("msgIdentity");
            anonelement = document.getAnonymousElementByAttribute(element,"class","menulist-label");
            anonelement.style.fontSize = "";
            anonelement = document.getAnonymousElementByAttribute(element,"class","menulist-label menulist-description");
            anonelement.style.fontSize = "";
        }
    }
};

window.addEventListener("load",accountColorsCompose.onLoad,false);
