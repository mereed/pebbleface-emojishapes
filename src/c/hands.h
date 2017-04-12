#pragma once

#include "pebble.h"


#ifdef PBL_PLATFORM_CHALK
static const GPathInfo MINUTE_HAND_POINTS = {
  6,
  (GPoint []) {
    { 0, -66 },
    { -10, -66 },
    { -10, -86 },
	{ 10, -86 },
	{10, -66},
	{0, -66},
  }
};

static const GPathInfo HOUR_HAND_POINTS = {
  5,
  (GPoint []) {
    { 0, -44 },
    { -10, -54 },
    { 0, -64 },
	{ 10, -54 },
	{0, -44},
  }
};

static const GPathInfo SECOND_HAND_POINTS = {
  4,
  (GPoint []) {
    { -10, -26 },
    { 0, -42 },
    { 10, -26 },
	{ -10, -26 },
  }
};

#else

static const GPathInfo MINUTE_HAND_POINTS = {
  6,
  (GPoint []) {
    { 0, -54 },
    { -10, -54 },
    { -10, -72 },
	{ 10, -72 },
	{10, -54},
	{0, -54},
  }
};

static const GPathInfo HOUR_HAND_POINTS = {
  5,
  (GPoint []) {
    { 0, -34 },
    { -10, -44 },
    { 0, -54 },
	{ 10, -44 },
	{0, -34},
  }
};

static const GPathInfo SECOND_HAND_POINTS = {
  4,
  (GPoint []) {
    { -10, -18 },
    { 0, -32 },
    { 10, -18 },
	{ -10, -18 },
  }
};

#endif