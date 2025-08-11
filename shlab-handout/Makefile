# Makefile for the CS:APP Shell Lab

TEAM = NOBODY
VERSION = 1
HANDINDIR = /afs/cs/academic/class/15213-f02/L5/handin
DRIVER = ./test/sdriver.pl
BUILDDIR = build
TSH = $(BUILDDIR)/tsh
TSHREF = ./tshref
TSHARGS = "-p"
CC = gcc
CFLAGS = -Wall -O2 -g
FILES = $(TSH) $(BUILDDIR)/myspin $(BUILDDIR)/mysplit $(BUILDDIR)/mystop $(BUILDDIR)/myint

all: $(FILES)

# Create build directory
$(BUILDDIR):
	mkdir -p $(BUILDDIR)

# Build rules
$(BUILDDIR)/tsh: tsh.c signal_handlers.c builtin.c jobs.c csapp.c | $(BUILDDIR)
	$(CC) $(CFLAGS) -o $@ $^

$(BUILDDIR)/myspin: myspin.c | $(BUILDDIR)
	$(CC) $(CFLAGS) -o $@ $<

$(BUILDDIR)/mysplit: mysplit.c | $(BUILDDIR)
	$(CC) $(CFLAGS) -o $@ $<

$(BUILDDIR)/mystop: mystop.c | $(BUILDDIR)
	$(CC) $(CFLAGS) -o $@ $<

$(BUILDDIR)/myint: myint.c | $(BUILDDIR)
	$(CC) $(CFLAGS) -o $@ $<

.PHONY: all clean test rtest

##################
# Handin your work
##################
handin:
	cp tsh.c $(HANDINDIR)/$(TEAM)-$(VERSION)-tsh.c


##################
# Test convenience targets
##################

# Run all tests with student shell
test: tests
tests: test01 test02 test03 test04 test05 test06 test07 test08 test09 test10 test11 test12 test13 test14 test15 test16

# Run all tests with reference shell
rtest: rtests
rtests: rtest01 rtest02 rtest03 rtest04 rtest05 rtest06 rtest07 rtest08 rtest09 rtest10 rtest11 rtest12 rtest13 rtest14 rtest15 rtest16


##################
# Regression tests
##################

# Run tests using the student's shell program
test01:
	$(DRIVER) -t test/trace01.txt -s $(TSH) -a $(TSHARGS)
test02:
	$(DRIVER) -t test/trace02.txt -s $(TSH) -a $(TSHARGS)
test03:
	$(DRIVER) -t test/trace03.txt -s $(TSH) -a $(TSHARGS)
test04:
	$(DRIVER) -t test/trace04.txt -s $(TSH) -a $(TSHARGS)
test05:
	$(DRIVER) -t test/trace05.txt -s $(TSH) -a $(TSHARGS)
test06:
	$(DRIVER) -t test/trace06.txt -s $(TSH) -a $(TSHARGS)
test07:
	$(DRIVER) -t test/trace07.txt -s $(TSH) -a $(TSHARGS)
test08:
	$(DRIVER) -t test/trace08.txt -s $(TSH) -a $(TSHARGS)
test09:
	$(DRIVER) -t test/trace09.txt -s $(TSH) -a $(TSHARGS)
test10:
	$(DRIVER) -t test/trace10.txt -s $(TSH) -a $(TSHARGS)
test11:
	$(DRIVER) -t test/trace11.txt -s $(TSH) -a $(TSHARGS)
test12:
	$(DRIVER) -t test/trace12.txt -s $(TSH) -a $(TSHARGS)
test13:
	$(DRIVER) -t test/trace13.txt -s $(TSH) -a $(TSHARGS)
test14:
	$(DRIVER) -t test/trace14.txt -s $(TSH) -a $(TSHARGS)
test15:
	$(DRIVER) -t test/trace15.txt -s $(TSH) -a $(TSHARGS)
test16:
	$(DRIVER) -t test/trace16.txt -s $(TSH) -a $(TSHARGS)

# Run the tests using the reference shell program
rtest01:
	$(DRIVER) -t test/trace01.txt -s $(TSHREF) -a $(TSHARGS)
rtest02:
	$(DRIVER) -t test/trace02.txt -s $(TSHREF) -a $(TSHARGS)
rtest03:
	$(DRIVER) -t test/trace03.txt -s $(TSHREF) -a $(TSHARGS)
rtest04:
	$(DRIVER) -t test/trace04.txt -s $(TSHREF) -a $(TSHARGS)
rtest05:
	$(DRIVER) -t test/trace05.txt -s $(TSHREF) -a $(TSHARGS)
rtest06:
	$(DRIVER) -t test/trace06.txt -s $(TSHREF) -a $(TSHARGS)
rtest07:
	$(DRIVER) -t test/trace07.txt -s $(TSHREF) -a $(TSHARGS)
rtest08:
	$(DRIVER) -t test/trace08.txt -s $(TSHREF) -a $(TSHARGS)
rtest09:
	$(DRIVER) -t test/trace09.txt -s $(TSHREF) -a $(TSHARGS)
rtest10:
	$(DRIVER) -t test/trace10.txt -s $(TSHREF) -a $(TSHARGS)
rtest11:
	$(DRIVER) -t test/trace11.txt -s $(TSHREF) -a $(TSHARGS)
rtest12:
	$(DRIVER) -t test/trace12.txt -s $(TSHREF) -a $(TSHARGS)
rtest13:
	$(DRIVER) -t test/trace13.txt -s $(TSHREF) -a $(TSHARGS)
rtest14:
	$(DRIVER) -t test/trace14.txt -s $(TSHREF) -a $(TSHARGS)
rtest15:
	$(DRIVER) -t test/trace15.txt -s $(TSHREF) -a $(TSHARGS)
rtest16:
	$(DRIVER) -t test/trace16.txt -s $(TSHREF) -a $(TSHARGS)


# clean up
clean:
	rm -rf $(BUILDDIR) *.o *~


