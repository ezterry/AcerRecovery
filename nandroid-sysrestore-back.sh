#!/sbin/sh
cd $1
#since we have the new bootloader, just extract the UID from the 
#kernel commandline
awk '{m=match($0,/androidboot.serialno=([0-9a-zA-Z\.]*)/) ; print("0x" substr($0,RSTART+21,RLENGTH-21))}' < /proc/cmdline > uid.txt

#extract the first 6656k of MMC0
dd if=/dev/block/mmcblk0 of=mmcblk0_start.img bs=1024 count=6656

#done
sync
return 1
