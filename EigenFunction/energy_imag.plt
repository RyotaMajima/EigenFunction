unset multiplot; reset

#set ter tikz standalone size 16cm,8cm
#set output 'C:\Users\sigmajima\Dropbox\TeX\thesis\thesis\graph/imag.tex'
#set output "C:/Users/U24E/Dropbox/TeX/thesis/thesis/graph/imag.tex"

load "params.txt"
set multiplot layout 1,2

set xlab "$\\varepsilon^{I} / \\hbar \\omega$"
set ylab "$|| \\Phi_{T}(\\varepsilon^{I}) ||^{2}$"# offset -2,0
set format x "$10^{%L}$"
set format y "%.2f"
set logscale x
set key spacing 2
set key left

f(x) = a * ((exp((x - b) * T) - 1)/((x - b) * T))**2 * exp(-1 * x * T)

set fit results

a = 1e-10; b = 1e-10
fit f(x) "./output/energy_imag.txt" us 1:2 via a,b
set title sprintf("$E_{0}^{R} = %.3f$ ($T = %.0f$)", ER0, T)
set label 1 sprintf("$E_{0}^{I}$ = %.3e", b) left at graph 0.1,0.8
pl f(x) ti "fitting curve" lc rgb "red", "" us 1:2 every 10 ti "data" lc rgb "navy"

if(peakNum > 1){
	a = 1e-10; b = 1e-10
	fit f(x) "./output/energy_imag.txt" us 1:3 via a,b
	set title sprintf("$E_{1}^{R} = %.3f$ ($T = %.0f$)", ER1, T)
	set label 1 sprintf("$E_{1}^{I}$ = %.3e", b) left at graph 0.1,0.8
	pl f(x) ti "fitting curve" lc rgb "red", "" us 1:3 every 10 ti "data" lc rgb "navy"
}

unset multiplot
set output
set ter pop
reset
