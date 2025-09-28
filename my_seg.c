#include <stdio.h>
#include <stdlib.h>

int main()
{
    int *ptr = NULL; // Declare a null pointer

    // Attempt to write to a null pointer, causing a segmentation fault
    printf("Attempting to dereference a null pointer...\n");
    *ptr = 10;

    printf("This line will not be reached.\n"); // This line will not be executed
    return 0;
}

/*
gcc -g my_seg.c -o my_seg
ulimit -c unlimited && ./my_seg
gdb my_seg core.583354.my_seg.1758864537.kw
  (gdb) bt
  (gdb) thread apply all bt

https://g.co/gemini/share/82790b325e57 : good.

man systemd.unit >> man_systemd_unit.txt

cat /proc/sys/kernel/core_pattern;


https://github.com/purecloudlabs/gprovision/blob/b79a0d8da330cf23243c651e8d923e9450eeccde/cmd/img/corer/corer.go#L87
https://github.com/purecloudlabs/gprovision/blob/b79a0d8da330cf23243c651e8d923e9450eeccde/pkg/corer/backtrace/gdb.go#L28
https://github.com/purecloudlabs/gprovision/blob/b79a0d8da330cf23243c651e8d923e9450eeccde/pkg/corer/backtrace/gdb.go#L39


# get systemd-coredump
# systemd will handle coredump if it is the init; https://systemd.io/COREDUMP/ unless another handler is already installed.
#
1. apt -y update;apt -y install systemd-coredump
     # when you do it changes; cat /proc/sys/kernel/core_pattern;
         # The command-line arguments(to systemd-coredump) can include any of the % specifiers listed in; https://www.man7.org/linux/man-pages/man5/core.5.html
         |/usr/lib/systemd/systemd-coredump %P %u %g %s %t 9223372036854775808 %h %d
2. coredumpctl list
3.
echo'
[Unit]
Description=Unikraft Sentry Coredump Uploader
After=systemd-coredump@.service

[Service]
Type=oneshot
ExecStart=/home/kw/mystuff/coredump-uploader/.venv/bin/python /home/kw/mystuff/coredump-uploader/handler.py %i %I %a % A %b %B %f %g %G %H %m %M %n %N %o %p %s %u %U %v
' > /etc/systemd/system/my-coredump-handler.service

sudo systemctl daemon-reload
systemctl list-unit-files | grep -i my

sudo apt update && sudo apt install gdb
# sudo apt install libc6-dbg # Maybe install debug symbols for core libraries like libc

def main():
    import sys
    print("====args======")
    print(sys.argv)
    log_file = "/tmp/coredump_log.txt"
    with open(log_file, "a") as f:
        f.write(f"Received arguments: {str(sys.argv)}\n\n")
    print("====args======")

if __name__ == "__main__":
    main()
python3 /root/handler.py ok

https://docs.sentry.io/platforms/go/enriching-events/
*/
