sudo make clean
sudo make
sudo mknod /dev/ATtiny85 c 60 0
sudo chmod 666 /dev/ATtiny85
sudo insmod ATtiny85.ko
echo "Done!\n"
