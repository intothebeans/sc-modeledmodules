Ripples : UGen {
	*ar { |input, freq, rq, mode, fm_in, fm_mul=0, mul = 1.0, add = 0.0|
		/* TODO */
		^this.multiNew('audio', input, gain).madd(mul, add);
	}
	checkInputs {
		/* TODO */
		^this.checkValidInputs;
	}
}