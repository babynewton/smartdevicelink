/*
 * simplectrl.c
 * This file is part of OsciPrime
 *
 * Copyright (C) 2011 - Manuel Di Cerbo
 *
 * OsciPrime is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 *
 * OsciPrime is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with OsciPrime; if not, write to the Free Software
 * Foundation, Inc., 51 Franklin St, Fifth Floor,
 * Boston, MA  02110-1301  USA
 */

// Source: http://android.serverbox.ch/?p=262


#include <stdio.h>
#include <libusb.h>
#include <string.h>
#include <unistd.h>
#include <stdlib.h>
#include <assert.h>

#define IN 0x81
#define OUT 0x02

#define ACCESSORY_VID 0x18d1
#define ACCESSORY_PID 0x2D01
#define ACCESSORY_PID_ALT 0x2D00

#define LEN 2

#define ERR_DEVICE_NOT_OPEN -2

/*
 If you are on Ubuntu you will require libusb as well as the headers...
 We installed the headers with "apt-get source libusb"
 gcc simplectrl.c -I/usr/include/ -o simplectrl -lusb-1.0 -I/usr/include/ -I/usr/include/libusb-1.0
 
 Tested for Nexus S with Gingerbread 2.3.4
 */

typedef unsigned char byte;

static int mainPhase();
static int mainPhaseBenchmark();
static int init(const uint16_t vid, const uint16_t pid);
static int deInit(void);
static void error(int code);
static void status(int code);
static int setupAccessory(
                          const char* manufacturer,
                          const char* modelName,
                          const char* description,
                          const char* version,
                          const char* uri,
                          const char* serialNumber);
static int connectToAccessory();

//static
static struct libusb_device_handle* handle;
//static char stop;
//static char success = 0;

int main (int argc, char *argv[]){
    if (argc < 3) {
        printf("%s takes the following parameters in order:\n"
               "* [mandatory] USB VID and\n"
               "* [mandatory] USB PID of the device to connect to\n"
               "* [optional] -b flag to run throughput benchmark test (use with "
               "UsbTest android app)\n"
               "e.g.:\n%s 0x1234 0xFEDC\n", argv[0], argv[0]);
        return -1;
    }
    
    long vid = strtol(argv[1], NULL, 0);
    long pid = strtol(argv[2], NULL, 0);
    int runBenchmark = (argc >= 4) && (strcmp(argv[3], "-b") == 0);
    
	int ret = init(vid, pid);
    switch (ret) {
        case ERR_DEVICE_NOT_OPEN: {
            if (connectToAccessory() < 0) {
                fprintf(stdout, "Error connecting to accessory\n");
                deInit();
                return -1;
            }
            break;
        }
            
        case 0: {
            //doTransfer();
            if(setupAccessory(
                              "Ford",
                              "HMI",
                              "Human machine interface",
                              "1.0",
                              "http://www.ford.com",
                              "N000000") < 0){
                fprintf(stdout, "Error setting up accessory\n");
                deInit();
                return -1;
            };
            break;
        }
            
        default:
            // other error
            return -1;
    }
    
    ret = runBenchmark ? mainPhaseBenchmark() : mainPhase();
	if(ret < 0){
		fprintf(stdout, "Error during main phase\n");
		deInit();
		return -1;
	}
	deInit();
	fprintf(stdout, "Done, no errors\n");
	return 0;
}

static void printCharArray(unsigned char *buf, unsigned int len) {
    // http://stackoverflow.com/questions/5189071/print-part-of-char-array/5191818#5191818
    fprintf(stdout, "%.*s\n", len, buf);
}

static void intToByteArray(int i, byte **outArray, int *outLen) {
    *outLen = 4;
    byte *buf = (byte *)malloc(*outLen);
    buf[0] = (i >> 24) & 0xFF;
    buf[1] = (i >> 16) & 0xFF;
    buf[2] = (i >> 8) & 0xFF;
    buf[3] = i & 0xFF;
    *outArray = buf;
}

static int mainPhase() {
    unsigned char buffer[4096];
	int response = 0;
	int transferred;
    
    while (1) {
        // reading data
        response = libusb_bulk_transfer(handle,IN,buffer,sizeof(buffer), &transferred,0);
        fprintf(stdout, "Received %d bytes\n", transferred);
        if(response < 0){error(response);return -1;}
        printCharArray(buffer, transferred);
        int dataLength = transferred;
        
        const int USEC_IN_MSEC = 1000;
        usleep(10 * USEC_IN_MSEC);

        // writing data
        response = libusb_bulk_transfer(handle, OUT, buffer, dataLength, &transferred, 0);
        if (response < 0) {
            error(response);
            return -1;
        }
        printf("Sent %d bytes\n", transferred);
    }
    
    return 0;
}

