OTHER
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
