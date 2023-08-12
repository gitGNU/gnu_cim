PROG    = hello \
          right-text \
          gcd \
          for-loop \
          geometry.o \
          test-geom \
          stacks.o \
          test-stacks-char \
          test-stacks-var \
          sim-nested \
          sim-office \
          sim-man-machine \
          sim-interview \
          demo-simset \
          sim-active-server \
          sim-active-customer \
          chess-two-masters \
          chess-master-two-slaves \
          chess.o \
          chess-with-classes \
          noughts-and-crosses

all:    $(PROG)

clean:
	rm -f $(PROG) *.o *.atr

%:      %.sim
	gnucim $<

%.o:    %.sim
	gnucim -c $<

chess-with-classes:: chess.o
test-stacks-char:: stacks.o
test-stacks-var:: stacks.o
