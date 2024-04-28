# Distributed under the OSI-approved BSD 3-Clause License.  See accompanying
# file Copyright.txt or https://cmake.org/licensing for details.

cmake_minimum_required(VERSION 3.5)

file(MAKE_DIRECTORY
  "/home/boony/Documents/srcs/esp-idf/components/bootloader/subproject"
  "/run/media/boony/data/boony/documents/dev/IDF_PROJS/ESP32_wireless_relay/wireless_relay/build/bootloader"
  "/run/media/boony/data/boony/documents/dev/IDF_PROJS/ESP32_wireless_relay/wireless_relay/build/bootloader-prefix"
  "/run/media/boony/data/boony/documents/dev/IDF_PROJS/ESP32_wireless_relay/wireless_relay/build/bootloader-prefix/tmp"
  "/run/media/boony/data/boony/documents/dev/IDF_PROJS/ESP32_wireless_relay/wireless_relay/build/bootloader-prefix/src/bootloader-stamp"
  "/run/media/boony/data/boony/documents/dev/IDF_PROJS/ESP32_wireless_relay/wireless_relay/build/bootloader-prefix/src"
  "/run/media/boony/data/boony/documents/dev/IDF_PROJS/ESP32_wireless_relay/wireless_relay/build/bootloader-prefix/src/bootloader-stamp"
)

set(configSubDirs )
foreach(subDir IN LISTS configSubDirs)
    file(MAKE_DIRECTORY "/run/media/boony/data/boony/documents/dev/IDF_PROJS/ESP32_wireless_relay/wireless_relay/build/bootloader-prefix/src/bootloader-stamp/${subDir}")
endforeach()
if(cfgdir)
  file(MAKE_DIRECTORY "/run/media/boony/data/boony/documents/dev/IDF_PROJS/ESP32_wireless_relay/wireless_relay/build/bootloader-prefix/src/bootloader-stamp${cfgdir}") # cfgdir has leading slash
endif()
