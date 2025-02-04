# -*- tcl -*-
# Tcl package index file, version 1.1
#
if {[package vsatisfies [package provide Tcl] 9.0-]} {
    package ifneeded thiredis 0.8 \
	    [list load [file join $dir libtcl9thiredis0.8.so] [string totitle thiredis]]
} else {
    package ifneeded thiredis 0.8 \
	    [list load [file join $dir libthiredis0.8.so] [string totitle thiredis]]
}
