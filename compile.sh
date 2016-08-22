if [ $1 == "DEBUG" ]; then 
	cp -rf ./src/Shaders/ ./bin/debug/
elif [ $1 == "RELEASE" ]; then
	cp -rf ./src/Shaders/ ./bin/release/
else
	echo "Unknown Build Type"
fi
make

