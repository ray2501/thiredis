#
# Tcl package index file
#
package ifneeded thiredis 0.6 \
    [list load [file join $dir libthiredis0.6.so] thiredis]
