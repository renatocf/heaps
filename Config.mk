
################################################################################
##                                   HEAPS                                    ##
################################################################################

# Project info
# ===============
PROJECT         := Heaps
VERSION         := 1.0.9
STD_NAMESPACE   := # heap
GIT_REMOTE_PATH :=
MAINTEINER_NAME := Renato Cordeiro Ferreira
MAINTEINER_MAIL := renato.cferreira@hotmail.com
COPYRIGHT       := Renato Cordeiro Ferreira
SYNOPSIS        := Heaps - a heap library implementing heaps \
                   to compare their performance
DESCRIPTION     := Heaps - a heap library implementing heaps \
                   to compare their performance

# Program settings
# ==================
BENCHBIN        := benchmark
SHRLIB          := # one is a dir, all srcs within will

# Flags
# =======
CPPFLAGS        := # Precompiler Flags
CXXFLAGS        := -std=c++14 -Wall -Wextra -Wpedantic -g
LDFLAGS         := -g # Linker flags

# Makeball list
# ===============
# 'include conf/makeball.mk' for pre-configured options
# to use the library 'makeball'
include conf/googletest.mk
include conf/googlebenchmark.mk
