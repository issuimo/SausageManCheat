#pragma once
#include <pch.h>
#include "PipeMessage.h"

#include "Base/PipeTransfer.h"

enum class NetIODirection
{
	Send, Receive
};

class PipePacketData : public PipeMessage
{
public:
	using PipeMessage::PipeMessage;

	bool manipulationEnabled;

	NetIODirection direction;
	int16_t messageID;
	std::vector<byte> head;
	std::vector<byte> content;

	// Inherited via PipeSerializedObject
	void Write(PipeTransfer* transfer);
	void Read(PipeTransfer* transfer);
};
