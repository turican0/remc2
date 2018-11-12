#   $Id: Makefile,v 1.5 2008/03/23 17:21:34 karl Exp $
#
#   Copyright (C) 2008 Rocky Bernstein <rocky@gnu.org>
#
#   This program is free software: you can redistribute it and/or modify
#   it under the terms of the GNU General Public License as published by
#   the Free Software Foundation, either version 3 of the License, or
#   (at your option) any later version.
#
#   This program is distributed in the hope that it will be useful,
#   but WITHOUT ANY WARRANTY; without even the implied warranty of
#   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
#   GNU General Public License for more details.
#
#   You should have received a copy of the GNU General Public License
#   along with this program.  If not, see <http://www.gnu.org/licenses/>.

INCLUDES = -I .
all: cueparser cuelexer tocparser

lex.cue.c: cue.L cue.tab.h
	flex -Pcue cue.L

lex.cuelex.c: cue.L cue.tab.h
	flex -Pcuelex cue.L

lex.cue.o: lex.cue.c
	gcc -g -Wall -c lex.cue.c $(INCLUDES)

lex.cuelex.o: lex.cuelex.c
	gcc -g -DSTANDALONE -Wall -c lex.cuelex.c

cue.tab.c, cue.tab.h: cue.y
	bison -p cue -d cue.y

cue.tab.o: cue.tab.c cue.tab.h
	gcc -g -Wall -DSTANDALONE -c cue.tab.c $(INCLUDES)

cueparser: lex.cue.o cue.tab.o
	gcc -g lex.cue.o cue.tab.o -lfl -o cueparser

cuelexer: lex.cuelex.o 
	gcc -g lex.cuelex.o -lfl -o cuelexer

toc.tab.h: toc.tab.c

toc.tab.c: toc.y
	bison -p toc -d toc.y

toclexer.o: toclexer.c
	gcc -g -Wall -c toclexer.c $(INCLUDES)

toc.tab.o: toc.tab.c toc.tab.h
	gcc -g -Wall -DSTANDALONE -c toc.tab.c $(INCLUDES)

tocparser: toc.tab.o toclexer.o
	gcc -g toclexer.o toc.tab.o -o tocparser

clean: 
	rm -f lex.cue.c lex.cuelex.c lex.cue.o lex.cuelex.o cue.tab.c \
	    cue.tab.o cueparser cuelexer

check:
	(cd test && ./runall)
