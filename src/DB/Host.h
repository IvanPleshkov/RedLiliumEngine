#pragma once

#include <Core/Common.h>

namespace RED_LILIUM_NAMESPACE
{
namespace db
{

class IStream;

using TransactionId = u64;

class Host : public RedLiliumObject
{
public:	
	Host(ptr<IStream> stream);

	std::vector<char> ReadData(u64 Id, TransactionId revision);

	void StartTransaction();
	u64 Add(const std::vector<char>& data);
	void Replace(u64 id, const std::vector<char>& data);
	void Remove(u64 id);
	TransactionId EndTransaction(const std::string& revisionComment);

private:
	ptr<IStream> m_stream;
	std::atomic<bool> m_isTransactionNow;
	std::atomic<TransactionId> m_actualTransaction;
};

} // namespace db
} // namespace RED_LILIUM_NAMESPACE
