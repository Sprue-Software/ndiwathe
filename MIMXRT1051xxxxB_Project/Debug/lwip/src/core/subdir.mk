################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../lwip/src/core/altcp.c \
../lwip/src/core/altcp_alloc.c \
../lwip/src/core/altcp_tcp.c \
../lwip/src/core/def.c \
../lwip/src/core/dns.c \
../lwip/src/core/inet_chksum.c \
../lwip/src/core/init.c \
../lwip/src/core/ip.c \
../lwip/src/core/mem.c \
../lwip/src/core/memp.c \
../lwip/src/core/netif.c \
../lwip/src/core/pbuf.c \
../lwip/src/core/raw.c \
../lwip/src/core/stats.c \
../lwip/src/core/sys.c \
../lwip/src/core/tcp.c \
../lwip/src/core/tcp_in.c \
../lwip/src/core/tcp_out.c \
../lwip/src/core/timeouts.c \
../lwip/src/core/udp.c 

OBJS += \
./lwip/src/core/altcp.o \
./lwip/src/core/altcp_alloc.o \
./lwip/src/core/altcp_tcp.o \
./lwip/src/core/def.o \
./lwip/src/core/dns.o \
./lwip/src/core/inet_chksum.o \
./lwip/src/core/init.o \
./lwip/src/core/ip.o \
./lwip/src/core/mem.o \
./lwip/src/core/memp.o \
./lwip/src/core/netif.o \
./lwip/src/core/pbuf.o \
./lwip/src/core/raw.o \
./lwip/src/core/stats.o \
./lwip/src/core/sys.o \
./lwip/src/core/tcp.o \
./lwip/src/core/tcp_in.o \
./lwip/src/core/tcp_out.o \
./lwip/src/core/timeouts.o \
./lwip/src/core/udp.o 

C_DEPS += \
./lwip/src/core/altcp.d \
./lwip/src/core/altcp_alloc.d \
./lwip/src/core/altcp_tcp.d \
./lwip/src/core/def.d \
./lwip/src/core/dns.d \
./lwip/src/core/inet_chksum.d \
./lwip/src/core/init.d \
./lwip/src/core/ip.d \
./lwip/src/core/mem.d \
./lwip/src/core/memp.d \
./lwip/src/core/netif.d \
./lwip/src/core/pbuf.d \
./lwip/src/core/raw.d \
./lwip/src/core/stats.d \
./lwip/src/core/sys.d \
./lwip/src/core/tcp.d \
./lwip/src/core/tcp_in.d \
./lwip/src/core/tcp_out.d \
./lwip/src/core/timeouts.d \
./lwip/src/core/udp.d 


