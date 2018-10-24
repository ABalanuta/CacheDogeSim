#!/bin/bash

docker run -it --name cachedoge --rm -v "$(pwd)"/src:/root/src cachedoge/v1 /bin/bash
