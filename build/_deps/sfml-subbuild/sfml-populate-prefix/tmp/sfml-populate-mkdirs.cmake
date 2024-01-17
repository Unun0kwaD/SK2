# Distributed under the OSI-approved BSD 3-Clause License.  See accompanying
# file Copyright.txt or https://cmake.org/licensing for details.

cmake_minimum_required(VERSION 3.5)

file(MAKE_DIRECTORY
  "/home/ununkwad/Dokumenty/Sieci komputerowe 2 projekt/SK2/build/_deps/sfml-src"
  "/home/ununkwad/Dokumenty/Sieci komputerowe 2 projekt/SK2/build/_deps/sfml-build"
  "/home/ununkwad/Dokumenty/Sieci komputerowe 2 projekt/SK2/build/_deps/sfml-subbuild/sfml-populate-prefix"
  "/home/ununkwad/Dokumenty/Sieci komputerowe 2 projekt/SK2/build/_deps/sfml-subbuild/sfml-populate-prefix/tmp"
  "/home/ununkwad/Dokumenty/Sieci komputerowe 2 projekt/SK2/build/_deps/sfml-subbuild/sfml-populate-prefix/src/sfml-populate-stamp"
  "/home/ununkwad/Dokumenty/Sieci komputerowe 2 projekt/SK2/build/_deps/sfml-subbuild/sfml-populate-prefix/src"
  "/home/ununkwad/Dokumenty/Sieci komputerowe 2 projekt/SK2/build/_deps/sfml-subbuild/sfml-populate-prefix/src/sfml-populate-stamp"
)

set(configSubDirs )
foreach(subDir IN LISTS configSubDirs)
    file(MAKE_DIRECTORY "/home/ununkwad/Dokumenty/Sieci komputerowe 2 projekt/SK2/build/_deps/sfml-subbuild/sfml-populate-prefix/src/sfml-populate-stamp/${subDir}")
endforeach()
if(cfgdir)
  file(MAKE_DIRECTORY "/home/ununkwad/Dokumenty/Sieci komputerowe 2 projekt/SK2/build/_deps/sfml-subbuild/sfml-populate-prefix/src/sfml-populate-stamp${cfgdir}") # cfgdir has leading slash
endif()
