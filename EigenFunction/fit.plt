###################################
#プログラムにパイプで渡すスクリプト
###################################
reset

load "params.txt"
filename = "./output/fit_result.txt"
set fit quiet

f(x) = a * ((exp((x - b) * T) - 1)/((x - b) * T))**2 * exp(-2 * x * T)
a = 1e-2; b = 1e-4

fit f(x) "./output/energy_imag.txt" us 1:2 via a,b
set print filename
print b

if(peakNum > 1){
	a = 1e-2; b = 1e-4
	fit f(x) "./output/energy_imag.txt" us 1:3 via a,b
	set print filename append
	print b
}
