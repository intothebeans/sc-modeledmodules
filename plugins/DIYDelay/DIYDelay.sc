DIYDelay : UGen {
	*ar { |input, delayTime=0.5, feedback=1, tape=0, overlap=0, freeze=0, reverse=0, maxDelayTime=0.2 mul=1, add=0|
		^this.multiNew('audio', input, delayTime, feedback, overlap, freeze, reverse, maxDelayTime).madd(mul, add);
	}
	checkInputs {
		^this.checkValidInputs;
	}
}