# Each subdirectory must supply rules for building sources it contributes
lwip/src/core/%.o: ../lwip/src/core/%.c
	@echo 'Building file: $<'
	@echo 'Invoking: MCU C Compiler'
	arm-none-eabi-gcc -std=gnu99 -DCPU_MIMXRT1052DVL6B -DCPU_MIMXRT1052DVL6B_cm7 -DSDK_DEBUGCONSOLE=1 -DXIP_EXTERNAL_FLASH=1 -DXIP_BOOT_HEADER_ENABLE=1 -DFSL_SDK_ENABLE_DRIVER_CACHE_CONTROL=0 -DHIGH_SPEED_SDIO_CLOCK -DUSE_RTOS=1 -DMBEDTLS_CONFIG_FILE='"aws_mbedtls_config.h"' -DFSL_RTOS_FREE_RTOS -DSERIAL_PORT_TYPE_UART=1 -DWIFI_WICED -DSDK_DEBUGCONSOLE_UART -DCR_INTEGER_PRINTF -DPRINTF_FLOAT_ENABLE=0 -D__MCUXPRESSO -D__USE_CMSIS -DDEBUG -DSDK_OS_FREE_RTOS -I"C:\Users\ndiwathe\Documents\MCUXpressoIDE_11.1.1_3241\workspace\EnsoAgent\component\serial_manager" -I"C:\Users\ndiwathe\Documents\MCUXpressoIDE_11.1.1_3241\workspace\EnsoAgent\amazon-freertos\libraries\freertos_plus\standard\utils\include" -I"C:\Users\ndiwathe\Documents\MCUXpressoIDE_11.1.1_3241\workspace\EnsoAgent\CMSIS" -I"C:\Users\ndiwathe\Documents\MCUXpressoIDE_11.1.1_3241\workspace\EnsoAgent\drivers" -I"C:\Users\ndiwathe\Documents\MCUXpressoIDE_11.1.1_3241\workspace\EnsoAgent\xip" -I"C:\Users\ndiwathe\Documents\MCUXpressoIDE_11.1.1_3241\workspace\EnsoAgent\wiced\43xxx_Wi-Fi\WICED\RTOS\FreeRTOS\WWD" -I"C:\Users\ndiwathe\Documents\MCUXpressoIDE_11.1.1_3241\workspace\EnsoAgent\wiced\43xxx_Wi-Fi\WICED\WWD\include\RTOS" -I"C:\Users\ndiwathe\Documents\MCUXpressoIDE_11.1.1_3241\workspace\EnsoAgent\wiced\43xxx_Wi-Fi\WICED\WWD\include\network" -I"C:\Users\ndiwathe\Documents\MCUXpressoIDE_11.1.1_3241\workspace\EnsoAgent\wiced\43xxx_Wi-Fi\WICED\WWD\include\platform" -I"C:\Users\ndiwathe\Documents\MCUXpressoIDE_11.1.1_3241\workspace\EnsoAgent\wiced\43xxx_Wi-Fi\WICED\WWD\include" -I"C:\Users\ndiwathe\Documents\MCUXpressoIDE_11.1.1_3241\workspace\EnsoAgent\wiced\43xxx_Wi-Fi\WICED\WWD\internal\bus_protocols\SDIO" -I"C:\Users\ndiwathe\Documents\MCUXpressoIDE_11.1.1_3241\workspace\EnsoAgent\wiced\43xxx_Wi-Fi\WICED\WWD\internal\bus_protocols" -I"C:\Users\ndiwathe\Documents\MCUXpressoIDE_11.1.1_3241\workspace\EnsoAgent\wiced\43xxx_Wi-Fi\WICED\WWD\internal" -I"C:\Users\ndiwathe\Documents\MCUXpressoIDE_11.1.1_3241\workspace\EnsoAgent\wiced\43xxx_Wi-Fi\WICED\WWD" -I"C:\Users\ndiwathe\Documents\MCUXpressoIDE_11.1.1_3241\workspace\EnsoAgent\wiced\43xxx_Wi-Fi\WICED\network\LwIP\WWD" -I"C:\Users\ndiwathe\Documents\MCUXpressoIDE_11.1.1_3241\workspace\EnsoAgent\wiced\43xxx_Wi-Fi\WICED\platform\MCU\LPC" -I"C:\Users\ndiwathe\Documents\MCUXpressoIDE_11.1.1_3241\workspace\EnsoAgent\wiced\43xxx_Wi-Fi\WICED\platform\MCU" -I"C:\Users\ndiwathe\Documents\MCUXpressoIDE_11.1.1_3241\workspace\EnsoAgent\wiced\43xxx_Wi-Fi\WICED\platform\include" -I"C:\Users\ndiwathe\Documents\MCUXpressoIDE_11.1.1_3241\workspace\EnsoAgent\wiced\43xxx_Wi-Fi\WICED\platform" -I"C:\Users\ndiwathe\Documents\MCUXpressoIDE_11.1.1_3241\workspace\EnsoAgent\wiced\43xxx_Wi-Fi\include" -I"C:\Users\ndiwathe\Documents\MCUXpressoIDE_11.1.1_3241\workspace\EnsoAgent\component\uart" -I"C:\Users\ndiwathe\Documents\MCUXpressoIDE_11.1.1_3241\workspace\EnsoAgent\sdmmc\inc" -I"C:\Users\ndiwathe\Documents\MCUXpressoIDE_11.1.1_3241\workspace\EnsoAgent\amazon-freertos\demos\dev_mode_key_provisioning\include" -I"C:\Users\ndiwathe\Documents\MCUXpressoIDE_11.1.1_3241\workspace\EnsoAgent\amazon-freertos\libraries\abstractions\platform\freertos\include\platform" -I"C:\Users\ndiwathe\Documents\MCUXpressoIDE_11.1.1_3241\workspace\EnsoAgent\amazon-freertos\libraries\abstractions\platform\include\platform" -I"C:\Users\ndiwathe\Documents\MCUXpressoIDE_11.1.1_3241\workspace\EnsoAgent\amazon-freertos\libraries\abstractions\platform\include\types" -I"C:\Users\ndiwathe\Documents\MCUXpressoIDE_11.1.1_3241\workspace\EnsoAgent\amazon-freertos\demos\include" -I"C:\Users\ndiwathe\Documents\MCUXpressoIDE_11.1.1_3241\workspace\EnsoAgent\amazon-freertos\demos\network_manager" -I"C:\Users\ndiwathe\Documents\MCUXpressoIDE_11.1.1_3241\workspace\EnsoAgent\sdmmc\port" -I"C:\Users\ndiwathe\Documents\MCUXpressoIDE_11.1.1_3241\workspace\EnsoAgent\amazon-freertos\libraries\freertos_plus\standard\pkcs11\include" -I"C:\Users\ndiwathe\Documents\MCUXpressoIDE_11.1.1_3241\workspace\EnsoAgent\amazon-freertos\freertos_kernel\include" -I"C:\Users\ndiwathe\Documents\MCUXpressoIDE_11.1.1_3241\workspace\EnsoAgent\amazon-freertos\libraries\freertos_plus\standard\tls\include" -I"C:\Users\ndiwathe\Documents\MCUXpressoIDE_11.1.1_3241\workspace\EnsoAgent\amazon-freertos\libraries\c_sdk\aws\shadow\include" -I"C:\Users\ndiwathe\Documents\MCUXpressoIDE_11.1.1_3241\workspace\EnsoAgent\amazon-freertos\libraries\c_sdk\aws\shadow\include\types" -I"C:\Users\ndiwathe\Documents\MCUXpressoIDE_11.1.1_3241\workspace\EnsoAgent\amazon-freertos\libraries\c_sdk\aws\shadow\src" -I"C:\Users\ndiwathe\Documents\MCUXpressoIDE_11.1.1_3241\workspace\EnsoAgent\amazon-freertos\libraries\c_sdk\aws\shadow\src\private" -I"C:\Users\ndiwathe\Documents\MCUXpressoIDE_11.1.1_3241\workspace\EnsoAgent\amazon-freertos\libraries\c_sdk\standard\common\include" -I"C:\Users\ndiwathe\Documents\MCUXpressoIDE_11.1.1_3241\workspace\EnsoAgent\amazon-freertos\libraries\c_sdk\standard\common\include\private" -I"C:\Users\ndiwathe\Documents\MCUXpressoIDE_11.1.1_3241\workspace\EnsoAgent\amazon-freertos\libraries\c_sdk\standard\common\include\types" -I"C:\Users\ndiwathe\Documents\MCUXpressoIDE_11.1.1_3241\workspace\EnsoAgent\amazon-freertos\libraries\c_sdk\standard\mqtt\include" -I"C:\Users\ndiwathe\Documents\MCUXpressoIDE_11.1.1_3241\workspace\EnsoAgent\amazon-freertos\libraries\c_sdk\standard\mqtt\include\types" -I"C:\Users\ndiwathe\Documents\MCUXpressoIDE_11.1.1_3241\workspace\EnsoAgent\amazon-freertos\libraries\c_sdk\standard\mqtt\src\private" -I"C:\Users\ndiwathe\Documents\MCUXpressoIDE_11.1.1_3241\workspace\EnsoAgent\amazon-freertos\libraries\c_sdk\standard\serializer\include" -I"C:\Users\ndiwathe\Documents\MCUXpressoIDE_11.1.1_3241\workspace\EnsoAgent\wiced\43xxx_Wi-Fi\platforms\MURATA_TYPE1DX" -I"C:\Users\ndiwathe\Documents\MCUXpressoIDE_11.1.1_3241\workspace\EnsoAgent\amazon-freertos\libraries\abstractions\wifi\include" -I"C:\Users\ndiwathe\Documents\MCUXpressoIDE_11.1.1_3241\workspace\EnsoAgent\mbedtls\port\ksdk" -I"C:\Users\ndiwathe\Documents\MCUXpressoIDE_11.1.1_3241\workspace\EnsoAgent\amazon-freertos\libraries\3rdparty\pkcs11" -I"C:\Users\ndiwathe\Documents\MCUXpressoIDE_11.1.1_3241\workspace\EnsoAgent\wiced\43xxx_Wi-Fi\WICED\WWD\internal\chips\4343W" -I"C:\Users\ndiwathe\Documents\MCUXpressoIDE_11.1.1_3241\workspace\EnsoAgent\amazon-freertos\libraries\3rdparty\jsmn" -I"C:\Users\ndiwathe\Documents\MCUXpressoIDE_11.1.1_3241\workspace\EnsoAgent\lwip\port" -I"C:\Users\ndiwathe\Documents\MCUXpressoIDE_11.1.1_3241\workspace\EnsoAgent\amazon-freertos\freertos_kernel\portable\GCC\ARM_CM4F" -I"C:\Users\ndiwathe\Documents\MCUXpressoIDE_11.1.1_3241\workspace\EnsoAgent\amazon-freertos\libraries\abstractions\secure_sockets\include" -I"C:\Users\ndiwathe\Documents\MCUXpressoIDE_11.1.1_3241\workspace\EnsoAgent\amazon-freertos\vendors\nxp\mflash\mimxrt1052" -I"C:\Users\ndiwathe\Documents\MCUXpressoIDE_11.1.1_3241\workspace\EnsoAgent\utilities" -I"C:\Users\ndiwathe\Documents\MCUXpressoIDE_11.1.1_3241\workspace\EnsoAgent\amazon-freertos\libraries\freertos_plus\standard\crypto\include" -I"C:\Users\ndiwathe\Documents\MCUXpressoIDE_11.1.1_3241\workspace\EnsoAgent\component\lists" -I"C:\Users\ndiwathe\Documents\MCUXpressoIDE_11.1.1_3241\workspace\EnsoAgent\device" -I"C:\Users\ndiwathe\Documents\MCUXpressoIDE_11.1.1_3241\workspace\EnsoAgent\board" -I"C:\Users\ndiwathe\Documents\MCUXpressoIDE_11.1.1_3241\workspace\EnsoAgent\source" -I"C:\Users\ndiwathe\Documents\MCUXpressoIDE_11.1.1_3241\workspace\EnsoAgent\amazon-freertos\libraries\abstractions\pkcs11\include" -I"C:\Users\ndiwathe\Documents\MCUXpressoIDE_11.1.1_3241\workspace\EnsoAgent\amazon-freertos\libraries\abstractions\pkcs11\mbedtls" -I"C:\Users\ndiwathe\Documents\MCUXpressoIDE_11.1.1_3241\workspace\EnsoAgent\lwip\port\arch" -I"C:\Users\ndiwathe\Documents\MCUXpressoIDE_11.1.1_3241\workspace\EnsoAgent\lwip\src\include\compat\posix\arpa" -I"C:\Users\ndiwathe\Documents\MCUXpressoIDE_11.1.1_3241\workspace\EnsoAgent\lwip\src\include\compat\posix\net" -I"C:\Users\ndiwathe\Documents\MCUXpressoIDE_11.1.1_3241\workspace\EnsoAgent\lwip\src\include\compat\posix" -I"C:\Users\ndiwathe\Documents\MCUXpressoIDE_11.1.1_3241\workspace\EnsoAgent\lwip\src\include\compat\posix\sys" -I"C:\Users\ndiwathe\Documents\MCUXpressoIDE_11.1.1_3241\workspace\EnsoAgent\lwip\src\include\compat\stdc" -I"C:\Users\ndiwathe\Documents\MCUXpressoIDE_11.1.1_3241\workspace\EnsoAgent\lwip\src\include\lwip" -I"C:\Users\ndiwathe\Documents\MCUXpressoIDE_11.1.1_3241\workspace\EnsoAgent\lwip\src\include\lwip\priv" -I"C:\Users\ndiwathe\Documents\MCUXpressoIDE_11.1.1_3241\workspace\EnsoAgent\lwip\src\include\lwip\prot" -I"C:\Users\ndiwathe\Documents\MCUXpressoIDE_11.1.1_3241\workspace\EnsoAgent\lwip\src\include\netif" -I"C:\Users\ndiwathe\Documents\MCUXpressoIDE_11.1.1_3241\workspace\EnsoAgent\lwip\src\include\netif\ppp" -I"C:\Users\ndiwathe\Documents\MCUXpressoIDE_11.1.1_3241\workspace\EnsoAgent\lwip\src\include\netif\ppp\polarssl" -I"C:\Users\ndiwathe\Documents\MCUXpressoIDE_11.1.1_3241\workspace\EnsoAgent\lwip\src" -I"C:\Users\ndiwathe\Documents\MCUXpressoIDE_11.1.1_3241\workspace\EnsoAgent\lwip\src\include" -I"C:\Users\ndiwathe\Documents\MCUXpressoIDE_11.1.1_3241\workspace\EnsoAgent\amazon-freertos\libraries\abstractions\platform\freertos\include" -I"C:\Users\ndiwathe\Documents\MCUXpressoIDE_11.1.1_3241\workspace\EnsoAgent\amazon-freertos\libraries\abstractions\platform\include" -I"C:\Users\ndiwathe\Documents\MCUXpressoIDE_11.1.1_3241\workspace\EnsoAgent\amazon-freertos\libraries\c_sdk\standard\mqtt\src" -I"C:\Users\ndiwathe\Documents\MCUXpressoIDE_11.1.1_3241\workspace\EnsoAgent\mbedtls\include" -I"C:\Users\ndiwathe\Documents\MCUXpressoIDE_11.1.1_3241\workspace\EnsoAgent\component\serial_manager" -I"C:\Users\ndiwathe\Documents\MCUXpressoIDE_11.1.1_3241\workspace\EnsoAgent\amazon-freertos\libraries\freertos_plus\standard\utils\include" -I"C:\Users\ndiwathe\Documents\MCUXpressoIDE_11.1.1_3241\workspace\EnsoAgent\CMSIS" -I"C:\Users\ndiwathe\Documents\MCUXpressoIDE_11.1.1_3241\workspace\EnsoAgent\drivers" -I"C:\Users\ndiwathe\Documents\MCUXpressoIDE_11.1.1_3241\workspace\EnsoAgent\xip" -I"C:\Users\ndiwathe\Documents\MCUXpressoIDE_11.1.1_3241\workspace\EnsoAgent\wiced\43xxx_Wi-Fi\WICED\RTOS\FreeRTOS\WWD" -I"C:\Users\ndiwathe\Documents\MCUXpressoIDE_11.1.1_3241\workspace\EnsoAgent\wiced\43xxx_Wi-Fi\WICED\WWD\include\RTOS" -I"C:\Users\ndiwathe\Documents\MCUXpressoIDE_11.1.1_3241\workspace\EnsoAgent\wiced\43xxx_Wi-Fi\WICED\WWD\include\network" -I"C:\Users\ndiwathe\Documents\MCUXpressoIDE_11.1.1_3241\workspace\EnsoAgent\wiced\43xxx_Wi-Fi\WICED\WWD\include\platform" -I"C:\Users\ndiwathe\Documents\MCUXpressoIDE_11.1.1_3241\workspace\EnsoAgent\wiced\43xxx_Wi-Fi\WICED\WWD\include" -I"C:\Users\ndiwathe\Documents\MCUXpressoIDE_11.1.1_3241\workspace\EnsoAgent\wiced\43xxx_Wi-Fi\WICED\WWD\internal\bus_protocols\SDIO" -I"C:\Users\ndiwathe\Documents\MCUXpressoIDE_11.1.1_3241\workspace\EnsoAgent\wiced\43xxx_Wi-Fi\WICED\WWD\internal\bus_protocols" -I"C:\Users\ndiwathe\Documents\MCUXpressoIDE_11.1.1_3241\workspace\EnsoAgent\wiced\43xxx_Wi-Fi\WICED\WWD\internal" -I"C:\Users\ndiwathe\Documents\MCUXpressoIDE_11.1.1_3241\workspace\EnsoAgent\wiced\43xxx_Wi-Fi\WICED\WWD" -I"C:\Users\ndiwathe\Documents\MCUXpressoIDE_11.1.1_3241\workspace\EnsoAgent\wiced\43xxx_Wi-Fi\WICED\network\LwIP\WWD" -I"C:\Users\ndiwathe\Documents\MCUXpressoIDE_11.1.1_3241\workspace\EnsoAgent\wiced\43xxx_Wi-Fi\WICED\platform\MCU\LPC" -I"C:\Users\ndiwathe\Documents\MCUXpressoIDE_11.1.1_3241\workspace\EnsoAgent\wiced\43xxx_Wi-Fi\WICED\platform\MCU" -I"C:\Users\ndiwathe\Documents\MCUXpressoIDE_11.1.1_3241\workspace\EnsoAgent\wiced\43xxx_Wi-Fi\WICED\platform\include" -I"C:\Users\ndiwathe\Documents\MCUXpressoIDE_11.1.1_3241\workspace\EnsoAgent\wiced\43xxx_Wi-Fi\WICED\platform" -I"C:\Users\ndiwathe\Documents\MCUXpressoIDE_11.1.1_3241\workspace\EnsoAgent\wiced\43xxx_Wi-Fi\include" -I"C:\Users\ndiwathe\Documents\MCUXpressoIDE_11.1.1_3241\workspace\EnsoAgent\component\uart" -I"C:\Users\ndiwathe\Documents\MCUXpressoIDE_11.1.1_3241\workspace\EnsoAgent\sdmmc\inc" -I"C:\Users\ndiwathe\Documents\MCUXpressoIDE_11.1.1_3241\workspace\EnsoAgent\amazon-freertos\demos\dev_mode_key_provisioning\include" -I"C:\Users\ndiwathe\Documents\MCUXpressoIDE_11.1.1_3241\workspace\EnsoAgent\amazon-freertos\libraries\abstractions\platform\freertos\include\platform" -I"C:\Users\ndiwathe\Documents\MCUXpressoIDE_11.1.1_3241\workspace\EnsoAgent\amazon-freertos\libraries\abstractions\platform\include\platform" -I"C:\Users\ndiwathe\Documents\MCUXpressoIDE_11.1.1_3241\workspace\EnsoAgent\amazon-freertos\libraries\abstractions\platform\include\types" -I"C:\Users\ndiwathe\Documents\MCUXpressoIDE_11.1.1_3241\workspace\EnsoAgent\amazon-freertos\demos\include" -I"C:\Users\ndiwathe\Documents\MCUXpressoIDE_11.1.1_3241\workspace\EnsoAgent\amazon-freertos\demos\network_manager" -I"C:\Users\ndiwathe\Documents\MCUXpressoIDE_11.1.1_3241\workspace\EnsoAgent\sdmmc\port" -I"C:\Users\ndiwathe\Documents\MCUXpressoIDE_11.1.1_3241\workspace\EnsoAgent\amazon-freertos\libraries\freertos_plus\standard\pkcs11\include" -I"C:\Users\ndiwathe\Documents\MCUXpressoIDE_11.1.1_3241\workspace\EnsoAgent\amazon-freertos\freertos_kernel\include" -I"C:\Users\ndiwathe\Documents\MCUXpressoIDE_11.1.1_3241\workspace\EnsoAgent\amazon-freertos\libraries\freertos_plus\standard\tls\include" -I"C:\Users\ndiwathe\Documents\MCUXpressoIDE_11.1.1_3241\workspace\EnsoAgent\amazon-freertos\libraries\c_sdk\aws\shadow\include" -I"C:\Users\ndiwathe\Documents\MCUXpressoIDE_11.1.1_3241\workspace\EnsoAgent\amazon-freertos\libraries\c_sdk\aws\shadow\include\types" -I"C:\Users\ndiwathe\Documents\MCUXpressoIDE_11.1.1_3241\workspace\EnsoAgent\amazon-freertos\libraries\c_sdk\aws\shadow\src" -I"C:\Users\ndiwathe\Documents\MCUXpressoIDE_11.1.1_3241\workspace\EnsoAgent\amazon-freertos\libraries\c_sdk\aws\shadow\src\private" -I"C:\Users\ndiwathe\Documents\MCUXpressoIDE_11.1.1_3241\workspace\EnsoAgent\amazon-freertos\libraries\c_sdk\standard\common\include" -I"C:\Users\ndiwathe\Documents\MCUXpressoIDE_11.1.1_3241\workspace\EnsoAgent\amazon-freertos\libraries\c_sdk\standard\common\include\private" -I"C:\Users\ndiwathe\Documents\MCUXpressoIDE_11.1.1_3241\workspace\EnsoAgent\amazon-freertos\libraries\c_sdk\standard\common\include\types" -I"C:\Users\ndiwathe\Documents\MCUXpressoIDE_11.1.1_3241\workspace\EnsoAgent\amazon-freertos\libraries\c_sdk\standard\mqtt\include" -I"C:\Users\ndiwathe\Documents\MCUXpressoIDE_11.1.1_3241\workspace\EnsoAgent\amazon-freertos\libraries\c_sdk\standard\mqtt\include\types" -I"C:\Users\ndiwathe\Documents\MCUXpressoIDE_11.1.1_3241\workspace\EnsoAgent\amazon-freertos\libraries\c_sdk\standard\mqtt\src\private" -I"C:\Users\ndiwathe\Documents\MCUXpressoIDE_11.1.1_3241\workspace\EnsoAgent\amazon-freertos\libraries\c_sdk\standard\serializer\include" -I"C:\Users\ndiwathe\Documents\MCUXpressoIDE_11.1.1_3241\workspace\EnsoAgent\wiced\43xxx_Wi-Fi\platforms\MURATA_TYPE1DX" -I"C:\Users\ndiwathe\Documents\MCUXpressoIDE_11.1.1_3241\workspace\EnsoAgent\amazon-freertos\libraries\abstractions\wifi\include" -I"C:\Users\ndiwathe\Documents\MCUXpressoIDE_11.1.1_3241\workspace\EnsoAgent\mbedtls\port\ksdk" -I"C:\Users\ndiwathe\Documents\MCUXpressoIDE_11.1.1_3241\workspace\EnsoAgent\amazon-freertos\libraries\3rdparty\pkcs11" -I"C:\Users\ndiwathe\Documents\MCUXpressoIDE_11.1.1_3241\workspace\EnsoAgent\wiced\43xxx_Wi-Fi\WICED\WWD\internal\chips\4343W" -I"C:\Users\ndiwathe\Documents\MCUXpressoIDE_11.1.1_3241\workspace\EnsoAgent\amazon-freertos\libraries\3rdparty\jsmn" -I"C:\Users\ndiwathe\Documents\MCUXpressoIDE_11.1.1_3241\workspace\EnsoAgent\amazon-freertos\freertos_kernel\portable\GCC\ARM_CM4F" -I"C:\Users\ndiwathe\Documents\MCUXpressoIDE_11.1.1_3241\workspace\EnsoAgent\amazon-freertos\libraries\abstractions\secure_sockets\include" -I"C:\Users\ndiwathe\Documents\MCUXpressoIDE_11.1.1_3241\workspace\EnsoAgent\amazon-freertos\vendors\nxp\mflash\mimxrt1052" -I"C:\Users\ndiwathe\Documents\MCUXpressoIDE_11.1.1_3241\workspace\EnsoAgent\utilities" -I"C:\Users\ndiwathe\Documents\MCUXpressoIDE_11.1.1_3241\workspace\EnsoAgent\amazon-freertos\libraries\freertos_plus\standard\crypto\include" -I"C:\Users\ndiwathe\Documents\MCUXpressoIDE_11.1.1_3241\workspace\EnsoAgent\component\lists" -I"C:\Users\ndiwathe\Documents\MCUXpressoIDE_11.1.1_3241\workspace\EnsoAgent\device" -I"C:\Users\ndiwathe\Documents\MCUXpressoIDE_11.1.1_3241\workspace\EnsoAgent\board" -I"C:\Users\ndiwathe\Documents\MCUXpressoIDE_11.1.1_3241\workspace\EnsoAgent\source" -I"C:\Users\ndiwathe\Documents\MCUXpressoIDE_11.1.1_3241\workspace\EnsoAgent\amazon-freertos\libraries\abstractions\pkcs11\include" -I"C:\Users\ndiwathe\Documents\MCUXpressoIDE_11.1.1_3241\workspace\EnsoAgent\amazon-freertos\libraries\abstractions\pkcs11\mbedtls" -I"C:\Users\ndiwathe\Documents\MCUXpressoIDE_11.1.1_3241\workspace\EnsoAgent\lwip\port\arch" -I"C:\Users\ndiwathe\Documents\MCUXpressoIDE_11.1.1_3241\workspace\EnsoAgent\lwip\src\include\compat\posix\arpa" -I"C:\Users\ndiwathe\Documents\MCUXpressoIDE_11.1.1_3241\workspace\EnsoAgent\lwip\src\include\compat\posix\net" -I"C:\Users\ndiwathe\Documents\MCUXpressoIDE_11.1.1_3241\workspace\EnsoAgent\lwip\src\include\compat\posix" -I"C:\Users\ndiwathe\Documents\MCUXpressoIDE_11.1.1_3241\workspace\EnsoAgent\lwip\src\include\compat\posix\sys" -I"C:\Users\ndiwathe\Documents\MCUXpressoIDE_11.1.1_3241\workspace\EnsoAgent\lwip\src\include\compat\stdc" -I"C:\Users\ndiwathe\Documents\MCUXpressoIDE_11.1.1_3241\workspace\EnsoAgent\lwip\src\include\lwip" -I"C:\Users\ndiwathe\Documents\MCUXpressoIDE_11.1.1_3241\workspace\EnsoAgent\lwip\src\include\lwip\priv" -I"C:\Users\ndiwathe\Documents\MCUXpressoIDE_11.1.1_3241\workspace\EnsoAgent\lwip\src\include\lwip\prot" -I"C:\Users\ndiwathe\Documents\MCUXpressoIDE_11.1.1_3241\workspace\EnsoAgent\lwip\src\include\netif" -I"C:\Users\ndiwathe\Documents\MCUXpressoIDE_11.1.1_3241\workspace\EnsoAgent\lwip\src\include\netif\ppp" -I"C:\Users\ndiwathe\Documents\MCUXpressoIDE_11.1.1_3241\workspace\EnsoAgent\lwip\src\include\netif\ppp\polarssl" -I"C:\Users\ndiwathe\Documents\MCUXpressoIDE_11.1.1_3241\workspace\EnsoAgent\lwip\port" -I"C:\Users\ndiwathe\Documents\MCUXpressoIDE_11.1.1_3241\workspace\EnsoAgent\lwip\src" -I"C:\Users\ndiwathe\Documents\MCUXpressoIDE_11.1.1_3241\workspace\EnsoAgent\lwip\src\include" -I"C:\Users\ndiwathe\Documents\MCUXpressoIDE_11.1.1_3241\workspace\EnsoAgent\amazon-freertos\libraries\abstractions\platform\freertos\include" -I"C:\Users\ndiwathe\Documents\MCUXpressoIDE_11.1.1_3241\workspace\EnsoAgent\amazon-freertos\libraries\abstractions\platform\include" -I"C:\Users\ndiwathe\Documents\MCUXpressoIDE_11.1.1_3241\workspace\EnsoAgent\amazon-freertos\libraries\c_sdk\standard\mqtt\src" -I"C:\Users\ndiwathe\Documents\MCUXpressoIDE_11.1.1_3241\workspace\EnsoAgent\mbedtls\include" -I"C:\Users\ndiwathe\Documents\MCUXpressoIDE_11.1.1_3241\workspace\EnsoAgent\drivers\freertos" -I"C:\Users\ndiwathe\Documents\MCUXpressoIDE_11.1.1_3241\workspace\EnsoAgent" -I"C:\Users\ndiwathe\Documents\MCUXpressoIDE_11.1.1_3241\workspace\EnsoAgent\amazon-freertos\config_files" -I"C:\Users\ndiwathe\Documents\MCUXpressoIDE_11.1.1_3241\workspace\EnsoAgent\source\Logger" -I"C:\Users\ndiwathe\Documents\MCUXpressoIDE_11.1.1_3241\workspace\EnsoAgent\source\OSAL" -I"C:\Users\ndiwathe\Documents\MCUXpressoIDE_11.1.1_3241\workspace\EnsoAgent\source\LocalShadow\Api" -I"C:\Users\ndiwathe\Documents\MCUXpressoIDE_11.1.1_3241\workspace\EnsoAgent\source\Applications\Common" -I"C:\Users\ndiwathe\Documents\MCUXpressoIDE_11.1.1_3241\workspace\EnsoAgent\source\OSAL\RT1050\wisafe_drv" -I"C:\Users\ndiwathe\Documents\MCUXpressoIDE_11.1.1_3241\workspace\EnsoAgent\source\OSAL\RT1050\efs" -I"C:\Users\ndiwathe\Documents\MCUXpressoIDE_11.1.1_3241\workspace\EnsoAgent\source\EnsoComms" -I"C:\Users\ndiwathe\Documents\MCUXpressoIDE_11.1.1_3241\workspace\EnsoAgent\source\HAL" -I"C:\Users\ndiwathe\Documents\MCUXpressoIDE_11.1.1_3241\workspace\EnsoAgent\source\LocalShadow\ObjectStore" -I"C:\Users\ndiwathe\Documents\MCUXpressoIDE_11.1.1_3241\workspace\EnsoAgent\source\Configuration\RT1050" -I"C:\Users\ndiwathe\Documents\MCUXpressoIDE_11.1.1_3241\workspace\EnsoAgent\source\OSAL\RT1050\spiflash" -I"C:\Users\ndiwathe\Documents\MCUXpressoIDE_11.1.1_3241\workspace\EnsoAgent\source\OSAL\RT1050\sntp" -I"C:\Users\ndiwathe\Documents\MCUXpressoIDE_11.1.1_3241\workspace\EnsoAgent\source\OSAL\RT1050\power" -I"C:\Users\ndiwathe\Documents\MCUXpressoIDE_11.1.1_3241\workspace\EnsoAgent\source\KeyStore" -I"C:\Users\ndiwathe\Documents\MCUXpressoIDE_11.1.1_3241\workspace\EnsoAgent\source\OSAL\RT1050\sto" -I"C:\Users\ndiwathe\Documents\MCUXpressoIDE_11.1.1_3241\workspace\EnsoAgent\source\OSAL\RT1050\led" -I"C:\Users\ndiwathe\Documents\MCUXpressoIDE_11.1.1_3241\workspace\EnsoAgent\source\CloudComms\AWS" -I"C:\Users\ndiwathe\Documents\MCUXpressoIDE_11.1.1_3241\workspace\EnsoAgent\source\CloudComms" -I"C:\Users\ndiwathe\Documents\MCUXpressoIDE_11.1.1_3241\workspace\EnsoAgent\source\DeviceHandlers\WiSafeHandler" -I"C:\Users\ndiwathe\Documents\MCUXpressoIDE_11.1.1_3241\workspace\EnsoAgent\source\DeviceHandlers\Api" -I"C:\Users\ndiwathe\Documents\MCUXpressoIDE_11.1.1_3241\workspace\EnsoAgent\source\DeviceHandlers\Common" -I"C:\Users\ndiwathe\Documents\MCUXpressoIDE_11.1.1_3241\workspace\EnsoAgent\source\DeviceHandlers\LEDHandler" -I"C:\Users\ndiwathe\Documents\MCUXpressoIDE_11.1.1_3241\workspace\EnsoAgent\source\DeviceHandlers\UpgradeHandler" -I"C:\Users\ndiwathe\Documents\MCUXpressoIDE_11.1.1_3241\workspace\EnsoAgent\source\Storage" -O0 -fno-common -g3 -Wall -fomit-frame-pointer  -c  -ffunction-sections  -fdata-sections  -ffreestanding  -fno-builtin -fmerge-constants -fmacro-prefix-map="../$(@D)/"=. -mcpu=cortex-m7 -mfpu=fpv5-d16 -mfloat-abi=hard -mthumb -fstack-usage -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@:%.o=%.o)" -MT"$(@:%.o=%.d)" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


