#!/system/bin/sh

LIBNAME=libcancom.so
TMP_DIR=/data/local
INSTALL_DIR=/system/lib

echo "uninstalling $LIBNAME"

#su
mount -o remount,rw -t ext4 /dev/block/mmcblk0p2 /system
sync
busybox rm $INSTALL_DIR/$LIBNAME
sync
mount -o remount,ro -t ext4 /dev/block/mmcblk0p2 /system
sync

echo "uninstalling $LIBNAME complete"
