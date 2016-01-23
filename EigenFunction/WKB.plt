unset multiplot; reset

#set ter tikz stand size 297mm,210mm font ",12"
#set output "./graph/WKB.tex"
set multiplot layout 1,2

load "params.txt"

set ls 1 lc rgb "black" lw 2
set ls 2 lc rgb "black" dt (10,20)

set xran [-4:1/b + 1]; set yran [-1/(6 * b*b)-1:2]
set xlab "$x$"; set ylab "$E$"
set zeroaxis
set key bottom

set arrow 1 from first -3,ER0 to 3,ER0 nohead lc rgb "red"
if(peakNum > 1){
	set arrow 2 from first -3,ER1 to 3,ER1 nohead lc rgb "blue"
}

if(peakNum > 2){
	set arrow 3 from first -3,ER2 to 3,ER2 nohead lc rgb "green"
}

V(x) = (1.0 / 2) * x**2 - (b / 3) * x**3 - 1 / (6*b**2)

w = 1.0
pl V(x) ti "$V(x)$" ls 1,\
 (1.0 / 2.0)* w**2 * x**2 - 1.0 / (6*b**2) ti "" ls 2

unset arrow
set xran [*:*]; set yran [*:*]
set logscale y
set format y "%.1e"
set grid lw 2
set xlab "$E$"; set ylab "$\\tau^{-1}$"

set ls 1 pt 1 ps 3
set ls 2 pt 6 ps 3

FIT = "./output/eigenValueFit.txt"
DECAY = "./output/eigenValueDecay.txt"

if(peakNum == 1){
	pl FIT us 1:(abs($2)) every ::0::0 ti ""  w p ls 1 lc rgb "red", \
 	 DECAY every ::0::0 ti ""  w p ls 2 lc rgb "red", \
 	 "./output/WKB.txt" ti "" w l lc rgb "black"
}
if(peakNum == 2){
	pl FIT us 1:(abs($2)) every ::0::0 ti ""  w p ls 1 lc rgb "red", \
	 DECAY every ::0::0 ti ""  w p ls 2 lc rgb "red", \
	 FIT every ::1::1 ti ""  w p ls 1 lc rgb "blue", \
	 DECAY every ::1::1 ti ""  w p ls 2 lc rgb "blue", \
	 "./output/WKB.txt" ti "" w l lc rgb "black"
}
if(peakNum == 3){
	pl FIT us 1:(abs($2)) every ::0::0 ti ""  w p ls 1 lc rgb "red", \
	 DECAY every ::0::0 ti "" w p ls 2 lc rgb "red", \
	 FIT every ::1::1 ti ""  w p ls 1 lc rgb "blue", \
	 DECAY every ::1::1 ti ""  w p ls 2 lc rgb "blue", \
	 FIT every ::2::2 ti ""  w p ls 1 lc rgb "green", \
	 DECAY every ::2::2 ti ""  w p ls 2 lc rgb "green", \
	 "./output/WKB.txt" ti "" w l lc rgb "black"
}

unset multiplot
set output
set ter pop
reset
