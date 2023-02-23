# Make x86-64 random byte generators.

# Copyright 2015, 2020, 2021 Paul Eggert

# This program is free software: you can redistribute it and/or
# modify it under the terms of the GNU General Public License as
# published by the Free Software Foundation, either version 3 of the
# License, or (at your option) any later version.

# This program is distributed in the hope that it will be useful, but
# WITHOUT ANY WARRANTY; without even the implied warranty of
# MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
# General Public License for more details.

# You should have received a copy of the GNU General Public License
# along with this program.  If not, see <http://www.gnu.org/licenses/>.  */

# Optimization level.  Change this -O2 to -Og or -O0 or whatever.
OPTIMIZE =

# The C compiler and its options.
CC = gcc
CFLAGS = $(OPTIMIZE) -g3 -Wall -Wextra -fanalyzer \
  -march=native -mtune=native -mrdrnd

# The archiver command, its options and filename extension.
TAR = tar
TARFLAGS = --gzip --transform 's,^,randall/,'
TAREXT = tgz

object-files = options.o output.o rand64-hw.o rand64-sw.o randall.o

default: randall

randall: *.c
	$(CC) $(CFLAGS) *.c -o $@

assignment: randall-assignment.$(TAREXT)
assignment-files = COPYING Makefile randall.c
randall-assignment.$(TAREXT): $(assignment-files)
	$(TAR) $(TARFLAGS) -cf $@ $(assignment-files)

submission-tarball: randall-submission.$(TAREXT)
submission-files = $(assignment-files) \
  notes.txt # More files should be listed here, as needed.
randall-submission.$(TAREXT): $(submission-files)
	$(TAR) $(TARFLAGS) -cf $@ $(submission-files)

repository-tarball:
	$(TAR) -czf randall-git.tgz .git

.PHONY: default clean assignment submission-tarball repository-tarball

check:
	[ 5 = "$$(./randall 5 | wc -c)" ]
	[ 10 = "$$(./randall 10 | wc -c)" ]
	[ 0 = "$$(./randall 0 | wc -c)" ]
clean:
	rm -f *.o *.$(TAREXT) randall



# randall: $(object-files)
# 	$(CC) $(CFLAGS) $(object-files) -o $@

# options.o: options.c options.h
# 	$(cc) -c options.c

# output.o: output.c output.h
# 	$(cc) -c output.c

# rand64-hw.o: rand64-hw.c rand64-hw.h
# 	$(cc) -c rand64-hw.c

# rand64-sw.o: rand64-sw.c rand64-sw.h
# 	$(cc) -c rand64-sw.o

# randall.o: randall.c # and others
