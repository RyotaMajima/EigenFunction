###################################
#プログラムにパイプで渡すスクリプト
###################################

unset multiplot; reset
load "params.txt"

set fit quiet
set fit errorvariables

filename = "./output/fit_result_decay.txt"
set print filename

f(x) = exp(-2*b*x); b0 = 1e-5

b = b0
fit f(x) "./output/decay.txt" index 0 via b
print b, " ", b_err

if(peakNum > 1){
	b = b0
	fit f(x) "./output/decay.txt" index 1 via b
	set print filename append
	print b, " ", b_err
}

if(peakNum > 2){
	b = b0
	fit f(x) "./output/decay.txt" index 2 via b
	set print filename append
	print b, " ", b_err
}
