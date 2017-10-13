# SneakyAmp

SneakyAmp is a super old WinAmp plugin that I wrote ages ago. It allows you to control Winamp from within other programs by watching the keyboard, and acting on commands you type between two control strings. 

I'm not sure if it works on modern versions of Windows, or what versions of WinAmp it'll work with (if you can find one). It was pushed to Github for posterity. I don't expect anyone will actually use it ever.

## Documentation
### These are the original docs, last updated around 2006

Requirements:
I've personally tested SneakyAmp on Windows 98SE and XP, it should also work on other versions. Besides that, your computer should be able to run Winamp 2. Also, for best functionality your sound card should support multiple sound streams - as most newer cards do. SneakyAmp *will not* work with Winamp 3, not even with the fancy Winamp 2 plugin loader someone wrote. 

How to get started and use the beast:

After installation, SneakyAmp should show up in Winamp's general plug-in list. Double-click SneakyAmp's entry to open the Options dialog for the first time.

If you want to use SneakyAmp, you must enable it using the checkbox in the upper corner of the options window

Also, you'll want to change at least one thing, the "Directory 1" entry. Just enter the path to your .mp3's directory. Also, make note of 2 items in the options Dialog:

"Begin Command"
-This is the string you use to start your input to SneakyAmp. It defaults to "winamp". I prefer using "/", but I left the default as "winamp" to not confuse people who still use defaults. 

"End Command"
-This is the string you use to end your input to SneakyAmp. This defaults to double quotes (""). Again, not the best choice, but for past users reading the new docs, this was left as the default.

Then just hit OK, and you should be set. Now, to test things out, hit ctrl-shift-1. If you haven't changed the hotkey settings, and everything is working okay, SneakyAmp will run off and scan your hard drive, and play some songs. If not, double check that your mp3 directory is correct, that you've checked the "Enable SneakyAmp" box, and that you actually have some songs to play.

Now, to actually play songs you want, you would type something like:
(Begin Command) play some song(End Command)
-or, with the defaults
winamp play some song""

With this, SneakyAmp will search your mp3 files for an .mp3 with the words "some" and "song" in it. Basically, it will look for filenames that fit the search key "*some*song*.mp3", and play all of those songs in Winamp.

Hotkeys:

Currently there are 15 different hotkeys defined. The first five are similar to Winamp's defauly keys for the main 5 buttons
Ctrl-Shift-Z - Previous Track
Ctrl-Shift-X - Play
Ctrl-Shift-C - Pause
Ctrl-Shift-V - Stop
Ctrl-Shift-B - Next Track
 
The other 10 hotkeys are defined by clicking to Hotkeys button in the options menu. And example, "play * rand 5" for ctrl-shift-1, is already defined. You use the commands explained in the next section.
 
Commands:

play
-plays the songs it finds (Ex: winamp play super freak""), or just hits the Play button (Ex: winamp play"")
 
list
-Opens a m3u file, and starts playing the songs it lists. (Ex: winamp list goodsongs"" )

add
-adds the songs it finds to the playlist (Ex: winamp add super freak"")

stop
-Stops Winamp (Ex: winamp stop"")

pause
-Pauses/Unpauses Winamp (Ex: winamp pause"")

next
-Skips to the next song in the playlist (Ex: winamp next"")

previous
-Skips to the previous song in the playlist (Ex: winamp prev"")

forward
-Jumps forward within a playing song a default amount (Ex: winamp forward"") or a specified amount in seconds (Ex: winamp forward 20"")

rewind
-Jumps backwards within a playing song a default amount (Ex: winamp rewind"") or a specified amount in seconds (Ex: winamp rewind 20"")

volume
-Sets Winamp's volume to a value from 0 to 255 (Ex: winamp volume 100"")

balance
-Sets Winamp's balance to a value from -127 to 127 (Ex: winamp balance 0"")

shuffle
-Manipulates Winamp's shuffle button. Either toggles it (Ex: winamp shuffle""), turns it on (Ex: winamp shuffle on""), or turns it off (Ex: winamp shuffle off"").

repeat
-Manipulates Winamp's repeat button. Either toggles it (Ex: winamp repeat""), turns it on (Ex: winamp repeat on""), or turns it off (Ex: winamp repeat off"").

random
-Used in conjunction with the add or play command, picks a specified number of songs to play out of all the matches (Ex: winamp play * rand 10"" (plays any 10 random songs), winamp play jump rand 5"" (plays 5 randomly picked songs that have 'jump' in the filename))

Trickier features:

The Directory prefix
-Used in conjuction with the add and play commands for specifying which directory to search in. For example, to play all the song in the directory "Frank": winamp play * -frank"". Or, to play a random 10 songs in the "Big Shoe" directory with "the" in the filename: winamp play the rand 10 -big -shoe"". Yeah, its tricky, but you'll get it. :)

The 'Assume MP3 extenstion' checkbox
-All this checkbox in the options does is add .mp3 to your search key when using the add or play command. For example, with the box checked `winamp play bean""will search for *bean*.mp3. Without the box checked it will search for *bean*.* . Not really a tricky feature, but I didn't know where to put it. :)

The Partial Command Feature
-Wow, thats a bad name for a feature. Anyways, this just means that you dont have to type in the whole command. For example, with the default settings, 'winamp play""', 'winamp pl""', and 'winamp ay""' all do the same thing. This is quite handy for shortening command names once you've got the hang of SneakyAmp...
