#!/system/bin/sh

(sleep 5; echo "bringing up can0"; ifconfig can0 up; echo "bringing up can0 complete")&


