ESP8266_RTOS_SDK ProjectTemplate for vscode

#开发环境构建

 下载SDK，并设置IDF_PATH环境变量
    https://github.com/espressif/ESP8266_RTOS_SDK.git

 ·linux
	建议参考原始文档，当然，也可以考虑使用docker-ce进行构建
	
 ·windows 10 Pro
	a. 现在和安装Docker-ce，下载地址：https://download.docker.com/win/stable/Docker%20for%20Windows%20Installer.exe
	b. 打开Powershell，输入以下命令，下载Docker镜像
		docker pull liui/build-env:esp8266-rtos
	c. 输入以下命令，如果成功的打印出xtensa-lx106-elf-gcc版本好，代表安装成功
		docker run --rm liui/build-env:esp8266-rtos xtensa-lx106-elf-gcc -v

 ·windows (其他版本)
	因目前其他版本存在docker无法安装的情况，所以暂时其他版本暂不支持

#如何使用
	a.创建新项目
		1.复制当前目录到任意地方，并更名
		2.在VSCode中，打开工程目录
		3.依据实际需求，修改Makefile
		4.打开文件${workspaceDir}/.vscode/task.json，查找字符串“vscode_project_template”，将其替换成实际的项目输出名称
		
	b.项目配置
		1.根据实际需求，修改Makefile的项目名称，以及工程文件引用
		2.打开Powershell，执行下面命令，注意将${workspaceRoot}替换成你工程的路径。
		（这里吐槽一下，我尝试将这个${workspaceRoot}换成${pwd}.Path 让Powershell自动获取当前路径，然而和Docker就执行失败）
			docker run --rm -it -v ${env:IDF_PATH}:/opt/ESP8266_RTOS_SDK -v ${workspaceRoot}:/mnt/prj -w /mnt/prj -e PWD=/mnt/prj liui/build-env:esp8266-rtos make menuconfig
		3.开始正常流程配置
		
	c.可选的插件和下载
		1.推荐安装一个在状态栏运行vsc任务的扩展
		2.可考虑在.vscode/task.json 中添加如下命令，以支持下载。
		  但需要注意修改命令中的端口名称、SPI Flash参数、输出的名称等参数信息 
		
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
		