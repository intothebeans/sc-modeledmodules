BooleanLogic : UGen {
    classvar operations;
    *initClass {
        operations=(
            and: 0,
            or: 1,
            xor: 2,
            not: 3,
            nand: 4,
            nor: 5,
            xnor: 6,
            0: 0, 1: 1, 2: 2, 3: 3, 4: 4, 5: 5, 6: 6
        );
    }
	*kr { |input1, input2, operation=0|
        var operationNumber = this.operationNumberFor(operation);
        ^this.multiNew('control', input1, input2, operationNumber);
    }

	checkInputs {
		^this.checkValidInputs;
	}

    *operationNumberFor{ |operation=(\and)|
        ^operations[operation];
    }
}
