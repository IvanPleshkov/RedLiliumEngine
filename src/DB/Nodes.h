#pragma once

#include <Core/Common.h>

namespace RED_LILIUM_NAMESPACE
{
namespace db
{

struct NodeId
{
	u64 Host;
	u64 Id;
};

enum class NodeType : u8
{
	Null			= 0,
	Empty			= 1,

	// containers
	Object			= 10,
	List			= 11,
	Array			= 12,

	// db data references
	User			= 20,
	Reference		= 21,

	// value types
	Bool			= 30,
	Int				= 31,
	Double			= 32,
	BigNumber		= 33,
	Date			= 34,
	Time			= 35,
	DateTime		= 36,
	String			= 37,
	BinaryData		= 38,
};

class NodeNull
{
	NodeType GetType() { return NodeType::Null; }
};

class NodeBase
{
	NodeType GetType() { return NodeType::Null; }
};

class NodeEmpty : public NodeBase
{
	NodeType GetType() { return NodeType::Empty; }
};

class NodeObject : public NodeBase
{
	NodeType GetType() { return NodeType::Object; }
};

class NodeList : public NodeBase
{
	NodeType GetType() { return NodeType::List; }
};

class NodeArray : public NodeBase
{
	NodeType GetType() { return NodeType::Array; }
};

class NodeUser : public NodeBase
{
	NodeType GetType() { return NodeType::User; }
};

class NodeReference : public NodeBase
{
	NodeType GetType() { return NodeType::Reference; }
};

class NodeBool : public NodeBase
{
	NodeType GetType() { return NodeType::Bool; }
};

class NodeInt : public NodeBase
{
	NodeType GetType() { return NodeType::Int; }
};

class NodeDouble : public NodeBase
{
	NodeType GetType() { return NodeType::Double; }
};

class NodeBigNumber : public NodeBase
{
	NodeType GetType() { return NodeType::BigNumber; }
};

class NodeDate : public NodeBase
{
	NodeType GetType() { return NodeType::Date; }
};

class NodeTime : public NodeBase
{
	NodeType GetType() { return NodeType::Time; }
};

class NodeDateTime : public NodeBase
{
	NodeType GetType() { return NodeType::DateTime; }
};

class NodeString : public NodeBase
{
	NodeType GetType() { return NodeType::String; }
};

class NodeBinaryData : public NodeBase
{
	NodeType GetType() { return NodeType::BinaryData; }
};

using Node = std::variant<
	NodeNull,
	NodeEmpty,
	NodeObject,
	NodeList,
	NodeArray,
	NodeUser,
	NodeReference,
	NodeBool,
	NodeInt,
	NodeDouble,
	NodeBigNumber,
	NodeDate,
	NodeTime,
	NodeDateTime,
	NodeString,
	NodeBinaryData>;

} // namespace db
} // namespace RED_LILIUM_NAMESPACE
