#!/bin/bash

./DummyCreatureAgent res/creatures/cat/cat.prototxt &> agent.log &
sleep 2 && ./DummyCreatureExperiment localhost &> experiment.log &
./sim res/cat_env.prototxt 2>&1 | tee env.log
