#ifndef RTES_HEADER
#define RTES_HEADER

#include "FreeRTOS.h"
#include "task.h"
#include "queue.h"

#include <stdbool.h>

/* Used to turn on or off the printf calls. 0 turns all off, 2 only keeps the important onces */
/*	(0)	-> Print no information
	(1)	-> Print all information
	(2) -> Only print the information which is important when the noise has been recognized
*/
#define RTES_PRINT_INFORMATION		(0)

/* Print if a deadline was missed, it also prints everytime the monitoring task is called
	to verify the task works (this should be set to 0 when running on the dsPIC) */
#define RTES_PRINT_DEADLINEMISSED	(1)

#define RTES_PERIOD_INPUT_MS		(1)
#define RTES_PERIOD_OUTPUT_MS		(2)
#define RTES_PERIOD_RECOGNIZE_MS	(20)
#define RTES_TIME_TO_OUTPUT_MS		(50)
#define RTES_FRAMES_TO_OUTPUT		(RTES_TIME_TO_OUTPUT_MS / RTES_PERIOD_OUTPUT_MS)
#define RTES_PERIOD_MONITOR_MS		(75)
static const TickType_t xDelay_Monitor = pdMS_TO_TICKS(RTES_PERIOD_MONITOR_MS);

#define RTES_MONITORING_BUFFER_SIZE	 (100)

/* The input queue is used for communication between the Input Task and the Recognize Task */
QueueHandle_t xQueue_Monitoring;
static const size_t size_QueueMonitoring = RTES_MONITORING_BUFFER_SIZE;

/* Enum used to indicate which task the data is received from,
	this is only used by the Monitoring Task */
typedef enum {
	id_input = 0,
	id_output = 1,
	id_recognize = 2,
	id_predict = 3,
	id_cancelnoise = 4,
} RTES_TASKID_t;

/* Struct used by the Monitoring Task */
typedef struct {
	RTES_TASKID_t taskID;
	TickType_t tickcount_start;
	TickType_t tickcount_end;
} RTES_Monitoring_t;

/* Structure used to store the data from the microphone and speaker,
	a struct is used since more data is likely to be added */
typedef struct {
	float data;
} RTES_Frame_t;

#define RTES_SIZE_RECOGNIZE_BUFFER	(40)
#define RTES_FRAMES_TO_RECOGNIZE	(RTES_FRAMES_TO_OUTPUT)
/* Simple circular buffer used to store I/O-data, the recognize task will look at the frames 
	data[indexFirstData] until data[indexFirstData+(RTES_SIZE_RECOGNIZE_BUFFER)] */
typedef struct {
	RTES_Frame_t data[RTES_SIZE_RECOGNIZE_BUFFER];
	uint32_t indexFirstData;		/* Index of the place to read the first Frame from  */
	uint32_t indexNextData;			/* Index of the place to add the next Frame to */
} RTES_Buffer_t;

/* Structure used to store all meta data of the recognized noise,
	a struct is used because more data is likely to be added
   Note the noise usually occurs like this:
	<long delay> peak <short delay> peak <long delay>
	Which is why we need to record the time for both peaks */
typedef struct {
	TickType_t tickCount_firstPeak;
	TickType_t tickCount_secondPeak;
} RTES_Noise_Metadata_t;

/* The input queue is used for communication between the Input Task and the Recognize Task */
QueueHandle_t xQueue_Input;
static const size_t size_QueueInput = RTES_PERIOD_RECOGNIZE_MS / RTES_PERIOD_INPUT_MS;

/* The output queue is used for communication between the NoiseCancel Task and the Output Task */
QueueHandle_t xQueue_Output;
static const size_t size_QueueOutput = RTES_FRAMES_TO_OUTPUT;

/* The predict queue is used for communication between the Recognize Task and the Predict Task */
/* The prediction process is low priority, therefore it might not run for some time so it needs a larger queue */
QueueHandle_t xQueue_Predict;
static const size_t size_QueuePredict = 10;

/* The noisecancel queue is used for communication between the Recognize Task the NoiseCancel Task */
QueueHandle_t xQueue_CancelNoise;
static const size_t size_QueueCancelNoise = RTES_FRAMES_TO_OUTPUT;

static const TickType_t xDelay_Input = pdMS_TO_TICKS(RTES_PERIOD_INPUT_MS);
static const TickType_t xDelay_Output = pdMS_TO_TICKS(RTES_PERIOD_OUTPUT_MS);
static const TickType_t xDelay_Recognize = pdMS_TO_TICKS(RTES_PERIOD_RECOGNIZE_MS);

void vRTES_Input(void* pvParameters);
void vRTES_Output(void* pvParameters);
void vRTES_Recognize(void* pvParameters);
void vRTES_Predict(void* pvParameters);
void vRTES_CancelNoise(void* pvParameters);
void vRTES_Monitoring(void* pvParameters);

TaskHandle_t xTaskHandle_CancelNoise;

RTES_Frame_t takeSample(void);
void outputToSpeaker(RTES_Frame_t output_frame);
bool runRecognition(RTES_Buffer_t* buffer, RTES_Noise_Metadata_t* metadata);
void predict(void);
void runNoiseCancelling(void);

#endif /* RTES_HEADER */