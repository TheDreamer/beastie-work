/************************************************************************/
/*                                                                      */
/*      Account Colors  -  Thunderbird Extension  -  3Pane Window       */
/*                                                                      */
/*      Javascript for 3Pane Window overlay                             */
/*                                                                      */
/*      Copyright (C) 2008-2013  by  DW-dev                             */
/*                                                                      */
/*      Last Edit  -  25 May 2013                                       */
/*                                                                      */
/************************************************************************/

"use strict";

var accountColors3Pane =
{
    accountManager: Components.classes["@mozilla.org/messenger/account-manager;1"].getService(Components.interfaces.nsIMsgAccountManager),
    
    atomServ: Components.classes["@mozilla.org/atom-service;1"].getService(Components.interfaces.nsIAtomService),
    
    prefs: Components.classes["@mozilla.org/preferences-service;1"].getService(Components.interfaces.nsIPrefService).getBranch("extensions.accountcolors."),
    
    mailPrefs: Components.classes["@mozilla.org/preferences-service;1"].getService(Components.interfaces.nsIPrefService).getBranch("mail."),
    
    otherPrefs: Components.classes["@mozilla.org/preferences-service;1"].getService(Components.interfaces.nsIPrefBranch),
    
    /* Listen for changes to accountcolors settings */
    
    prefsObserver:
    {
        register: function()
        {
            /* Query nsIPrefBranch2 interface to observe changes to pref values */
            accountColors3Pane.prefs.QueryInterface(Components.interfaces.nsIPrefBranch2);
            
            /* Add the observer */
            accountColors3Pane.prefs.addObserver("",this,false);
        },
        
        observe: function(subject,topic,data)
        {
            var boxobject,element;
            
            if (topic != "nsPref:changed") return;
            
            /* Generate CSS tree coloring rules */
            
            accountColors3Pane.generateRules();
            
            /* Update Folder Pane */
            
            accountColors3Pane.folderPane();
            
            boxobject = document.getElementById("folderTree").boxObject;  /* Force re-load of Folder Tree */ 
            boxobject.QueryInterface(Components.interfaces.nsITreeBoxObject);
            boxobject.invalidate();
            
            element = document.getElementById("folderTree");  /* Causes CSS Folder Tree -moz-tree-row height change to take effect */
            element.style.visibility = "hidden";
            element.style.visibility = "";
            
            /* Update Thread Pane */
            
            accountColors3Pane.threadPane();
            
            boxobject = document.getElementById("threadTree").boxObject;  /* Force re-load of Folder Tree */
            boxobject.QueryInterface(Components.interfaces.nsITreeBoxObject);
            boxobject.invalidate();
            
            element = document.getElementById("threadTree");  /* Causes CSS Thread Tree -moz-tree-row height change to take effect */
            element.style.visibility = "hidden";
            element.style.visibility = "";
            
            /* Update Message Pane & Message Tab */
            
            accountColors3Pane.messagePane();
        }
    },
    
    /* Listen for changes to mail settings - to detect add/remove account */
    
    mailPrefsObserver:
    {
        register: function()
        {
            /* Query nsIPrefBranch2 interface to observe changes to pref values */
            accountColors3Pane.mailPrefs.QueryInterface(Components.interfaces.nsIPrefBranch2);
            
            /* Add the observer */
            accountColors3Pane.mailPrefs.addObserver("",this,false);
        },
        
        observe: function(subject,topic,data)
        {
            if (topic != "nsPref:changed") return;
			
            /* Validate coloring preferences */
            
            accountColors3Pane.validatePrefs();
        }
    },
    
    /* On Load */
    
    onLoad: function()
    {
        window.removeEventListener("load",accountColors3Pane.onLoad,false);
        
        /* Determine Thunderbird theme and set attribute */
        
        document.getElementById("messengerWindow").setAttribute("accountcolors-tbtheme",accountColors3Pane.otherPrefs.getCharPref("general.skins.selectedSkin"));
        
        /* Validate coloring preferences */
        
        accountColors3Pane.validatePrefs();
        
        /* Generate CSS tree coloring rules */
        
        accountColors3Pane.generateRules();
        
        /* Register preferences observers */
        
        accountColors3Pane.prefsObserver.register();
        
        accountColors3Pane.mailPrefsObserver.register();
        
        /* Add listeners for Folder/Thread/Message Panes */
        
        window.addEventListener("load",accountColors3Pane.folderPane,false);
        
        document.getElementById("folderTree").addEventListener("select",accountColors3Pane.threadPane,false);
        
        //document.getElementById("threadTree").addEventListener("select",accountColors3Pane.messagePane,false);  /* changes color to soon */
        //document.getElementById("msgHeaderView").addEventListener("load",accountColors3Pane.messagePane,false);  /* not fired if no content */
        document.getElementById("tabmail").addEventListener("load",accountColors3Pane.messagePane,true);
        
        /* Setup for Folder/Thread Panes */
        
        accountColors3Pane.folderSetup();
        
        accountColors3Pane.threadSetup();
        
        /* Initial calls for Folder/Thread/Message Panes */
        
        accountColors3Pane.folderPane();
        
        accountColors3Pane.threadPane();
        
        accountColors3Pane.messagePane();
    },
    
    /* Validate account/identity font and background coloring preferences */
    
    validatePrefs: function()
    {
        var index,acc,account,id,identity,accountkey,identitykey;
        var accounts = new Array();
        var identities = new Array();
        var accountsprefs = new Array();
        var identitiesprefs = new Array();
        
        /* Add coloring preferences for added account */
        
        index = 0;
        
        accounts = accountColors3Pane.accountManager.accounts;
        
        for (acc = 0; acc < accountColorsUtilities.getLength(accounts) && index < 128; acc++)
        {
            account = accountColorsUtilities.getAccount(accounts,acc);
            
            identities = account.identities;
            
            if (accountColorsUtilities.getLength(identities) == 0)  /* Local Folders account or Blogs & Newsfeeds account */
            {
                if (!accountColors3Pane.prefs.prefHasUserValue(account.key + "-fontcolor") ||
                    !accountColors3Pane.prefs.prefHasUserValue(account.key + "-bkgdcolor"))
                {
                    accountColors3Pane.prefs.setCharPref(account.key + "-fontcolor","#000000");
                    accountColors3Pane.prefs.setCharPref(account.key + "-bkgdcolor","#FFFFFF");
                }
                
                index++;
            }
            else
            {
                for (id = 0; id < accountColorsUtilities.getLength(identities) && index < 128; id++)
                {
                    identity = accountColorsUtilities.getIdentity(identities,id);
                    
                    if (!accountColors3Pane.prefs.prefHasUserValue(identity.key + "-fontcolor") ||
                        !accountColors3Pane.prefs.prefHasUserValue(identity.key + "-bkgdcolor"))
                    {
                        accountColors3Pane.prefs.setCharPref(identity.key + "-fontcolor","#000000");
                        accountColors3Pane.prefs.setCharPref(identity.key + "-bkgdcolor","#FFFFFF");
                    }
                    
                    index++;
                }
            }
        }
        
        /* Remove coloring preferences for removed account */
        
        accountsprefs = accountColors3Pane.prefs.getChildList("account",{});
        
        for (acc = 0; acc < accountsprefs.length; acc++)
        {
            accountkey = accountsprefs[acc].substr(0,accountsprefs[acc].indexOf("-"));
            
            if (!accountColors3Pane.mailPrefs.prefHasUserValue("account." + accountkey + ".server") ||
                accountColors3Pane.mailPrefs.prefHasUserValue("account." + accountkey + ".identities"))
            {
                accountColors3Pane.prefs.clearUserPref(accountkey + "-fontcolor");
                accountColors3Pane.prefs.clearUserPref(accountkey + "-bkgdcolor");
            }
        }
        
        identitiesprefs = accountColors3Pane.prefs.getChildList("id",{});
        
        for (id = 0; id < identitiesprefs.length; id++)
        {
            identitykey = identitiesprefs[id].substr(0,identitiesprefs[id].indexOf("-"));
            
            if (!accountColors3Pane.mailPrefs.prefHasUserValue("identity." + identitykey + ".useremail"))
            {
                accountColors3Pane.prefs.clearUserPref(identitykey + "-fontcolor");
                accountColors3Pane.prefs.clearUserPref(identitykey + "-bkgdcolor");
            }
        }
    },
    
    /* Generate CSS coloring rules for folderTree and threadTree */
    
    generateRules: function()
    {
        var i,sheet,preftype,color;
        var children = new Array();
        
        for (i = 0; i < document.styleSheets.length; i++)
        {
            if (document.styleSheets[i].href == "chrome://accountcolors/skin/accountcolors-3panewindow-generated.css")
            {
                sheet = document.styleSheets[i];
            }
        }
        
        while (sheet.cssRules.length > 0) sheet.deleteRule(0);
        
        children = accountColors3Pane.prefs.getChildList("account",{});
        
        for (i = 0; i < children.length; i++)
        {
            preftype = accountColors3Pane.prefs.getPrefType(children[i]);
            
            color = accountColors3Pane.prefs.getCharPref(children[i]).substr(1);
            
            if (children[i].substr(children[i].length-9) == "fontcolor")
            {
                sheet.insertRule("#folderTree treechildren::-moz-tree-cell-text(ac-fc-" + color + ",unselected) { color: #" + color + "; }",sheet.cssRules.length);
                sheet.insertRule("#threadTree treechildren::-moz-tree-cell-text(ac-fc-" + color + ",unselected) { color: #" + color + "; }",sheet.cssRules.length);
            }
            else if (children[i].substr(children[i].length-9) == "bkgdcolor")
            {
                sheet.insertRule("#folderTree treechildren::-moz-tree-row(ac-bc-" + color + ",unselected) { background-color: #" + color + "; }",sheet.cssRules.length);
                sheet.insertRule("#threadTree treechildren::-moz-tree-row(ac-bc-" + color + ",unselected) { background-color: #" + color + "; }",sheet.cssRules.length);
            }
        }
        
        children = accountColors3Pane.prefs.getChildList("id",{});
        
        for (i = 0; i < children.length; i++)
        {
            preftype = accountColors3Pane.prefs.getPrefType(children[i]);
            
            color = accountColors3Pane.prefs.getCharPref(children[i]).substr(1);
            
            if (children[i].substr(children[i].length-9) == "fontcolor")
            {
                sheet.insertRule("#folderTree treechildren::-moz-tree-cell-text(ac-fc-" + color + ",unselected) { color: #" + color + "; }",sheet.cssRules.length);
                sheet.insertRule("#threadTree treechildren::-moz-tree-cell-text(ac-fc-" + color + ",unselected) { color: #" + color + "; }",sheet.cssRules.length);
            }
            else if (children[i].substr(children[i].length-9) == "bkgdcolor")
            {
                sheet.insertRule("#folderTree treechildren::-moz-tree-row(ac-bc-" + color + ",unselected) { background-color: #" + color + "; }",sheet.cssRules.length);
                sheet.insertRule("#threadTree treechildren::-moz-tree-row(ac-bc-" + color + ",unselected) { background-color: #" + color + "; }",sheet.cssRules.length);
            }
        }
    },
    
    /* Folder Pane in Main Tab */
    
    folderSetup: function()
    {
        /* replace getCellProperties in folderPane.js with new_getCellProperties */
        
        gFolderTreeView.accountcolors_old_getCellProperties = gFolderTreeView.getCellProperties;
        
        gFolderTreeView.getCellProperties =   
        function new_getCellProperties(row,col,props)
        {
            /* Thunderbird 22.0 - properties parameter removed - extra properties now returned as string */
            
            var atomflag,server,account,accountidkey;
            var fontcolor,fontstyle,fontsize;

            atomflag = (typeof props != "undefined");  /* props will be undefined for Thunderbird 22.0+ */
            
            if (!atomflag) props = gFolderTreeView.accountcolors_old_getCellProperties(row,col);  /* call original handler */
            else gFolderTreeView.accountcolors_old_getCellProperties(row,col,props);  /* call original handler */
            
            if (col.id == "folderNameCol")
            {
                gFolderTreeView._rowMap[row].getProperties(props,col);
                
                server = gFolderTreeView._rowMap[row]._folder.server;
                account = accountColors3Pane.accountManager.FindAccountForServer(server);
                
                if (account.defaultIdentity == null) accountidkey = account.key;
                else accountidkey = account.defaultIdentity.key;
                
                /* add extra properties for unselected, font color, font style, font size, folder font color, no bold on unread, and show tree lines */
                /* required to select tree element styles defined in ac-3panewindow.css */
                
                if (!gFolderTreeView.selection.isSelected(row))
                {
                    if (!atomflag) props += " " + "unselected";
                    else props.AppendElement(accountColors3Pane.atomServ.getAtom("unselected"));
                }
                
                if (gFolderTreeView._rowMap[row]._folder.isServer)
                {
                    if (accountColors3Pane.prefs.getBoolPref("folder-colorfont"))
                    {
                        fontcolor = accountColorsUtilities.fontColorPref(accountidkey);
                        if (!atomflag) props += " " + "ac-fc-"+fontcolor.substr(1,6);
                        else props.AppendElement(accountColors3Pane.atomServ.getAtom("ac-fc-"+fontcolor.substr(1,6)));
                    }
                    
                    if (accountColors3Pane.prefs.getBoolPref("folder-setfontstyle"))
                    {
                        fontstyle = accountColors3Pane.prefs.getIntPref("folder-fontstyle");
                        switch (fontstyle)
                        { 
                            case 0: /* Normal */
                                if (!atomflag) props += " " + "ac-fs-normal";
                                else props.AppendElement(accountColors3Pane.atomServ.getAtom("ac-fs-normal"));
                                break;
                            case 1: /* Italic */
                                if (!atomflag) props += " " + "ac-fs-italic";
                                else props.AppendElement(accountColors3Pane.atomServ.getAtom("ac-fs-italic"));
                                break;
                            case 2: /* Bold */
                                if (!atomflag) props += " " + "ac-fs-bold";
                                else props.AppendElement(accountColors3Pane.atomServ.getAtom("ac-fs-bold"));
                                break;
                              case 3: /* Bold Italic */
                                if (!atomflag) props += " " + "ac-fs-bolditalic";
                                else props.AppendElement(accountColors3Pane.atomServ.getAtom("ac-fs-bolditalic"));
                              break;
                        }
                    }
                    
                    if (accountColors3Pane.prefs.getBoolPref("folder-setfontsize"))
                    {
                        if (accountColors3Pane.prefs.getBoolPref("folder-incspacing"))
                        {
                            fontsize = accountColors3Pane.prefs.getIntPref("folder-fontsize");
                            if (!atomflag) props += " " + "ac-fs-"+fontsize+"-is";
                            else props.AppendElement(accountColors3Pane.atomServ.getAtom("ac-fs-"+fontsize+"-is"));
                        }
                        else
                        {
                            fontsize = accountColors3Pane.prefs.getIntPref("folder-fontsize");
                            if (!atomflag) props += " " + "ac-fs-"+fontsize;
                            else props.AppendElement(accountColors3Pane.atomServ.getAtom("ac-fs-"+fontsize));
                        }
                    }
                } 
                else
                {
                    if (accountColors3Pane.prefs.getBoolPref("folder-colorfldfont"))
                    {
                        fontcolor = accountColorsUtilities.fontColorPref(accountidkey);
                        if (!atomflag) props += " " + "ac-fc-"+fontcolor.substr(1,6);
                        else props.AppendElement(accountColors3Pane.atomServ.getAtom("ac-fc-"+fontcolor.substr(1,6)));
                    }
                    
                    if (accountColors3Pane.prefs.getBoolPref("folder-noboldunread"))
                    {
                        if (!atomflag) props += " " + "ac-noboldunread";
                        else props.AppendElement(accountColors3Pane.atomServ.getAtom("ac-noboldunread"));
                    }
                } 
                
                if (accountColors3Pane.prefs.getBoolPref("folder-showlines"))
                {
                    if (!atomflag) props += " " + "ac-showlines";
                    else props.AppendElement(accountColors3Pane.atomServ.getAtom("ac-showlines"));
                }
            }
            
            if (!atomflag) return props;
        };
        
        /* replace getRowProperties in folderPane.js with new_getRowProperties */
        
        gFolderTreeView.accountcolors_old_getRowProperties = gFolderTreeView.getRowProperties;
        
        gFolderTreeView.getRowProperties =   
        function new_getRowProperties(row,props)
        {
            /* Thunderbird 22.0 - properties parameter removed - extra properties now returned as string */
            
            var atomflag,server,account,accountidkey;
            var bkgdcolor;
            
            atomflag = (typeof props != "undefined");  /* props will be undefined for Thunderbird 22.0+ */
            
            if (!atomflag) props = gFolderTreeView.accountcolors_old_getRowProperties(row);  /* call original handler */
            else gFolderTreeView.accountcolors_old_getRowProperties(row,props);  /* call original handler */
            
            gFolderTreeView._rowMap[row].getProperties(props);
            
            server = gFolderTreeView._rowMap[row]._folder.server;
            account = accountColors3Pane.accountManager.FindAccountForServer(server);
            
            if (account.defaultIdentity == null) accountidkey = account.key;
            else accountidkey = account.defaultIdentity.key;
            
            /* add extra properties for unselected, unfocused, background color, folder background color and darker selection bar */
            /* required to select tree element styles defined in accountcolors-3panewindow.css */
            
            if (!gFolderTreeView.selection.isSelected(row))
            {
                if (!atomflag) props += " " + "unselected";
                else props.AppendElement(accountColors3Pane.atomServ.getAtom("unselected"));
            }
            
            if (!document.getElementById("folderTree").treeBoxObject.focused)
            {
                if (!atomflag) props += " " + "unfocused";
                else props.AppendElement(accountColors3Pane.atomServ.getAtom("unfocused"));
            }
            
            if (gFolderTreeView._rowMap[row]._folder.isServer)
            {
                if (accountColors3Pane.prefs.getBoolPref("folder-colorbkgd"))
                {
                    bkgdcolor = accountColorsUtilities.bkgdColorPref(accountidkey);
                    if (bkgdcolor != "#FFFFFF" || !accountColors3Pane.prefs.getBoolPref("folder-defaultbkgd"))
                    {
                        if (!atomflag) props += " " + "ac-bc-"+bkgdcolor.substr(1,6);
                        else props.AppendElement(accountColors3Pane.atomServ.getAtom("ac-bc-"+bkgdcolor.substr(1,6)));
                    }
                }
            }
            else
            {
                if (accountColors3Pane.prefs.getBoolPref("folder-colorfldbkgd"))
                {
                    bkgdcolor = accountColorsUtilities.bkgdColorPref(accountidkey);
                    if (bkgdcolor != "#FFFFFF" || !accountColors3Pane.prefs.getBoolPref("folder-defaultbkgd"))
                    {
                        if (!atomflag) props += " " + "ac-bc-"+bkgdcolor.substr(1,6);
                        else props.AppendElement(accountColors3Pane.atomServ.getAtom("ac-bc-"+bkgdcolor.substr(1,6)));
                    }
                }
            }
            
            if (accountColors3Pane.prefs.getBoolPref("folder-darkerbar"))
            {
                if (!atomflag) props += " " + "ac-darkerbar";
                else props.AppendElement(accountColors3Pane.atomServ.getAtom("ac-darkerbar"));
            }
            
            if (!atomflag) return props;
        };
    },
    
    folderPane: function()
    {
        var element,fontsize;
        
        if (accountColors3Pane.prefs.getBoolPref("folder-boldnewmail"))
        {
            element = document.getElementById("folderTree");
            element.setAttribute("ac-boldnewmail","");
        }
        else
        {
            element = document.getElementById("folderTree");
            element.removeAttribute("ac-boldnewmail");
        }
        
        if (accountColors3Pane.prefs.getBoolPref("folder-undernewmail"))
        {
            element = document.getElementById("folderTree");
            element.setAttribute("ac-undernewmail","");
        }
        else
        {
            element = document.getElementById("folderTree");
            element.removeAttribute("ac-undernewmail");
        }
        
        if (accountColors3Pane.prefs.getBoolPref("folder-incspacing"))
        {
            fontsize = accountColors3Pane.prefs.getIntPref("folder-fontsize");
            element = document.getElementById("folderTree");
            element.setAttribute("ac-is",fontsize);
        }
        else
        {
            element = document.getElementById("folderTree");
            element.removeAttribute("ac-is");
        }
        
        element = document.getElementById("folderTree");  /* Causes CSS Folder Tree -moz-tree-row height change to take effect */
        element.style.visibility = "hidden";
        element.style.visibility = "";
    },
    
    /* Thread Pane in Main Tab */
    
    threadSetup: function()
    {
        var obsServ;
        
        /* Observer for creation of new view */
        
        obsServ = Components.classes["@mozilla.org/observer-service;1"].getService(Components.interfaces.nsIObserverService);
        obsServ.addObserver(accountColors3Pane.threadDBObserver,"MsgCreateDBView",false);
    },
    
    threadDBObserver:
    {
        observe: function(msgfolder,topic,data)
        { 
            var i,col,tree;
            
            /* Add same column handler for subjectCol and senderCol columns in Thread Tree */
            
            gDBView.addColumnHandler("subjectCol",accountColors3Pane.threadColumnHandler);
            gDBView.addColumnHandler("senderCol",accountColors3Pane.threadColumnHandler);
            gDBView.addColumnHandler("dateCol",accountColors3Pane.threadColumnHandler);
            gDBView.addColumnHandler("sizeCol",accountColors3Pane.threadColumnHandler);
            gDBView.addColumnHandler("accountCol",accountColors3Pane.threadColumnHandler);
            gDBView.addColumnHandler("priorityCol",accountColors3Pane.threadColumnHandler);
        }
    }, 
    
    threadColumnHandler:
    {
        /* Functions for nsITreeView */
        
        getCellProperties: function(row,col,props)
        {
            /* Thunderbird 22.0 - properties parameter removed - extra properties now returned as string - existing properties no longer passed in */
            
            var atomflag,msgHdr,msgUri,accountkey,account,accountidkey,folder,server;
            var fontcolor,fontstyle,fontsize,fromcolor,fromstyle,fromsize,othercolor;
            
            atomflag = (typeof props != "undefined");  /* props will be undefined for Thunderbird 22.0+ */
            
            if (!atomflag) props = " ";
            
            msgUri = gDBView.getURIForViewIndex(row);
            msgHdr = messenger.msgHdrFromURI(msgUri);
            
            if (accountColors3Pane.prefs.getBoolPref("thread-hdraccount"))  /* color using account in message header */
            {
                accountkey = msgHdr.accountKey;  /* null string if sent message or IMAP received message */
                account = accountColors3Pane.accountManager.getAccount(accountkey);
                
                if (account == null) accountidkey = null;  /* sent message or IMAP received message */
                else if (account.defaultIdentity == null) accountidkey = account.key;
                else accountidkey = account.defaultIdentity.key;
            }
            else  /* color using account in which folder is located */
            {
                folder = msgHdr.folder;
                server = folder.server;
                account = accountColors3Pane.accountManager.FindAccountForServer(server);

                if (account.defaultIdentity == null) accountidkey = account.key;
                else accountidkey = account.defaultIdentity.key;
            }
            
            if (col.id == "subjectCol")
            {
                /* add extra properties for unselected, font color, font style, font size and bold subject */
                /* required to select tree element styles defined in accountcolors-3panewindow.css */
                
                if (!gDBView.selection.isSelected(row))
                {
                    if (!atomflag) props += " " + "unselected";
                    else props.AppendElement(accountColors3Pane.atomServ.getAtom("unselected"));
                }
                
                if (accountColors3Pane.prefs.getBoolPref("thread-colorfont"))
                {
                    fontcolor = accountColorsUtilities.fontColorPref(accountidkey);
                    if (!atomflag) props += " " + "ac-fc-"+fontcolor.substr(1,6);
                    else props.AppendElement(accountColors3Pane.atomServ.getAtom("ac-fc-"+fontcolor.substr(1,6)));
                }
                
                if (accountColors3Pane.prefs.getBoolPref("thread-setfontstyle"))
                {
                    fontstyle = accountColors3Pane.prefs.getIntPref("thread-fontstyle");
                    switch (fontstyle)
                    { 
                        case 0: /* Normal */
                            if (!atomflag) props += " " + "ac-fs-normal";
                            else props.AppendElement(accountColors3Pane.atomServ.getAtom("ac-fs-normal"));
                            break;
                        case 1: /* Italic */
                            if (!atomflag) props += " " + "ac-fs-italic";
                            else props.AppendElement(accountColors3Pane.atomServ.getAtom("ac-fs-italic"));
                            break;
                        case 2: /* Bold */
                            if (!atomflag) props += " " + "ac-fs-bold";
                            else props.AppendElement(accountColors3Pane.atomServ.getAtom("ac-fs-bold"));
                            break;
                        case 3: /* Bold Italic */
                            if (!atomflag) props += " " + "ac-fs-bolditalic";
                            else props.AppendElement(accountColors3Pane.atomServ.getAtom("ac-fs-bolditalic"));
                            break;
                    }
                }
                
                if (accountColors3Pane.prefs.getBoolPref("thread-setfontsize"))
                {
                    if (accountColors3Pane.prefs.getBoolPref("thread-incspacing"))
                    {
                        fontsize = accountColors3Pane.prefs.getIntPref("thread-fontsize");
                        if (!atomflag) props += " " + "ac-fs-"+fontsize+"-is";
                        else props.AppendElement(accountColors3Pane.atomServ.getAtom("ac-fs-"+fontsize+"-is"));
                    }
                    else
                    {
                        fontsize = accountColors3Pane.prefs.getIntPref("thread-fontsize");
                        if (!atomflag) props += " " + "ac-fs-"+fontsize;
                        else props.AppendElement(accountColors3Pane.atomServ.getAtom("ac-fs-"+fontsize));
                    }
                }
                
                if (accountColors3Pane.prefs.getBoolPref("thread-boldsubject"))
                {
                    if (!atomflag) props += " " + "ac-boldsubject";
                    else props.AppendElement(accountColors3Pane.atomServ.getAtom("ac-boldsubject"));
                }
            }
            else if (col.id == "senderCol")
            {
                /* add extra properties for unselected, from color, from style, from size and bold */
                /* required to select tree element styles defined in accountcolors-3panewindow.css */
                
                if (!gDBView.selection.isSelected(row))
                {
                    if (!atomflag) props += " " + "unselected";
                    else props.AppendElement(accountColors3Pane.atomServ.getAtom("unselected"));
                }
                
                if (accountColors3Pane.prefs.getBoolPref("thread-colorfrom"))
                {
                    fromcolor = accountColorsUtilities.fontColorPref(accountidkey);
                    if (!atomflag) props += " " + "ac-fc-"+fromcolor.substr(1,6);
                    else props.AppendElement(accountColors3Pane.atomServ.getAtom("ac-fc-"+fromcolor.substr(1,6)));
                }
                
                if (accountColors3Pane.prefs.getBoolPref("thread-setfromstyle"))
                {
                    fromstyle = accountColors3Pane.prefs.getIntPref("thread-fromstyle");
                    switch (fromstyle)
                    { 
                        case 0: /* Normal */
                            if (!atomflag) props += " " + "ac-fs-normal";
                            else props.AppendElement(accountColors3Pane.atomServ.getAtom("ac-fs-normal"));
                            break;
                        case 1: /* Italic */
                            if (!atomflag) props += " " + "ac-fs-italic";
                            else props.AppendElement(accountColors3Pane.atomServ.getAtom("ac-fs-italic"));
                            break;
                        case 2: /* Bold */
                            if (!atomflag) props += " " + "ac-fs-bold";
                            else props.AppendElement(accountColors3Pane.atomServ.getAtom("ac-fs-bold"));
                            break;
                        case 3: /* Bold Italic */
                            if (!atomflag) props += " " + "ac-fs-bolditalic";
                            else props.AppendElement(accountColors3Pane.atomServ.getAtom("ac-fs-bolditalic"));
                            break;
                    }
                }
                
                if (accountColors3Pane.prefs.getBoolPref("thread-setfromsize"))
                {
                    if (accountColors3Pane.prefs.getBoolPref("thread-incspacing"))
                    {
                        fromsize = accountColors3Pane.prefs.getIntPref("thread-fromsize");
                        if (!atomflag) props += " " + "ac-fs-"+fromsize+"-is";
                        else props.AppendElement(accountColors3Pane.atomServ.getAtom("ac-fs-"+fromsize+"-is"));
                    }
                    else
                    {
                        fromsize = accountColors3Pane.prefs.getIntPref("thread-fromsize");
                        if (!atomflag) props += " " + "ac-fs-"+fromsize;
                        else props.AppendElement(accountColors3Pane.atomServ.getAtom("ac-fs-"+fromsize));
                    }
                }
                
                if (accountColors3Pane.prefs.getBoolPref("thread-boldfrom"))
                {
                    if (!atomflag) props += " " + "ac-boldfrom";
                    else props.AppendElement(accountColors3Pane.atomServ.getAtom("ac-boldfrom"));
                }
            }
            else if (col.id == "dateCol" || col.id == "sizeCol" || col.id == "accountCol" || col.id == "priorityCol")
            {
                /* add extra properties for unselected, date/size/account/priority (other) color */
                /* required to select tree element styles defined in accountcolors-3panewindow.css */
                
                if (!gDBView.selection.isSelected(row))
                {
                    if (!atomflag) props += " " + "unselected";
                    else props.AppendElement(accountColors3Pane.atomServ.getAtom("unselected"));
                }
                
                if (accountColors3Pane.prefs.getBoolPref("thread-colorother"))
                {
                    othercolor = accountColorsUtilities.fontColorPref(accountidkey);
                    if (!atomflag) props += " " + "ac-fc-"+othercolor.substr(1,6);
                    else props.AppendElement(accountColors3Pane.atomServ.getAtom("ac-fc-"+othercolor.substr(1,6)));
                }
            }
            
            if (!atomflag) return props;
        },
        
        getRowProperties: function(row,props)
        {
            /* Thunderbird 22.0 - properties parameter removed - extra properties now returned as string - existing properties no longer passed in */
            
            var atomflag,msgHdr,msgUri,accountkey,account,accountidkey,folder,server;
            var bkgdcolor;
            
            atomflag = (typeof props != "undefined");  /* props will be undefined for Thunderbird 22.0+ */
            
            if (!atomflag) props = " ";
            
            msgUri = gDBView.getURIForViewIndex(row);
            msgHdr = messenger.msgHdrFromURI(msgUri);
            
            if (accountColors3Pane.prefs.getBoolPref("thread-hdraccount"))  /* color using account in message header */
            {
                accountkey = msgHdr.accountKey;  /* null string if sent message or IMAP received message */
                account = accountColors3Pane.accountManager.getAccount(accountkey);
                
                if (account == null) accountidkey = null;  /* sent message or IMAP received message */
                else if (account.defaultIdentity == null) accountidkey = account.key;
                else accountidkey = account.defaultIdentity.key;
            }
            else  /* color using account in which folder is located */
            {
                folder = msgHdr.folder;
                server = folder.server;
                account = accountColors3Pane.accountManager.FindAccountForServer(server);
                
                if (account.defaultIdentity == null) accountidkey = account.key;
                else accountidkey = account.defaultIdentity.key;
            }
            
            /* add extra properties for unselected, unfocused, background color, show row stripes, darker selection bar */
            /* required to select tree element styles defined in accountcolors-3panewindow.css */
            
            if (!gDBView.selection.isSelected(row))
            {
                if (!atomflag) props += " " + "unselected";
                else props.AppendElement(accountColors3Pane.atomServ.getAtom("unselected"));
            }
            
            if (!document.getElementById("threadTree").treeBoxObject.focused)
            {
                if (!atomflag) props += " " + "unfocused";
                else props.AppendElement(accountColors3Pane.atomServ.getAtom("unfocused"));
            }
            
            if (accountColors3Pane.prefs.getBoolPref("thread-colorbkgd"))
            {
                bkgdcolor = accountColorsUtilities.bkgdColorPref(accountidkey);
                if (!atomflag) props += " " + "ac-bc-"+bkgdcolor.substr(1,6);
                else props.AppendElement(accountColors3Pane.atomServ.getAtom("ac-bc-"+bkgdcolor.substr(1,6)));
            }
            
            if (accountColors3Pane.prefs.getBoolPref("thread-showstripes"))
            {
                if (!atomflag) props += " " + "ac-showstripes";
                else props.AppendElement(accountColors3Pane.atomServ.getAtom("ac-showstripes"));
            }
            
            if (accountColors3Pane.prefs.getBoolPref("thread-darkerbar"))
            {
                if (!atomflag) props += " " + "ac-darkerbar";
                else props.AppendElement(accountColors3Pane.atomServ.getAtom("ac-darkerbar"));
            }
            
            if (!atomflag) return props;
        },
        
        getImageSrc: function(row,col)
        {
            /* original handler always returns nothing */
            
            return null;
        },
        
        getCellText: function(row,col)
        {
            /* original handler called */
            
            var value;
            
            gDBView.removeColumnHandler(col.id);
            value = gDBView.getCellText(row,col);
            gDBView.addColumnHandler(col.id,this);
            return value;
        },
        
        cycleCell: function(row,col)
        {
            /* original handler called */
            
            var value;
            
            gDBView.removeColumnHandler(col.id);
            value = gDBView.cycleCell(row,col);
            gDBView.addColumnHandler(col.id,this);
            return;
        },
        
        isEditable: function(row,col)
        {
            /* original handler always returns false */
            
            return false;
        },
        
        /* Functions for nsIMsgCustomColumnHandler */
        
        getSortStringForRow: function(hdr)
        {
            /* custom handler not called for standard columns */
            
            return "";
        },
        
        getSortLongForRow: function(hdr)
        {
            /* custom handler not called for standard columns */
            
            return 0;
        },
        
        isString: function()
        {
            /* custom handler not called for standard columns */
            
            return true;
        }
    },
    
    threadPane: function()
    {
        var element,fontsize;
        
        if (accountColors3Pane.prefs.getBoolPref("thread-boldsubject") || accountColors3Pane.prefs.getBoolPref("thread-boldfrom"))
        {
            element = document.getElementById("threadTree");
            element.setAttribute("ac-boldsubjectfrom","");
        }
        else
        {
            element = document.getElementById("threadTree");
            element.removeAttribute("ac-boldsubjectfrom");
        }
        
        if (accountColors3Pane.prefs.getBoolPref("thread-incspacing"))
        {
            fontsize = accountColors3Pane.prefs.getIntPref("thread-fontsize");
            element = document.getElementById("threadTree");
            element.setAttribute("ac-is",fontsize);
        }
        else
        {
            element = document.getElementById("threadTree");
            element.removeAttribute("ac-is");
        }
        
        element = document.getElementById("threadTree");  /* Causes CSS Thread Tree -moz-tree-row height change to take effect */
        element.style.visibility = "hidden";
        element.style.visibility = "";
    },
    
    /* Message Pane in Main Tab, Message Tab or Conversation Tab */
    
    messagePane: function()
    {
        var msgHdr,msgUri,accountkey,account,accountidkey,folder,server;
        var element,anonelement,fontcolor,bkgdcolor,fontsize,fontstyle;
        
        try
        { 
            msgUri = gMessageDisplay.folderDisplay.selectedMessageUris[0];  
        }
        catch (e)
        {
            return;  /* message not selected */
        }
        
        msgHdr = messenger.msgHdrFromURI(msgUri);
        
        if (accountColors3Pane.prefs.getBoolPref("message-hdraccount"))  /* color using account in message header */
        {
            accountkey = msgHdr.accountKey;  /* null string if sent message or IMAP received message */
            account = accountColors3Pane.accountManager.getAccount(accountkey);
            
            if (account == null) accountidkey = null;  /* sent message or IMAP received message */
            else if (account.defaultIdentity == null) accountidkey = account.key;
            else accountidkey = account.defaultIdentity.key;
        }
        else  /* color using account in which folder is located */
        {
            folder = msgHdr.folder;
            server = folder.server;
            account = accountColors3Pane.accountManager.FindAccountForServer(server);
            
            if (account.defaultIdentity == null) accountidkey = account.key;
            else accountidkey = account.defaultIdentity.key;
        }
        
        if (accountColors3Pane.prefs.getBoolPref("message-colorfont"))
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
        
        if (accountColors3Pane.prefs.getBoolPref("message-colorbkgd"))
        {
            bkgdcolor = accountColorsUtilities.bkgdColorPref(accountidkey);
            element = document.getElementById("expandedHeaderView");
            if (bkgdcolor == "#FFFFFF" && accountColors3Pane.prefs.getBoolPref("message-defaultbkgd")) element.style.backgroundColor = "";
            else element.style.backgroundColor = bkgdcolor;
        }
        else
        {
            element = document.getElementById("expandedHeaderView");
            element.style.backgroundColor = "";
        }
        
        if (accountColors3Pane.prefs.getBoolPref("message-setfontstyle"))
        {
            fontstyle = accountColors3Pane.prefs.getIntPref("message-fontstyle");
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
        
        if (accountColors3Pane.prefs.getBoolPref("message-setfontsize"))
        {
            fontsize = accountColors3Pane.prefs.getIntPref("message-fontsize");
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

window.addEventListener("load",accountColors3Pane.onLoad,false);
