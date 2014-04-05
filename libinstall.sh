#!/system/bin/sh

LIBNAME=libcancom.so
TMP_DIR=/data/local
INSTALL_DIR=/system/lib

echo "installing $LIBNAME"

#su
mount -o remount,rw -t ext4 /dev/block/mmcblk0p2 /system
sync
busybox cp $TMP_DIR/$LIBNAME $INSTALL_DIR/
busybox chmod 644 $INSTALL_DIR/$LIBNAME
sync
mount -o remount,ro -t ext4 /dev/block/mmcblk0p2 /system
sync

echo "installing $LIBNAME complete"
