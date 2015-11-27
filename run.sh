#!/bin/bash

./DummyCreatureAgent 2>&1 > agent.log &
sleep 2 && ./DummyCreatureExperiment localhost 2>&1 > experiment.log &
./gui res/cat_env.prototxt
