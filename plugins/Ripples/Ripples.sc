Ripples : MultiOutUGen {

	*ar { arg in, bpCF = 440.0, bpR = 1, bpBW = 1, lp2CF= 0.0, lp2R=0.0, lp4CF=0.0, lp4R=0.0,
		fmFreq=440.0, fmMul=1.0, fmIndex=1.0, mul=1.0, add=0.0;
		^this.multiNew('audio', in, bpCF, bpR, bpBW, lp2CF, lp2R, lp4CF, lp4R, fmFreq, fmMul, fmIndex).madd(mul, add);
	}

	*kr { arg in, bpCF = 440.0, bpR = 1, bpBW = 1, lp2CF= 0.0, lp2R=0.0, lp4CF=0.0, lp4R=0.0,
		fmFreq=440.0, fmMul=1.0, fmIndex=1.0, mul=1.0, add=0.0;
		^this.multiNew('control', in, bpCF, bpR, bpBW, lp2CF, lp2R, lp4CF, lp4R, fmFreq, fmMul, fmIndex).madd(mul, add);
	}

	init { arg ... theInputs;
		inputs = theInputs;
		channels = [
			OutputProxy(rate, this, 0),
			OutputProxy(rate, this, 1),
			OutputProxy(rate, this, 2),
		];
		^channels
	}

	checkInputs {
		^this.checkValidInputs;
	}
}

