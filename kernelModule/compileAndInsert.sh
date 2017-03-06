sudo make clean
sudo make
sudo mknod /dev/characterDevice c 60 0
sudo chmod 666 /dev/characterDevice
sudo insmod characterDevice.ko
echo "Done!\n"