static int mainPhaseBenchmark(){
    const int BUFFER_LENGTH = 4096;
	unsigned char buffer[BUFFER_LENGTH];
	int response = 0;
	int transferred;
    int blocksReceived = 0;
    const int reportPeriod = 1000;
    const int blocksExpected = 10000;
    struct timeval startTime;
    
    while (blocksReceived < blocksExpected) {
        // reading data
        response = libusb_bulk_transfer(handle,IN,buffer,sizeof(buffer), &transferred,0);
//        fprintf(stdout, "Received %d bytes\n", transferred);
        if(response < 0){error(response);return -1;}
        
        if (blocksReceived == 0) {
            printf("Benchmark start\n");
            gettimeofday(&startTime, NULL);
        }
        
        ++blocksReceived;
        if (blocksReceived % reportPeriod == 0) {
            printf("tick\n");
            fflush(stdout);
        }
        int dataLength = transferred;
        
        // writing data
        response = libusb_bulk_transfer(handle, OUT, buffer, dataLength, &transferred, 0);
        if (response < 0) {
            error(response);
            return -1;
        }
        assert(transferred == dataLength);
//        printf("Sent %d bytes\n", transferred);
    }
    
    struct timeval finishTime;
    gettimeofday(&finishTime, NULL);
    unsigned long runTime = ((finishTime.tv_sec - startTime.tv_sec) * 1000000 +
                             (finishTime.tv_usec - startTime.tv_usec)) / 1000;
    unsigned int totalBytes = blocksReceived * BUFFER_LENGTH * 2;
    double avgSpeed = totalBytes / (runTime / 1000);
    printf("Benchmark finish; run time = %lu ms. for %u bytes, "
           "which is about %.2f B/s\n", runTime, totalBytes,
           avgSpeed);
    
    return 0;
}


static int init(const uint16_t vid, const uint16_t pid){
	int response = libusb_init(NULL);
    if (response != 0) {
        error(response);
        return -1;
    }
    printf("Using VID %u and PID %u\n", vid, pid);
	if((handle = libusb_open_device_with_vid_pid(NULL, vid, pid)) == NULL){
		fprintf(stdout, "Problem acquiring handle for device\n");
		return ERR_DEVICE_NOT_OPEN;
	}
	libusb_claim_interface(handle, 0);
	return 0;
}

static int connectToAccessory() {
    fprintf(stdout, "Waiting for accessory\n");
    int tries = 10;
	for(;;){//attempt to connect to new PID, if that doesn't work try ACCESSORY_PID_ALT
        fprintf(stdout, "%d ", tries);
	fflush(stdout);
		tries--;
		if((handle = libusb_open_device_with_vid_pid(NULL, ACCESSORY_VID, ACCESSORY_PID)) == NULL){
			if(tries < 0){
                fprintf(stdout, "\nProblem acquiring handle for accessory\n");
				return -1;
			}
		}else{
			break;
		}
		sleep(1);
	}
	libusb_claim_interface(handle, 0);
	fprintf(stdout, "\nInterface claimed, ready to transfer data\n");
    return 0;
}

static int deInit(){
	//TODO free all transfers individually...
	//if(ctrlTransfer != NULL)
	//	libusb_free_transfer(ctrlTransfer);
	if(handle != NULL)
		libusb_release_interface (handle, 0);
	libusb_exit(NULL);
	return 0;
}

static int setupAccessory(
                          const char* manufacturer,
                          const char* modelName,
                          const char* description,
                          const char* version,
                          const char* uri,
                          const char* serialNumber){
    
	unsigned char ioBuffer[2];
	int devVersion;
	int response;
    
	response = libusb_control_transfer(
                                       handle, //handle
                                       0xC0, //bmRequestType
                                       51, //bRequest
                                       0, //wValue
                                       0, //wIndex
                                       ioBuffer, //data
                                       2, //wLength
                                       0 //timeout
                                       );
    
	if(response < 0){error(response);return-1;}
    
	devVersion = ioBuffer[1] << 8 | ioBuffer[0];
	fprintf(stdout,"Verion Code Device: %d\n", devVersion);
	
	usleep(1000);//sometimes hangs on the next transfer :(
    
	response = libusb_control_transfer(handle,0x40,52,0,0,(byte*)manufacturer,strlen(manufacturer),0);
	if(response < 0){error(response);return -1;}
	response = libusb_control_transfer(handle,0x40,52,0,1,(byte*)modelName,strlen(modelName)+1,0);
	if(response < 0){error(response);return -1;}
	response = libusb_control_transfer(handle,0x40,52,0,2,(byte*)description,strlen(description)+1,0);
	if(response < 0){error(response);return -1;}
	response = libusb_control_transfer(handle,0x40,52,0,3,(byte*)version,strlen(version)+1,0);
	if(response < 0){error(response);return -1;}
	response = libusb_control_transfer(handle,0x40,52,0,4,(byte*)uri,strlen(uri)+1,0);
	if(response < 0){error(response);return -1;}
	response = libusb_control_transfer(handle,0x40,52,0,5,(byte*)serialNumber,strlen(serialNumber)+1,0);
	if(response < 0){error(response);return -1;}
    
	fprintf(stdout,"Accessory Identification %d sent\n", devVersion);
    
	response = libusb_control_transfer(handle,0x40,53,0,0,NULL,0,0);
	if(response < 0){error(response);return -1;}
    
	fprintf(stdout,"Attempted to put device into accessory mode %d\n", devVersion);
    
	if(handle != NULL) {
		libusb_release_interface (handle, 0);
        libusb_close(handle);
    }
    
    return connectToAccessory();
}

