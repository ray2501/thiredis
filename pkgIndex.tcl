#
# Tcl package index file
#
package ifneeded thiredis 0.2 \
    [list load [file join $dir libthiredis0.2.so] thiredis]