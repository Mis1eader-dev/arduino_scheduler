# Arduino Scheduler
Simple scheduler for Arduino-based libraries. Suitable for single threaded microcontrollers.

# Usage
```c++
#include "scheduler.hpp"

static void t1();
static void t2();

void setup()
{
	t1();
	t2();
}

void loop()
{
	scheduler::loop();
}

static bool t1State = false;
static void t1()
{
	t1State = !t1State;
	
	if(t1State)
	{
		scheduler::add(t1, 2000);
		return;
	}
	
	scheduler::add(t1, 1000);
}

static bool t2State = false;
static void t2()
{
	t2State = !t2State;
	
	if(t2State)
	{
		scheduler::add(t2, 1500);
		return;
	}
	
	scheduler::add(t2, 2500);
}
```

# License
WTFPL. Also present in the `scheduler.hpp` file.
