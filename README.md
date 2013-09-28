beastie-work
============
Somewhere to save the FreeBSD ports that I'm fiddling with...

	2: Well, you've been fiddling with it, haven't you?
	3: It was perfectly all right until you touched it.
	   Now if only you'd leave things to me.
	2: If we were to leave things with you, my dear fellow,
	   we'd be in a fine pickle wouldn't we?

ports
=====
www/p5-WWW-Shorten-Yourls

	www/yourls exists in ports, but not this Perl module?

sysutils/grive

	I got grive-0.2.0 to build on FreeBSD, and thought about turning
	it into a port.  But, I think I'm out of my depth on whether this
	is even right.  Plus I don't think I want to be stuck maintain this.

sysutils/grive-devel

	I took a look at the current work on grive-0.3.0 on github, but it
	needs at least 0.10 of devel/json-c and ports stops at 0.9.  I had
	started work on making a patch to update the port, but sounds like
	development is heading towards needing 0.11 now.

	I then ran into it needing a new version of devel/boost-libs than
	currently in ports.  Given the difficulty I've had in buidling as
	a dependency of other ports, I don't feel like tackling this update.

	Plus I still haven't figured out how to do distfiles from GitHub,
	MASTER_SITES=GH

	Otherwise, I might reveal my stab at print/cups-cloud-print.

devel/json-c

	This was to form the patch for a PR to get json-c-0.9 updated to
	json-c-0.10, though the latest is json-c-0.11.

print/cups-cloud-print

	While researching options for replacing my old inkjet printer, and
	the apparently obsession of vendors to do GDI and Windows only...
	though some support Mac OS X and some might add some Linx support.

	Since I already ran into this mistake with my laser printer, which
	I hardly use now.  Though I run a small ubuntu VM on one of my
	FreeBSD systems to make use of it.  It appears unlikely that I'll
	have Windows again, and no idea when a Mac will become a reality.

	One of the draws to the idea of working for a University was to
	get discounted Apple hardware, but I've yet to get anything this way
	in over 7 years.

	I had seen printers with direct Google Cloud printing support, but
	the capability disappeared from Chromium on FreeBSD some time ago.
	I do have the capability to print to my printers at home, since I
	own a ChromeBook.  Again it involves running chrome in that small
	ubuntu VM.

	So, I had gotten a version of this to build and run.  And, I
	successfully printed a web page from my browser on FreeBSD, out to
	Google, back to chrome running in VM on same computer, to printer
	sitting next to said computer.  Though probably wouldn't be any
	less weird if I did get a printer with direct cloud printing support.

	The port probably needs a lot more work to get it to FreeBSD standards,
	but this work staled on trying to figure out how MASTER_SITES=GH works.

	Not sure I remember all what I did anymore, so thought I should
	save my work somewhere before it got lost.
	
	http://www.niftiestsoftware.com/cups-cloud-print/

patches
=======
mail/dovecot2

	ports/175813: [patch] mail/dovecot2 doesn't detect libstemmer or
		exttextcat

	Patches to get dovecot2 with fts_lucene to work, because
	textproc/libexttextcat has incremented its library name so configure,
	has trouble finding it.

	libstemmer is incorporated into lib-clucene-contribs-lib.so in
	textproc/clucene

	libextextcat has gone from:
	    libexttextcat.so(.*) ->
		libexttextcat-1.0.so(.*)->
			libexttextcat-2.0.so(.*)

sysutils/cfengine35

	Kind of odd making a patch for FreeBSD when the problem is upstream.
	
	So, opted to report it upstream, but in the meantime I rebuild cfengine
	using this patch.

	-------------------------------------------

	Issue #3207 - configure fails to find libxml2

	configure doesn't find -lxml2, because it doesn't call
		"xml2-config --libs" to get the LDFLAGS needed to find it.

	-------------------------------------------

	But, then also reported for FreeBSD.

	ports/180896

	sysutils/cfengine35: can't use edit_xml because libxml2 not found

	========================================================================

	patch-3.5.1-3

	Noticed that the new CFEngine Standard Library for 3.5.1 is missing,
	so that's what 3.5.1-3 is.

	ports/181509

Other
=====
xpi/accountcolors

	I have a lot of accounts in Thunderbird, and have been looking for
	extensions to find certain accounts easier.  This one seemed helpful,
	except that it wouldn't show all my accounts.

	Probably is hard coded to some maximum number of accounts...

	So, I decided to hack the extension to increase it.  After counting
	the number of accounts visible, I deduced the limit to be 60.  I then
	counted the rest of my accounts and rounded up to 128. ;)

	Replacing the right occurrences of 60 with 128 was easy.  The hard
	part was the options dialog.  Which was ugly, which I added to more.
	You would think there's a cleaner way to generate the rows, either
	staticly or dynamicly.  Oh well, it does what I want it to now :)

	****

	Later I found an extension, called QuickFolders, which was more of
	what I was really looking for.  I did try to match the colors I set
	with accountcolors with the colors I set for the folder bookmarks.

	Most are just the inbox of an account, but its about jumping to where
	I want to be in my folderpane.
