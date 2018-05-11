#	-----------------------------------------------------------------------
#
#	Copyright 2017 Markus Toepfer
#
#	Licensed under the Apache License, Version 2.0 (the "License");
#	you may not use this file except in compliance with the License.
#	You may obtain a copy of the License at
#
#		http://www.apache.org/licenses/LICENSE-2.0
#
#	Unless required by applicable law or agreed to in writing, software
#	distributed under the License is distributed on an "AS IS" BASIS,
#	WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
#	See the License for the specific language governing permissions and
#	limitations under the License.
#
#	This file is part of the testrun project. http://testrun.info
#
#	-----------------------------------------------------------------------
#
#	Content		Generic makefile for a project using 
#			the testrun framework.
#
#	Description	Target of this makefile is an independent library or 
#			executable to be installed under the directory 
#			PREFIX/lib or PREFIX/bin 
#	
#	Authors		Markus Toepfer
#	
#	Usage		An EXAMPLE makefile using this makefile may look like:
#			------------------------------------------------------
#			PROJECT		:= <name>
#			PROJECT_URL	:= <e.g. http://testrun.info>
#			VERSION		:= <number>
#
#			HOME		= ../../../
#			PROJECTMK	:= $(abspath \
#					$(lastword $(MAKEFILE_LIST)))
#
#			# base directory to install the program
#			PREFIX		:= /usr/local/
#
#			# used libraries
#			# LIBS		+= -lsystemd
#
#			# extra CFLAGS
#			# MODCFLAGS	+=
#
#			include testrun_makefile.src
#			-------------------------------------------------------
#
#	------------------------------------------------------------------------
#
# Switch on colors
GCC_COLORS ?= 'gcc colors available, use them!'
export GCC_COLORS

# ----- PARAMETER DEFINITION --------------------------------------------------

CC = gcc

# CFLAGS
# -g		enable Debugging symbols
# -Ox		code optimization
# -Wall		enable Warnings
# -Wextra	additional Warnings

CFLAGS		= -c -Wall -Wextra -fPIC
CFLAGS		+= $(EXTRAHEADER)
CFLAGS		+= $(MODCFLAGS)

PROJECTPATH 	:= $(abspath $(dir $(PROJECTMK)))
DIRNAME		:= $(notdir $(patsubst %/,%,$(dir $(PROJECTMK))))

LIBNAME		:= lib$(DIRNAME)
LIBNAMEPC	:= $(LIBNAME).pc

INSTALL		:= install

INCDIR		:= $(PREFIX)/include/$(DIRNAME)
LIBDIR		:= $(PREFIX)/lib
EXECDIR		:= $(PREFIX)/bin
PRODIR		:= $(LIBDIR)/$(DIRNAME)

EXECUTABLE	= bin/$(DIRNAME)

CFLAGS		+= -Iinclude

MODMAKE		:= $(patsubst %,src/%/mod.mk,$(MODULES))

STATIC		= build/lib$(DIRNAME).a
SHARED		= $(patsubst %.a,%.so,$(STATIC))

