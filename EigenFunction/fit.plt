###################################
#�v���O�����Ƀp�C�v�œn���X�N���v�g
###################################
unset multiplot; reset

load "params.txt"

set fit quiet
set fit errorvariables

filename = "./output/fit_result.txt"

f(x) = a * ((exp((x - b) * T) - 1)/((x - b) * T))**2 * exp(-2 * x * T)
a = 1e-2; b = 1e-4

fit f(x) "./output/energy_imag.txt" us 1:2 via a,b
set print filename
print b, b_err

if(peakNum > 1){
	a = 1e-2; b = 1e-4
	fit f(x) "./output/energy_imag.txt" us 1:3 via a,b
	set print filename append
	print b, b_err
}