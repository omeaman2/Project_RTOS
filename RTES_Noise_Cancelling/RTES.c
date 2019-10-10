#include "RTES.h"

/*-------------------------------------------------------------------------------------------------*/
/*-------------------------------------------------------------------------------------------------*/
/*							
										ALL COMMUNICATION:	
	[Input Task]
	vRTES_Input				--->		xQueue_Input			--->		vRTES_Recognize	
	(1)	vRTES_Input executes once every millisecond, every millisecond it will place a RTES_Frame_t 
		in xQueue_Input. This data will be retrieved once every 20 milliseconds by vRTES_Recognize,
		which will analyse this data.

	[Output Task]
	vRTES_Output			--->		TaskNotification		--->		vRTES_CancelNoise
	(2)	vRTES_CancelNoise is only allowed to send the to be outputted frames to vRTES_Output once
		vRTES_Output has finished outputting previous frames, vRTES_Output will notify vRTES_CancelNoise
		once this is the case.
	vRTES_Output			<---		xQueue_Output			<---		vRTES_CancelNoise
	(3)	vRTES_CancelNoise calculates the 25 Frames which will be outputted by the speakers, it will
		place these in xQueue_Output once it's notified to do so.

	[Recognize Task]
	vRTES_Recognize			<---		xQueue_Input			<---		vRTES_Input	
		Same as (1)
	vRTES_Recognize			--->		xQueue_Predict			--->		vRTES_Predict
	(4)	If vRTES_Recognize recognizes the noise then it will send all metadata to vRTES_Predict,
		this allows vRTES_Predict to update it's prediction
	vRTES_Recognize			--->		xQueue_CancelNoise		--->		vRTES_CancelNoise
	(5)	If vRTES_Recognize recognizes the noise it will send the data containing the noise to
		vRTES_CancelNoise which allows it to calculate the noise cancellation.

	[Predict Task]
	vRTES_Predict			<---		xQueue_Predict			<---		vRTES_Recognize
		Same as (4)

	[CancelNoise Task]
	vRTES_CancelNoise		<---		xQueue_CancelNoise		<---		vRTES_Recognize
		Same as (5)
	vRTES_CancelNoise		<---		TaskNotification		<---		vRTES_Output
		Same as (2)
	vRTES_CancelNoise		--->		xQueue_Output			--->		vRTES_Output
		Same as (3)	

	All tasks also communicate to the Monitor Task using xQueue_Monitoring, the data send is the 
	taskID, the tickcount the task started and the tickcount the task ended (for every task period).  
																								   */	
/*-------------------------------------------------------------------------------------------------*/
/*-------------------------------------------------------------------------------------------------*/

void vRTES_Input(void* pvParameters) {
	const char* pcTaskName = "Task Input";
	TickType_t xLastWakeTime = xTaskGetTickCount();
	RTES_Frame_t input_frame;
	
	/* Data used by the monitoring task */
	TickType_t xTimeTaskStarted = xTaskGetTickCount;
	TickType_t xTimeTaskEnded = xTaskGetTickCount;
	RTES_TASKID_t taskid = id_input;
	RTES_Monitoring_t monitoring = {taskid, xTimeTaskStarted, xTimeTaskEnded };

	for (;;) {
#if (RTES_PRINT_INFORMATION == 1)
		printf("[%d]: %s\n", xTaskGetTickCount(), pcTaskName);
#endif
		/* Record the time this period of the task has been started */
		xTimeTaskStarted = xTaskGetTickCount();

		/* Read data from the microphone */
		input_frame = takeSample();

#if (RTES_PRINT_INFORMATION == 1)
		printf("Data read from microphone: %f\n", input_frame.data);
#endif

		/* Send data to the input queue, this task doesn't block if the queue is full.
			Note the queue should never be full */
		if (xQueueSend(xQueue_Input, &input_frame, 0) != pdPASS) {
#if (RTES_PRINT_INFORMATION != 0)
			printf("Error, xQueue_Input is full!\n");
#endif
		}

		/* Record the time this period of the task has finished */
		xTimeTaskEnded = xTaskGetTickCount();

		/* Send monitoring data to the monitoring queue */
		monitoring.tickcount_start = xTimeTaskStarted;
		monitoring.tickcount_end = xTimeTaskEnded;
		if (xQueueSend(xQueue_Monitoring, &monitoring, 0) != pdPASS) {
#if (RTES_PRINT_INFORMATION != 0)
			printf("Error, xQueue_Monitoring is full!\n");
#endif
		}

		vTaskDelayUntil(&xLastWakeTime, xDelay_Input);
	}
}

