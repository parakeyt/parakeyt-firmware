//! util.h
//!
//! utility functions
#pragma once

/// Maximum of two integers
int imax(int a, int b);

/// Minimum of two integers
int imin(int a, int b);

/// Return a random number on range [low, high]
int get_rand_inclusive(int low, int high);

int wrap(int val, int min, int max);

int clamp(int val, int min, int max);
