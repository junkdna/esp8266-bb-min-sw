# SPDX-License-Identifier: GPL-3.0-only
# Copyright 2020 Alexander Willner
# Copyright 2019-2020 Tillmann Heidsieck <theidsieck@leenox.de>
# Copyright 2020 Dominik Laton <dominik.laton@web.de>
#
# ambient-condition-sensor Makefile

## Main
SKETCH = .
## Helper
OS = $(shell uname)
.PHONY: help install compile upload uploadfs tags check clean

PIO = $(shell which platformio)
ifeq (, $(PIO))
	PIO = $(HOME)/.platformio/penv/bin/platformio
endif

help:
	$(info ambient-condition-sensor Makefile)
	$(info ================)
	$(info )
	$(info )
	$(info Available commands:)
	$(info - install : installation of required binaries (arduino-cli/platformio))
	$(info - compile : create binary)
	$(info - upload  : store binary on board flash)
	$(info - uploadfs: store binary file on board flash)
	$(info - tags    : generate ctags and cscope files)
	$(info - check   : run some static checks on the code)
	$(info - clean   : delete temporary files)
	@true

install:
ifeq (,$(shell find $(PIO)))
	@pip3 install -U --user setuptools wheel
	@pip3 install -U --user platformio
endif

compile:
	@$(PIO) run -j 16

upload: compile
	@$(PIO) run -t upload

uploadfs:
	@$(PIO) run -t uploadfs

monitor:
	@$(PIO) device monitor

tags: *.ino *.h
	@../tools/gen_tags.sh

check: *.ino *.h
	@cppcheck --enable=all --std=c++20 --language=c++ *.ino *.h

clean:
	@$(PIO) run -t clean
	@rm -rf ..arduino*
	@rm -rf ".pio/build/"
	@rm -rf tags cscope*
