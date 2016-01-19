unset multiplot; reset

#set term tikz standalone size 16cm,10cm
#set output "C:/Users/U24E/Dropbox/TeX/thesis/thesis/graph/error.tex"

set multiplot layout 2,1

set logscale y
set xran [100:500]
set xlab "$T$"

set ls 1 lc rgb "red"

set ylab "$E_{0}^{I}$"
pl "./output/error.txt" us 1:2 ti "" w p ls 1

set ylab "$E_{1}^{I}$"
pl "" us 1:3 ti "" w p ls 1

unset multiplot

set output
set term pop
