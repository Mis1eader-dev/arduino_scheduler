#include "Arduino.h"
#include "scheduler.hpp"
#include <deque>

struct Task
{
	int64_t runAfterMillis;
	scheduler::TaskCallback callback;
};
std::deque<Task> tasks;

static void insert_sorted(const Task&& val)
{
	int64_t before = millis();
	
	for(std::deque<Task>::iterator it = tasks.begin(); it < tasks.end(); ++it)
	{
		if(it->runAfterMillis > val.runAfterMillis)
		{
			tasks.insert(it, std::move(val));
			return;
		}
	}

	tasks.push_back(std::move(val));
	
	for(Task& task : tasks)
		task.runAfterMillis -= int64_t(millis()) - before;
}

void scheduler::add(
	const TaskCallback& callback,
	uint64_t runAfterMillis)
{
	insert_sorted(
		std::move(
			Task
			{
				(int64_t)runAfterMillis,
				callback,
			}
		)
	);
}

void scheduler::loop()
{
	int64_t before = millis();
	
	if(tasks.empty())
		return;
	
	TaskCallback callback;
	{
		const Task& first = tasks.front();
		callback = first.callback;
		
		// Sleep until first task is to be executed
		auto runAfterMillis = first.runAfterMillis;
		if(runAfterMillis > 0)
			delay(runAfterMillis - (int64_t(millis()) - before));
	}
	
	// Woke up
	
	// Remove first task that we will execute
	tasks.pop_front();
	
	int64_t dur = int64_t(millis()) - before;
	for(Task& task : tasks)
		task.runAfterMillis -= dur;
	
	before = millis();
	
	// Now execute the task
	callback();
	
	// Reduce other tasks' lifetime
	dur = int64_t(millis()) - before;
	for(Task& task : tasks)
		task.runAfterMillis -= dur;
}