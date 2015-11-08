/*
 * MessageBuilder.h
 *
 *  Created on: Sep 21, 2015
 *      Author: quyen
 */

#ifndef SRC_UTIL_MESSAGEBUILDER_H_
#define SRC_UTIL_MESSAGEBUILDER_H_

class MessageBuilder {
public:
	MessageBuilder();
	virtual ~MessageBuilder();

	static void buildMessage(unsigned char type, unsigned char* output, int nArgs, ...);
private:
	static void buildFireAlarm(unsigned char* output, unsigned char srcId);
	static void buildIntruderAlarm(unsigned char* output, unsigned char srcId);
	static void buildVideo(unsigned char* output, unsigned char srcId,
			unsigned char cols, unsigned char rows, unsigned char depth,
			unsigned char idx, unsigned char total, unsigned int size,
			unsigned char* buffer);
};

#endif /* SRC_UTIL_MESSAGEBUILDER_H_ */
