unset multiplot; reset



#set ter tikz standalone
#set output ""

set multiplot layout 2,1


set grid lw 2
set yran [0:1]

set ls 1 lc rgb "red" lw 2


pl "./output/decay.txt" every :::0::0 ti "" w l ls 1

pl "" every :::1::1 ti "" w l ls 1


unset multiplot

set ter pop

reset
