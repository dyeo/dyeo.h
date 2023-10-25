jmp @start
counter: .raw
    1 2 3 4 5 6 7 8 9 10

start:
    set r0 0
    set r1 1
    set i0 4
    lod i1 @counter
    sys r0
    add i1 r1
    sys r0
    add i1 r1
    sys r0
    add i1 r1
    sys r0
    add i1 r1
    sys r0
    add i1 r1
    sys r0
    add i1 r1
    sys r0
    add i1 r1
    sys r0
    add i1 r1
    sys r0
    add i1 r1
    sys r0
    add i1 r1
    sys r0
    add i1 r1
    sys r0