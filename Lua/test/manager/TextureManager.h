#pragma once
#include "..\base\Singleton.h"
#include "..\base\Ref.h"
#include "..\base\TextureBuffer.h"
#include <map>
#include "..\base\Image.h"

class ID3D11ShaderResourceView;


class TextureManager
{
	SingletonClase(TextureManager);
public:
	TextureManager();
	~TextureManager();
public:	
	TextureBuffer*		loadTexture(const std::string& fileName);
	void				releaseTexture(const std::string&fileName);
	void				releaseAllTexture();

	Image*				loadImage(const std::string&name);
private:
	std::map<std::string, TextureBuffer*>	m_mapTexture;
	std::map<std::string, Image*>			m_mapImage;

};
