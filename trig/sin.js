let synaptic = require ("synaptic");
let Layer, Network, Neuron, Trainer;
Layer = synaptic.Layer;
Network = synaptic.Network;
Neuron = synaptic.Neuron;
Trainer = synaptic.Trainer;

let input, hidden, output;
input = new Layer (1).set ({squash: Neuron.squash.RELU});
hidden = new Layer (8).set ({squash: Neuron.squash.RELU});
output = new Layer (1).set ({squash: Neuron.squash.RELU});

input.project (hidden);
hidden.project (output);

let net = new Network ({input, hidden: [hidden], output});
let trainer = new Trainer (net);

let random = require ("random-js");
const native = random.engines.nativeMath;

const length = 1024 * 1024;
let set = new Array (length).fill ().map (a => ({input: 0, output: 0}));
for (let x = 0; x < length; x++) {
	set[x].input = [random.real (-Math.PI, Math.PI, true) (native)];
	set[x].output = [Math.sin(set[x].input[0])];
}

trainer.train (set);
console.log (net.standalone ().toString ());