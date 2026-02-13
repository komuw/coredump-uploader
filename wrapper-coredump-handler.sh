#!/bin/bash

# This script receives the coredump on its standard input and metadata as arguments.

# 1. First, execute the standard systemd-coredump handler, passing the coredump
#    data (from stdin) and all metadata arguments ($@) to it.
/usr/lib/systemd/systemd-coredump "$@"

# 2. Next, execute your custom Python script, passing the same metadata arguments.
#    Run it in the background (&) so it doesn't block the kernel process.
/home/kw/mystuff/coredump-uploader/.venv/bin/python /home/kw/mystuff/coredump-uploader/handler.py "$@" &

# chmod +x wrapper-coredump-handler.sh

# cat /etc/sysctl.d/99-komu-custom-coredump.conf:
#   check specifiers at; https://www.man7.org/linux/man-pages/man5/core.5.html
#   kernel.core_pattern=|/home/kw/mystuff/coredump-uploader/wrapper-coredump-handler.sh %e %E %F %g %h %i %I %p %P %t %u
#
# sudo sysctl --system
# cat /proc/sys/kernel/core_pattern
#

# %%  A single % character.
# %c  Core file size soft resource limit of crashing process
#     (since Linux 2.6.24).
# %d  Dump mode—same as value returned by prctl(2)
#     PR_GET_DUMPABLE (since Linux 3.7).
# %e  The process or thread's comm value, which typically is the
#     same as the executable filename (without path prefix, and
#     truncated to a maximum of 15 characters), but may have
#     been modified to be something different; see the
#     discussion of /proc/pid/comm and /proc/pid/task/tid/comm
#     in proc(5).
# %E  Pathname of executable, with slashes ('/') replaced by
#     exclamation marks ('!') (since Linux 3.0).
# %F  PIDFD of dumped process (since Linux 6.16).
# %g  Numeric real GID of dumped process.
# %h  Hostname (same as nodename returned by uname(2)).
# %i  TID of thread that triggered core dump, as seen in the PID
#     namespace in which the thread resides (since Linux 3.18).
# %I  TID of thread that triggered core dump, as seen in the
#     initial PID namespace (since Linux 3.18).
# %p  PID of dumped process, as seen in the PID namespace in
#     which the process resides.
# %P  PID of dumped process, as seen in the initial PID
#     namespace (since Linux 3.12).
# %s  Number of signal causing dump.
# %t  Time of dump, expressed as seconds since the Epoch,
#     1970-01-01 00:00:00 +0000 (UTC).
# %u  Numeric real UID of dumped process.