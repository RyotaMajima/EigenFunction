unset multiplot; reset

set grid lw 2
set yran [0:1]

set ls 1 lc rgb "red" lw 2


pl "./output/decay.txt" ti "" w l ls 1





set ter pop
reset
