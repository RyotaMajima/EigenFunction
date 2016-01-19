unset multiplot; reset

#set ter tikz standalone size 16cm, 8cm
#set output "C:/Users/sigmajima/Dropbox/TeX/thesis/interim/slide/graph_slide/eigenfunction.tex"
#set output 'C:\Users\U24E\Dropbox\TeX\thesis\thesis\graph/phi.tex'

set multiplot layout 1,2
load "params.txt"

set xran [-5:5]; set yran [-2:2]
set xlab "$x$"
set zeroaxis
set key spacing 2

set linestyle 1 lc rgb "black" lw 2
set linestyle 2 lc rgb "red"
set linestyle 3 lc rgb "blue"
set style fill transpa solid 0.5 noborder

set label 1 "(a)" left at first -4,1.5
set ylab "$|| \\varphi_{0}(x) ||^{2}$"
set title sprintf("$E_{0} = %.3f - %.8f i$", ER0, abs(EI0))
pl "./output/phi0.txt" us 1:2 ti "$V(x)$" w l ls 1, \
 "" us 1:3 ti "first peak" w filledcur ls 2, "./output/ho.txt" us 1:2 ti "" w l dt 3 lc rgb "red" lw 2

if(peakNum > 1){
	set label 1 "(b)" left at first -4,1.5
	set ylab "$|| \\varphi_{1}(x) ||^{2}$"
	set title sprintf("$E_{1} = %.3f - %.6f i$", ER1, abs(EI1))
	pl "./output/phi1.txt" us 1:2 ti "$V(x)$" w l ls 1, \
	 "" us 1:3 ti "second peak" w filledcur ls 3, "./output/ho.txt" us 1:3 ti "" w l dt 3 lc rgb "blue" lw 2
}

unset multiplot
set output
set ter pop
reset
