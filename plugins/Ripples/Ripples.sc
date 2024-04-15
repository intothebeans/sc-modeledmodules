Ripples : MultiOutUGen {

	*ar { arg in, bpCF = 440.0, bpR = 0.5, bpRQ = 1, lp2CF= 0.0, lp2R=0.0, lp4CF=0.0, lp4R=0.0, mul=1.0, add=0.0;
		^this.multiNew('audio', in, bpCF, bpR, bpRQ, lp2CF, lp2R, lp4CF, lp4R).madd(mul, add);
	}

	*kr { arg in, bpCF = 440.0, bpR = 0.5, bpRQ = 1, lp2CF= 0.0, lp2R=0.0, lp4CF=0.0, lp4R=0.0, mul=1.0, add=0.0;
		^this.multiNew('control', in, bpCF, bpR, bpRQ, lp2CF, lp2R, lp4CF, lp4R).madd(mul, add);
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

