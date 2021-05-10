# About

This an example of concept for acquiring the lock while iterating the container

# Building

Three modes are implemented as an example. Same workflow is used for all modes.

## Plain - no locks

`$ g++ locked_iterator.cpp -DPLAIN_MODE`

## Recursive lock

`$ g++ locked_iterator.cpp -DRECURSIVE_LOCK -pthread`

## Exclusive lock

`$ g++ locked_iterator.cpp -DEXCLUSIVE_LOCK -pthread`

Note: this mode is expected to fail.
