#! /bin/bash
if [[ $1 == "DEBUG" ]]; then
	(cd ./bin/debug/ && exec ./OpenGL)
elif [[ $1 == "RELEASE" ]]; then
	(cd ./bin/release/ && exec ./OpenGL)
else
	echo "Unknown Build Type"
fi
