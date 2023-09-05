#!/bin/bash
#
if [ -z $1 ] || [ -z $2 ]
then
    echo "[FAILED] Invalid arguments."
    exit 1
fi
if ip addr ls dev $1 > /dev/null 2>&1
then
    echo "[  OK  ] Found device $1."
    if ip link set $1 down
    then
        ip link set $1 type can bitrate $2 restart-ms 100
        ip link set $1 up
        echo "[  OK  ] Enabled SocketCAN interface $1 (bitrate = $2)."
        exit 0
    else
        echo "[FAILED] Failed to configure and enable SocketCAN interface $1."
        exit 3
    fi
else
    echo "[FAILED] Failed to found device $1."
    exit 2
fi
