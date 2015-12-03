#!/bin/bash

./DummyCreatureAgent res/creatures/cat/cat.prototxt &> agent.log &
sleep 2 && ./DummyCreatureExperiment localhost &> experiment.log &
./gui res/cat_env.prototxt
