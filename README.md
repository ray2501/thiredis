thiredis
=====

Tcl wrapper for Redis client [hiredis](https://github.com/redis/hiredis).
It is using Synchronous API and only for test.


License
=====

BSD 3-Clause License


UNIX BUILD
=====

Building under most UNIX systems is easy, just run the configure script and 
then run make. For more information about the build process, see the 
tcl/unix/README file in the Tcl src dist. The following minimal example will 
install the extension in the /opt/tcl directory.

    $ cd thiredis
    $ ./configure --prefix=/opt/tcl
    $ make
    $ make install

If you need setup directory containing tcl configuration (tclConfig.sh), 
below is an example:

    $ cd thiredis
    $ ./configure --with-tcl=/opt/activetcl/lib
    $ make
    $ make install


Commands
=====

hiredis connect hostname port  
hiredis connectUnix path  
hiredis disconnect  
hiredis command request  
hiredis appendcommand request  
hiredis reply  


Example
=====

    package require thiredis
    hiredis connect localhost 6379

    set result [hiredis command "AUTH danilo"]
    puts "AUTH: $result"

    set result [hiredis command "PING"]
    puts "PING: $result"

    set result [hiredis command "SET foo Hello"]
    puts "SET: $result"

    set result [hiredis command "GET foo"]
    puts "GET: $result"

    hiredis appendcommand "SET cow king"
    hiredis appendcommand "GET cow"
    set result [hiredis reply]
    puts $result
    set result [hiredis reply]
    puts $result

    hiredis disconnect

