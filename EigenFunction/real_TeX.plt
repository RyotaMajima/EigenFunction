unset multiplot; reset

set ter tikz standalone size 16cm,8cm
set output 'C:\Users\sigmajima\Dropbox\TeX\thesis\thesis\graph/real.tex'

set multiplot

load "./params.txt"

set origin 0,0; set size 0.4,1.0

set xran [-3:1/b + 1]; set yran [-1/(6 * b*b)-1:2]
set xlab "$x$"; set ylab "$E$"
set zeroaxis
set ytics 1
set key right at graph 1.0,0.2

set label 1 "(a)" center at graph 0.9,0.9
set arrow 1 from first -0.9,ER0 to 1.1,ER0 nohead lc rgb "red" lw 2

if(peakNum > 1){
	set arrow 2 from first -1.4,ER1 to 2.35,ER1 nohead lc rgb "blue" lw 2
}

V(x) = (1.0 / 2) * x**2 - (b / 3) * x**3 - 1 / (6*b**2)
pl V(x) ti "$V(x)$" lc rgb "black" lw 2

unset arrow; unset label
unset zeroaxis

set origin 0.4,0; set size 0.6,1.0

set xran [*:*]; set yran [0:0.4]
set xlab "$\\varepsilon^{R}$"
set ylab "$|| \\Phi_{T}(\\varepsilon^{R}) ||^{2}$"
set format y "%.1f"
set ytics 0.1

set label 1 sprintf("$E_{0}^{R} = %.3f$", ER0) center at first ER0,ER0_val+0.1
set arrow 1 from first ER0,ER0_val+0.08 to ER0,ER0_val+0.01 \
 lc rgb "red" lw 2

if(peakNum > 1){
 set label 2 sprintf("$E_{1}^{R} = %.3f$", ER1) \
  center at first ER1-0.08,ER1_val+0.1
 set arrow 2 from first ER1,ER1_val+0.08 to ER1,ER1_val+0.01 \
  lc rgb "blue" lw 2
}

set label 3 sprintf("$T = %.0f$", T) right at graph 0.9,0.9
set label 5 "(b)" center at graph 0.1,0.9

pl "./output/energy_peak_real.txt" ti "" w l lc rgb "black" lw 2

unset multiplot
set output
set term pop
