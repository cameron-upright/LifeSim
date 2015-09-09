#!/bin/bash

./DummyCreatureAgent 2>&1 &
./DummyCreatureExperiment 2>&1 &
rl_glue 2>&1 &
./gui 2>&1 &
wait