static void error(int code){
	fprintf(stdout,"\n");
	switch(code){
        case LIBUSB_ERROR_IO:
            fprintf(stdout,"Error: LIBUSB_ERROR_IO\nInput/output error.\n");
            break;
        case LIBUSB_ERROR_INVALID_PARAM:
            fprintf(stdout,"Error: LIBUSB_ERROR_INVALID_PARAM\nInvalid parameter.\n");
            break;
        case LIBUSB_ERROR_ACCESS:
            fprintf(stdout,"Error: LIBUSB_ERROR_ACCESS\nAccess denied (insufficient permissions).\n");
            break;
        case LIBUSB_ERROR_NO_DEVICE:
            fprintf(stdout,"Error: LIBUSB_ERROR_NO_DEVICE\nNo such device (it may have been disconnected).\n");
            break;
        case LIBUSB_ERROR_NOT_FOUND:
            fprintf(stdout,"Error: LIBUSB_ERROR_NOT_FOUND\nEntity not found.\n");
            break;
        case LIBUSB_ERROR_BUSY:
            fprintf(stdout,"Error: LIBUSB_ERROR_BUSY\nResource busy.\n");
            break;
        case LIBUSB_ERROR_TIMEOUT:
            fprintf(stdout,"Error: LIBUSB_ERROR_TIMEOUT\nOperation timed out.\n");
            break;
        case LIBUSB_ERROR_OVERFLOW:
            fprintf(stdout,"Error: LIBUSB_ERROR_OVERFLOW\nOverflow.\n");
            break;
        case LIBUSB_ERROR_PIPE:
            fprintf(stdout,"Error: LIBUSB_ERROR_PIPE\nPipe error.\n");
            break;
        case LIBUSB_ERROR_INTERRUPTED:
            fprintf(stdout,"Error:LIBUSB_ERROR_INTERRUPTED\nSystem call interrupted (perhaps due to signal).\n");
            break;
        case LIBUSB_ERROR_NO_MEM:
            fprintf(stdout,"Error: LIBUSB_ERROR_NO_MEM\nInsufficient memory.\n");
            break;
        case LIBUSB_ERROR_NOT_SUPPORTED:
            fprintf(stdout,"Error: LIBUSB_ERROR_NOT_SUPPORTED\nOperation not supported or unimplemented on this platform.\n");
            break;
        case LIBUSB_ERROR_OTHER:
            fprintf(stdout,"Error: LIBUSB_ERROR_OTHER\nOther error.\n");
            break;
        default:
            fprintf(stdout, "Error: unkown error\n");
	}
}

static void status(int code){
	fprintf(stdout,"\n");
	switch(code){
		case LIBUSB_TRANSFER_COMPLETED:
			fprintf(stdout,"Success: LIBUSB_TRANSFER_COMPLETED\nTransfer completed.\n");
			break;
		case LIBUSB_TRANSFER_ERROR:
			fprintf(stdout,"Error: LIBUSB_TRANSFER_ERROR\nTransfer failed.\n");
			break;
		case LIBUSB_TRANSFER_TIMED_OUT:
			fprintf(stdout,"Error: LIBUSB_TRANSFER_TIMED_OUT\nTransfer timed out.\n");
			break;
		case LIBUSB_TRANSFER_CANCELLED:
			fprintf(stdout,"Error: LIBUSB_TRANSFER_CANCELLED\nTransfer was cancelled.\n");
			break;
		case LIBUSB_TRANSFER_STALL:
			fprintf(stdout,"Error: LIBUSB_TRANSFER_STALL\nFor bulk/interrupt endpoints: halt condition detected (endpoint stalled).\nFor control endpoints: control request not supported.\n");
			break;
		case LIBUSB_TRANSFER_NO_DEVICE:
			fprintf(stdout,"Error: LIBUSB_TRANSFER_NO_DEVICE\nDevice was disconnected.\n");
			break;
		case LIBUSB_TRANSFER_OVERFLOW:
			fprintf(stdout,"Error: LIBUSB_TRANSFER_OVERFLOW\nDevice sent more data than requested.\n");
			break;
		default:
			fprintf(stdout,"Error: unknown error\nTry again(?)\n");
			break;
	}
}
