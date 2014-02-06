/************************************************************************/
/*                                                                      */
/*      Account Colors  -  Thunderbird Extension  -  Utilities          */
/*                                                                      */
/*      Javascript for Utilities for all overlays                       */
/*                                                                      */
/*      Copyright (C) 2008-2013  by  DW-dev                             */
/*                                                                      */
/*      Last Edit  -  19 Mar 2013                                       */
/*                                                                      */
/************************************************************************/

"use strict";

var accountColorsUtilities =
{
    accountManager: Components.classes["@mozilla.org/messenger/account-manager;1"].getService(Components.interfaces.nsIMsgAccountManager),
    
    prefs: Components.classes["@mozilla.org/preferences-service;1"].getService(Components.interfaces.nsIPrefService).getBranch("extensions.accountcolors."),
    
    debugCount: 0,
    
    fontColorPref: function(accountidkey)
    {
        var fontcolor;
        
        if (accountColorsUtilities.findAccountId(accountidkey) < 128)
        {
            try
            {
                fontcolor = accountColorsUtilities.prefs.getCharPref(accountidkey+"-fontcolor")
            }
            catch (e)
            {
                fontcolor = "#000000";
            }
        }
        else
        {
            fontcolor = "#000000";
        }
        
        return fontcolor;
    },
    
    bkgdColorPref: function(accountidkey)
    {
        var bkgdcolor;
        
        if (accountColorsUtilities.findAccountId(accountidkey) < 128)
        {
            try
            {
                bkgdcolor = accountColorsUtilities.prefs.getCharPref(accountidkey+"-bkgdcolor")
            }
            catch (e)
            {
                bkgdcolor = "#FFFFFF";
            }
        }
        else
        {
            bkgdcolor = "#FFFFFF";
        }
        
        return bkgdcolor;
    },
    
    findAccountId: function(accountidkey)
    {
        var accounts,account,identities,identity,acc,id,index;
        
        index = 0;
        
        accounts = accountColorsUtilities.accountManager.accounts;
        
        for (acc = 0; acc < accountColorsUtilities.getLength(accounts); acc++)
        {
            account = accountColorsUtilities.getAccount(accounts,acc);
            
            identities = account.identities;
            
            if (accountColorsUtilities.getLength(identities) == 0)  /* Local Folders account or Blogs & Newsfeeds account */
            {
                if (accountidkey == account.key) return index;
                index++;
            }
            else
            {
                for (id = 0; id < accountColorsUtilities.getLength(identities); id++)
                {
                    identity = accountColorsUtilities.getIdentity(identities,id);
                    if (accountidkey == identity.key) return index;
                    index++;
                }
            }
        }
        
        return 10000;
    },
    
    getLength: function(items)
    {
        /* Thunderbird 20.0 - nsISupportsArray deprecated - length replaced Count()  */
        
        if (typeof items.length != "undefined") return items.length;
        else return items.Count();
    },
    
    getAccount: function(accounts,index)
    {
        /* Thunderbird 20.0 - nsISupportsArray deprecated - queryElementAt() replaced QueryElementAt()  */
        
        if (typeof accounts.length != "undefined") return accounts.queryElementAt(index,Components.interfaces.nsIMsgAccount);
        else return accounts.QueryElementAt(index,Components.interfaces.nsIMsgAccount);
    },
    
    getIdentity: function(identities,index)
    {
        /* Thunderbird 20.0 - nsISupportsArray deprecated - queryElementAt() replaced QueryElementAt()  */
        
        if (typeof identities.length != "undefined") return identities.queryElementAt(index,Components.interfaces.nsIMsgIdentity);
        else return identities.QueryElementAt(index,Components.interfaces.nsIMsgIdentity);
    },
    
    debugMessage: function(module,information)
    {
        var info;
        
        accountColorsUtilities.debugCount++;
        info = document.getElementById("accountcolors-debug-info");
        info.label = accountColorsUtilities.debugCount + " - " + module + " - " + information;
    }
};
