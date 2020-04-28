#!/bin/sh

esdir="$(dirname $0)"
while true; do
    rm -f /tmp/es-restart /tmp/es-sysrestart /tmp/es-shutdown /tmp/es-wifi
    "$esdir/emulationstation" "$@"
    ret=$?
    [ -f /tmp/es-restart ] && continue
    if [ -f /tmp/es-sysrestart ]; then
        rm -f /tmp/es-sysrestart
        sudo reboot
        break
    fi
    if [ -f /tmp/es-shutdown ]; then
        rm -f /tmp/es-shutdown
        sudo poweroff
        break
    fi
    if [ -f /tmp/es-wifi ]; then
        rm -f /tmp/es-wifi
        wifish
        break
    fi
    break
done
exit $ret