# Source and object files to compile
SOURCES		= $(wildcard src/**/*.c src/*.c)
OBJECTS		= $(patsubst %.c,%.o,$(SOURCES))

# ----- DEFAULT MAKE RULES ----------------------------------------------------

%.o : %.c
	@echo " (CC)    $@"
	@$(CC) $(CFLAGS) -g -o $@ -c $< $(LIBS)

all:		all_exec
install:	install_exec
uninstall:	uninstall_exec

all_lib:	start lib tests pkgconfig done
all_exec:	start lib tests $(EXECUTABLE) done
all_service: 	all_exec

lib:		build sources
sources:	build $(STATIC) $(SHARED)

$(STATIC):  $(OBJECTS)
	@echo " (AR)    $@ $(OBJECTS)"
	@ar rcs $@ $(OBJECTS)
	@ranlib $@

$(SHARED): $(STATIC) $(OBJECTS)
	@echo " (CC)    $@ $(OBJECTS)"
	@$(CC) -shared -o $@ $(OBJECTS) $(LIBS)

$(EXECUTABLE): $(OBJECTS)
	@echo " (CC)    $@ $(OBJECTS)"
	$(CC) -o $@ $(STATIC) $(LIBS) 

# ----- BUILD & CLEANUP -----------------------------------------------------

build:
	@mkdir -p bin
	@mkdir -p build
	@mkdir -p build/test
	@mkdir -p build/test/unit
	@mkdir -p build/test/acceptance
	@mkdir -p build/test/log
	@echo " (MK)    directories for build"

.PHONY: clean
clean:
	@echo " (CLEAN) $(LIBNAME)"
	@rm -rf build bin $(OBJECTS) $(TESTS_OBJECTS) \
		$(LIBNAMEPC) $(TESTS_TMP_FILES)

# ----- DOCUMENATION -------------------------------------------------------

.PHONY: documentation
documentation:
	doxygen doxygen/doxygen.config

# ----- PKGCONFIG LIBRARY BUILD --------------------------------------------

.PHONY: pkgconfig
pkgconfig:
	@echo 'prefix='$(PREFIX) > $(LIBNAMEPC)
	@echo 'exec_prefix=$${prefix}' >> $(LIBNAMEPC)
	@echo 'libdir=$${prefix}/lib' >> $(LIBNAMEPC)
	@echo 'includedir=$${prefix}/include' >> $(LIBNAMEPC)
	@echo '' >>$(LIBNAMEPC)
	@echo 'Name: '$(LIBNAME) >> $(LIBNAMEPC)
	@echo 'Description: ' >> $(LIBNAMEPC)
	@echo 'Version: '$(VERSION) >> $(LIBNAMEPC)
	@echo 'URL: '$(PROJECT_URL) >> $(LIBNAMEPC)
	@echo 'Libs: -L$${libdir} -l'$(DIRNAME) >> $(LIBNAMEPC)
	@echo 'Cflags: -I$${includedir}' >> $(LIBNAMEPC)

# ----- INSTALLATION ----------------------------------------------------------

# Installation as a library --------------------------------------------------

install_lib: $(SHARED) $(STATIC)
	@echo " (OK)    installed $(LIBNAME) to $(LIBDIR)"
	@mkdir -p $(PRODIR)
	@mkdir -p $(LIBDIR)/pkgconfig
	@mkdir -p $(INCDIR)
	@$(INSTALL) -m 0644 -t $(INCDIR) $(shell find include -name "*.h")
	@$(INSTALL) -m 0755 $(SHARED) $(PRODIR)
	@$(INSTALL) -m 0755 $(STATIC) $(PRODIR)
	@$(INSTALL) -m 0644 $(LIBNAMEPC) $(LIBDIR)/pkgconfig
	@ldconfig $(PRODIR)

uninstall_lib:
	@echo " (OK)    uninstalled $(LIBNAME) from $(LIBDIR)"
	@rm -rf $(INCDIR)
	@rm -rf $(PRODIR)
	@rm -rf $(PRODIR)/$(LIBNAME).a
	@rm -rf $(PRODIR)/$(LIBNAME).so
	@rm -rf $(LIBDIR)/pkgconfig/$(LIBNAMEPC)

# Installation as an executable ----------------------------------------------

install_exec: $(SHARED) $(STATIC)
	@echo " (OK)    installed $(DIRNAME) to $(EXECDIR)"
	@$(INSTALL) -m 0755 bin/$(DIRNAME) $(EXECDIR)

uninstall_exec:
	@echo " (OK)    uninstalled $(DIRNAME) from $(EXECDIR)"
	@rm -rf $(EXECDIR)/$(DIRNAME)

# Installation as a service (outsourced to script)----------------------------

install_service: $(EXECUTABLE)
	./config/install/install.sh

uninstall_service:
	./config/install/uninstall.sh

# ----- INFORMATION PRINTING --------------------------------------------------

# print out a variable of the make file (e.g. "make print-PROJECTPATH")
.PHONY: print
print-%  : ; @echo $* = $($*)

.PHONY: start
start:
	@echo "\n (HINT)    $(PROJECT) \t\t ==> running make\n"

.PHONY: done
done:
	@echo
	@echo " (DONE)  make $(PROJECT)"
	@echo " (HINT)  with unit testing      ==> 'make tested'"
	@echo " (HINT)  perform installation   ==> 'sudo make install\n"
	@echo " (HINT)  generate documentation ==> 'make documentation\n"

# ----- TESTRUN SPECIFIC ------------------------------------------------------
# 
# 	The following part contains as required functionality to use the 
# 	testrun tools via a makefile. This part may be seen as a makefile
# 	integrated testrunner framework. 
# 	
# 	in particular:
# 	
# 		"make clean && make tested" 
# 		
# 	may be used to build sources as well as tests from scratch and perform
# 	an integrated testrun over the sources after compilation.
# 	
# 	THIS PART MAY BE USED TO INTEGRATE THE TESTRUN TOOLS IN OTHER PROJECTS. 
# 	
# 	All scripts as well as testrun.h may be used in parts or as whole in 
# 	other projects. 
# 	
# 	This kind of makefile based testrun functionality may be used in 
# 	projects, which like the idea of an integrated testrun environment, 
# 	but try to reduce external build, tool, library or whatever 
# 	kind of dependencies. 
# 	
# 	(NOTE: ALL TEST SCRIPTS MAY BE EXCHANGED WITH CUSTOM TEST RUNNERS)
# 	
# -----------------------------------------------------------------------------

# (1) TESTRUN SOURCE DEFINITION --------------------------------------------

TESTS_SOURCES	= $(wildcard tests/**/*_tests.c tests/*_tests.c)
TESTS_TARGET	= $(patsubst %.c,%.test,$(TESTS_SOURCES))
TESTS_TMP_FILES	= $(wildcard /tmp/test_*)

# (2) TESTRUN MAKE RULES ---------------------------------------------------

# ALL IN ONE CALL (compile source, test and run test)
tested:	all testrun done

# compile tests only
tests:	tests-resources $(TESTS_TARGET)

# copy test resources to build
tests-resources:
	@echo " (CP)    tests/resources"
	@cp -r tests/resources build/test

# build all executable tests under build/tests
$(TESTS_TARGET): $(TESTS_SOURCES)
	@echo " (CC)    $(@)"
	@$(CC) $(MODCFLAGS) $(patsubst %.test,%.c,$(@)) \
		-ldl $(STATIC) -Wl,-rpath=$(RPATH) \
		-g -o $(subst tests/,build/test/,$(@)) $(LIBS)

# build a specific executable test (testname) under build/tests
# NOTE: May be usefull for module development in large projects
test:
	@echo " (CC)    $(testname)"
	@$(CC) $(MODCFLAGS) $(patsubst build/test/%.test, \
		tests/%.c,$(testname)) -ldl $(STATIC) -Wl,-rpath=$(RPATH) -g -o\
		$(patsubst tests/%.c,build/test/%.test,$(testname)) $(LIBS)

# (3) TESTRUN runners ------------------------------------------------------

# ACCEPTANCE TEST script invocation
.PHONY: testrun-acceptance
testrun-acceptance:
	sh tests/tools/testrun_simple_acceptance_tests.sh

# UNIT TEST script invocation
.PHONY: testrun-unit
testrun-unit:
	sh tests/tools/testrun_simple_unit_tests.sh

# COVERAGE TEST script invocation
.PHONY: testrun-coverage
testrun-coverage:
	sh tests/tools/testrun_simple_coverage_tests.sh $(PROJECTMK)

# LOC TEST script invocation
.PHONY: testrun-loc
testrun-loc:
	sh tests/tools/testrun_simple_loc.sh $(PROJECTPATH)

# TESTRUN all scripts
.PHONY: testrun
testrun: $(TESTS_EXEC)
	@echo " (HINT)  $(PROJECT) \t\t\t==> running tests\n"
	sh tests/tools/testrun_simple_unit_tests.sh
	sh tests/tools/testrun_simple_acceptance_tests.sh
	sh tests/tools/testrun_simple_coverage_tests.sh $(PROJECTPATH)
	sh tests/tools/testrun_simple_loc.sh $(PROJECTPATH)