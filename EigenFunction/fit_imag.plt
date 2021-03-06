###################################
#プログラムにパイプで渡すスクリプト
###################################
unset multiplot; reset

load "params.txt"
b = 0

set fit quiet
set fit errorvariables

filename = "./output/fit_result.txt"

f(x) = a * ((exp((x - b) * T) - 1)/((x - b) * T))**2 * exp(-1 * x * T)
a = 1e-10; b = 1e-10

fit f(x) "./output/energy_imag.txt" us 1:2 via a,b
set print filename
print b, " ", b_err

if(peakNum > 1){
	a = 1e-10; b = 1e-10
	fit f(x) "./output/energy_imag.txt" us 1:3 via a,b
	set print filename append
	print b, " ", b_err
}

if(peakNum > 2){
	a = 1e-10; b = 1e-10
	fit f(x) "./output/energy_imag.txt" us 1:4 via a,b
	set print filename append
	print b, " ", b_err
}
