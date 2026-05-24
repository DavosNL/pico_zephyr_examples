#pico-zephyr-project
#-------------------------------------------------------------------------------
# Zephyr Example Application
#
# Copyright (c) 2021 Nordic Semiconductor ASA
# SPDX-License-Identifier: Apache-2.0
#-------------------------------------------------------------------------------
# NOTE: Please do not remove the #pico-zephyr-project header, it is used by
# the Raspberry Pi Pico VS Code extension to identify the project type.
#-------------------------------------------------------------------------------

cmake_minimum_required(VERSION 3.20.0)

find_package(Zephyr REQUIRED HINTS $ENV{ZEPHYR_BASE})
project(WiFi_example LANGUAGES C)

target_sources(app PRIVATE src/main.c)

# Lees environment variabelen
target_compile_definitions(app PRIVATE
    WIFI_SSID="$ENV{WIFI_SSID}"
    WIFI_PSK="$ENV{WIFI_PSK}"
)
