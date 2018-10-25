#!/bin/bash

docker run \
	-it \
	--name cachedoge \
	--privileged \
	--rm \
	-v "$(pwd)"/src:/root/src \
	cachedoge/v1 /bin/bash