RTES_Frame_t takeSample(void) {
	RTES_Frame_t newFrame;

	/* To simulate the microphone "reading" data, a array with test values is used.
		This array contains background noise (between 9.0f and 11.0f) and the noise
		that has to be recognized and cancelled (between 40.f and 60.0f) */
	static volatile float testData[27] = {10.0f, 11.0f,  9.0f, 10.0f, 11.0f, 9.0f, 10.0f, 11.0f,  9.0f, 
										  10.0f, 60.0f, 45.0f, 10.0f, 11.0f, 9.0f, 10.0f, 55.0f, 40.0f,
										  10.0f, 11.0f,  9.0f, 10.0f, 11.0f, 9.0f, 10.0f, 11.0f,  9.0f};
	static uint8_t index = 0;
	
	newFrame.data = testData[index]; /* read from microcontroller, test data is used to simulate the "reading" */
	index++;
	if (index == 27) index = 0;

	return newFrame;
}

/*-------------------------------------------------------------------------------------------------*/
/*-------------------------------------------------------------------------------------------------*/

void vRTES_Output(void* pvParameters) {
	const char* pcTaskName = "Task Output";
	TickType_t xLastWakeTime = xTaskGetTickCount();
	RTES_Frame_t output_frame;

	/* Data used by the monitoring task */
	TickType_t xTimeTaskStarted = xTaskGetTickCount;
	TickType_t xTimeTaskEnded = xTaskGetTickCount;
	RTES_TASKID_t taskid = id_output;
	RTES_Monitoring_t monitoring = { taskid, xTimeTaskStarted, xTimeTaskEnded };

	for (;;) {
		/* Read data from the output queue, if empty: stay blocked until data is available */
		xQueueReceive(xQueue_Output, &output_frame, portMAX_DELAY);

#if (RTES_PRINT_IFNROMATION == 1 || RTES_PRINT_INFORMATION == 2)
		printf("[%d]: %s\n", xTaskGetTickCount(), pcTaskName);
#endif
		/* Record the time this period of the task has been started */
		xTimeTaskStarted = xTaskGetTickCount();

		/* Output the data to the speakers */
		outputToSpeaker(output_frame);

		/* The NoiseCancel Task has to be notified if all data of the output queue has been used,
			because it needs to know when it can enter new data to the queue */
		if (uxQueueMessagesWaiting(xQueue_Output) == 0) {
			xTaskNotifyGive(xTaskHandle_CancelNoise);
		}

		/* Record the time this period of the task has finished */
		xTimeTaskEnded = xTaskGetTickCount();

		/* Send monitoring data to the monitoring queue */
		monitoring.tickcount_start = xTimeTaskStarted;
		monitoring.tickcount_end = xTimeTaskEnded;
		if (xQueueSend(xQueue_Monitoring, &monitoring, 0) != pdPASS) {
#if (RTES_PRINT_INFORMATION != 0)
			printf("Error, xQueue_Monitoring is full!\n");
#endif
		}

		vTaskDelayUntil(&xLastWakeTime, xDelay_Output);
	}
}

void outputToSpeaker(RTES_Frame_t output_frame) {
	/* Let's pretend the data is outputted here */
}

/*-------------------------------------------------------------------------------------------------*/
/*-------------------------------------------------------------------------------------------------*/

