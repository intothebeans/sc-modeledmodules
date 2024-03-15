RipplesLP2 : Filter {
	*ar { |in=0.0, freq=440, rq=1, mul=1.0, add=0.0|
		^this.multiNew('audio', in, freq, rq).madd(mul, add);
	}
	checkInputs {
		^this.checkValidInputs;
	}
}