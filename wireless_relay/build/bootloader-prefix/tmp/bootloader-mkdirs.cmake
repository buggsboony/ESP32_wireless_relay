# Distributed under the OSI-approved BSD 3-Clause License.  See accompanying
# file Copyright.txt or https://cmake.org/licensing for details.

cmake_minimum_required(VERSION 3.5)

file(MAKE_DIRECTORY
  "/home/boony/Documents/srcs/esp-idf/components/bootloader/subproject"
  "/home/boony/Documents/dev/IDF_PROJ/ESP32/ESP32_wireless_relay/wireless_relay/build/bootloader"
  "/home/boony/Documents/dev/IDF_PROJ/ESP32/ESP32_wireless_relay/wireless_relay/build/bootloader-prefix"
  "/home/boony/Documents/dev/IDF_PROJ/ESP32/ESP32_wireless_relay/wireless_relay/build/bootloader-prefix/tmp"
  "/home/boony/Documents/dev/IDF_PROJ/ESP32/ESP32_wireless_relay/wireless_relay/build/bootloader-prefix/src/bootloader-stamp"
  "/home/boony/Documents/dev/IDF_PROJ/ESP32/ESP32_wireless_relay/wireless_relay/build/bootloader-prefix/src"
  "/home/boony/Documents/dev/IDF_PROJ/ESP32/ESP32_wireless_relay/wireless_relay/build/bootloader-prefix/src/bootloader-stamp"
)

set(configSubDirs )
foreach(subDir IN LISTS configSubDirs)
    file(MAKE_DIRECTORY "/home/boony/Documents/dev/IDF_PROJ/ESP32/ESP32_wireless_relay/wireless_relay/build/bootloader-prefix/src/bootloader-stamp/${subDir}")
endforeach()
if(cfgdir)
  file(MAKE_DIRECTORY "/home/boony/Documents/dev/IDF_PROJ/ESP32/ESP32_wireless_relay/wireless_relay/build/bootloader-prefix/src/bootloader-stamp${cfgdir}") # cfgdir has leading slash
endif()
