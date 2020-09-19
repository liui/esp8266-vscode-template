# esp8266-vscode-template

this repo is a clean esp8266 cookiecutter project for Windows 10 WSL2 with VS Code support. a cookiecutter project allows you to easily create new projects without copying around files, all you need is python and [cookiecutter](https://cookiecutter.readthedocs.io/en/latest/installation.html).

Before cookiecutting you must setup [Tool-chain and RTOS](#tool-chain-and-rtos). once you're done, cookiecut the project. if you've followed the instructions in this README default answers to cookiecutter responses should be good.

# TOC

- [How to use this repo](#how-to-use-this-repo)
  * [Creating A Project](#creating-a-project)
  * [Tool-chain and RTOS](#tool-chain-and-rtos)
    + [Changes required to compile](#changes-required-to-compile)
    + [Getting RTOS](#getting-rtos)
    + [Directory structure](#directory-structure)
  * [Flashing from WSL2](#flashing-from-wsl2)
    + [Flashing](#flashing)
      - [Windows Machine:](#windows-machine-)
      - [WSL2 Machine:](#wsl2-machine-)
- [Translation of the original document:](#translation-of-the-original-document-)
  * [Requirements](#requirements)
  * [How to use](#how-to-use)
    + [Create a new project](#create-a-new-project)
    + [Project configuration](#project-configuration)
    + [Optional plugins and downloads](#optional-plugins-and-downloads)

# How to use this repo

**NOTE**: 
1. *This repo is still work in progress.*
2. assuming workspace `~/dev` (all operations are done there)

## Creating A Project

1. Create a template for your project, note that project_name will also be the name of the directory

```bash
cookiecutter https://github.com/bedoron/esp8266-vscode-template
```

```bash
project_name [my-project-name]: your-project-name
```

2. `cd your-project-name` and hack away
   
3. Generate `sdkconfig.h` by going to wsl and running `make menuconfig`

## Tool-chain and RTOS
### Changes required to compile
* lwp branch: `2.1.2-esp` (fixed)
* libcoap git url is dead (fixed)
* edit `ESP8266_RTOS_SDK/components/lwip/apps/ping/ping_sock.c` and add missing `#include <string.h>` (requires manual fix)


### Getting RTOS
These instructions assume WSL2

A dependency of the Espressif repo is broken so the [original instructions](https://docs.espressif.com/projects/esp8266-rtos-sdk/en/latest/get-started/index.html#get-esp8266-rtos-sdk) might not work. here's a workaround:
```bash
git clone https://github.com/espressif/ESP8266_RTOS_SDK.git && \
    cd ESP8266_RTOS_SDK && \
    git config --file=.gitmodules submodule.components/coap/libcoap.url git@github.com:obgm/libcoap.git && \
	git config --file=.gitmodules submodule.components/lwip/lwip.branch 2.1.2-esp && \
    git submodule sync && \
    git submodule update --init --recursive --remote
```

run installation script

Next - get the compiler (we just need the header files here):
`wget https://dl.espressif.com/dl/xtensa-lx106-elf-linux64-1.22.0-100-ge567ec7-5.2.0.tar.gz`
and extract it to an adjacent library:
```bash
tar -xzf xtensa-lx106-elf-linux64-1.22.0-100-ge567ec7-5.2.0.tar.gz
```

Don't forget to create the environment variable `IDF_PATH` which points to the root folder of the repo above.

NOTE: Do not run vscode from WSL, intellisense will be VERY slow

### Directory structure
```
.
├── ESP8266_RTOS_SDK
├── my-cookiecutter-project
└── xtensa-lx106-elf
```

## Flashing from WSL2

### Flashing
Instructions based on:
https://github.com/espressif/esptool/wiki/Remote-Serial-Ports

#### Windows Machine:
1. Clone `git@github.com:espressif/esptool.git`
2. Make sure you have python 3+ & pip installed
3. `pip install pyserial`
4. run an elevated command prompt and run the server: 
   `python esp_rfc2217_server.py -p <PORT> COM<n>`

#### WSL2 Machine:
1. `make menuconfig` and configure com port to: `rfc2217://<ADDRESS_OF_WINDOWS>:<PORT>?ign_set_control`
   you can get the address by opening a windows command prompt and running `ipconfig`

2. `make flash`

3. you can also `make monitor` to inspect the output of the chip

# Translation of the original document:
ESP8266_RTOS_SDK ProjectTemplate for vscode

## Requirements

1. Setup IDF_PATH according to:  https://github.com/espressif/ESP8266_RTOS_SDK.git
2. Get the builder image: docker pull liui/build-env:esp8266-rtos
3. to get xtensa-lx106-elf-gcc version: `docker run --rm liui/build-env:esp8266-rtos xtensa-lx106-elf-gcc -v`

## How to use
### Create a new project
1. Copy the current directory to any place and rename it
2. In VSCode, open the project directory
3. Modify the Makefile according to actual needs
4. Open the file ${workspaceDir}/.vscode/task.json, find the string "vscode_project_template", and replace it with the actual project output name
To

### Project configuration
1. According to actual needs, modify the project name of the Makefile and the project file reference
2. Open Powershell, execute the following command, pay attention to replace `${workspaceRoot}` with the path of your project.
(Tucao here, I tried to replace this ${workspaceRoot} with ${pwd}.Path to let Powershell automatically obtain the current path, but the execution failed with Docker)
```bash
docker run --rm -it -v ${env:IDF_PATH}:/opt/ESP8266_RTOS_SDK -v ${workspaceRoot}:/mnt/prj -w /mnt/prj -e PWD=/mnt/prj liui/build-env:esp8266-rtos make menuconfig
```

3. Start the normal process configuration
To

### Optional plugins and downloads
1. It is recommended to install an extension that runs the vsc task in the status bar
2. Consider adding the following commands to .vscode/task.json to support downloading.
But you need to pay attention to modify the port name, SPI Flash parameters, output name and other parameter information in the command

```json
//.vscode/task.json
{
	"label": "download",
	"type": "shell",
	"windows": {
		"command": "python ${env:IDF_PATH}/components/esptool_py/esptool/esptool.py --chip esp8266 --port COM9 --baud 115200 --before default_reset --after hard_reset write_flash -z --flash_mode qio --flash_freq 40m --flash_size 4MB 0x0000 ./build/bootloader/bootloader.bin 0x10000 ./build/vscode_project_template.bin 0x8000 ./build/partitions_singleapp.bin"
	},
	"linux": {
		"command": "python ${IDF_PATH}/components/esptool_py/esptool/esptool.py --chip esp8266 --port /dev/ttyUSB0 --baud 115200 --before default_reset --after hard_reset write_flash -z --flash_mode qio --flash_freq 40m --flash_size 4MB 0x0000 ./build/bootloader/bootloader.bin 0x10000 ./build/vscode_project_template.bin 0x8000 ./build/partitions_singleapp.bin"
	},
}
```
