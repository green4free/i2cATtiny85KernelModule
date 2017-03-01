sudo make clean
sudo make
sudo mknod /dev/ATtiny c 60 0
sudo chmod 666 /dev/ATtiny
sudo insmod ATtiny.ko
echo "Done!\n"
