Ripples : Filter {
    // array of inputs, cutoff frequencies, and rqs
	*ar { |specificationsArrayRef, input, fm_freq=440.0, fm_mul=1.0, fm_index=1.0, mul=1.0, add=0.0|
	    specificationsArrayRef = specificationsArrayRef.multiChannelExpandRef(2);
		^this.multiNew('audio', input, fm_freq, fm_mul, fm_index, specificationsArrayRef).madd(mul, add);
	}

	*kr { |specificationsArrayRef, input, fm_freq=440, fm_mul=1.0, fm_index=1.0, mul=1.0, add=0.0|
	    specificationsArrayRef = specificationsArrayRef.multiChannelExpandRef(2);
		^this.multiNew('control', input, fm_freq, fm_mul, fm_index, specificationsArrayRef).madd(mul, add);
	}

	*new1 {|rate, input, fm_freq, fm_mul, fm_index, arrayRef|
		var specs, cfs, rqs;
		# cfs, rqs = arrayRef.dereference;
		specs = [
			cfs,
			rqs ?? {Array.fill(cfs.size,1.0)}
		].flop.flat;
		^super.new.rate_(rate).addToSynth.init([input, fm_freq, fm_mul, fm_index] ++ specs)
	}

	init {|theInputs|
		inputs = theInputs;
	}

	checkInputs {
		^this.checkValidInputs;
	}


	argNamesInputOffset { ^2 }
}