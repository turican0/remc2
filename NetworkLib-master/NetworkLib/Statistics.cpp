#include "targetver.h"
#include "Statistics.h"
#include <iomanip>
#include <array>
#include <sstream>

namespace NetworkLib {
	Statistics::Statistics() :
		receivedMessages(0), sentMessages(0), receivedBytes(0), sentBytes(0)
	{
	}

	Statistics::Statistics(const Statistics& other) :
		receivedMessages(other.GetReceivedMessages()),
		sentMessages(other.GetSentMessages()),
		receivedBytes(other.GetReceivedBytes()),
		sentBytes(other.GetSentBytes())
	{
	}

	unsigned Statistics::GetReceivedMessages() const
	{
		return receivedMessages;
	}

	uint64_t Statistics::GetReceivedBytes() const
	{
		return receivedBytes;
	}

	unsigned Statistics::GetSentMessages() const
	{
		return sentMessages;
	}

	uint64_t Statistics::GetSentBytes() const
	{
		return sentBytes;
	}

	void Statistics::RegisterSentMessage(int32_t num_bytes)
	{
		++sentMessages;
		sentBytes.fetch_add(num_bytes);
	}

	void Statistics::RegisterReceivedMessage(int32_t messageSize)
	{
		++receivedMessages;
		receivedBytes.fetch_add(messageSize);
	}

	std::string data_size_to_string(uint64_t size)
	{
		std::array<const char*, 4> sizeStrings = { "B", "KB", "MB", "GB" };
		for (int i = sizeStrings.size() - 1; i >= 0; i--)
		{
			auto referenceSize = size_t(1 << i * 10);
			if (size < referenceSize)
				continue;
			auto scaledSize = static_cast<double>(size) / static_cast<double>(referenceSize);
			std::ostringstream oss;
			oss << std::setprecision(2) << static_cast<uint32_t>(scaledSize) << sizeStrings[i];
			return oss.str();
		}
		return std::to_string(size) + " bytes";
	}

	std::ostream& operator<<(std::ostream& os, const Statistics& stat)
	{
		os << "Sent " << stat.GetSentMessages() << " msgs ("
			<< data_size_to_string(stat.GetSentBytes()) << ") ";
		os << "Rcvd " << stat.GetReceivedMessages() << " msgs ("
			<< data_size_to_string(stat.GetReceivedBytes()) << ")";
		return os;
	}
}