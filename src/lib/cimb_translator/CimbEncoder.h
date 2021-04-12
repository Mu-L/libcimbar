/* This code is subject to the terms of the Mozilla Public License, v.2.0. http://mozilla.org/MPL/2.0/. */
#pragma once

#include "image.h"
#include <string>
#include <vector>

template <typename IMG>
class CimbEncoder
{
public:
	CimbEncoder(unsigned symbol_bits=4, unsigned color_bits=2, bool dark=true)
	    : _numSymbols(1 << symbol_bits)
	    , _numColors(1 << color_bits)
	    , _dark(dark)
	{
		load_tiles(symbol_bits); // TODO: smarter caching?
	}

	IMG load_tile(unsigned symbol_bits, unsigned index)
	{
		unsigned symbol = index % _numSymbols;
		unsigned color = index / _numSymbols;
		return cimbar::load<IMG>::getTile(symbol_bits, symbol, _dark, _numColors, color);
	}

	bool load_tiles(unsigned symbol_bits)
	{
		unsigned numTiles = _numColors * _numSymbols;
		for (unsigned i = 0; i < numTiles; ++i)
			_tiles.push_back(load_tile(symbol_bits, i));
		return true;
	}

	const IMG& encode(unsigned bits) const
	{
		bits = bits % _tiles.size();
		return _tiles[bits];
	}


protected:
	std::vector<IMG> _tiles;
	unsigned _numSymbols;
	unsigned _numColors;
	bool _dark;
};
