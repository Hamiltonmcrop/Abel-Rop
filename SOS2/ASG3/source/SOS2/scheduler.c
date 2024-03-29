/**
 * Student Name
*/
////////////////////////////////////////////////////////
// A Round-Robin Scheduler with 50% share to console
// 
// Process queue is maintained as a doubly linked list
// TODO: processes should be on different queues based 
//       on their state

#include "kernel_only.h"

extern GDT_DESCRIPTOR gdt[6];	// from startup.S
extern TSS_STRUCTURE TSS;	// from systemcalls.c

PCB console;	// PCB of the console (==kernel)
PCB *current_process; // the currently running process
PCB *processq_next = NULL; // the next user program to run

void init_scheduler() {
	current_process = &console; // the first process is the console
}

/*** Add process to process queue ***/
// Returns pointer to added process
PCB *add_to_processq(PCB *p) 
{
	disable_interrupts();

	// TODO: add process p to the queue of processes, always 
	// maintained as a circular doubly linked list;
	// processq_next always points to the next user process
	// that will get the time quanta;
	// if the process queue is non-empty, p should be added immediately
	// before processq_next
	// For details, read assignment background material

	//need to handle 2 possible cases:
		//1. list is empty (processq_next == null)
		//2. list is non-empty (insert p directly before the node at processq_next)

	
	//case 1- empty
	if(processq_next == NULL)
    {
        //set the processq_next pointer to point at p
        processq_next = p;

        //list is circular, so the next of p 
        //should point to itself
        p->prev_PCB = p;
        p->next_PCB = p;
    }

    //case 2- non empty
    else
    {
        //set p to mirror processq_next
        p->next_PCB = processq_next;
        p->prev_PCB = processq_next->prev_PCB;

        //set processq_next to p by pointing the next of it's previous to p
        processq_next->prev_PCB->next_PCB = p;

        //finally, we can lose the pointer to the current next, and point to p
        processq_next->prev_PCB = p;
    }

    //update state
    p->state = READY;
    
	enable_interrupts();

	return p;		
}

/*** Remove a TERMINATED process from process queue ***/
// Returns pointer to the next process in process queue
PCB *remove_from_processq(PCB *p) {
	// TODO: remove process p from the process queue
    //check for empty list
    if(p == NULL)
    {
        return NULL;
    }

    //case removing the last element of the list
    else if(p->prev_PCB == p)
    {
        processq_next = NULL;

        //list will now be empty, so return val should be null
        p->next_PCB = NULL;
        
    }
    
    else
    {
        //check the process to remove if next or current.
        if(processq_next == p)
        {
            //point process q to the next of current process
            processq_next = p->next_PCB;
        }

        //the process behind p should now point ahead of p
        p->prev_PCB->next_PCB = p->next_PCB;

        //the process receedin p now preceeds p.
        p->next_PCB->prev_PCB = p->prev_PCB;
    }


	// TODO: free the memory used by process p's image
    dealloc_memory((void*) p->memory_base);

	// TODO: free the memory used by the PCB
    dealloc_memory(p);

	// TODO: return pointer to next process in list
    return p->next_PCB;
}

/*** Schedule a process ***/
// This function is called whenever a scheduling decision is needed,
// such as when the timer interrupts, or the current process is done
void schedule_something() { // no interruption when here

	// TODO: see assignment background material on what this function should do 

    //Keep the current node being searched.
    PCB* current_node = processq_next;

    /**
     * Return command to console if list is Empty.
    */case where list is empty
    if(current_node == NULL)
    {
        current_process = &console;

        switch_to_kernel_process(&console);
        return;
    }

    /**
     * Keep a Loop Invariant that works by checking the Node recurrently for the Loop to complete round the Circle.
    */
    do
    {
        /**
         * If the process is waiting
         */
        if(current_node->state == WAITING)
        {
            /**
             * Get the current sleep elapsed time
             * Change state and move up in the List.
             */
            if(get_epochs() >= current_node->sleep_end)
            {
                current_node->state = READY;

                current_node = current_node->next_PCB;
                    
            }
        }

        /**
         * If urrent node is terminated remove current node and update current node
         */
        else if(current_node->state == TERMINATED)
        {
            current_node = remove_from_processq(current_node);
        }

        /**
         * Else move to the next Node
         */
        else
        {
            current_node = current_node->next_PCB;
        }

    }while(current_node != processq_next);

    /**
     * Find an invariant that alternated evenly between 0 and 1 in order to distribute the work evenly between console and process
     */
    if(get_epochs() % 2 == 1 && current_node != NULL)
    {
        /**
         * Dispatch the current to return the pointer to the head of the list and prevent Execting the complete Loop.
        */
        current_node = processq_next;

        do
        {
            if(current_node->state == READY)
            {
                current_process = current_node;
                current_node->state = RUNNING;

                processq_next = current_node->next_PCB;

                switch_to_user_process(current_node);

                return;
            }

            /**
             * Continue with the Search or return the control to the console.
            */
            current_node = current_node->next_PCB;
        }while(current_node != processq_next);

    }

    current_process = &console;

    // TODO: comment the following when you start working on this function
    switch_to_kernel_process(&console);
}

/*** Switch to kernel process described by the PCB ***/
// We will use the "fastcall" keyword to force GCC to pass 
// the pointer in register ECX;
// process switched to is a kernel process; so no ring change
__attribute__((fastcall)) void switch_to_kernel_process(PCB *p)  {

	// load CPU state from process PCB
	asm volatile ("movl %0, %%edi\n": :"m"(p->cpu.edi));
	asm volatile ("movl %0, %%esi\n": :"m"(p->cpu.esi));
	asm volatile ("movl %0, %%eax\n": :"m"(p->cpu.eax));
	asm volatile ("movl %0, %%ebx\n": :"m"(p->cpu.ebx));
	asm volatile ("movl %0, %%edx\n": :"m"(p->cpu.edx));
	asm volatile ("movl %0, %%ebp\n": :"m"(p->cpu.ebp));
	asm volatile ("movl %0, %%esp\n": :"m"(p->cpu.esp));

	// switching within the same ring; IRET requires the following in stack (see IRET details)
	asm volatile ("pushl %0\n"::"m"(p->cpu.eflags));
	asm volatile ("pushl %0\n": :"m"(p->cpu.cs));
	asm volatile ("pushl %0\n": :"m"(p->cpu.eip));

	// this should be the last one to be copied
	asm volatile ("movl %0, %%ecx\n": :"m"(p->cpu.ecx));

	// issue IRET; see IRET details
	asm volatile("sti\n"); // interrupts cleared in timer/syscall handler
	asm volatile("iretl\n"); // this completes the timer/syscall interrupt
}

/*** Switch to user process described by the PCB ***/
/*** DO NOT UNCOMMENT ***/
/*
__attribute__((fastcall)) void switch_to_user_process(PCB *p) {

	// You implemented this in the previous assignment
	...

}*/



