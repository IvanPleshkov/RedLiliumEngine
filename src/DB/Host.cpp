#include "pch.h"

#include "Host.h"

using namespace RED_LILIUM_NAMESPACE;
using namespace RED_LILIUM_NAMESPACE::db;

Host::Host(ptr<IStream> stream)
{}

std::vector<char> Host::ReadData(u64 Id, TransactionId revision)
{}

void Host::StartTransaction()
{}

u64 Host::Add(const std::vector<char>& data)
{}

void Host::Replace(u64 id, const std::vector<char>& data)
{}

void Host::Remove(u64 id)
{}

TransactionId Host::EndTransaction(const std::string& revisionComment)
{}
