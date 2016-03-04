PATCHES
=======
accessibility/redshift

	This for some reason was failing to build in _poudriere_ on work computer due to missing dependency for VIDMODE.
	The port had built fine outside of poudriere, so likely something else had installed the dependency.  Though
	was confused as to why it had built fine in _poudriere_ on my home computer.  After digging around for a while,
	discovered that I had changed the options for one of the deeper dependencies of `graphics/jasper`... and that
	enabling OPENGL for that port had pulled in the otherwise missed direct dependency.

	PR: 199004

archivers/quazip

	This port got updated, but it wouldn't build.  After some investigation, it was obviously a problem of conflict
	with installed port.  Specifically for libquazip.so. Instead of resorting to "make clean deinstall reinstall", I
	made it hack the Makefile so to force it use the built one instead of the installed one.
	
	PR: ports/187735

archivers/ruby-lha

	In UPDATING on 20141008, the default ruby version was updated from 1.9 to 2.0.  Long ago, in addition to listing
	where my *default versions* differed from *default* (ie `mysql=55p` instead of `mysql=55`), I had added in
	definitions for all of the then *default* versions to `DEFAULT_VERSIONS=` in my `make.conf` file.  Partly to avoid
	*surprise* changes, from forgetting to check `UPDATING` until after I had started updating my ports.  With the added
	benefit that if I'm not ready to make the switch, I don't have to right away.  Additionaly, now that I'm doing
	unattended daily poudriere builds, it avoids surprises to my package repo.
	
	With poudriere, I have two repos being built.  One for my *headless 'servers'* and one for *'zen'*.  For the first
	case, I was able with minor adjustments to get all my packages to build and have converted to updating those two
	systems from that repo.  Getting the repo for *'zen'* continues to be a work in progress.

	At the time of the switch, the only use of `lang/ruby19` was by `ports-mgmt/portupgrade`, so deleted 'portupgrade'
	and the resulting leaf packages. from these systems.  However, later I found that I still needed it for other
	automated tasks (`portsclean`) and the desire/need to occasionally build a port on these systems.  So, I changed
	these systems repo to the new default, and (re-)installed 'portupgrade' from it.

	Since the repo for *'zen'* was still a work in progress, I decided to be risky and switch its default to eventually
	make that transistion.  The next day after making the switch, I discovered this port to be one that didn't get
	survive the switch to 2.0.

	I found a post 0.8.1 commit to github that addressed the **BROKEN** issue, and threw in a patch to silence the
	deprecated function warning (which is removed from CAPI in 2.2.) and the #include warning.

	As I was about to unleash this patch, I discovered that between Nov 1st and Nov 21st.  The port had been updated
	to show that it also doesn't build with ruby 2.1.  So, with a bit of work, I run 'poudriere testport' with both
	ruby 2.0 and ruby 2.1.

	PR: 195268

audio/espeak

	As part of the sudden loss of Gnome2 and the appearance of Gnome3, `audio/pulseaudio` got a significant bump in
	version.  This resulted in (re)build failure for `audio/espeak` if something other than the default option of
	PORTAUDIO is selected.  RUNTIME includes both `audio/portaudio` and `audio/pulseaudio` as LIB_DEPENDS.

	PULSEAUDIO is now making use of features that require at C99 to succeed. So, at first I thought it was a simple
	matter of telling it to pass a switch to use c99 or c++99 or gnu99 or gnu++99(?), which would either invole using
	USE_CXXSTD=gnu99 or USE_CXXSTD=gnu++99, the first applies to C not C++ and the second doesn't exist, though was
	passed anyways so that things would fail differently.  Had opted to use the new OptionsNG way, so it was:

		PULSEAUDIO_USE=	CXXSTD=gnu99

	Or some variation of the sort.  Along the way, I considered the that perhaps the issue was the base GNU
	compilers weren't new enough (later found a document that indicated that one of the things it had
	complained about first appeared in 4.3, and base compilers are 4.2.1.) So the attempts became:

		PULSEAUDIO_USE= GCC=yes CXXSTD=gnu++99

	Still no progress.  And, new and different errors to confuse and baffle me.  Had introduced typos in places.
	Eventually, I looked through the files under `/usr/ports/Mk` to figure out what I should use.  I settled on:

		PULSEAUDIO_USES= compiler:c++0x

	Success!

	Now there some final things that 'poudriere testport' complained about, resulting in making some changes to
	`pkg-plist`.  Now to send it off into the world....

	PR: 195264

audio/gsm

	Got a strange error, while troubleshooting something else likely unrelated, about /usr/local/bin/tcat and
	/usr/local/bin/untoast.

	Examining the files, I found:

		lrwxr-xr-x    1 root      wheel            5  2 Jul 21:27 tcat@ -> untoast
		lrwxr-xr-x    1 root      wheel            5  2 Jul 21:27 untoast@ -> untoast

	Yeah, that doesn't look right.

	Rebuilt/reinstalled port, didn't help...

	But, the files under **STAGEDIR** are correct....

	Aha, pkg-plist is causing them to be wrong.

	PR: 191971

chinese/ttfm

	For the longest time when I started running poudriere, `chinese/arphicttf` would fail leading to
	`misc/freebsd-doc-en` getting skipped.

	Eventually, I found that the problem was with _Makefile.ttf_ under `chinese/ttfm`.  This 'Makefile' is not used by
	this port, but is for ports that depend on this port to use it during their build.

	Seems the post-install isn't applicable if PACKAGE_BUILDING, though it seems to me its not applicable for staging
	in general.  Then there was the package trying to remove something it didn't create.

	Now I finally have my own copy of `misc/freebsd-doc-en` from poudriere :)

	PR: 196702

