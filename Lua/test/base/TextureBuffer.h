#pragma once
#include "Size.h"
#include <string>
#include "Ref.h"
#include <map>

class ID3D11DeviceContext;
class ID3D11ShaderResourceView;
class ID3D11SamplerState;
class Image;

class TextureBuffer : public Ref
{
private:
	TextureBuffer();
public:
	~TextureBuffer();
	//²»°üº¬ºó×º
	static TextureBuffer* create(const std::string& fileName);
	bool init(const std::string&fileName);
	bool parse();
	void clear();
	Image* loadImage(const std::string&imageName);

	void PSSetView(ID3D11DeviceContext* context);
private:
	ID3D11ShaderResourceView*		colorMap_;
	ID3D11SamplerState*				colorMapSampler_;
	std::string						m_fileName;
	Size							m_size;
	std::map<std::string, Image*>	m_mapImage;
};