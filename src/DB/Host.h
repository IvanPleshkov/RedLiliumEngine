#pragma once

#include <Core/Common.h>

namespace RED_LILIUM_NAMESPACE
{
namespace db
{

class IStream;
class Host;

using TransactionId = u64;
using CellId = u64;
using TriggerId = u64;

using CellData = std::vector<char>;

struct CellTrigger
{
	virtual u64 Id() const;
	virtual void OnDelete(u64 id);
	virtual void OnUpdate(u64 id);
};

class HostTransaction : public RedLiliumObject
{
public:
	HostTransaction();
	~HostTransaction();

	u64 Add(const std::vector<char>& data);
	void Replace(u64 id, const std::vector<char>& data);
	void Remove(u64 id);
	void AddTrigger();
	void RemoveTrigger();
};

class Host : public RedLiliumObject
{
public:	
	Host(ptr<IStream> stream);

	std::vector<char> ReadData(u64 Id, TransactionId revision) const;
	TransactionId ActualRevision() const;

	uptr<HostTransaction> StartTransaction();

private:
	ptr<IStream> m_stream;
	std::atomic<bool> m_isTransactionNow;
	std::atomic<TransactionId> m_actualTransaction;
};

} // namespace db
} // namespace RED_LILIUM_NAMESPACE
