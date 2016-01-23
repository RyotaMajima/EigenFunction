unset multiplot; reset

#set ter tikz standalone
#set output ""

load "params.txt"
set multiplot layout 2,1

set grid lw 2
set xran [0:T]; set yran [0:1]
set key spacing 1.5

set ls 1 lc rgb "red" lw 2
set ls 2 lc rgb "navy" ps 3

f(x) = exp(-2*b*x); b0 = 1e-5
b = b0
set fit results

fit f(x) "./output/decay.txt" index 0 via b
set title sprintf("ground state $\\lambda = %.3e$", lambda0)
pl f(x) ti "fitting curve" ls 1, "" index 0 every 5 ti "data" ls 2

if(peakNum > 1){
	b = b0
	fit f(x) "./output/decay.txt" index 1 via b
	set title sprintf("first excited state $\\lambda = %.3e$", lambda1)
	pl f(x) ti "fitting curve" ls 1, "" index 1 every 5 ti "data" ls 2
}

unset multiplot
set ter pop; reset
