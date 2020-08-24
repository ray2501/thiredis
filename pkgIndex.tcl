#
# Tcl package index file
#
package ifneeded thiredis 0.5 \
    [list load [file join $dir libthiredis0.5.so] thiredis]
