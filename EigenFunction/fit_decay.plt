###################################
#プログラムにパイプで渡すスクリプト
###################################
unset multiplot; reset

load "params.txt"

set fit quiet
set fit errorvariables

filename = "./output/fit_result_decay.txt"

f(x) = a * exp(-2*b*x)
a = 1e-10; b = 1e-10

fit f(x) "./output/decay.txt" index 0 every 100 via a,b
set print filename
print b, " ", b_err

if(peakNum > 1){
	a = 1e-10; b = 1e-10
	fit f(x) "./output/decay.txt" index 1 every 100 via a,b
	set print filename append
	print b, " ", b_err
}

if(peakNum > 2){
	a = 1e-10; b = 1e-10
	fit f(x) "./output/decay.txt" index 2 every T*TN/100 via a,b
	set print filename append
	print b, " ", b_err
}
