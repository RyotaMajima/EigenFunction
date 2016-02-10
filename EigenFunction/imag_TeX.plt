unset multiplot; reset

set ter tikz standalone size 16cm,8cm
set output 'C:\Users\sigmajima\Dropbox\TeX\thesis\thesis\graph/imag.tex'
#set output "C:/Users/U24E/Dropbox/TeX/thesis/thesis/graph/imag.tex"

load "params.txt"
set multiplot layout 1,2

set xlab "$\\varepsilon^{I}$"
set format x "$10^{%L}$"
set logscale x
unset mxtics
set key spacing 2
set key left

f(x) = a * ((exp((x - b) * T) - 1)/((x - b) * T))**2 * exp(-1 * x * T)

set fit quiet

set ls 1 lc rgb "red" lw 2
set ls 2 lc rgb "navy" ps 2

a = 1e-10; b = 1e-10
fit f(x) "./output/energy_imag.txt" us 1:2 via a,b
set title sprintf("(a) $E_{0}^{R} = %.3f$ ($T = %.0f$)", ER0, T)
set label 1 "$E_{0}^{I} = 8.079 \\times 10^{-5}$" left at graph 0.1,0.7
set ylab "$\\| \\Phi_{T}'(E_{0}^{R}, \\varepsilon^{I}) \\|^{2} e^{\\varepsilon^{I} T}$" \
 offset -1,0
set ytics 0.1
set format y "%.1f"
pl f(x) ti "fitting curve" ls 1, \
 "" us 1:2 every 20 ti "data" ls 2

if(peakNum > 1){
	a = 1e-10; b = 1e-10
	fit f(x) "./output/energy_imag.txt" us 1:3 via a,b
	set title sprintf("(b) $E_{1}^{R} = %.3f$ ($T = %.0f$)", ER1, T)
	set label 1 "$E_{1}^{I} = 1.661 \\times 10^{-2}$" left at graph 0.1,0.7
	set ylab "$\\| \\Phi_{T}'(E_{1}^{R}, \\varepsilon^{I}) \\|^{2} e^{\\varepsilon^{I} T}$"
	set ytics 0.01
	set format y "%.2f"
	pl f(x) ti "fitting curve" ls 1, \
	 "" us 1:3 every 20 ti "data" ls 2
}

unset multiplot
set output
set ter pop
reset
