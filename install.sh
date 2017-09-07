# !/bin/sh
# echo 'Building Opal'
# if [ ninja ]; then
    # echo 'Build Success'
# else
    # echo 'Build Failed'
# fi
ninja
if [ $? -eq 0 ]; then
    echo 'Build Success'
    echo 'Installing libopal'
    rm -r /usr/local/include/Opal
    cp -r include/Opal /usr/local/include
    rm /usr/local/lib/libopal.so
    cp libopal.so /usr/local/lib
    echo 'Done'
else
    echo 'Build Failed'
    echo 'Exiting...'
fi
