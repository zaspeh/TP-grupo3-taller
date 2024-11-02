# Distributed under the OSI-approved BSD 3-Clause License.  See accompanying
# file Copyright.txt or https://cmake.org/licensing for details.

cmake_minimum_required(VERSION 3.22.1)

# We name the project and the target for the ExternalProject_Add() call
# to something that will highlight to the user what we are working on if
# something goes wrong and an error message is produced.

project(libsdl2pp-populate NONE)



include(ExternalProject)
ExternalProject_Add(libsdl2pp-populate
                     "UPDATE_DISCONNECTED" "False" "URL" "https://github.com/libSDL2pp/libSDL2pp/archive/cc198c9a5657048bee67ece82de620b2d5661084.zip"
                    SOURCE_DIR          "/home/joseph/Escritorio/TALLER/TP-grupo3/build/_deps/libsdl2pp-src"
                    BINARY_DIR          "/home/joseph/Escritorio/TALLER/TP-grupo3/build/_deps/libsdl2pp-build"
                    CONFIGURE_COMMAND   ""
                    BUILD_COMMAND       ""
                    INSTALL_COMMAND     ""
                    TEST_COMMAND        ""
                    USES_TERMINAL_DOWNLOAD  YES
                    USES_TERMINAL_UPDATE    YES
)


