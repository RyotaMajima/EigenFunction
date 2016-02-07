unset multiplot; reset

set ter tikz standalone size 15cm,10cm font ",8"
#set output 'C:\Users\sigmajima\Dropbox\TeX\thesis\slide\graph_slide/eigenvalue.tex'
set output 'C:\Users\U24E\Dropbox\TeX\thesis\slide\graph_slide/eigenvalue.tex'

load "params.txt"

set multiplot
set size 1.0,0.4
set origin 0.0,0.6

set yran [0:0.3]
set xlab "$\\varepsilon^{R}$"
set ylab "$|| \\Phi_{T}(\\varepsilon^{R}) ||^{2}$" offset -1,0
set ytics 0.1
set tics nomirror

set label 1 sprintf("$E_{0}^{R} = %.3f$", ER0) center at first ER0,ER0_val+0.1
set arrow 1 from first ER0,ER0_val+0.08 to ER0,ER0_val+0.01

if(peakNum > 1){
 set label 2 sprintf("$E_{1}^{R} = %.3f$", ER1) center at first ER1,ER1_val+0.1
 set arrow 2 from first ER1,ER1_val+0.08 to ER1,ER1_val+0.01
}

pl "./output/energy_peak_real.txt" ti "" w l lc rgb "red" lw 3

reset

set size 0.5,0.6
set origin 0.0,0.0

set xlab "$\\varepsilon^{I}$"
set format x "$10^{%L}$"
set ytics 0.1; unset mxtics
set tics nomirror
set logscale x
set key spacing 2
set key left

f(x) = a * ((exp((x - b) * T) - 1)/((x - b) * T))**2 * exp(-1 * x * T)

set fit quiet

a = 1e-10; b = 1e-10
fit f(x) "./output/energy_imag.txt" us 1:2 via a,b
set title sprintf("$E_{0}^{R} = %.3f$ \\quad ($T = %.0f$)", ER0, T)
set format y "%.1f"
set label 1 "$E_{0}^{I} = 8.079 \\times 10^{-5}$" left at graph 0.05,0.7
set ylab "$|| \\Phi_{T}(E_{0}^{R}, \\varepsilon^{I}) ||^{2}$" offset -1,0
pl f(x) ti "fitting curve" lc rgb "red" lw 2, \
 "" us 1:2 every 20 ti "data" lc rgb "navy" ps 2

set origin 0.5,0.0
set ytics 0.01

if(peakNum > 1){
	a = 1e-10; b = 1e-10
	fit f(x) "./output/energy_imag.txt" us 1:3 via a,b
	set title sprintf("$E_{1}^{R} = %.3f$ \\quad ($T = %.0f$)", ER1, T)
	set format y "%.2f"
	set label 1 "$E_{1}^{I} = 1.661 \\times 10^{-2}$" left at graph 0.05,0.7
	set ylab "$|| \\Phi_{T}(E_{1}^{R}, \\varepsilon^{I}) ||^{2}$" offset -1,0
	pl f(x) ti "fitting curve" lc rgb "red" lw 2, \
	 "" us 1:3 every 20 ti "data" lc rgb "navy" ps 2
}

unset multiplot
set output
set ter pop
reset
