unset multiplot; reset

set ter tikz stand size 15cm,10cm
set output 'C:\Users\sigmajima\Dropbox\TeX\thesis\slide\graph_slide/WKB_norm.tex'
#set output 'C:\Users\U24E\Dropbox\TeX\thesis\slide\graph_slide/WKB.tex'

set multiplot layout 1,3

set logscale y
set format y "$10^{%L}$"
set grid lw 2
set ytics nomirror
set xlab "$E / \\hbar \\omega$"; set ylab "$\\tau^{-1}$"

set ls 1 lc rgb "black"
set ls 2 pt 1 ps 3
set ls 3 pt 6 ps 3

set label 1 "from fitting" left at graph 0.2,0.15
set label 2 "from norm" left at graph 0.2,0.1
set label 3 point ls 2 ps 2 lc rgb "black" at graph 0.92,0.15
set label 4 point ls 3 ps 2 lc rgb "black" at graph 0.92,0.1

FIT2 = "./output/eigenValueFit2.txt"
FIT3 = "./output/eigenValueFit3.txt"
FIT4 = "./output/eigenValueFit4.txt"
DECAY2 = "./output/eigenValueDecay2.txt"
DECAY3 = "./output/eigenValueDecay3.txt"
DECAY4 = "./output/eigenValueDecay4.txt"
WKB2 = "./output/WKB2.txt"
WKB3 = "./output/WKB3.txt"
WKB4 = "./output/WKB4.txt"

set xtics 0.2
pl WKB2 ti "" w l ls 1, \
 FIT2 every ::0::0 ti "" w p ls 2 lc rgb "red", \
 DECAY2 every ::0::0 ti "" w p ls 3 lc rgb "red"

set xtics 0.4
pl WKB3 ti "" w l ls 1, \
 FIT3 every ::0::0 ti "" w p ls 2 lc rgb "red", \
 FIT3 every ::1::1 ti "" w p ls 2 lc rgb "blue", \
 DECAY3 every ::0::0 ti "" w p ls 3 lc rgb "red", \
 DECAY3 every ::1::1 ti "" w p ls 3 lc rgb "blue"

set xtics 1.0
pl WKB4 ti "" w l ls 1, \
 FIT4 every ::0::0 ti "" w p ls 2 lc rgb "red", \
 FIT4 every ::1::1 ti "" w p ls 2 lc rgb "blue", \
 FIT4 every ::2::2 ti "" w p ls 2 lc rgb "green", \
 DECAY4 every ::0::0 ti "" w p ls 3 lc rgb "red", \
 DECAY4 every ::1::1 ti "" w p ls 3 lc rgb "blue", \
 DECAY4 every ::2::2 ti "" w p ls 3 lc rgb "green"

unset multiplot
set output
set ter pop
reset
