PORTS
=====
devel/json-c

	This was to form the patch for a PR to get json-c-0.9 updated to json-c-0.10, though the latest is json-c-0.11.

devel/ruby-mmap

	Been trying to figure out how to generate the .fw files for sysutils/devcpu-data from the latest amd-ucode files.
	Found a ruby program that looked promising, but getting it to run was a challenge.  Especially since it wanted
	'mmap'.

	I got mmap to build after going through the source and figuring out what was needed to get it to work with 1.9.
	I was going to fork ruby-mmap on github to record my work, when I found that numerous other forks exist with
	similiar/better changes to mmap.c.  I took what looked appeared to be the most recent and it showed that it to
	be 0.2.7.

	So I tweaked the Makefile to create PORTVERSION 0.2.7 from DISTVERSION 0.2.6 :)

	Alas, it didn't extract anything.

print/cups-cloud-print

	While researching options for replacing my old inkjet printer, and the apparently obsession of vendors to do GDI and
	Windows only...  though some support Mac OS X and some might add some Linx support.

	Since I already ran into this mistake with my laser printer, which I hardly use now.  Though I run a small ubuntu VM
	on one of my FreeBSD systems to make use of it.  It appears unlikely that I'll have Windows again, and no idea when
	a Mac will become a reality.

	One of the draws to the idea of working for a University was to get discounted Apple hardware, but I've yet to get
	anything this way in over 7 years.

	I had seen printers with direct Google Cloud printing support, but the capability disappeared from Chromium on
	FreeBSD some time ago.  I do have the capability to print to my printers at home, since I own a ChromeBook.  Again
	it involves running chrome in that small ubuntu VM.

	So, I had gotten a version of this to build and run.  And, I successfully printed a web page from my browser on
	FreeBSD, out to Google, back to chrome running in VM on same computer, to printer sitting next to said computer.
	Though probably wouldn't be any less weird if I did get a printer with direct cloud printing support.

	The port probably needs a lot more work to get it to FreeBSD standards, but this work staled on trying to figure out
	how MASTER_SITES=GH works.

	Not sure I remember all what I did anymore, so thought I should save my work somewhere before it got lost.
	
	http://www.niftiestsoftware.com/cups-cloud-print/

	2014-02-04 (first snow day of 2...so far): Was about to take another stab at getting it to to work, when I noticed
	that an official port for it had appeared with timestamps of 2014-01-29.  Wonder if I want to switch to it.  Though
	my current install was not installed by port, so that might cause problems.  Noticed that the Makefile operates on
	/etc/cloudprint.conf (which is where my local install works on, but in files I had patched the appropriate files to
	use /usr/local/etc....

net/p5-Net-DHCP-Watch

	Couldn't get check_dhcp to work on FreeBSD, so had nagios invoke it through nrpe on a Linux server.  But, I'm
	running out of old Linux servers.

	So, I switched to using a check_dhcp perl script, which among the required modules was this one.  I CPAN'd it
	at first, but BSDPAN is not pkgng aware.  So, decided to whip up this port.

	PR: ports/187622

net-mgmt/nagios-check_dhcp.pl

	Somewhere along the way a "portsnap update"(?) removed "net/p5-Net-DHCP-Watch" from my /usr/ports directory,
	causing "portmaster -a" to abort.  Been getting kind of annoying lately with all the reasons it'll abort or
	do the wrong thing now.

	It also complains (and aborts) that I have packages installed that have recently been deleted from ports.  I
	haven't yet investigated an alternative to net/freenx and net/nxserver, it still works (wasn't that long ago that I
	submitted a patch for one of the components.)  But, the company has stopped making it generally available to the
	public, though there's a note about it working with certain open source projects.  Along with it being available to
	its paying customers.

	Anyways back to this port, one way to fix this would be to hit send on my submission to make net/p5-Net-DHCP-Watch
	into a real port.  But, since the reason for its existence is this nagios plugin.  I went to work on turning this
	into a port as well.  Following other ports and reading stuff in /usr/ports/Mk, I did staging with this.

	Will I hit send?

	PR: ports/187623

