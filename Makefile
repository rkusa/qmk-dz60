build:
	cp -r ./keyboards ./qmk_firmware/
	cd ./qmk_firmware && ./util/docker_build.sh dz60:rkusa
	mv ./qmk_firmware/dz60_rkusa.hex .