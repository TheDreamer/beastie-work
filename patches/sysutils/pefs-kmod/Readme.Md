Decided to give pefs a try.

After loading the module, I noticed in dmesg:

	cryptosoft0: <software crypto> on motherboard

Later I wondered if I work machine had an i7 with AESNI. So, I tried loading
that module.

This message appeared in dmesg:

	aesni0: <AES-CBC,AES-XTS> on motherboard

But, then I noticed that AESNI support is disabled in the build....its turned
on by a DEFINE.  Probably to have it build on a wider range of FreeBSD versions
it was disabled.  Though would be nice if it could be turned on for people
that want it and know they can, better if it could also tell if you can or can't
have it.

So, I quickly added a knob to turn it on.

Now when I load pefs on my work machine, I get:

	pefs: AESNI hardware acceleration enabled

in dmesg.

Now trying this at home (where I had only built it, but never got around to
trying it), I first try loading aesni.  These two lines appeared in dmsesg:

	cryptosoft0: <software crypto> on motherboard
	aesni0: No AESNI support.

I then unloaded aesni, resulting in this line in dmesg:

	cryptosoft0: detached

Rebuilt the port with AESNI enabled, and loaded int resulted in the same
two messages with loading aesni.  Guess that is expected, but wasn't
known because I did things out of order at work.

No message from pefs.  What I ideally want is something that'll work for
between home and work, and multiple operating systems.
