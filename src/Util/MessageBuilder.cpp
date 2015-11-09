/*
 * MessageBuilder.cpp
 *
 *  Created on: Sep 21, 2015
 *      Author: quyen
 */

#include <stdarg.h>
#include <iostream>

#include <string.h>
#include "MessageBuilder.h"
#include "../CommonDefine.h"

MessageBuilder::MessageBuilder() {
	// TODO Auto-generated constructor stub

}

MessageBuilder::~MessageBuilder() {
	// TODO Auto-generated destructor stub
}

void MessageBuilder::buildMessage(unsigned char type, unsigned char* output, int nArgs, ...)
{
	if(nArgs <= 0)
	{
		std::cout << "Number of arguments too small...." << std::endl;
		return;
	}
	else if(nArgs > 12)
	{
		std::cout << "Number of arguments too large...." << std::endl;
		return;
	}
	va_list args;
	va_start(args, nArgs);

	switch(type)
	{
	case MSG_TYPE_FIREALARM:
	{

		unsigned char srcId = va_arg(args, unsigned char);
		buildFireAlarm(output, srcId);
		break;
	}
	case MSG_TYPE_INTRUDER:
	{

		unsigned char srcId = va_arg(args, unsigned char);
		buildIntruderAlarm(output, srcId);
		break;
	}
	case MSG_TYPE_VIDEO:
	{

		unsigned char srcId = va_arg(args, unsigned char);
		unsigned char cols = va_arg(args, unsigned char);
		unsigned char rows = va_arg(args, unsigned char);
		unsigned char depth = va_arg(args, unsigned char);
		unsigned char idx = va_arg(args, unsigned char);
		unsigned char total = va_arg(args, unsigned char);
		unsigned int size = va_arg(args, unsigned int); // in bytes
		unsigned char* buffer = va_arg(args, unsigned char*);
		buildVideo(output, srcId, cols, rows, depth, idx, total, size, buffer);
		break;
	}
	default:
		std::cout << "No message support!" << std::endl;
		break;

	}

}
/**
 * Fire_Alarm_Msg:
 * Flag		: FF FF AA 55
 * MsgType	: 0x01
 * SrcId	: 1 byte (0x01 -> 0x04)
 */
void MessageBuilder::buildFireAlarm(unsigned char* output, unsigned char srcId)
{
	output[0] = output[1] = 0xFF;
	output[2] = 0xAA;
	output[3] = 0x55;
	output[4] = 0x00;
	output[5] = srcId;
}

/**
 * Intruder_Alarm_Msg:
 * Flag		: FF FF AA 55
 * MsgType	: 0x02
 * SrcId	: 1 byte (0x01 -> 0x04)
 */
void MessageBuilder::buildIntruderAlarm(unsigned char* output, unsigned char srcId)
{
	output[0] = output[1] = 0xFF;
	output[2] = 0xAA;
	output[3] = 0x55;
	output[4] = 0x01;
	output[5] = srcId;

}

/**
 * Fire_Video_Msg:
 * Flag		: FF FF AA 55
 * MsgType	: 0x03
 * SrcId	: 1 byte (0x01 -> 0x04)
 * Cols		: 1 byte - number of columns in image
 * Rows		: 1 byte - number of rows in image
 * Depth	: 1 byte - image's depth
 * Idx		: 1 byte - index of packet in image, if image is too large
 * Total	: 1 byte - total number of packets in image
 * Size		: 1 long value - size in byte of data buffer
 * Buffer	: unsigned char buffer to send
 */
void MessageBuilder::buildVideo(unsigned char* output, unsigned char srcId,
		unsigned char cols, unsigned char rows, unsigned char depth,
		unsigned char idx, unsigned char total, unsigned int size,
		unsigned char* buffer)
{
	output[0] = output[1] = 0xFF;
	output[2] = 0xAA;
	output[3] = 0x55;
	output[4] = 0x02;
	output[5] = srcId;
	output[6] = cols;
	output[7] = rows;
	output[8] = depth;
	output[9] = idx;
	output[10] = total;
	output[11] = (unsigned char)(0xFF & (size >> 24));
	output[12] = (unsigned char)(0xFF & (size >> 16));
	output[13] = (unsigned char)(0xFF & (size >> 8));
	output[14] = (unsigned char)(0xFF & (size >> 0));
	memcpy(&(output[15]), buffer, size * sizeof(unsigned char));
}
