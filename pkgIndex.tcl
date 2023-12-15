# -*- tcl -*-
# Tcl package index file, version 1.1
#
if {[package vsatisfies [package provide Tcl] 9.0-]} {
    package ifneeded thiredis 0.7 \
	    [list load [file join $dir libtcl9thiredis0.7.so] [string totitle thiredis]]
} else {
    package ifneeded thiredis 0.7 \
	    [list load [file join $dir libthiredis0.7.so] [string totitle thiredis]]
}
