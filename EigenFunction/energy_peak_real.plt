unset multiplot; reset

load "./params.txt"

#set term tikz standalone
#set output "C:/Users/sigmajima/Dropbox/TeX/thesis/interim/slide/graph_slide/real.tex"

set yran [0:1]
set xlab "$\\varepsilon^{R} / \\hbar \\omega$"
set ylab "$|| \\Phi_{T}(\\varepsilon^{R}) ||^{2}$"

set label 1 sprintf("$E_{0}^{R} = %.3f$", ER0) center at first ER0,ER0_val+0.2
set arrow 1 from first ER0,ER0_val+0.15 to ER0,ER0_val+0.01

if(peakNum > 1){
 set label 2 sprintf("$E_{1}^{R} = %.3f$", ER1) center at first ER1,ER1_val+0.2
 set arrow 2 from first ER1,ER1_val+0.15 to ER1,ER1_val+0.01
}

set label 3 sprintf("$T = %.0f$", T) right at graph 0.9,0.9

pl "./output/energy_peak_real.txt" ti "" w l lc rgb "red"

set output
set term pop
