#include "stdafx.h"
#include "FpsCounter.h"
#include <initflags.h>

FpsCounter::FpsCounter(const string & name) : _name(name)
{
}

FpsCounter::~FpsCounter()
{
}

u16 FpsCounter::Reset()
{
	auto count = _count;
	_clock.restart();
	_count = 0;

	return count;
}

void FpsCounter::Tick()
{
	_count++;

	if (Elapsed()) {
		_last = Reset();

		// The launch flag fps prints the counts each second
		static const bool print = lib::init::Flag("fps", false);
		if (print)
			cout << _name << " fps: " << _last << endl;
	}
}
