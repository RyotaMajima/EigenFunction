unset multiplot; reset

#set term tikz standalone size 16cm,10cm
#set output "C:/Users/U24E/Dropbox/TeX/thesis/thesis/graph/error.tex"

#set multiplot layout 2,1

set xlab "$t$"

set ls 1 lc rgb "red"

#set ylab "$|| \\Psi(t) ||^{2}$"
pl "./output/decay.txt" us 1:2 ti "" w l ls 1

unset multiplot

set output
set term pop