sysutils/grive

	I got grive-0.2.0 to build on FreeBSD, and thought about turning it into a port.  But, I think I'm out of my depth
	on whether this is even right.  Plus I don't think I want to be stuck maintaining this.

sysutils/grive-devel

	I took a look at the current work on grive-0.3.0 on github, but it needs at least 0.10 of devel/json-c and ports
	stops at 0.9.  I had started work on making a patch to update the port, but sounds like development is heading
	towards needing 0.11 now.

	I then ran into it needing a new version of devel/boost-libs than currently in ports.  Given the difficulty I've had
	in buidling as a dependency of other ports, I don't feel like tackling this update.

	Plus I still haven't figured out how to do distfiles from GitHub, MASTER_SITES=GH

	Otherwise, I might reveal my stab at print/cups-cloud-print.

	2014-03-09 I happened to be poking around on github for some reason, which I that there had been a response to a
	year old issue for grive on making it compile on FreeBSD.  Had never looked through issues before, but there were
	only two notifications for grive, and one had the word "FreeBSD" in it...so it stood out.  The added response
	was the it builds fine from ports (net/grive).

	Wonder how long that had been there, wonder if there's some way to find out when new ports are added?  Perhaps
	something I could do at the same time as when I do my daily portsnap?

www/p5-LWP-Protocol-connect

	Seems to me that proxy support for connecting to https sites with LWP::UserAgent had been broken forever.

	But, then I get asked to install a commericial product's perl script into one of our nagios instances, and its
	relying on LWP::UserAgent's proxy method to work correctly when POST'ng to its https site.  Or, it still thinks
	people only run such sites were direct access to the Internet is possible.

	I tried various hacks to get the script working, while trying to avoid rewriting it as little as possible.

	After trying what I thought was similar to what I had done once before, but from memory as the machine has long
	expired and I didn't have a way to access its archive (or indication that the archive would have what it.)  And,
	it was late, so I was tired and hungry....so all the attempts were doomed.

	In the end, the next day...I eventually settled on trying LWP::Protocol::connect, the first of many different
	suggestions online, to overcome something has been broken for over 10(?) years in LWP::UserAgent, that worked!

	There was even an new version of LWP::UserAgent released on July 1st, 2014 (the previous on April 16th, 2014).
	But, updating all the Perl modules that updated between the time I had installed the product and when it stopped
	working didn't help.
	
	Though the application user said it had been working, but stopped on around the 4th of July.  Later admitting that
	they had changed delivery method from email to POST.  Thinking that POST would be more direct.  Except that email
	means sending through one of a pair of smtp servers behind our F5.  And, POST involves using one of a pair of proxy
	servers behind our F5.
	
	The smtp servers get regular attention, while the proxy servers which had been abandonned on us to provide haven't
	gotten any attention since they were setup some 7 years ago.  Though we're considering upgrading the F5's to 11.5.1
	and using its in-built proxy capabilities.

	Though now that its working, it probably avoids any network issues that would cause the message to queue on our
	smtp servers.  Instead remaining in its queue on our nagios server.

	Where before it was working was starting an instance that would block all future instances, and it runs every
	minute from cron, until it eventually runs out of memory/swap and things stop. (though it stopped early, because
	kern.maxswzone hadn't been adjusted to reflect the size of the swap volume...)

	Meanwhile, made a one line change to the applications perl script for

		$ua-proxy('https', $opt_proxy);

	And, then invoke the application with `--proxy=connect://<our-proxy-server>:8080/'

	When I tried it by hand, it worked immediately.  So, I made it permanent.

	The reason I resisted trying this in the beginning, was that it wasn't in ports and I hadn't heard whether BSDPAN
	had been updated to support pkgng.
	
	(Bug #187111, reported February 27, 2014 - only one comment, dated March 20, 2014, from somebody saying its a
	blocker for their client, where client is willing to pay.)

	So, I had immediately thrown together a port for it.  Somehow it worked with an invalid pkg-plist (forgot to update
	it from the port I had used as my template...p5-LWP-Protocol-https)....wonder what happens if I were to try to
	uninstall it?

	Cleaned up now, guess its time to submit it?

	PR: 191784

www/p5-WWW-Shorten-Yourls

	www/yourls exists in ports, but not this Perl module?

	PR: ports/180547
