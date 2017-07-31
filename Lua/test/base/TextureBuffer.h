#pragma once
#include "Size.h"
#include <string>
#include "Ref.h"

class ID3D11ShaderResourceView;

class TextureBuffer : public Ref
{
public:
	//��������׺
	static TextureBuffer* create(const std::string& fileName);
	bool init(const std::string&fileName);
private:
	ID3D11ShaderResourceView* colorMap_;
	std::string m_fileName;
};