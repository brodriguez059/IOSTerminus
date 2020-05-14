#!/bin/bash
rm ./main
rm ./obj/commands/*
rm ./obj/events/*
rm ./obj/main.o
rm ./obj/terminuslib.o
rm ./bin/commands/*
rm ./bin/events/*

./compile terminuslib

./compile ev_cat ev_less ev_mv ev_rm ev_touch

./compile cd logout

./compile cat less mv rm touch

./compile cd cp grep ls man pwd whoami

./compile main