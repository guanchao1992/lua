#pragma once
#include <vector>
#include <wtypes.h>
#include "..\draw\DrawNode.h"

namespace tetris
{

#define POSTOX(l)           ((short)(((DWORD_PTR)(l)) & 0xffff))
#define POSTOY(l)           ((short)((((DWORD_PTR)(l)) >> 16) & 0xffff))

	enum BoxType
	{
		Box_Null = -1,
		Box_1,
		Box_2,
		Box_3,
		Box_4,
		Box_5,
		Box_6,
		Box_7,
		Box_Max,
	};
	struct BoxAtt
	{
		BoxAtt(BoxType type) :m_type(type) {};
		BoxAtt() {};
		BoxType m_type;
		std::string getBoxShow() 
		{
			switch (m_type)
			{
			case Box_1:
				return std::string("redbox");
			case Box_2:
				return std::string("bluebox");
			case Box_3:
				return std::string("greenbox");
			case Box_4:
				return std::string("orangebox");
			case Box_5:
				return std::string("purplebox");
			case Box_6:
				return std::string("yellowbox");
			case Box_7:
				return std::string("bluebox");
			default:
				return std::string("");
			}
		}
	};

	enum BlockType
	{
		BlockType_1,
		BlockType_2,
		BlockType_3,
		BlockType_4,
		BlockType_5,
		BlockType_6,
		BlockType_7,
		BlockType_Max,
	};
	typedef unsigned long BlockBoxPos;
	class Block : public DrawNode
	{
	protected:
		Block(BlockType type, BoxType boxType, const Size&boxSize);
	public:
		static Block*create(BlockType type, BoxType boxType, const Size&boxSize);
		~Block();
		void		init();
		void		reset();
		UINT		getBoxCount() const;
		BlockBoxPos getBlockBoxPos(UINT index) const;
		BoxAtt		getBoxAtt() const { return m_boxAtt; }

		void		rotateLeft();
		void		rotateRight();

		virtual void		redraw();
	private:
		std::vector<BlockBoxPos>	m_vecBox;
		BlockType					m_blockType;
		BoxAtt						m_boxAtt;
		Size						m_boxSize;
	public:
	};
}
