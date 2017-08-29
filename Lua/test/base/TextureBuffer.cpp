#include "TextureBuffer.h"
#include "..\config.h"
#include <D3DX11tex.h>
#include "..\manager\VideoManager.h"
#include "..\..\MyTool\tool_log.h"
#include "Image.h"

#include <xercesc/parsers/XercesDOMParser.hpp>
#include <xercesc/dom/DOM.hpp>
#include <xercesc/sax/HandlerBase.hpp>
#include <xercesc/util/XMLString.hpp>
#include <xercesc/util/PlatformUtils.hpp>
#include <xercesc/dom/DOMTreeWalker.hpp>
#include <xercesc\dom\DOMElement.hpp>


TextureBuffer::TextureBuffer()
	:colorMap_(nullptr)
	,m_fileName("")
{

}

TextureBuffer::~TextureBuffer()
{
	clear();
}

TextureBuffer* TextureBuffer::create(const std::string&fileName)
{
	TextureBuffer *ret = new TextureBuffer();
	if (ret->init(fileName))
	{
		ret->autorelease();
		return ret;
	}
	return nullptr;
}

bool TextureBuffer::init(const std::string&fileName)
{
	m_fileName = fileName;
	std::string path = getAccuratePath("imageset\\");
	path.append(fileName);
	path.append(".png");
	HRESULT result = D3DX11CreateShaderResourceViewFromFile(getD3DDevice(), path.c_str(), 0, 0, &colorMap_, 0);
	if (FAILED(result))
	{
		assert(1);
		LOG_F_F("TextureBuffer", "纹理文件不纯在:%s\n", fileName.c_str());
		return false;
	}

	D3D11_SAMPLER_DESC colorMapDesc;
	ZeroMemory(&colorMapDesc, sizeof(colorMapDesc));
	colorMapDesc.AddressU = D3D11_TEXTURE_ADDRESS_WRAP;
	colorMapDesc.AddressV = D3D11_TEXTURE_ADDRESS_WRAP;
	colorMapDesc.AddressW = D3D11_TEXTURE_ADDRESS_WRAP;
	colorMapDesc.ComparisonFunc = D3D11_COMPARISON_NEVER;
	colorMapDesc.Filter = D3D11_FILTER_MIN_MAG_MIP_LINEAR;
	colorMapDesc.MaxLOD = D3D11_FLOAT32_MAX;

	result = getD3DDevice()->CreateSamplerState(&colorMapDesc, &colorMapSampler_);
	if (FAILED(result))
	{
		assert(1);
		LOG_F_F("CreateSamplerState", "纹理莫名的错误:%s\n", fileName.c_str());
		return false;
	}

	D3D11_TEXTURE2D_DESC desc;
	ID3D11Resource * texture;
	colorMap_->GetResource(&texture);
	((ID3D11Texture2D*)texture)->GetDesc(&desc);
	m_size.setSize(desc.Width, desc.Height);
	texture->Release();

	parse();
	return true;
}

bool TextureBuffer::parse()
{
	std::string path = getAccuratePath("imageset\\");
	path.append(m_fileName);
	path.append(".xml");

	// 创建解析器及进行相关设置
	xercesc::XercesDOMParser* parser = new xercesc::XercesDOMParser();
	parser->setValidationScheme(xercesc::XercesDOMParser::Val_Always);
	parser->setDoNamespaces(true); // optional

	parser->parse(path.c_str());
	xercesc::DOMDocument *document = parser->getDocument();
	if (document == nullptr)
	{
		delete(parser);
		parser = NULL;
		return false;
	}
	xercesc::DOMElement *root = document->getDocumentElement();
	if (!root)
	{
		delete(parser);
		parser = NULL;
		return false;
	}

	xercesc::DOMRefList *nodes = root->getElementsByTagName(xercesc::XMLString::transcode("Image"));
	int number = nodes->getLength();
	for (int i = 0; i < number; i++)
	{
		xercesc::DOMNode *node = nodes->item(i);
		xercesc::DOMNamedNodeMap *pAtt = node->getAttributes();

		//<Image Name = "1" XPos = "32" YPos = "-76" Width = "600" Height = "337" / >
		std::string imagename = xercesc::XMLString::transcode(pAtt->getNamedItem(xercesc::XMLString::transcode("Name"))->getTextContent());

		int x = xercesc::XMLString::parseInt(pAtt->getNamedItem(xercesc::XMLString::transcode("XPos"))->getTextContent());
		int y = xercesc::XMLString::parseInt(pAtt->getNamedItem(xercesc::XMLString::transcode("YPos"))->getTextContent());
		int w = xercesc::XMLString::parseInt(pAtt->getNamedItem(xercesc::XMLString::transcode("Width"))->getTextContent());
		int h = xercesc::XMLString::parseInt(pAtt->getNamedItem(xercesc::XMLString::transcode("Height"))->getTextContent());

		Rect2D rect(x / m_size.getWidth(), 1 - (y + h) / m_size.getHeight(), w / m_size.getWidth(), h / m_size.getHeight());

		m_mapImage[imagename] = new Image(imagename, this, rect);
	}
	delete(parser);
}

void TextureBuffer::clear()
{
	for (auto it : m_mapImage)
	{
		it.second->release();
	}
	m_mapImage.clear();
	if (colorMap_)
	{
		colorMap_->Release();
		colorMap_ = nullptr;
	}
	if (colorMapSampler_)
	{
		colorMapSampler_->Release();
		colorMapSampler_ = nullptr;
	}
}

Image* TextureBuffer::loadImage(const std::string&imageName)
{
	if (m_mapImage.find(imageName) == m_mapImage.end())
	{
		return nullptr;
	}
	return m_mapImage[imageName];
}

void TextureBuffer::PSSetView(ID3D11DeviceContext* context)
{
	getD3DContext()->PSSetShaderResources(0, 1, &colorMap_);
	getD3DContext()->PSSetSamplers(0, 1, &colorMapSampler_);
}
