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
a = 1e-5; b = 1e-5

fit f(x) "./output/energy_imag.txt" us 1:2 every :::0::0 via a,b
set print filename
print b, " ", b_err

if(peakNum > 1){
	a = 1e-5; b = 1e-5
	fit f(x) "./output/energy_imag.txt" us 1:2 every :::1::1 via a,b
	set print filename append
	print b, " ", b_err
}

if(peakNum > 2){
	a = 1e-5; b = 1e-5
	fit f(x) "./output/energy_imag.txt" us 1:2 every :::2::2 via a,b
	set print filename append
	print b, " ", b_err
}
