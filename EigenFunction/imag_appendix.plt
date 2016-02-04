unset multiplot; reset

#set ter tikz standalone size 16cm,8cm
#set output 'C:\Users\sigmajima\Dropbox\TeX\thesis\thesis\graph/imag.tex'
#set output "C:/Users/U24E/Dropbox/TeX/thesis/thesis/graph/imag.tex"

load "params.txt"
set multiplot layout 3,2

set xlab "$\\varepsilon^{I}$"
set format x "$10^{%L}$"
set logscale x
unset mxtics
set key spacing 2
set key right

f(x) = a * ((exp((x - b) * T) - 1)/((x - b) * T))**2 * exp(-2 * x * T)

set fit quiet

set ls 1 lc rgb "red" lw 2
set ls 2 lc rgb "navy" ps 2

a = 1e-10; b = 1e-10
fit f(x) "./output/energy_imag1.txt" us 1:2 via a,b
set ylab "$|| \\Phi_{T}'(E_{0}^{R}, \\varepsilon^{I}) ||^{2}$" \
 offset -1,0
set ytics 0.1
set format y "%.1f"
pl f(x) ti "fitting curve" ls 1, \
 "" us 1:2 every 20 ti "data" ls 2

if(peakNum > 1){
	a = 1e-10; b = 1e-10
	fit f(x) "./output/energy_imag1.txt" us 1:3 via a,b
	set ylab "$|| \\Phi_{T}'(E_{1}^{R}, \\varepsilon^{I}) ||^{2}$"
	set ytics 0.01
	set format y "%.2f"
	pl f(x) ti "fitting curve" ls 1, \
	 "" us 1:3 every 20 ti "data" ls 2
}

######################################################

set key left
f(x) = a * ((exp((x - b) * T) - 1)/((x - b) * T))**2 * exp(-x * T)

set fit quiet

set ls 1 lc rgb "red" lw 2
set ls 2 lc rgb "navy" ps 2

a = 1e-10; b = 1e-10
fit f(x) "./output/energy_imag.txt" us 1:2 via a,b
set ylab "$|| \\Phi_{T}'(E_{0}^{R}, \\varepsilon^{I}) ||^{2}$" \
 offset -1,0
set ytics 0.1
set format y "%.1f"
pl f(x) ti "fitting curve" ls 1, \
 "" us 1:2 every 20 ti "data" ls 2

if(peakNum > 1){
	a = 1e-10; b = 1e-10
	fit f(x) "./output/energy_imag.txt" us 1:3 via a,b
	set ylab "$|| \\Phi_{T}'(E_{1}^{R}, \\varepsilon^{I}) ||^{2}$"
	set ytics 0.01
	set format y "%.2f"
	pl f(x) ti "fitting curve" ls 1, \
	 "" us 1:3 every 20 ti "data" ls 2
}

######################################################

set key left
f(x) = a * ((exp((x - b) * T) - 1)/((x - b) * T))**2# * exp(-2 * x * T)

set fit quiet

set ls 1 lc rgb "red" lw 2
set ls 2 lc rgb "navy" ps 2

a = 1e-10; b = 1e-10
fit f(x) "./output/energy_imag2.txt" us 1:2 via a,b
set ylab "$|| \\Phi_{T}'(E_{0}^{R}, \\varepsilon^{I}) ||^{2}$" \
 offset -1,0
set ytics 5.0
set format y "%.1f"
pl f(x) ti "fitting curve" ls 1, \
 "" us 1:2 every 20 ti "data" ls 2

if(peakNum > 1){
	a = 1e-10; b = 1e-10
	fit f(x) "./output/energy_imag2.txt" us 1:3 via a,b
	set ylab "$|| \\Phi_{T}'(E_{1}^{R}, \\varepsilon^{I}) ||^{2}$"
	set ytics 1.0
	set format y "%.2f"
	pl f(x) ti "fitting curve" ls 1, \
	 "" us 1:3 every 20 ti "data" ls 2
}

unset multiplot
set output
set ter pop
reset