void vRTES_Recognize(void* pvParameters) {
	const char* pcTaskName = "Task Recognize";
	TickType_t xLastWakeTime = xTaskGetTickCount();
	RTES_Buffer_t recognize_buffer;
	RTES_Frame_t frame;
	RTES_Noise_Metadata_t noiseMetaData;
#if (RTES_PRINT_INFORMATION == 1 || RTES_PRINT_INFORMATION == 2)
	uint8_t framesReceived = 0;
#endif

	/* Data used by the monitoring task */
	TickType_t xTimeTaskStarted = xTaskGetTickCount;
	TickType_t xTimeTaskEnded = xTaskGetTickCount;
	RTES_TASKID_t taskid = id_recognize;
	RTES_Monitoring_t monitoring = { taskid, xTimeTaskStarted, xTimeTaskEnded };

	/* Initialise the buffer indexes */
	recognize_buffer.indexFirstData = recognize_buffer.indexNextData = 0;

	for (;;) {
#if (RTES_PRINT_INFORMATION == 1 || RTES_PRINT_INFORMATION == 2)
		printf("[%d]: %s\n", xTaskGetTickCount(), pcTaskName);
#endif
		/* Record the time this period of the task has been started */
		xTimeTaskStarted = xTaskGetTickCount();

		/* Read and save all frames from the input queue */
		while (xQueueReceive(xQueue_Input, &frame, 0) == pdPASS) {
			recognize_buffer.data[recognize_buffer.indexNextData] = frame;
		
#if (RTES_PRINT_INFORMATION == 1 || RTES_PRINT_INFORMATION == 2)
			framesReceived++;
#endif

			/* Update next data index, set it to 0 if it exceeds the size of the array */
			recognize_buffer.indexNextData++;
			if (recognize_buffer.indexNextData == (RTES_SIZE_RECOGNIZE_BUFFER - 1)) recognize_buffer.indexNextData = 0;
		}

#if (RTES_PRINT_INFORMATION == 1 || RTES_PRINT_INFORMATION == 2)
		printf("[%d]: %s has received %d frames from the Input Queue\n", xTaskGetTickCount(), pcTaskName, framesReceived);
		framesReceived = 0;
#endif

		/* See if the noise is recognized, if not: try again the next period.
			If it's found the metadata has to be set */
		if (runRecognition(&recognize_buffer, &noiseMetaData)) {
			/* Send the metadata to the Predict Task */
			xQueueSend(xQueue_Predict, &noiseMetaData, 0);

			/* Send all data of the noise to the CancelNoise Task */
			for (int i = 0; i < RTES_FRAMES_TO_OUTPUT; i++) {
				frame = recognize_buffer.data[recognize_buffer.indexFirstData + i];
				xQueueSend(xQueue_CancelNoise, &frame, 0);
			}

			/* TODO: Prevent resending from noise data if noise found previous period(s) */
			/* Note recognize_noise always returns false right now */
		}

		/* Record the time this period of the task has finished */
		xTimeTaskEnded = xTaskGetTickCount();

		/* Send monitoring data to the monitoring queue */
		monitoring.tickcount_start = xTimeTaskStarted;
		monitoring.tickcount_end = xTimeTaskEnded;
		if (xQueueSend(xQueue_Monitoring, &monitoring, 0) != pdPASS) {
#if (RTES_PRINT_INFORMATION != 0)
			printf("Error, xQueue_Monitoring is full!\n");
#endif
		}

		vTaskDelayUntil(&xLastWakeTime, xDelay_Recognize);
	}
}

bool runRecognition(RTES_Buffer_t *buffer, RTES_Noise_Metadata_t *metadata) {
	RTES_Frame_t frame_buffer[RTES_FRAMES_TO_RECOGNIZE];

	/* Copy all frames from the buffer to the frame buffer */
	for (int i = 0; i < RTES_FRAMES_TO_RECOGNIZE; i++) {
		frame_buffer[i] = buffer->data[buffer->indexFirstData + i];
	}

	/* Run calculations on the frame_buffer */

	/* Assuming it doesn't recognize the noise */
	return false; /* Let's pretend it doesn't recognize the noise */

	/* Assuming it recognizes the noise */
	/* Set metadata, this data is gathered from the calculations on the frame_buffer */
	/* Current values for the tickcount is a placeholder */
	metadata->tickCount_firstPeak = xTaskGetTickCount() - 5;
	metadata->tickCount_secondPeak = xTaskGetTickCount();
	return true;
}

/*-------------------------------------------------------------------------------------------------*/
/*-------------------------------------------------------------------------------------------------*/

void vRTES_Predict(void* pvParameters) {
	const char* pcTaskName = "Task Predict";
	TickType_t xLastWakeTime = xTaskGetTickCount();
	RTES_Noise_Metadata_t noiseMetaData;

	/* Data used by the monitoring task */
	TickType_t xTimeTaskStarted = xTaskGetTickCount;
	TickType_t xTimeTaskEnded = xTaskGetTickCount;
	RTES_TASKID_t taskid = id_predict;
	RTES_Monitoring_t monitoring = { taskid, xTimeTaskStarted, xTimeTaskEnded };

	for (;;) {
#if (RTES_PRINT_INFORMATION == 1 || RTES_PRINT_INFORMATION == 2)
		printf("[%d]: %s\n", xTaskGetTickCount(), pcTaskName);
#endif
		/* Record the time this period of the task has been started */
		xTimeTaskStarted = xTaskGetTickCount();

		/* Read data from the predict queue, if empty: stay blocked until data is available */
		xQueueReceive(xQueue_Predict, &noiseMetaData, portMAX_DELAY);

		/* Use the new data to recalculate the next earliest predicted time */
		predict();

		/* Record the time this period of the task has finished */
		xTimeTaskEnded = xTaskGetTickCount();

		/* Send monitoring data to the monitoring queue */
		monitoring.tickcount_start = xTimeTaskStarted;
		monitoring.tickcount_end = xTimeTaskEnded;
		if (xQueueSend(xQueue_Monitoring, &monitoring, 0) != pdPASS) {
#if (RTES_PRINT_INFORMATION != 0)
			printf("Error, xQueue_Monitoring is full!\n");
#endif
		}

		/* This process can be rescheduled right away, but will shortely after remain blocked 
			until new data has been added to the queue */
		vTaskDelayUntil(&xLastWakeTime, 0);
	}
}

