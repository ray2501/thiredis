#
# Tcl package index file
#
package ifneeded thiredis 0.4 \
    [list load [file join $dir libthiredis0.4.so] thiredis]
