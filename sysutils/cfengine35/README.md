Patches
=======
sysutils/cfengine35

	Kind of odd making a patch for FreeBSD when the problem is upstream.
	
	So, opted to report it upstream, but in the meantime I rebuild cfengine
	using this patch.

	-------------------------------------------

	Issue #3207 - configure fails to find libxml2

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

	-------------------------------------------

	Also did a send-pr, since one way or another it'll affect the port.

	ports/180896:

	sysutils/cfengine35: can't use edit_xml because libxml2 not found

	========================================================================

	patch-3.5.1-3 - ports/181509

	Noticed that the new CFEngine Standard Library for 3.5.1 is missing,
	so that's what 3.5.1-3 is.

	Interesting problem about no '-' allowed in PORTVERSION, also that
	dist file extracts into cfengine-3.5.1.  Guess we'll see if I got
	it right.

	Had been using cfengine_stdlib.cf from 3.5.0, had forgotten to look
	to see if there had been an update with 3.5.1.

	There was only an extra body and an extra bundle, though didn't look
	to see if anything in the bundles or bodys changed.  Though had noticed
	some minor changes like

	useshell => "useshell" vs useshell => "true"

	And, some stuff in debian package management....

	And, I guess ssh has been added to services, but there's still no
	FreeBSD support in there.  So, I've been using my own version...which
	I'm not sure is working right or not.  Using inform, seems to get a
	lot of messages about things being wrong when they are in fact right.

	Like it'll say that the edit_line I'm doing will be non-convergent
	unless I'm starting with an emtpy file.  Which I clearly am!  So,
	I guess the "Service 'foo' could not be invoked successfully" is its
	way of saying the service is running so there was no need to start it?

	Plus 3.5.x is just a lot more noise than signal, which makes it harder
	to figure out why things aren't working when they don't.
