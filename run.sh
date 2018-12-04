#!/bin/bash

docker run \
	-it \
	--name cachedoge \
	--privileged \
	-v "$(pwd)"/src:/root/src \
	cachedoge/v1 /bin/bash
