#include "os.h"
#include <stdio.h>
#include <assert.h>
#include "readyQ.h"

#define TASK(t) TASK_##t()

int flag;

mutex_pt m1;
mutex_pt m2;
//extern Mutex* mutex_list;

#define JUMP_1(){\
        switch (current_pc[1])\
        {\
                case 0:\
                        goto L_1_0;break;\
                case 1:\
                        goto L_1_1;break;\
                case 2:\
                        goto L_1_2;break;\
                case 3:\
                        goto L_1_3;break;\
                case 4:\
                        goto L_1_4;break;\
                case 5:\
                        goto L_1_5;break;\
                case 6:\
                        goto L_1_6;break;\
        }\
}

void TASK(1)
{
	JUMP_1();
L_1_0:
	current_pc[1]++;
	flag = mutex_lock(m1);
	if (flag == 0) {
		scheduler();
		return;
	}
L_1_1:
	current_pc[1]++;
	flag = task_create(2);
	if (flag == 0)
	{
		scheduler();
		return;
	}

L_1_2:
	current_pc[1]++;
	printf("task1 prio is %d \n", task_dyn_info[1].dyn_prio);
	flag = mutex_lock(m2);
	if (flag == 0) {
		scheduler();
		return;
	}
L_1_3:
		current_pc[1]++;
		flag = task_create(3);
		if (flag == 0) {
			scheduler();
			return;
		}

	L_1_4:
		printf("task1 prio is %d \n", task_dyn_info[1].dyn_prio);
		current_pc[1]++;
		flag = mutex_unlock(m1);
		printf("task1 prio is %d \n", task_dyn_info[1].dyn_prio);
		if (flag == 0) {
			scheduler();
			return;
		}
L_1_5:
		printf("task1 prio is %d \n", task_dyn_info[1].dyn_prio);
		current_pc[1]++;
		flag = mutex_unlock(m2);
		printf("task1 prio is %d \n", task_dyn_info[1].dyn_prio);
		if (flag == 0) {
			scheduler();
			return;
		}
L_1_6:
		current_pc[1] = 0;
		flag = TerminateTask();
		if (flag == 0)
		{
			scheduler();
			return;
		}

	}

#define JUMP_2(){\
        switch (current_pc[2])\
        {\
                case 0:\
                        goto L_2_0; break;\
                case 1:\
                        goto L_2_1; break;\
                case 2:\
                        goto L_2_2; break;\
        }\
}


int counter;
void TASK(2)
{
	JUMP_2();
L_2_0:
		current_pc[2]++;
		flag = mutex_lock(m1);
		if (flag == 0) {
			scheduler();
			return;
		}

L_2_1:

		current_pc[2]++;
		flag = mutex_unlock(m1);
		if (flag == 0) {
			scheduler();
			return;
		}

L_2_2:
		current_pc[2] = 0;
		flag = TerminateTask();
		if (flag == 0) {
			scheduler();
			return;
		}
}

#define JUMP_3(){\
        switch(current_pc[3])\
        {\
                case 0:\
                        goto L_3_0;break;\
                case 1:\
                        goto L_3_1;break;\
                case 2:\
                        goto L_3_2;break;\
        }\
}

int task3_terminate;
int task3_unlock;
void TASK(3)
{
	JUMP_3();

L_3_0:
	current_pc[3]++;
	flag = mutex_lock(m2);
	if (!flag) {
		scheduler();
		return;
	}

L_3_1:
	current_pc[3]++;
	//task3_unlock = 1;
	flag = mutex_unlock(m2);
	if (!flag) {
		scheduler();
		return;
	}
L_3_2:
	task3_terminate = 1;
	current_pc[3] = 0;
	flag = TerminateTask();
	if (!flag) {
		scheduler();
		return;
	}
}
void main()
{

	mutex_create(&m1);
	mutex_create(&m2);
	printf("create task1\n\n");
	task_create(1);
	ubik_comp_start();
	mutex_delete(&m1);
	mutex_delete(&m2);
	ShutDownOS();

}

void running()
{
	while (current_tid >= 0)
	{
		if (current_tid == 1)
			TASK(1);
		else if (current_tid == 2)
			TASK(2);
		else if (current_tid == 3)
			TASK(3);
	}
//	assert(current_tid == -1);
	//assert(0);
}
