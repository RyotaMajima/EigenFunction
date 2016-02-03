unset multiplot; reset

set ter tikz standalone size 16cm,8cm
set output 'C:\Users\sigmajima\Dropbox\TeX\thesis\thesis\graph/WKB3.tex'
set multiplot layout 1,2

load "params.txt"

set ls 1 lc rgb "black" lw 2
set ls 2 lc rgb "black" dt (10,20)

set xran [-3:1/b + 1]; set yran [-1/(6 * b*b)-1:2]
set xlab "$x$"; set ylab "$E$"
#set ytics 1.0
set zeroaxis
set label 1 "(a)" center at graph 0.9,0.9

set arrow 1 from first -0.9,ER0 to 1.12,ER0 nohead lc rgb "red" lw 2

if(peakNum > 1){
	set arrow 2 from first -1.4,ER1 to 2.35,ER1 nohead lc rgb "blue" lw 2
}

if(peakNum > 2){
	set arrow 3 from first -1.85,ER2 to 3,ER2 nohead lc rgb "green" lw 2
}

V(x) = (1.0 / 2) * x**2 - (b / 3) * x**3 - 1 / (6*b**2)

pl V(x) ti "" ls 1,\
 (1.0 / 2.0)*x**2 - 1.0 / (6*b**2) ti "" ls 2

unset arrow; unset label

set xran [*:*]; set yran [*:*]
set xlab "$E^{R}$"; set ylab "$E^{I}$"
set xtics 0.4
set ytics
set logscale y; set format y "$10^{%L}$"
set grid lw 2
#set tics font ",9"

unset mytics; set ytics nomirror

set ls 1 pt 1 ps 3

FIT = "./output/eigenValueFit3.txt"

set label 1 "from fitting" right at graph 0.8,0.15
set label 2 point ls 1 center at graph 0.9,0.15
set label 3 "(b)" center at graph 0.1,0.9

if(peakNum == 1){
	pl FIT us 1:(abs($2)) every ::0::0 ti ""  w p ls 1 lc rgb "red", \
 	 "./output/WKB3.txt" ti "" w l lc rgb "black"
}
if(peakNum == 2){
	pl FIT us 1:(abs($2)) every ::0::0 ti ""  w p ls 1 lc rgb "red", \
	 FIT every ::1::1 ti ""  w p ls 1 lc rgb "blue", \
	 "./output/WKB3.txt" ti "" w l lc rgb "black"
}
if(peakNum == 3){
	pl FIT us 1:(abs($2)) every ::0::0 ti ""  w p ls 1 lc rgb "red", \
	 FIT every ::1::1 ti ""  w p ls 1 lc rgb "blue", \
	 FIT every ::2::2 ti ""  w p ls 1 lc rgb "green", \
	 "./output/WKB3.txt" ti "" w l lc rgb "black"
}

unset multiplot
set output
set ter pop
reset
