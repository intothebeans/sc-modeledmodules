DIYDelay : UGen {
	*ar { |input, delayTime=0.2, feedback=0.5, tape=0, hold=0, reverse=0, maxDelayTime=0.2 mul=1, add=0|
		^this.multiNew('audio', input, delayTime, feedback, tape, hold, reverse, maxDelayTime).madd(mul, add);
	}
	checkInputs {
		^this.checkValidInputs;
	}
}
