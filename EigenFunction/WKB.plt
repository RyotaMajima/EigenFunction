unset multiplot; reset

set ter tikz stand size 15cm,10cm font ",8"
#set output 'C:\Users\sigmajima\Dropbox\TeX\thesis\slide\graph_slide/WKB.tex'
set output 'C:\Users\U24E\Dropbox\TeX\thesis\slide\graph_slide/WKB.tex'
set multiplot

set xran [-4:5]; set yran [-3.0:0.5]
set xlab "$x$"; set ylab "$E^{R}$" offset -1,0
set xtics 2; set ytics 1
set zeroaxis

set ls 1 lc rgb "black" lw 2
set ls 2 lc rgb "black" dt (10,20)
V(x, b) = (1.0 / 2) * x**2 - (b / 3) * x**3 - 1 / (6*b**2)
f(x, b) = (1.0 / 2.0)*x**2 - 1.0 / (6*b**2)

set origin 0.0,0.6; set size 0.33,0.4
set arrow 1 from first -0.8,-0.22 to 1.2,-0.22 nohead lc rgb "red" lw 2
pl V(x, 1.0 / 2.0) ti "" ls 1, f(x, 1.0 / 2.0) ti "" ls 2

set origin 0.33,0.6; set size 0.33,0.4
set arrow 1 from first -0.87,-1.02 to 1.1,-1.02 nohead lc rgb "red" lw 2
set arrow 2 from first -1.4,-0.165 to 2.35,-0.165 nohead lc rgb "blue" lw 2
pl V(x, 1.0 / 3.0) ti "" ls 1, f(x, 1.0 / 3.0) ti "" ls 2

set origin 0.66,0.6; set size 0.33,0.4
set arrow 1 from first -0.9,-2.17 to 1.1,-2.17 nohead lc rgb "red" lw 2
set arrow 2 from first -1.5,-1.24 to 2.08,-1.24 nohead lc rgb "blue" lw 2
set arrow 3 from first -1.85,-0.4 to 3.0,-0.4 nohead lc rgb "green" lw 2
pl V(x, 1.0 / 4.0) ti "" ls 1, f(x, 1.0 / 4.0) ti "" ls 2

unset arrow; unset title
set ytics auto
set grid lw 2
set xran [*:*]; set yran [*:*]
set logscale y; unset mytics
set ytics nomirror
set format y "$10^{%L}$"
set xlab "$E^{R}$"; set ylab "$E^{I}$"
set tmar 2

set ls 1 lc rgb "black"
set ls 2 pt 1 ps 3

set label 1 "from fitting" left at graph 0.3,0.1
set label 2 point ls 2 ps 2 lc rgb "black" at graph 0.92,0.1

FIT2 = "./output/eigenValueFit2.txt"
FIT3 = "./output/eigenValueFit3.txt"
FIT4 = "./output/eigenValueFit4.txt"
WKB2 = "./output/WKB2.txt"
WKB3 = "./output/WKB3.txt"
WKB4 = "./output/WKB4.txt"

set origin 0.0,0.0; set size 0.33,0.6
set xtics 0.2
pl WKB2 ti "" w l ls 1, \
 FIT2 every ::0::0 ti "" w p ls 2 lc rgb "red"

set origin 0.33,0.0; set size 0.33,0.6
set xtics 0.4
pl WKB3 ti "" w l ls 1, \
 FIT3 every ::0::0 ti "" w p ls 2 lc rgb "red", \
 FIT3 every ::1::1 ti "" w p ls 2 lc rgb "blue"

set origin 0.66,0.0; set size 0.33,0.6
set xtics 1.0
pl WKB4 ti "" w l ls 1, \
 FIT4 every ::0::0 ti "" w p ls 2 lc rgb "red", \
 FIT4 every ::1::1 ti "" w p ls 2 lc rgb "blue", \
 FIT4 every ::2::2 ti "" w p ls 2 lc rgb "green"

unset multiplot
set output
set ter pop
reset
