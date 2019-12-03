#!/bin/bash

gcc $1 -g && valgrind --leak-check=full ./a.out
