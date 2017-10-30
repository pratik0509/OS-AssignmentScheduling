# INTRODUCTION

xv6 is a re-implementation of Dennis Ritchie's and Ken Thompson's Unix
Version 6 (v6).  xv6 loosely follows the structure and style of v6,
but is implemented for a modern x86-based multiprocessor using ANSI C.

## PRIORITY BASED SCHEDULER

Priority scheduling is a method of scheduling processes based on priority.
In this method, the scheduler chooses the tasks to work as per the priority,
which is different from other types of scheduling, for example, a simple
round robin.

Priority scheduling involves priority assignment to every process, and
processes with higher priorities are carried out first, whereas tasks with
equal priorities are carried out on a first-come-first-served (FCFS) or
round robin basis. An example of a general-priority-scheduling algorithm
is the shortest-job-first (SJF) algorithm.

```
//PAGEBREAK: 42
// Per-CPU process scheduler.
// Each CPU calls scheduler() after setting itself up.
// Scheduler never returns.  It loops, doing:
//  - choose highest priority process to run
//  - swtch to start running that process
//  - eventually that process transfers control
//      via swtch back to the scheduler.
void
scheduler(void)
{
  struct proc *p;
  struct cpu *c = mycpu();
  struct proc *min_priority_proc = 0;
  c->proc = 0;

  for(;;){
    // Enable interrupts on this processor.
    sti();
    min_priority_proc = 0;

    // Loop over process table looking for process to run.
    acquire(&ptable.lock);
    for(p = ptable.proc; p < &ptable.proc[NPROC]; p++){
      if(p->state != RUNNABLE)
        continue;
      if(min_priority_proc == 0){
        min_priority_proc = p;
        continue;
      }
      if(min_priority_proc->priority > p->priority)
        min_priority_proc = p;
    }

    // Switch to chosen process.  It is the process's job
    // to release ptable.lock and then reacquire it
    // before jumping back to us.
    if(min_priority_proc != 0){
      c->proc = min_priority_proc;
      switchuvm(min_priority_proc);
      min_priority_proc->state = RUNNING;

      swtch(&(c->scheduler), min_priority_proc->context);
      switchkvm();

      // Process is done running for now.
      // It should have changed its p->state before coming back.
      c->proc = 0;
    }
    release(&ptable.lock);

  }
}
```

## ```waitx``` SYSTEM CALL

```waitx``` system call returns the number of context switches performed from
 **RUNNING** to **WAITING** and number of context switches performed during the
 **RUNNING** state and total number of context switches.

```
// Wait for a child process to exit and return its pid.
// Return -1 if this process has no children.
// Also assign the wait time and running time to the passed arguments
int
waitx(int *wtime, int *rtime, int *ttime)
{
  struct proc *p;
  int havekids, pid;
  struct proc *curproc = myproc();

  acquire(&ptable.lock);
  for(;;){
    // Scan through table looking for exited children.
    havekids = 0;
    for(p = ptable.proc; p < &ptable.proc[NPROC]; p++){
      if(p->parent != curproc)
        continue;
      havekids = 1;
      if(p->state == ZOMBIE){
        // Found one.

        // Assign the wait and running time values
        *ttime = p->total_time;
        *wtime = p->wait_time;
        *rtime = p->run_time + 1;

        pid = p->pid;
        kfree(p->kstack);
        p->kstack = 0;
        freevm(p->pgdir);
        p->pid = 0;
        p->parent = 0;
        p->name[0] = 0;
        p->killed = 0;
        p->state = UNUSED;
        release(&ptable.lock);
        return pid;
      }
    }

    // No point waiting if we don't have any children.
    if(!havekids || curproc->killed){
      release(&ptable.lock);
      return -1;
    }

    // Wait for children to exit.  (See wakeup1 call in proc_exit.)
    sleep(curproc, &ptable.lock);  //DOC: wait-sleep
  }
}
```

## ```set_priority``` SYSTEM CALL

```set_priority``` system call sets the priority of the process and
returns the old priority of the process.

```
// Assign the Process Priority to the process
int
set_priority(int prior)
{
  struct proc *curproc;
  int init_prior;
  if(prior < 0)
    prior = 0;
  else if(prior > 100)
    prior = 100;
  curproc = myproc();
  // Acquire the lock on process table
  acquire(&ptable.lock);
  init_prior = curproc->priority;
  curproc->priority = prior;
  curproc->state = RUNNABLE;
  sched();
  // Release the lock on process table
  release(&ptable.lock);
  return init_prior;
}
```