conf

	These are patches I've made to /etc/periodic/daily/100.clean-disks and /etc/periodic/security/100.chksetuid

	1. The patch to /etc/periodic/daily/100.clean-disks is to have it skip fuserfs filesystems.  Namely my .gvfs
	directory which periodic doesn't have permission to look in.

	   While that's the only fuserfs filesystem on my systems, this may change when I get around to trying to set up an
	encrypted filesystem.

	2. The patch to /etc/periodic/daily/100.chksetuid is to make it handle mountpoints that contain spaces.

	   Namely I had made '"~/VirtualBox VMs"' a separate ZFS dataset from '/home'.  This was so that I could play with
	different settings to try to improve my VirtualBox performance with the images, or to see if the settings I was
	using on '/home' were impacting my VMs.   The big difference was probably 'dedup', which I was using on most
	datasets in this zpool.  And, it was helpful in that I had various recovery attempts of my old Zen (Windows 7)
	disk in the zpool.  Where the zpool was created using the old Zen drives.  Though unfortunately there wasn't a
	useful way to reveal what had been deduped so to consider as a keeper, plus it would really impact performance
	as the pool filed up and/or accumulated snapshots.  I have since turned off dedup on everything...and the ratio
	has dropped from ~1.5?x to ~1.04x...still need to whittle away more, since the big purge was in trying to get
	the poolsize below 90% (since things were really slow while it was over, even crashed it a few times while try
	to delete large directories.  Rough thing was that pool usage didn't drop much at first since the two largest
	directories I was removing files were from the two remaining recovery attempts of Zen.  And, they'll have
	dups that matched the files that making up its successor running in VirtualBox.

	   Virtualized Zen has been much more stable...though its slower as only a single CPU VM.  There's currently two
	Virtual Zen's...the main difference has been eliminated between the two, so I'll probably nuke one as it falls
	behind the one I use routinely (but I've been looking at opensource alternatives to the commercial apps that I
	use routinely on here.)  The more current used to be a multi-CPU one, initially 2, but recently found reference
	showing that 2 is worse than 1, while 4 was better.  However, it didn't appear to be the case for me...in fact
	I think it was even slower than 2.  So, this instance is now a single as well.  I had made lots of changes to
	the multi-CPU version in trying to get performance up, a lot of which would probably help the single CPU case...
	no benchmark to confirm....since the ones I tried lead to BSOD.

	   Hmmm, just realized that the BackupPC problem I was having hadn't been updated to reflect that this directory
	is on a separate filesystem from '/home'.  Performance would probably not be as good allowing it to backup both
	filesystems at the same time, but making them mutually exclusive would cut down on the nagios alerts when one of
	the backups fails and remains that way for more than day because its blocked by the other.  My BackupPC is
	set up to create/destroy zfs snapshots (named backuppc) in its pre/post commands.  The create snapshot script
	destroy's any existing snapshot by this name when its called.  There's a pre/post lock to avoid different backup
	jobs that would create snapshots of the same dataset to only backup portitions of it.  (IE: the directory that is
	mapped into Virtual Zen to be "My Documents", "My Music", "My Pictures", and "My Videos", is handled separately
	from the rest of '/home'.)  Though coming up with a way to allow 2 instead of only 1 backup job for this zpool
	might still be something for the future...with this case being the one that'll require special work.


	Not sure if I'll submit....one or both of these patches.

databases/memcached

	It is becoming a peeve of ports that work fine, but PORTREVISION is bumped so that it will disrupted "portmaster -a"
	or disrupt my system.  Especially when it causes portmaster to remove the working build and fail during the install
	of the bumped port.  Especially, when the the bump is of questionable importance, though it might only be
	'especially' since it broke something that wasn't really broken.  Had it not broken things but was an
	enhancement....well, hard to say in this case.

	Here's the patch I make the new port option work.  Both for my 'servers' where I have "OPTIONS_UNSET = DOCS" set
	in /etc/make.conf, and on my desktop where I left it with its defaults.

	PR: ports/187337

	Discovered that I couldn't get into my email when checking to see what number had been assigned.  Roundcube logs
	have "DB Error: Failed to connect to memcached."  Also noticed that memcached is consuming 100% of CPU.

	I backed out parts of the "Enforce libevent2" patch, to get things working again.

databases/pecl-mysqlnd_qc

	Keep seeing the advise to use memcache instead of query cache in mysql, but it stumped me on how would I do this
	without requiring that I change the applications (the big one being cacti.)

	When I finally came across this extension, which sounded like it would fit.
	Except it didn't, because I use spine. :(

	Anyways, while trying it out, I couldn't figure out how to get it to use memcached.  Well, these are enabled at
	compile time, and the port didn't ask if I wanted to enable MEMCACHE or sqlite storage handlers.  So fiddled with
	the Makefile to have it ask.

	Wonder if I'll submit it?

	Decided to first remove reference to APC.

databases/pecl-mysqlnd_qc/patch-php_mysqlnd_qc.c

	Going with mysqlnd_qc.cache_by_default, there needed to be a way (to me) to have it use MEMCACHE by default.
	I started trying to add a new INI option to do this, but doing development on a 'production' server is bad,
	so I went with the QAD way....

databases/sqlitemanager

	I should write something, except I forget what and why I made this patch or what its status is.  Except that I
	recall it was what enabled me to overcome whatever problem I was having at the time....

	PR: ??

deskutils/calibre

	It has been a while since I've used this app, like evidently pre-gnome3 or was it pre-9.3, having trouble remembering
	when was the last time I used it.  Feels like it was before my last KUMC trip in December.  But, don't recall when I had
	made the jump to gnome3...

	But, clicking on it in the Appplications menu, or from the favorites section in my gnome-shell, wouldn't start.  It would
	start in terminal window.

	Eventually, it occurred to me that it might be doing something different from menu than starting it naked on commandline.

	Yup.  The `calibre-gui.desktop` file has "Exec=calibre --detach %F".  I tried "calibre --detach" in a terminal window.

	Result, it fails due to `"calibre: error: no such option: --detach"`.  Hmmm, do a online search.  Online man page has
	next to the option `(linux only)`, guess we're not linux anymore....

	Remove the `--detach` from the `calibre-gui.desktop` files, update the desktop database, restart gnome-shell, and try
	starting it from menu.  It starts.  Now to figure out where its coming from, since they aren't source files.

	It involves patching `linux.py`.

	PR: 196817

deskutils/gcalcli

	Been trying to make use of the more advanced options of this, but haven't had much luck.  Went to see if there
	was a newer version, and found that the project had moved to github and the latest tag is 2.4.2.  Seems ports
	have of way of not getting updated if the site moves, and new releases aren't posted to the old site, etc.

	I started with a quick update the port so that I can have the latest, which wasn't too hard since its a NO_BUILD
	port.  Then I thought about sending it up, which resulted in more changes to things.  Like option knobs for the
	3 optional dependencies.  The default matching the one that had been listed as required dependency for previous
	package.

	And, then added copying/installing of docs, which led to staging.

	Haven't tried the options yet, but it did make one calendar that I could see in the previous version visible.
	Under other calendars on Google, I have various shared public calendars and one privately shared (exchange) calendar.
	Which was the main reason I was fiddling with gcalcli today.

	PR: ports/187619

	On November 17th (2014), Google pulled the plug on its deprecated V2 Calendar API.  Found that gcalcli had apparently
	undergone a complete rewrite to support V3 differently, but no releases had been made/tagged.  So, I devised a quick
	and dirty patch to grab its state on 2014-10-31.  So, created a PORTVERSION of 3.0.a.20141031.  Not really clear
	whether its 3.0.a.20141031 or 3.1.a.20141031 that comes before 3.1, so opted to err on the cautious side, figuring
	I'd get corrected after I submitted....

	Now on December 3rd, after returning from my annual trip to Chicago TARDIS, I went to work on cleaning this up for
	submission....only to find that releases have now been tagged.  The v3.1 tag appears to have occured yesterday.

	So, off to update my patch to do this version.

	Got my answer.... on zen I had installed it as 3.1.a.20141031, while on dbox as 3.0.a.20141031.  Locked the pkg on
	both to protect against it getting downgraded to 2.4.2 when doing an upgrade from my private poudriere package
	repos.  Well, went to install 3.1 on zen, and portmaster stumbled that an older version 3.1.a.20141031 was already
	installed....

	Should go through `poudriere testport` without a hitch right....

	PR: 195617

	On January 21st, 2016, found that gcalcli had stopped working with 'Missing module - cannot import name run'.
	Eventually tracked this down to having upgraded to py27-oauth2client-1.5.2 (which occured on January 9, guess it had
	been that long since I used it on my desktop.)  Wonder why nobody mentioned that it had broken my irc bot?  [Guess
	I upgraded that on January 15, so hasn't been long enough.]

	Noticed in the comments for tools.py for oauth2client that it said 'run()' but the function was 'run_flow()',
	either it was recently renamed or something that made 'run()' the same as 'run_flow()' was removed.  After a quick
	google search to confirm syntax, I patched 'gcalcli': 

		--- gcalcli.orig	2016-01-18 13:26:30.000000000 -0600
		+++ gcalcli	2016-01-20 20:11:44.962327852 -0600
		@@ -185,7 +185,7 @@ try:
		     from apiclient.discovery import build
		     from oauth2client.file import Storage
		     from oauth2client.client import OAuth2WebServerFlow
		-    from oauth2client.tools import run
		+    from oauth2client.tools import run_flow as run
		 except ImportError as e:
		     print "ERROR: Missing module - %s" % e.args[0]
		     sys.exit(1)

	I then decided to see if the bug had been reported to FreeBSD.  There was a bug dated January 8, 2016.  So, I
	generated a FreeBSD port patch, and added it to the bug.

	In the preparation of the report, found that on August 25, 2015: The old_run [old run] module was removed, had been
	deprecated since July 2, 2013.

		"""This module holds the old run() function which is deprecated, the
		tools.run_flow() function should be used in its place."""

	I also noted, that gcalcli upstream's most recent commit was on January 21, 2016, still hadn't reflected this change.
	Presumably, they run operating systems that don't provide the latest py-oauth2client.  Also noticed that the latest
	tagged release is v3.3.2 (Sep 14, 2015 - 4 commits behind master), while ours is v3.2 (Dec 16, 2014).  Perhaps v3.3
	was buggy, but given the amount of time since v3.3.2, likely stable now?

	Oh, I see that in v3.3.1, they pinned oauth2client to <=1.4.12 to dodge the issue.  Looking at oauth2client, the
	oldest tag is v1.3 (dated Sep 29, 2014), so making my patch shouldn't cause a regression.

	Wonder how to re-open an issue?

	PR: 206045

deskutils/recoll

	On May 7, 2014, this port was updated for version 1.19.13 (previous was 1.19.10.)  I didn't get around to trying
	to update until around May 18.  Where there were problems with it trying to install port dependencies that were
	already install.  Fixing the *_DEPENDS lines to look for files that exist in the port fixes that.  Next there were
	problems in pkg-plist, which hadn't been updated to reflect STAGING.  Namely the *.pyc files aren't present when
	staged, but pkg-plist listed the *.pyc files.  Removing them got it working.

	Interestingly, in researching the background of this, I found that on May 24, 2014 the port was marked BROKEN
	due to the issues with *.pyc.  Strange way to fix a bug.

	Sidenote: for what I wanted to use this for, it hasn't been doing so well.  So, it might be ditching its use
	on my computers eventually....

	PR: ports/190295

devel/gdcm

	graphics/openjpeg was recently updated to v1.5.2 and then v2.1, with v1.5.2 moved to graphics/openjpeg15, for
	the few ports that have trouble with 2.1.

	This is one of those ports.

	Problem is that both openjpeg and openjpeg15 can exist together on FreeBSD...with most ports that work with v2.1
	depending on that, and the ports that don't having openjpeg15.  This part is a good thing.  But, cmake can't me
	asked to find out if a specific version of something is installed and how to include/link to that version.  So,
	it sees that 2.1 is installed and returns that.

	now gdcm claims to support OPENJPEG_MAJOR_VERSION of 1 and 2.  It comes with source for 1.4.0 and 2.0.0.  Though
	the default is to use the v1 version, there's an advanced define that can be used to select the v2 version.
	Alternatively, which is what FreeBSD does, is the knob to have it use SYSTEM OPENJPEG (which is actually another
	port ;)

	The first attempt to update this port failed because it OPJ_UINT32 wasn't defined, found it in
	`/usr/local/include/openjpeg-2.1/openjpeg.h` while not in `/usr/local/include/openjpeg.h` (_1.5.2_).  So I thought
	its wanting the new openjpeg...so patch the Makefile by add `-I${LOCALBASE}/include/openjpeg-2.1` to `CFLAGS+=`
	before the `-I${LOCALBASE}/include`, and update LIB_DEPENDS.

	It got further until it complained that there was no declaration for opj_get_reversible(), which is present in
	the openjpeg.h for the bundled 2.0.0 version (though it is strangely the only declaration without a comment block
	before it.)  Because, tried searching through the nearly 2800 svn revisions to the openjpeg project (gave up have
	worked backward to revision 954)...to not find where it came into existence and then disappeared.

	Though I had already concluded that the code didn't support v2.1, in fact there was mention of ABI incompatibilities
	just before this release.  Though I was this that got me trying to see if I could find where this function was
	part of that reason...

	So, it was how do I force it to use the graphics/openjpeg15 port?  Decided I would go with forcing it early in
	the process with two patches in files (one new, one changed) and `post-patch:` entry in the Makefile.

	The patch forces the openjpeg library to be `%%LOCALBASE%%/lib/libopenjpeg.so`, and Makefile replaces
	`%%LOCALBASE%%` with the value from `${LOCALBASE}`.  Wonder if there's a way to do things this way with CFE, since
	have current where it copies files (and abuses single file copy nirvana)...but have a specific host where doing
	expand_template is desired.

	PR: ports/190476

devel/git

	So, as I went to record what I had done to sysutils/screen this morning, there was no `'git'` command to be found.
	Darn it....must have failed the upgrade without telling me and leaving me with no git.  Kind of annoying that
	if late in the reinstall something goes wrong, it leaves you with nothing.  Portmaster should have an automatic
	rollback.  Though ports should know the finished build is going to fail the install, and not let portmaster
	uninstall the old package and then install the new package while will fail to register.

	Though pkgng will lock the database (sometimes for hours?) while other things are using it, and cause all sorts
	of mayhem.

	PR: 191032
	
devel/libexecinfo

	So, I have not had a working chromium on FreeBSD since 33.0.1750.152_1.  And, this has been really annoying.  I
	had tried compiling everything it depended on, which only made the problem worse.  But, everything seemed to point
	to libexecinfo as the problem dependency.

	I would keep rebuilding this every now and then, but it didn't do anything.  But, one day I glanced at its Makefile,
	before rebuilding it yet again.  Where I noticed that it seemed to be missing a compiler flag.

	The port makefile is using CFLAGS_amd64 to add `-fno-omit-frame-pointer`, but in watching the build output, this
	flag was no where to be seen.

	I played around with 'make -dv' and other options, but could not figure out why it was getting lost when it goes
	to doing the actual build.

	It appears to be because the port's Makefile uses the system make process and `/usr/share/mk/bsd.lib.mk`, that it
	does it own processing to determine what CFLAGS to use.  Which, doesn't know anything about `CFLAGS_${ARCH}`.

	So, I added a patch to the port's Makefile to check `MACHINE_CPUARCH` to add this flag.  And, now it shows up.

	Right after I installed this port, I tried to start chrome....and it worked!

	Now to use chromium-35.0.1916.153_1 to enter a bug.

	PR: 191465

devel/pybugz

	In the announcement of FreeBSD switching from GNATS to Bugzilla, there was reference to this port.  And, in the
	source and help was reference to ~/.bugzrc.  Except the ports version doesn't contain any code to use a configfile.

	So, I looked to see what the latest on github is, and then worked on updating the port to fetch the latest from
	github and see if that worked better.

	Well, it took some fiddling around to figure out that setting one or more status conditions is mandatory for
	search to return any results.  Now if there were a way to search for ALL except resolved or what all the status
	codes for that are.

	But, before I got to trying to figure out 'post', I found in the search results that somebody else had submitted
	a patch to update to 0.10.1.  They had slightly different GH_* parameters, to actually get a GH_COMMIT that
	matched what was found on github for the tag.  While I used parameters similar to a previous patch I had done and
	it extracted to a different GH_COMMIT string.  But, I compared the files and found that other than the 7 character
	COMMIT string, they were identical.  Same date stamps too.  Don't know why it did that.

        PR: ports/190795

editors/lazarus

	This build dependency got updated, as did the port that depended on it.  But, it wouldn't build.  When it was
	doing eidtors/lazarus-lcl-units, steps were in need of lcl to have been built...but it wasn't going to build these
	until last.

	So, I patched it to make it build sooner.

	PR: ports/187961

	Turns out the problem was stray files left behind by the old version causing problems.  The maintainer says he
	fixed the deinstall in this version, and a later rebuild/reinstall and then delete of this build depend worked
	without my hack.

emulators/pipelight

	Due to the death of support for NPAPI plugins by `www/chromium`, I've been looking for alternatives to restoring
	some of the lost plugin functionality.  During the search, I had come across a pointer to `multimedia/moonlight`.
	Namely by trying to join a Microsoft Lync meeting, which requires Silverlight (which I had partly been able to do
	with an earlier chromium release) and was suggested that I look for Moonlight.  Which may or may not still be a
	supported offering on Linux, but had dropped out of ports back in 2012.

	Meanwhile, in earlier searches, I had come across reference to `emulators/pipelight`.  And, thought I had already
	installed it, or was at least already building it in *poudriere*.  I discovered that while I had created a UFS zvol
	for ~/.wine-pipelight, and induced a number of system panics due to trying to extend my snapshot backup scripts to
	handle this.  But, since its not in use yet, currently skip for now.  Though someday I'll need to, as I also have
	other UFS zvol filesystems that I will eventually want to back up.  The other being bounced around using CARP/HAST,
	which I haven't had time to continue working on that project.

	The *pipelight* project being described as __"Windows plugins in Linux browsers"__, where FreeBSD is listed as a
	supported system ;)  Was mainly looking for Flash, but Silverlight is on the list.  Not sure about others.

	Though what had diverted by attention for a while, was whether it was possible to install the Chrome for CentOS 6
	into the linuxulator and have `www/chromium` access that.  But, there didn't seem to be a way to port compatible
	way to wrap it, the download is always the latest version, or a simple way to call it from chromium (have to parse
	plugin's manifest to generate some of the commandline flags to add it to the browser's command line.  Given more
	time I'd probably have devised something, but haven't got past the first step of having it work.)  In an earlier
	time, I had wondered if it would be possible to use the Mac OS X Flash on FreeBSD....and since learning that clang
	and LLVM are associated with Apple, it does raise the question of whether code generated to use LLVM libraries
	code be made to run with mimimal effort by switching out the LLVM libraries?  Perhaps my recollection is blurred
	by my past where (*commercial*) software products that were sufficient abstracted that supporting other systems
	started by creating new system specific libraries.  Where other products had been developed onto a proprietary 'xVM'
	(which I believe also had orginated from the University of Illinois) where the push was to convert it to use an
	industry recognized language and use that xVM....namely Java & JVM.

	Anyways...I found this port would not build in *poudriere*.  Eventually, I tried to build it outside of *poudriere*
	where it worked.  Causing me to finally start an interactive jail to see what was different inside of *poudriere*
	versus on the outside.  Found the port uses gpg, but its set to depend on gpg2.  Which in the absence of gpg,
	creates a symlink.  Evidentally, not sufficiently compatible for pipelight though.

	So, switched it to use `security/gnupg1`.

	PR: 196708

finance/gnucash

	On Dec 25th, 2014, `www/webkit-gtk2` was updated to 2.4.7.  This lead to a number of ports failing to build due to
	its use of __compiler:c++11-lib__.  And, since I had waited to after Christmas to finally get around to updating
	to 9.3, which requires a rebuild of all my ports, this was preventing from reaching that target until late on
	January 8th, 2015.  There was a patch to this port (on December 30th, specifically for 8.x but also applicable to
	9.x) to also use that, but it wasn't sufficient to get things to build for me.

	After lots of trial and error, I eventually settled on injecting `LD_LIBRARY_PATH=/usr/local/lib/gcc48` into the
	*MAKE_ENV*.

	Wonder if I should wait to see if everything still builds with the update of `www/webkit-gtk2` to 2.4.8?

	PR: 196704

graphics/jpeg-turbo

	When I discovered this port, by installing `net/tigervnc` it used to be a single port named `graphics/libjpeg-turbo`
	and was a drop-in replacement for `graphics/jpeg`, though had to make a tweak here and there for it.  Initially, I
	had done it with 'portmaster -o graphics/libjpeg-turbo jpeg-....`  But, as ports got updated it soon caused lots
	of problems.

	So, after numerous iterations, I have CFEngine invoke a script called `do-jpeg-override` which goes through my
	ports tree(s) and changes the dependencies for `graphics/jpeg` to `graphics/libjpeg-turbo`.  Later it got a bit more
	complicated when the part that I wanted got renamed `graphics/jpeg-turbo`.

	The *pkg-descr* of `graphics/jpeg-turbo` is:

		This is a drop-in replacement for the graphics/jpeg library. It does not
		include libturbojpeg.so (see graphics/libjpeg-turbo).

	And, *pkg-descr* of `graphics/libjpeg-turbo` is:

		This is the libjpeg-turbo library.

	The latter depends on the first, and is pulled in by `net/tigervnc`.  The challenge was avoid the run away
	substitution.

	But, then on Oct 20, 2014 there's a commit of just 'Cleanup plist', which ends this port as a drop in replacement,
	and no longer in conflict, etc.  Not what it says it does or how I want to use it.  It was updated from 1.3.0 to
	1.3.1 as well.

	So, I made a patch to make it a drop-in replacement again.  And, continue to use it as such.

	But, then yesterday (Dec 8, 2014), there's a commit to "Replace USES=libtool:oldver with USES=libtool or
	USES=libtool:keepla in 32 ports that still use it."  Which drastically changes the Makefile, and bumps the version
	number.  Among the other ports, is `graphics/jpeg`.

	I decide it time to give up and just use `graphics/jpeg` everywhere except with `net/tigervnc`....

	Maybe I'll submit a PR that the *pkg-descr*, is now wrong.

graphics/shotwell

	Finally, there was this port that broke after the update to `www/webkit-gtk3`.  Which was the last that I expected
	to run into.  But, was a little bit more complicated to resolve.  In that, the OPENMP option has code compiled with
	gcc that links against libc++ which was a dependency of `www/webkit-gtk3` prior to its switch to being compiled with
	`USES= compiler:c++11-lib`.

	This does bring in to question whether using `compiler:c++11-lib` is limited to FreeBSD releases less than 10, but
	I come up with something that maintains the separation.  Though a quick glance at freshports, seems to say that
	`www/webkit-gtk3` was the primary consumer of `devel/libc++`.  "libc++" being a c++11-lib for use with prior gcc
	compilers.  Where *USES* of `compiler:openmp` had done *USE_GCC* part of this, and a *USES* of
	`compiler:gcc-c++11-lib` would take this futher with a *LIB_DEPENDS* for 'libc++.so' but make changes to other
	environment variables that differed from what was needed in this port.  So, opted to only switch to *USES* of
	`compiler:openmp` and insert a *LIB_DEPENDS* for the pre-existing *LDFLAGS* mod.

	PR: 196707

	Through discussions on OPENMP or not OPENMP and OSVERSION vs DragonFly BSD....settled on just doing:

		OPENMP_USES=		compiler:gcc-c++11-lib
		OPENMP_USES_OFF=	compiler:c++11-lib

	The framework then takes care of whether base compiler or the ports compiler can meet the need, and in OPENMP case
	if works out if `devel/libc++` is need from ports or not.  Suspect where DragonFly has base gcc that supports
	c++11-lib that it doesn't need it. While FreeBSD 10+ has clang as its base compiler, which does support c++11-lib,
	but isn't gcc so gcc from ports is used and therefore libc++ from ports is also needed ???
	
irc/irssi

	Since DateTime::TimeZone 1.70, a number of my irssi plugins (including twirssi) had stopped working.  Errors of
	being "unable to determine local time zone" or "Attempt to reload DateTime::TimeZone::America::Chicago.pm aborted..."

	I had originally opened a ticket against DateTime::TimeZone,
	[RT #97227](https://rt.cpan.org/Public/Bug/Display.html?id=97227), indicating that something had changed between
	1.69 and 1.70 to cause this.  Though among the differences, I couldn't see anything that would account for this.

	Finally, today...I took a much deeper look and, eventually, found that List::AllUtils is complaining that List::Util
	(v1.25) needs to be > 1.31.  But List::Util is a base perl module, and I'm running 5.16.  While 5.16 is already
	considered EOL, with 5.18 and 5.20 being the two stable releases, with development in 5.21...

	Suspect 5.16 when EOL when 5.20 was released on May 27, 2014.

	When I had originally installed my systems, the default install was 5.12...and later when the default version was
	changed to 5.14 (on June 30, 2012), I opted to jump my systems 5.16.  5.16 became the default version on
	October 23, 2013.  I suppose some day (perhaps when I finally get my poudriere setup working), I'll be making the
	jump to 5.20.

	In the meantime, the List::Util version problem is addressed by installing `lang/p5-Scalar-List-Utils` package,
	which the `devel/p5-List-AllUtils` correctly depends on when the installed perl version is less than 5.20.

	So, why wasn't it working under irssi?  Well, its altering the @INC path by claiming to move the directory where
	Irssi.pm is installed up in the search order.  Which its looking for the path of INSTALLARCHLIB, which is where
	the all ARCH dependent base modules got.  But, it isn't a base module so it is actually going into INSTALLSITEARCH,
	where SITE paths normally come before 'base', allowing List::AllUtils to find the newer List::Util that overrides
	the base version.  So, with irssi forcing 'base' arch to come first, its preventing the newer module to be found
	and causing DateTime::TimeZone to fail in strange ways.

	I made a little patch to fix configure for irssi, that get me going (alternatively, the path it uses [promotes] can
	be set in its config file:

	settings = {
	    "perl/core" = {
		perl_use_lib = "/usr/local/lib/perl5/site_perl/5.16/mach";
	    };
	};

	or

	/set perl_use_lib /usr/local/ib/perl5/site_perl/5.16/mach
	/save
	_and restart irssi_

	GH: https://github.com/irssi/irssi/issues/132

irc/irssi-otr

	irc/bitlbee was updated to need security/libotr (4.0.0) while irc/irssi-otr still depends on security/libotr3.  With
	security/libotr and security/libotr3 conflicting, its hard to use both together.

	After some investigation, it appears irssi-otr moved to github and was tagged v1.0.0 on Feb 12, 2014.  It contains
	changes to make use of libotr 4.0.x.

	After many attempts, I got portmaster to build and install it.

	PR: 192026

mail/dcc-dccd

	Recently notice that this package got staged and has reappeared in ports.  I was kind of using it, though not really
	sure how much impact it has.

	However, it just won't build in poudriere.  Seems poudriere does a mixture of installing a pile of depends before
	starting the build, and then various depends as the build discovers its in need of additional dependencies.

	This seem to not meet this ports Makefile's expectation that a prior OptionsNG knob introducing a dependency for
	`mail/sendmail`, and the older PORT_OPTIONS check of another options requires `mail/sendmail` to have been installed
	before starting the pre-build parts of the Makefile.

	After some investigation of other ports I'd expect to have issues, found the define **PACKAGE_BUILDING**, and that
	get's this port to build into my private poudriere pkgng repo.

	PR: ???

mail/dovecot2

	ports/175813: [patch] mail/dovecot2 doesn't detect libstemmer or exttextcat

	Patches to get dovecot2 with fts_lucene to work, because textproc/libexttextcat has incremented its library name so
	configure, has trouble finding it.

	libstemmer is incorporated into lib-clucene-contribs-lib.so in textproc/clucene

	libextextcat has gone from:
	    libexttextcat.so(.*) ->
		libexttextcat-1.0.so(.*)->
			libexttextcat-2.0.so(.*)

	================================================================================

	After years, my patch fails to apply.  Upstream version 2.2.18 seems to have fixed the libexttextcat issue, but the
	libstemmer issue remains.

	Wondering what has becoming of my PR, find that it was a fatality of the convert from GNATS to bugzilla.
	My "Reply-to:" was lost, so the PR wasn't associated with my external email address and my internal addressing isn't
	'real'.  The PR had been created before I had hacked send-pr to use my "Reply-to:" (from env REPLY_TO) to use as
	my "From:", and not expose my internal home network name.

	I have thought about getting a 'real' personal domain, so that my home network can have more a real connection with
	the outside world.  Possibly involving having some external servers, either by finding a VPS offering FreeBSD or
	using AWS.  In part of relocating my home email mess somewhere with greater availability....and overhauling my
	out of control email address situation.  Perhaps in conjunction for when I finally get around to redoing my home
	network and do IPv6, etc.

	Since the original PR got closed abruptly in its brief bit of attention post conversion to bugzilla, a new PR will
	be born.

	PR: 

mail/davmail

	Update to 4.4.0

mail/evolution

	Got updated to do staging, but now its missing files (shared libraries).  pkg-plist didn't include them.  So,
	updated pkg-plist.

	PR: ports/188525

	After looking at `x11-fm/sushi`, I look see why this port had failed and found the same issue.  Resolved with the
	same fix.

	PR: 196706

mail/thunderbird

	A strange little bug that was introduced somewhere, but didn't surface until the recent 31.3.0 update.  Appears
	the definition of CONFIGURE_TARGET changed in `bsd.gecko.mk` as part of this update.  Switching the order of two
	lines fixes the configuration error that had resulted.

	PR: 195728

math/octave
math/octave-forge

	Was looking for a way to process DICOM files on FreeBSD, and got pointed at this.  It would build, as the Makefiles
	were missing numerous build dependencies.

	So, as I worked through them, I updated the Makefiles so that ports will bring in the build dependencies
	automatically.

	PR: ports/??????

math/orpie

	Was searching around on how to do something with orpie, when I noticed that on 04/11/2014 there had been a new
	version (1.5.2).

	It is described mainly for compatbility with OCaml 3.11+, my system is using 4.01.0, so might be useful.

	What's interesting is that 1.5.1 came out 2007-09-13, where the first change after it was 2007-09-17.  The changes
	for compability with OCaml 3.11 was on 2010-03-06, and the update catches a division by zero.

	*Forgot about this port, and somebody else updated it....*

	PR: 190901

	This is a work in progress...not sure where I left things...the change to how OCAML is done has broken the building
	or this package.  No PORTREVISION bump was done to this port though on '04 May 2015 15:45:52', even though the
	description makes mention of doing it on dependent ports.  And orpie does depend on the specific package of ports
	that should get a bummp.

	But, the pre-OCAML shift from 3 to 4, hasn't broken orpie (installed on Dec 31st, which is when I finished the
	most of the upgrading at home to FreeBSD 9.3.  Doing it at work, is still a work in progress.)  So, I've forgotten
	where things got left.  There are other ports that I need to get to creating or updating patches for that I just
	haven't gotten around to, and wonder why nobody else has....

multimedia/ffmpegthumbnailer

	On December 1st, this updated port failed to build for me.  After a quick head scratch, I figured out the next
	day that adding `compiler:c++11-lang` to **USES** would make it build.

	But, didn't get to it until the 8th, to decide I should submit.  Though somebody submitted about 4.5 hours after
	I had made my patch to fix build on 8.4, followed by a report today that it fixes the problem on 9.3. I'm still on
	9.2, should maybe see about upgrading soon.

	One difference though is the submitter used `compiler:c++11-lib` and patched a header file.  Wonder which is
	rigth?

	PR: 195600
	
net/boinc-client

	When I first started running boinc on FreeBSD, I discovered to my delight that VirtualBox was detected and usable
	for Test4Theory.  But, somewhere along the way, whenever I checked on things, it would report that it didn't detect
	VirtualBox.  This was usually resolved by restarting it, as this was usually contention between the respective rc
	scripts.

	Except that I was seeing it more and more lately, but having solved the rcorder problem I was having where
	/etc/rc.d/tmp and /etc/rc.d/cleartmp were being called after /usr/local/etc/rc.d/vboxheadless (partly my fault,
	because I run an NFS server in the headless VM which my host mounts.  Its how I get dropbox into my FreeBSD
	system.

	But, the problem continued to happen with boinc-client.  So, I decided to take a closer look.

	I found that hostinfo_unix.cpp thinks its a linux like system, it looks runs "/usr/lib/virtualbox/VBoxManage",
	which is located at "/usr/local/lib/virtualbox/VBoxManage" (of course.).  Else if its __APPLE__, it looks for
	the app, etc.

	So, I added a patch to handle __FreeBSD__.

	Not sure when or how this got broke.  At first I thought the patch had disappeared, but didn't find anything
	resembling my patch (though I only went back a couple of years...) or if the support disappeared in the 7.x
	version of the code.

	Strange how I had failed to notice for so long....

	PR: ports/188710

net/freeradius2

	Updating to 2.2.4 was failing for me, turns out configure has an auto-activation on execinfo.h, except it wasn't
	using it correctly for FreeBSD.  So, I made it an explicit library depend and made it link to the library.

	PR: ports/188089

net/p5-IO-Interface

	After this port was upgraded from 1.06 to 1.09, I had incorrectly assumed that my port `net-mgmt/nagios-check_dhcp.pl`
	failing was due to a problem with its use of IO::Interface methods marked as deprecated.

	However, the patch (PR: 196528) didn't solve this problem.

	The problem was that the check would fail with:

	     Error: Illegal seek

	And, in dmesg, lines like this would appear:

	     WARNING pid 10917 (perl): ioctl sign-extension ioctl ffffffffc0206933
	     WARNING pid 10917 (perl): ioctl sign-extension ioctl ffffffffc0206921

	In deeper investigation, found that the probem was in `net/p5-IO-Interface`.

	In looking at the change log, between 1.06 and 1.09.  The upstream applied a patch for a segfault in 1.07, and
	another in 1.08.  1.08 was also the first Git version.  In 1.09, converted to use Module::Build.

	The problem is that the CONFIGURE portion that was in Makefile.PL was not replicated into Build.PL, so it wasn't
	checking if it was being built on FreeBSD, OpenBSD or NetBSD.  Or testing fo the presence of two headerfiles.  So,
	the compile was missing 3 compiler flags (DEFINES) for FreeBSD.

	I waited to see if this port would get fixed, or rolled back to 1.08, but didn't seem either wanted to take place.
	So, after figureing out what specific change to Build.PL I needed and fed it into 'poudriere testport', I submitted
	a report to CPAN for IO::Interface (#101985).

	Made an additional change to the Makefile to satisfy a warning from *poudriere* QA.

	PR: 197404

net/nxserver

	PORTREVISION got bumped to 9, due to newer libaudiofile.  But, it wouldn't build.  audiofile-config has been
	removed, for pkgconf but nxserver (specifically nxesd's) configure doesn't know how to deal with this.  Started
	trying to patch configure, when I went with setting AUDIOFILE_CONFIG="/usr/local/bin/pkgconf audiofile" and
	passing '--disable-audiofiletest' to configure.  It can still get the necessary lib flags and cflags using the first
	part, but the process of checking that 0.3.6 is greater than minimum version of 0.1.5 was easier solved by disabling
	the test.

	Could have port system check version, but can't do that on a LIB_DEPENDS and went for simple.

	PR: ports/183825

net-mgmt/nagios-check_dhcp.pl

	Oh noes, it seems to be broke.  Well, I updated some of its dependencies.  Which one was it.  Looks like
	IO::Interface is deprecated, IO::Interface::Simple is the way to go now.  Whip up a quick patch to make it use
	that and it looks good.

	Then turn it into a patch, run it through `poudriere testport` and off to submit....

	PR: 196528

security/gnupg1

	With the `security/gnupg` moving up to 2.1, and the previous 2.0 becoming `security/gnupg20`, caused conflicts
	despite the claim that they should all be able to be installed together.  What had been done upstream to make this
	possible was to suppress the installation of `gnupg.7` in the 2.0 version, to avoid colliding with the one installed
	by the 1.x version.

	But, this suppression wasn't being done with 2.1, there were various bugs going back and forth between having other
	ports move forward to working with 2.1 or staying behind with 2.0.  Where it wasn't possible to have both.  But, as
	I understand, it is intended that, at least, `gnupg1` can be installed with either `gnupg20` or `gnupg(21)`.

	Eventually, I reported the conflict of the 2.1 version installing 'gnupg.7' versus the 1.x version installing it as
	well.  Decision upstream was about time that the 2.1 version win, and that in a future update (which would likely be
	dependent on somebody finding a security vulnerability...) the patch to stop install the the 1.x version of `gnupg.7`
	would get included.

	This doesn't help until that time, so a patch of some form would be necessary.  And, I had commented on a bug or two
	that had been filed against `security/gnupg`, and eventually things seemed to settle on suppressing `gnupg.7` from
	`security/gnupg1`.

security/gnutls3

	Recently this port updated, and it wants security/openssl to be installed as well.  But, I don't want that.
	So, I tracked down the port that had installed this port and removed it, and removed this and other leafs.

	But, word is that this port might be replacing gnutls, which is a much bigger mess to purge.

	So, I looked at why gnutls3 wanted openssl-1.x.  And, made it an option, preferring to build without it.

	Its being pulled in by dns/unbound, which its default options cause it to require openssl from ports on <10.
	And, this is to create libgnutls-dane, which adds DNSSEC verification support to DANE.  And, is needed for
	the '--check' option with danetool3.

	PR: ports/188184

security/gpgme

	Before the resolution with ports such as `security/seahorse`, the conflict problem of version 2.0 vs version 2.1.
	I had run into this port with depends for the 2.1 version.  Where `security/seahorse` below was at first being
	patched to use 2.0, rather than my patch to have it accept 2.1.  So, I had started to do the same here.  Until
	it was decided to go with 2.1....

security/seahorse

	I had started a patch to change the line below in the `configure` script for seahorse.

		GNUPG_ACCEPTED="1.2 1.4 2.0"

	To

		GNUPG_ACCEPTED="1.2 1.4 2.0 2.1"

	Even had a preliminary 'poudriere testport', when I noticed the time stamps on another system showed it had
	been updated.  Appears it changed the dependency from `security/gnupg` to `security/gnupg20`.

	So, I deleted the work and cleaned up my port trees, so that poudriere could try to complete its update with
	Gnome 3.  Unfortunately, this port still bombed out.  Turns out that this port has a dependency on `security/gpgme`
	Which depends on `security/gnupg` for the 2 version.  So, its pulling in gnupg-2.1... to conflict with the
	gnupg20-2.0... installed in advance for the build of seahorse in poudriere.

	Started to work on (likely trivial) patch/bug report for `security/gpgme`, when I saw that a bug had already
	been entered (195242).  And, figured I'll just chime in on it instead.  As I was writing the comment, I wondered
	what else on my system depended on gnupg.  Which is current the 2.0 version as the 2.1 update was added
	yesterday (the 20th)...which was after Gnome3 displaced Gnome2 on the 19th.

	Quick invocation of `pkg info -r gnupg`, yielded an addtional two ports:

		mail/thunderbird
		security/clamav-unofficial-sigs

	A default port option, ENIGMAIL, is what adds the dependency for `security/gnupg` for *thunderbird*,
	while *clamav-unofficial-sigs* just does (its only options are DOCS and EXAMPLES.)  Not sure what gnupg (2.1)
	breaks, if anything, so they would likely work either way.  Just need to decide what everybody using as
	the 2.x version.  Or devise a way that all 3 versions can co-exist?  Since I have both the previous
	security/gnupg (2.0.26 which became security/gnupg20 when the default port was upgraded to 2.1) and
	security/gnupg1 (1.4.18) installed on my system.  No conflicts between the two.  Harder to have 2.1
	and 2.0 together.  Might be beter trying to find out if seahorse works with 2.1 or not, and it just doesn't
	build currently because 2.1 didn't exist until recently?

	Not sure what I'm going to opt for in the mean time.

	PR: ???

sysutils/am-utils

	Wanted to see if installing newer version of this port than base would support NFSv4, but in quick build test I
	ran into dbm_pagfno() undefined error.  Which in a quick search, found a post on [forums.freebsd.org](https://forums.freebsd.org/threads/52103/#post-292331) and PR 201140.
	But, the PR was closed as "Overcome By Events", meaning what?  Since the problem still exists...

	Well following what was in this PR, where part of the options handling was changed to `OPENLDAP_CONFIGURE_WITH` but
	the how to change the other options wasn't understood by the original submitter.  So, I completed the other options
	resulting in changes to `Makefile` and `pkg-plist`.

	This however, didn't fully resolve the problem or how it works in poudriere.

	Well, the issue is that when `OPENLDAP` (default), it adds `/usr/local/include` to the include search path.  Where
	the automatic ndbm discovery, can get tripped up if gdbm is installed and was built with the (non-default) `COMPAT`
	option.  Which was the case on my systems.

	In base ndbm, dbm_pagfno is a macro to always return undefined, with intent to force devs away from it.  OTOH, it
	exists in the `COMPAT` library for gdbm.  And, the gdbm install puts its own version of 'ndbm.h' in '/usr/local/include'
	which doesn't block the function.  But, the configure script doesn't handle this case correctly.  So, I patched
	`configure.ac` to handle this.

	The issue doesn't occur in poudriere, because all ports are built in clean jails and this port doesn't depend on
	gdbm, so it doesn't stumble on this edge case.

	PR: ???

	However, the NFSv4 support in am-utils seems to be Linux only, so a bigger hacking of the code will be needed to get
	it to work on FreeBSD...  Which I started, but have abandonned as it is way more work to get around the fact that
	the implemented code is Linux only, and apparently none of the other OSs can do NFSv4 either. :no_good:

	Plus its missing various other mount options, partly because the defines aren't the same as Linux.

	I guess I'll leave my Linux servers as NFSv3, and only do NFSv4 serving from FreeBSD.  Only need it for mounting
	shares to my OS X box, which all come from my FreeBSD server.  Had setup NFSv4 on Linux while trouble shooting
	Kerberos on Mac...FreeBSD & Linux worked in both directions, but Mac wouldn't work either until I ran Wireshark
	and saw what evil thing it was doing w.r.t Kerberos...

	Both FreeBSD and Linux have my uid as 1000, its OS X that did it as 501.  The Linux share that I mount on FreeBSD
	is my Dropbox folder, which isn't necessary with OS X.

sysutils/backuppc

	Apparently, in Perl 5.18 the randomization of hash keys was taken a step further.  As always, there was no guarantee
	on the order of hash keys, but until 5.18 the random order was constant.  For years, and many different versions of
	Perl up to 5.16, the order of 'tabs' in configuration editing has always been the same in the various places that
	I've run BackupPC.  But, when FreeBSD changed the default perl to be 5.18, I opted to take the leap to 5.20 on my
	BackupPC server.  In trying to avoid massive rebuilds with perl changes, I had previously skipped 5.14....  Though
	the leap to 5.20 caused lots of problems, which I've had to make adjustments here and there to deal with.

	So, in Perl 5.20 (and likely 5.18), hash keys are not only random in order, but they are randomized every time.
	Possibly instead of a constant seed for how the keys are randomized, they now use some time component for determining
	the seed.  This causes the 'Edit Config' page tabs to not only be in a different order than what I had gotten acustom
	to, but they are in a different order every time the page is refreshed.  So, clicking on a 'tab', results in the
	order changing.  Confusing at first, annoying constantly.

	Once this started happening, I lost record of what the order had always been in the past.  No attempt was made to
	recover that order, even though work server is still running 5.16.  My patch had made it into the poudriere
	regeneration of all packages at the current freeze (the last svn commit before gnome2 disappeared and gnome3
	appeared.)  Some day I'll have time to be prepared to give up my current 'perfected' work environment and switch
	to gnome3, etc.

	The simple solution to this problem is to replace 'keys(%hash)' with 'sort(keys(%hash))'.

	Wonder if I should submit this forgotten patch....

	PR:

sysutils/cfengine35

	Recently in /usr/ports/UPDATING:

	The library version for textproc/libxml2 was brought in line with what the upstream expects.  Causing a problem for
	cfengine.

	Using "portmaster -r libxml2" would've missed updating this port as it doesn't list it as a depend, but uses it.
	Though I updated it, because I ran into the args too long problem...so I used pkg_libchk method.

	This however broke all my cfengine installs, as the copy in /var/cfengine/bin is updated by cfengine.  cf-execd
	is running unaffected by this, but it is unable to call cf-agent as its missing libxml2.so.5 (as it has become
	libxml2.so.2.)

	This made me recall the contortions a co-worker seemed to go through in making static executables for cfengine2.
	Fortunately, we didn't run into any problems between the different Sun architectures like I had encountered once
	in a previous life.

	After much effort, I came up with a couple of solutions.  The full static or partly static cf-agent.  Where the
	partly static cf-agent has just libxml2 and libpromises linked in statically.  I wasn't able to get tokyocabinet
	to link into partly static, since it doesn't have a libtool file.

	While I was working on this, I figured out the FreeBSD issue with libxml2.  Its an auto-activiation in the
	upstream configure script, while the proper way is to have knob.  So, I added one.

	PR: ports/180896

	Because I had bumped the PORTREVISION to 4, I missed the real PORTREVISION == 4, which included a change in how
	libtool files are saved on FreeBSD.  Namely that it strips out the dependency library information for the library,
	suggesting that without such info, the files are now useless so eventually they should just go away.  But,
	while the dependency information isn't necessary when linking against the shared library...since the shared
	library contains the information on what its dependency shared libraries are.  This doesn't work if you're trying
	to link something static.  And, it was libtool options '--static-libtool-libs' and '--all-static' that were the
	basis for the PARTIAL or full STATIC builds of cf-agent/cf-promises that I had added in.

	Not wanting to track down all the lost dependency information, I removed the full STATIC build option.  And,
	updated the PARTIAL option to include all the dependency libraries to libxml2.a.  Not sure which are unique to
	libxml2.a and which aren't.  The main one that got me was the libiconv.la is listed, so that libtool will add
	/usr/local/lib/libiconv.a into the static linking of libtool libs.

	This problem came up because I was trying to add another FreeBSD system to my CFEngine 3.5.x environment.

	Since I was kind of annoyed about this bizzare secret change to FreeBSD right before I was about to head off to
	KUMC...I fired off a PR calling this calluous change a major bug.  There have been a lot of changes lately that
	makes me question if FreeBSD(-STABLE) is really the way to go for building stable production servers anymore.

sysutils/cfengine35/old

	Kind of odd making a patch for FreeBSD when the problem is upstream.
	
	So, opted to report it upstream, but in the meantime I rebuild cfengine using this patch.

	-------------------------------------------

	Issue #3207 - configure fails to find libxml2

	configure doesn't find -lxml2, because it doesn't call
		"xml2-config --libs" to get the LDFLAGS needed to find it.

	-------------------------------------------

	But, then also reported for FreeBSD.

	PR: ports/180896

	sysutils/cfengine35: can't use edit_xml because libxml2 not found

	========================================================================

	patch-3.5.1-3

	Noticed that the new CFEngine Standard Library for 3.5.1 is missing, so that's what 3.5.1-3 is.

	PR: ports/181509

sysutils/devcpu-data

	I've been working on updating this port for while, but had been stubling on how to decode AMD microcode files.  At
	the time, I was trying to update the port to use Intel `microcode-20130906.tgz` and `amd-ucode-latest.tar.bz2` with
	internal file dates of 20130925.

	So, I was happy to see that this port got updated and included the tool that handles both Intel and AMD microcode
	files.

	However, I was disappointed that the microcode files don't appear to be any newer than before.  It was using
	Intel `microcode-20130222.tgz` and AMD `amd-ucode-2012-09-10.tgz`.  I don't actually have any FreeBSD systems
	using AMD processors, but wanted to incorporate the latest amd-ucodes for completeness.

	On Debian, the _latest_ files are the same in the source archive for `amd64-microcode-2.20131007.1+really20130710.1`
	and the files have a date stamp of 20130907.  While all the files at www.amd64.org/microcode have a date stamp of
	20130927.

	In checking the Intel site, found that the latest microcode file is 20140430.

	Wonder what the newer revision for my CPU brings?

	PR: ports/190712

	While doing some cleanup of old CFEngine policy to update this package after installation, I spotted a newer
	Intel microcode release of 2014-06-24.  So, I updated this port to that file.

	PR: ports/191411

sysutils/dirdiff

	Since meld got displaced during upgrade to Gnome3, and re-installing it from ports seems to give why.  It just
	doesn't seem to want to pick colors that are close to legible.  What came in its place with Gnome3 doesn't do
	diffs of directories.  So, I finally decided to look at other ports.

	Cheating, I first tried to install this port outside of _poudriere_.  It failed because I have CFLAGS set in my
	/etc/make.conf, which overrides CFLAGS passed into the build environment.  The port had adjusted the CFLAGS line
	in the port's Makefile to be conditional, so to not override what it was setting through environment.
	Concluded the better `post-patch:` would be to change the CFLAGS in the Makefile to the CFLAGS that has been
	generated through framework... 

	PR: 199058

	Occurs to me this was probably the issue with a previous port, though probably dealt with by other PRs since all
	seems to have been working well for some time now.
	
sysutils/memtest86+

	Doesn't build if ISO option is selected, the distribution Makefile is
	calling make but the port is using gmake.

	Patch the Makefile to use gmake.

	PR: ports/188149

sysutils/panicmail

	Recently, I had yet another panic.  Couldn't remember how I had examined the previous vmcore, while searching online
	on how again I came across this port.

	Almost immediately, I made some tweaks to it.  Like change the From: address for autosubmit to use a proper email
	address rather than my internal root account.

	I forget if I ever got around to creating a genericstable config for my FreeBSD sendmail servers, which have
	replaced my Ubuntu postfix servers.  Though if I had, the address I would use in place of root on my primary desktop
	would differ from the email address I use for FreeBSD related correspondence.
	
	Initially, I made the tweaks by hand and then coded up a CFE3 promise to recreate it on my other FreeBSD servers.

	Then, I started to set things up for possible PR submission to send up my 'enhancements'.  But, as I was doing that,
	I thought up other enhancements and it got a little out of hand.  Now, I'm not sure I want to send it up....

	new rc.conf(.local) entries (with defaults):

		panicmail_autonotify="YES"
		panicmail_sendfrom="root"
		panicmail_usecrashinfo="NO"
		panicmail_usekernel=""

        2014-04-13: After updating to 9.2-RELEASE-p4, which has a kernel.symbols file.  Wonder why -p3 didn't.  And, not
	rebuilding a GENERIC debug kernel, decided I needed to fix the logic for using kernel.symbols.

	Decided to do the nasty, and patch /usr/sbin/crashinfo to use kernel.symbols with kgdb (if available, or
	specified.)

	2015-01-03: Post upgrade to 9.3-RELEASE-p7, there have been a number of panics.  The first few from needing to
	update ports (particularly `emulators/virtualbox-ose-kmod`).  Though there have been later panics as well. 
	Wonder what's keep my modified panicmail script in place against my personal poudriere package repository.
	I probably locked it.  But, notice that my version differs from what I had in git, and also notice that the port
	got updated a bit recently.  So, I rework it quickly as patch against the current-ish, and continue to be
	undecided if I want to submit anything at all...  Not sure if anything happens to the reports yet....

sysutils/pefs-kmod

	Decided to give pefs a try.

	After loading the module, I noticed in dmesg:

		cryptosoft0: <software crypto> on motherboard

	Later I wondered if I work machine had an i7 with AESNI. So, I tried loading that module.

	This message appeared in dmesg:

		aesni0: <AES-CBC,AES-XTS> on motherboard

	But, then I noticed that AESNI support is disabled in the build....its turned on by a DEFINE.  Probably to have it
	build on a wider range of FreeBSD versions it was disabled.  Though would be nice if it could be turned on for
	people that want it and know they can, better if it could also tell if you can or can't have it.

	So, I quickly added a knob to turn it on.

	Now when I load pefs on my work machine, I get:

		pefs: AESNI hardware acceleration enabled

	in dmesg.

	Now trying this at home (where I had only built it, but never got around to trying it), I first try loading aesni.
	These two lines appeared in dmsesg:

		cryptosoft0: <software crypto> on motherboard
		aesni0: No AESNI support.

	I then unloaded aesni, resulting in this line in dmesg:

		cryptosoft0: detached

	Rebuilt the port with AESNI enabled, and loaded int resulted in the same two messages with loading aesni.  Guess
	that is expected, but wasn't known because I did things out of order at work.

	No message from pefs.  What I ideally want is something that'll work for between home and work, and multiple
	operating systems.  So, not sure this fulfills that need, though there is a need for it on work computer.

	PR: ports/187620

sysutils/screen

	This morning a PORTREVISION came out, bring this port to 4.2.1_2.  The commit description was
	_"Make screen 4.0 compatible, prefer fifo over sockets."_  And, the PR referenced was 191017, which was
	_"systuils/screen: 4.3 will not load 4.0 sessions due to default socket usage"_

	But, in making this change, it has made screen 4.2 incompatible.  And, I already endured the pain when screen-4.2.1
	came out back on April 30th.  Why do I care about 4.0 sessions now, especially as there had been two kernel
	updates between updating to screen-4.2.1 and screen-4.2.1_2.  Was on FreeBSD-RELEASE-p5 when screen-4.2.1 appeared,
	now on FreeBSD-RELEASE-p7.  (Actually I'm at FreeBSD-RELEASE-p8, but there was no kernel update for ...and I had
	delayed updating my core servers that it picked up -p8 when I thought I was finally getting around to apply -p7.

	But apply -p8 to my workstation should be okay without a reboot.  Don't think freebsd-update has ever said that
	a reboot is required, though ones that involve new kernels usually suggest that I should.

	So, I opened today's PR via the Bugzilla web interface.  Probably better, since using send-pr I had been Cc: to an
	internal address of the external address that I put in as `'Reply-to:'`.  Where I didn't expect for it to remain
	as part of the bugs contact list, don't recall seeing any such messages from GNATS...but its doing it with
	bugzilla.

	I did later hack my send-pr to Bcc: to my internal address.

	PR: 191029

ports-mgmt/pkg_rmleaves

	Everytime that I've tried this program, it always fails with:

	Dialog Error, try to resize your terminal to at least 80x24.

	But, my terminal window is 80x24, and changing the size doesn't make any difference.  Eventually, I looked deeper
	into the problem, and its having trouble with package comments that contain quotation marks.

	So wiped up an ugly kluge to deal with them, and now its working... ;)

	PR: ports/186904

ports-mgmt/portrac

	For the last few days its been saying that there are no ports needing update.  Seems odd that nothing has been
	changing in for this long, but there were lots of ports I know I hadn't updated at home that I had to deal with
	on work computer.

	Finally, took a peek inside and its not PKGNG aware :(

	Patched it in a similar manner to how ports-mgmt/pkg_cutleaves handles
	PKGNG.

	PR: ports/186963

	Version 0.5 of portrac has never built for me, I continue to use version 0.4_2 (the PORTREVISION bump due to the
	above ;) even though I was pushed into rebuilding all ports in upgrading to FreeBSD 9.3.  Since I do this all with
	poudriere and pkgng...portrac remained because there's no new version of it in my private repo.

	What is interesting is that 0.5 was released back on '15 Oct 2014 21:48:04', I suspect that I'm the only <10 user?
	Some day I plan to make the leap, it might be more likely should I ever come to own a computer capable of ByHVE use.

	Decided after almost 8 months...to take another crack at getting it to build.

	Next question is will it run?

	PR: ???

pr178818

	docs/178818 
		`gmirror(8)` says to use rc.eary which is no longer available

	proposed fix is to automate this in the dumpon script, which had one small flaw, which I corrected.  And, then tried
	to recall how to include a patch that won't get munged by mail client....

print/cloudprint

	While browsing through ports looking for something else, I stopped to take a peek at this port.  I've been building
	this port for sometime, with the intent of trying to get it set up.  Currently, I'm running a small ubuntu VM for
	this...

	On further browsing into the details of this port, I saw that it had recently been updated to version 0.12.  Mainly
	there's OAUTH2 support, since sometime last month the old method got turned off.

	So, I do a (kind of) quick update of the port...

	PR: 200878

print/hplip

	LIB_DEPENDS is testing for a library that is no longer part of print/cups-base to determine if is installed.
	So, updated to look for a different library.  Update to 3.14.4 while I'm here.  And, patch the deprecation warning
	emitted by foomatic-rip-hplip.

	However, this leaves one remaining problem. All the PPD files appear to contain:

		*cupsFilter: "application/vnd.cups-postscript 100 foomatic-rip-hplip"
		*cupsFilter: "application/vnd.cups-pdf 0 foomatic-rip-hplip"

	But, unlike foomatic-rip (apparently), foomatic-rip-hplip only handles text or postscript.
	Removing the "application/vnd.cups-pdf" line, makes it so I can print again.

		*cupsFilter: "application/vnd.cups-postscript 100 foomatic-rip-hplip"

	2014-04-27: So, I had created a script to address the above problem, but got distracted with work/life, and when
	I finally took a look at putting it all together, I find that this port has been updated to 3.14.4 and the
	installing of hpcups PPDs is enabled.  This makes raster printers out of everything, which is a lot better than
	the hpijs.  Since they take into account quirks of duplexing.  (there's a different Letter page size for when
	using autoduplexing.)

	But, it didn't apply the other two patches that I did, so I rerolled it for submission.

	PR: ports/189056

x11/nvidia-driver
x11/nvidia-driver-304

	On my *workstation* at work, I was not able to (completely) get X to working on the graphics card that came with
	the system, but
	eventually got things working used an NVidia Quadro X1400 that I scrounged from another workstation (Sun Ultra 20).
	It was after this that I got X working on *'zen'*, where I had tried 5 other graphics cards before picking up an
	NVidia Quadro X1700.

	While the Quadro X1700 continues to be supported by the `x11/nvidia-driver`, in early 2013 the Quadro X1400 was EOL'd
	and '304.xx' became the new legacy port `x11/nvidia-driver-304`.  The X1700 continued to work under the then '310.32'
	driver and continues work under the current '331.67'.

	Over time, I started having more and more strange video problems.  Due to *chromium*s heavy use of GPU.  So, I
	started looking to see if there were settings I could change/disable inorder to address these problems.  Along the
	way I found that there continued to be newer versions after '304.88', the latest at the time of discovery was
	'304.123'.

	After some fiddling around, I was able to figure out how to get it to build and install.  And, it fixed the issues
	I was having.  Now comes to whether I'll be able to turn it into a proper patch.

	One problem is that the master port assumes version numbers are either in the form XXX.XX.XX or in the form XXX.XX,
	where it adds '00'.  The previous version '304.88' becomes '3048800', the current '331.67' becomes '3316700', but
	for this new '304.123', its '30412300'.  Which causes it apply patches incorrectly to make it build on FreeBSD.

	First there's some needed patches for versions >= '3048800', plus an option, which get missed.  It also appears
	that a stack buffer overflow problem that existed prior to '319.23' had been backported in to '304.xx' at some point
	and is not needed for '304.123'.

	Additionally, I note the other latest versions of nvidia drivers.

		FreeBSD x64
		-----------
		Latest Long Lived Branch version: 340.46			> 331.67
		Latest Short Lived Branch version: 343.22
		Latest Legacy GPU version (304.xx series): 304.123		> 304.88

		FreeBSD x86
		-----------
		Latest Long Lived Branch version: 340.46			> 331.67
		Latest Short Lived Branch version: 343.22
		Latest Legacy GPU version (304.xx series): 304.123		> 304.88
		Latest Legacy GPU version (71.86.xx series): 71.86.15		= 71.86.15
		Latest Legacy GPU version (96.43.xx series): 96.43.23		= 96.43.23
		Latest Legacy GPU version (173.14.xx series): 173.14.39		> 173.14.35

		331.67 - April 09, 2014
		331.79 - May 20, 2014
		331.89 - July 4, 2014
		
		340.17 - June 9, 2014
		340.24 - July 8, 2014
		340.46 - September 30, 2014

		304.88 - April 2, 2013
		304.108 - August 08, 2013
		304.116	- November 6, 2013
		304.117 - December 16, 2013
		304.119 - January 23, 2014
		304.121 - March 11, 2014
		304.123 - July 10, 2014

		319.23 - May 23, 2013	so everything after 304.88 doesn't need this fix?
					wonder if it als means after 173.14.37?

		173.14.39 - December 6, 2013
		173.14.38 - October 1, 2013
		173.14.37 - April 5, 2013
		173.14.36 - October 4, 2012
		173.14.35 - June 28, 2012

	Someday... *I'll come up with something, and get back to fiddling with other ports....*

	So, decided to change the conversions to do this:

		MMM.mm.uu => MMM0mmuu or MMM.0mmuu
		MMM.mm	  => MMM0mm00 or MMM.0mm
		MMM.mmm   => MMMmmm00 or MMM.mmm

	The latter is simplier....

	Now to test.... which didn't go so well, as there were numerous pkg-plist problems lurking in the port that needed to be
	taken care of first.  `poudriere testport` tests for things that apparently aren't normally the end of the world.

	No idea where to look on why removing the port is removing one library that it didn't install.

	PR:

x11-fm/sushi

	Well, with the update of `www/webkit-gtk2` & `www/webkit-gtk3` to 2.4.8, the result is that the latter now also USES
	`compiler:c++11-lib`.  Which causes this port to fail until a modification similiar to getting ports affected by
	the update of `www/webkit-gtk2` to 2.4.7 to be done.

	PR: 196705
	
x11-server/xorg-nestserver

	Options wouldn't stick, causing portmaster to ask for them over and over again.  A quick patch to fix.

	PR: ports/188848

x11-toolkits/wxgtk30

	Related to the Dec 25th, 2014 upgrade of `www/webkit-gtk2`, this port was also failing to build and resulting in
	a number of skipped ports.  On January 4th, 2015, I noticed the change to `finance/gnucash`'s Makefile, so I
	tried that addition here as well.  Eventually, found that I needed to make one additional change.  In addition to
	this and the aforementioned, there had been two other ports failing.  But, after I fixed this the only one left
	was `finance/gnucash`.  Didn't look to see if the other two had been related to this or if they had some other
	problem.

	I made one additional change of adding `--disable-precomp-headers`, didn't really want to chase down why this
	had apparently worked with base gcc but not with gcc48.

	PR: 196703

www/lynx

	Would you believe that this bug has existed since Mar 21, 2012?  It was to add NOPORTDOCS.  Something that I had
	been using on my 'dedicated' FreeBSD servers.  Though I suppose this is not a normal thing to set globally, and
	its something that escapes detection when using portmaster/portupgrade to upgrade this port.  Or its something
	that became a problem as a result of staging and the importance of a proper pkg-plist to the creation of a package
	and its installation?

	Anyways, I've been playing around with `ports-mgmt/poudriere` to provide a custom pkg repo for both my 'dedicated'
	FreeBSD servers and my main FreeBSD system.  The latter is still a work in progress.  But, I had recently gotten
	everything to build on for my servers.  Other PRs outstanding, perhaps, on what I needed to do to get here.

	So, while doing my first `pkg upgrade`, I noticed some error messages about problem of two messages of:

		rm: /usr/local/@comment: is a directory

	So, the DEINSTALL script filed, but the upgrade happened.

	Sure, enough there's a `/usr/local/@comment` directory.  I remove it.  And, try force (re-)install again....it comes
	back.

	Hmmm.

		@exec mkdir -p %D/%%PORTDOCS%%/share/lynx_help
		...
		@exec ln -sf %D/%F %D/%%PORTDOCS%%/share/lynx_help
		@unexec rm -f %D/%%PORTDOCS/share/lynx_help/COPYHEADER.asc
		...
		@exec ln -sf %D/%F %D/%%PORTDOCS%%/share/lynx_help
		@unexec rm -f %D/%%PORTDOCS/share/lynx_help/COPYING.asc

	Suspect some kind of error in global injection of `%%PORTDOCS%%` ???

	When NOPORTDOCS is not set, PORTDOCS="", so the path becomes `%D//share/lynx_help`... which is tolerated, but when
	NOPORTDOCS is defined, PORTDOCS="@comment ", so the path becomes `%D/@comment /share/lynx_help`.  Sure enough
	there's also a stray directory of `/share/lynx_help` on my servers.

	PR: ??

www/nginx

	The port was updated from 1.4.7 to 1.6.0, but a number of 3rd party modules were disabled rather than the maintainer
	tracking down updates for them.

	I use TCP_PROXY, so I was impacted.  I tracked down a fork of this 3rd party module that had been updated to work
	with nginx-1.6.0.

	After extremely minimal test (it compiled ;) on dev, I deployed it to my 'production' servers.  Though the application
	that needs it hasn't gone into production yet...but will hopefully later today.

	PR: ports/189393

www/squid

	The port got upgraded to 3.5.8 (from 3.5.7), but it won't build with TP_IPF.  The next day 3.5.8_1 came out with a
	fix TP_IPF build.  Not sure if I had this update when it was encountered by my automated poudriere setup.  But, it
	failed again the next day, so there's something else wrong.

	Digging into the code, found where the offending block of code was...and that it was to clutter up logs about running
	less than ipfilter 5.1 and having IPv6.  Well, I don't have IPv6, so the encounter shouldn't happen (though someday 
	I'll get around to doing IPv6, but plan to do networking different so this might not matter.)
	
	I do see that for 3.5.8, squid.conf.documented changes the note that 'intercept' disables 'authentication and IPv6'
	on the port, to just disables 'authentication'.  

	Anyways, the quick fix is comment out the nag that is causing compile failure.  FreeBSD 9.3 has 4.1.28, while
	FreeBSD 10 got 5.1.2.  Guess nobody wanted to backport 5.1.2 to 9....
