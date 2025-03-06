# hmm-cpp

A C++ implementation of Haxe Module Manager [hmm](https://github.com/andywhite37/hmm)

# Installation

Download the latest release from the [releases](https://github.com/maybekoi/hmm-cpp/releases) page.
Put the hmm.exe file somewhere and then add the path to the hmm.exe file to your system's PATH environment variable.

# differences between regular hmm and hmm-cpp

* uhh the language it was coded in

* 2 new commands:

* hmm set (sets the version of a haxelib, works exactly like the "haxelib set" command but this edits hmm.json too!) (totally didn't look at [ninjamuffin's issue in the regular hmm git repo](https://github.com/andywhite37/hmm/issues/25) 

and

* hmm list (lists all of your installed haxelib libraries, and if it uses git it shows the commit and branch!) (totally didn't look at [ninjamuffin's issue in the regular hmm git repo](https://github.com/andywhite37/hmm/issues/24) 

thats all I think

# Credits

- [andywhite37](https://github.com/andywhite37) - Original hmm project
- [maybekoi](https://github.com/maybekoi) - C++ implementation
- [ninjamuffin99](https://github.com/ninjamuffin99) - hmm set and hmm list command ideas
