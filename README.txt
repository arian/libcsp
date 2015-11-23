                       libcsp - CSP For Posix Threads
                       ==============================

"Communicating Sequential Processes" is a mathematical algebra for analysing
the behaviour of concurrent systems. Because most software systems interact
with the real world, they are concurrent to a greater or lesser extent.

libcsp is a library that provides for C programmers a level of abstraction
that makes programming with threads much easier. By providing an easy
mechanism to express and reason about programs with multiple threads, it
becomes easier to fit software into the real world.


In essence, libcsp is a C library that provides the benefits previously
known to occam programmers:

    threads communicate by passing messages;

    when a message is passed beteeen threads, the sender and the receiver
       have to rendezvous in time - usually this means one thread has to
       wait for the other;

    receiver threads can select between alternate sources - called
        'alternation' in CSP;

    as in occam and unlike CSP itself, senders cannot alternate between
        output sources (this would cause serious implementation problems
        and is provably not necessary).


For further information, see

    libcsp website:
        http://www.beton.freeserve.co.uk/libcsp/

    CSP
        http://wotug.ukc.ac.uk/csp.shtml
        http://wotug.ukc.ac.uk/parallel/theory/formal/csp/

    occam
        http://archive.comlab.ox.ac.uk/occam.html
        http://wotug.ukc.ac.uk/parallel/occam/index.html


Changes
-------

v0.50 2001-06-12  Uploaded to Sourceforge.net

v0.41 2000-11-21  Minor change to commstime benchmark (but not to
                  how it measures!)

v0.4  2000-09-15  Improved channel algorithm.

v0.3  2000-09-08  Added commstime benchmark.

v0.2              Added alternation.

v0.1              First release. Buggy and very incomplete. Contains simple
                  channels only.


