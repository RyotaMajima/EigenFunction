unset multiplot; reset

#set ter tikz standalone size 15cm,8cm
#set output 'C:\Users\sigmajima\Dropbox\TeX\thesis\slide\graph_slide/eigenfunction.tex'
#set output 'C:\Users\U24E\Dropbox\TeX\thesis\slide\graph_slide/eigenfunction.tex'

set multiplot layout 1,2
load "params.txt"

set xran [-4:1/b + 1]; set yran [-1/(6 * b*b)-1:2]
set xlab "$x$"
set zeroaxis
set key spacing 2

set linestyle 1 lc rgb "black" lw 2
set linestyle 2 lc rgb "red" lw 2
set linestyle 3 lc rgb "blue" lw 2
set style fill transpa solid 0.5 noborder

set label 1 "(a)" left at graph 0.05,0.9
set ylab "$|| \\varphi_{0}(x) ||^{2}$"
set title sprintf("$E_{0} = %.3f - %.8f i$", ER0, abs(EI0))
pl "./output/ho.txt" us 1:2 ti "$V(x)$" w l ls 1, \
 "" us 1:3 ti "" w l ls 2 dt 3, \
 "./output/phi.txt" index 0 ti "1st peak" w filledcur ls 2

if(peakNum > 1){
	set label 1 "(b)" left at graph 0.05,0.9
	set ylab "$|| \\varphi_{1}(x) ||^{2}$"
	set title sprintf("$E_{1} = %.3f - %.6f i$", ER1, abs(EI1))
	pl "./output/ho.txt" us 1:2 ti "$V(x)$" w l ls 1, \
	 "" us 1:4 ti "" w l ls 3 dt 3, \
	 "./output/phi.txt" index 1 ti "2nd peak" w filledcur ls 3
}

unset multiplot
set output
set ter pop
reset
