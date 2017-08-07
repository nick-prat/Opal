# !/bin/sh
echo 'Installing libopal'
rm -r /usr/local/include/Opal
cp -r include/Opal /usr/local/include
rm /usr/local/lib/libopal.so
cp libopal.so /usr/local/lib
echo 'Done'
