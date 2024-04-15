BooleanLogic : UGen {
	// aliases for UGen boolean operations
	const <and = 0;
	const <or = 1;
	const <xor = 2;
	const <nand = 3;
	const <nor = 4;
	const <xnor = 5;

	*kr { |input1, input2, operation=0|
        ^this.multiNew('control', input1, input2, operation);
    }

	checkInputs {
		^this.checkValidInputs;
	}

}