void predict(void) {

}

/*-------------------------------------------------------------------------------------------------*/
/*-------------------------------------------------------------------------------------------------*/

void vRTES_CancelNoise(void* pvParameters) {
	const char* pcTaskName = "Task CancelNoise";
	TickType_t xLastWakeTime = xTaskGetTickCount();
	RTES_Frame_t frame;
	RTES_Frame_t frame_buffer[RTES_FRAMES_TO_OUTPUT];

	/* Data used by the monitoring task */
	TickType_t xTimeTaskStarted = xTaskGetTickCount;
	TickType_t xTimeTaskEnded = xTaskGetTickCount;
	RTES_TASKID_t taskid = id_cancelnoise;
	RTES_Monitoring_t monitoring = { taskid, xTimeTaskStarted, xTimeTaskEnded };

	for (;;) {
#if (RTES_PRINT_INFORMATION == 1 || RTES_PRINT_INFORMATION == 2)
		printf("[%d]: %s\n", xTaskGetTickCount(), pcTaskName);
#endif
		/* Record the time this period of the task has been started */
		xTimeTaskStarted = xTaskGetTickCount();

		/* Read all data from the NoiseCancel queue */
		for (int i = 0; i < RTES_FRAMES_TO_OUTPUT; i++) {
			xQueueReceive(xQueue_CancelNoise, &frame_buffer[i], portMAX_DELAY);
		}

		/* run noise cancelling */
		runNoiseCancelling();

		/* Data can't be put into the Output queue until it's empty */
		if (uxQueueMessagesWaiting(xQueue_Output) != 0) {
			/* Block until the task is notified by the Output Task of the Output Queue being empty */
			ulTaskNotifyTake(pdTRUE, portMAX_DELAY);
		}

		/* Send the frames with noise cancellation to the Output Queue */
		for (int i = 0; i < RTES_FRAMES_TO_OUTPUT; i++) {
			xQueueSend(xQueue_Output, &frame_buffer[i], 0);
		}

		/* Record the time this period of the task has finished */
		xTimeTaskEnded = xTaskGetTickCount();

		/* Send monitoring data to the monitoring queue */
		monitoring.tickcount_start = xTimeTaskStarted;
		monitoring.tickcount_end = xTimeTaskEnded;
		if (xQueueSend(xQueue_Monitoring, &monitoring, 0) != pdPASS) {
#if (RTES_PRINT_INFORMATION != 0)
			printf("Error, xQueue_Monitoring is full!\n");
#endif
		}

		vTaskDelayUntil(&xLastWakeTime, 0);
	}
}

void runNoiseCancelling(void) {

}

/*-------------------------------------------------------------------------------------------------*/
/*-------------------------------------------------------------------------------------------------*/

void vRTES_Monitoring(void* pvParameters) {
	const char* pcTaskName = "Task Monitoring";
	TickType_t xLastWakeTime = xTaskGetTickCount();
	RTES_Monitoring_t monitoringBuffer;
	TickType_t xDelay = 0;

	for (;;) {
#if (RTES_PRINT_DEADLINEMISSED == 1 || RTES_PRINT_INFORMATION == 1 || RTES_PRINT_INFORMATION == 2)
		printf("[%d]: %s\n", xTaskGetTickCount(), pcTaskName);
#endif

		/* Read all data from the Monitoring queue */
		while (xQueueReceive(xQueue_Monitoring, &monitoringBuffer, 0) == pdPASS) {
			switch (monitoringBuffer.taskID) {
			case id_input:			xDelay = xDelay_Input; break;
			case id_output:			xDelay = xDelay_Output; break;
			case id_recognize:		xDelay = xDelay_Recognize; break;
				/* These are calculated only when the noise was recognized */
			case id_predict:		break;
			case id_cancelnoise:	break;
			default:				xDelay = 0; break;
			}

			if (monitoringBuffer.tickcount_end > (monitoringBuffer.tickcount_start + xDelay)) {
#if (RTES_PRINT_DEADLINEMISSED == 1)
				printf("task[%d] deadline missed. Start:%d, end:%d\n", monitoringBuffer.taskID, monitoringBuffer.tickcount_start, monitoringBuffer.tickcount_end);
#endif			
			}
		}
		
		vTaskDelayUntil(&xLastWakeTime, xDelay_Monitor);
	}
}