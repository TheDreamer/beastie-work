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

	Issue #3207 - configure fails to find libxml2kk

	configure doesn't find -lxml2, because it doesn't call
		"xml2-config --libs" to get the LDFLAGS needed to find it.

	-------------------------------------------

	I had started to write edit_line action to promise lines in an xml file
	but was having trouble with differing use of whitespace among the
	systems existing systems.  Largely because I had added the lines in
	by hand, but the xml file organization differs a bit on all 4 systems
	probably due to each system starting with different initial versions
	of the application.

	But, decided this would be useful for when I get around to building
	that new server.
