#!/bin/bash

./DummyCreatureAgent &> agent.log &
sleep 2 && ./DummyCreatureExperiment localhost &> experiment.log &
./gui res/cat_env.prototxt
