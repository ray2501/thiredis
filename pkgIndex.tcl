#
# Tcl package index file
#
package ifneeded thiredis 0.3 \
    [list load [file join $dir libthiredis0.3.so] thiredis]
