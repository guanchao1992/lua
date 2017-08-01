#include "TextureManager.h"
#include "..\base\NodeList.h"
#include <D3DX11tex.h>
#include "..\..\MyTool\tool_log.h"
#include "VideoManager.h"

#include <xercesc/parsers/XercesDOMParser.hpp>
#include <xercesc/dom/DOM.hpp>
#include <xercesc/sax/HandlerBase.hpp>
#include <xercesc/util/XMLString.hpp>
#include <xercesc/util/PlatformUtils.hpp>
#include <xercesc/dom/DOMTreeWalker.hpp>
#include <xercesc\dom\DOMElement.hpp>

//using namespace xercesc_3_1;

//#define xercesc xercesc_3_1;

SingletonClaseCpp(TextureManager);

TextureManager::TextureManager()
{
}


TextureManager::~TextureManager()
{
}

bool TextureManager::init()
{
	parseImageset(getAccuratePath("imageset\\image.imageset"));
	return true;
}

bool TextureManager::parseImageset(const std::string&dir)
{
	m_mapImageset.clear();
	try
	{
		// 创建解析器及进行相关设置
		xercesc::XercesDOMParser* parser = new xercesc::XercesDOMParser();
		parser->setValidationScheme(xercesc::XercesDOMParser::Val_Always);
		parser->setDoNamespaces(true); // optional

		parser->parse(dir.c_str());
		// 获取文档对象
		xercesc::DOMDocument *document = parser->getDocument();
		// 这是文档的根节点
		xercesc::DOMElement *root = document->getDocumentElement();
		// 判断根节点是否为空，最好是判断一下，否则遇到空的程序就崩溃了。
		if (!root)
		{
			delete(parser);
			parser = NULL;
			return false;
		}

		// 获取所有名称为node的节点
		xercesc::DOMNodeList *nodes = root->getElementsByTagName(xercesc::XMLString::transcode("Image"));
		int number = nodes->getLength();
		// 遍历所有node节点的信息
		for (int i = 0; i < number; i++)
		{
			xercesc::DOMNode *node = nodes->item(i);
			// 获取节点属性值
			xercesc::DOMNamedNodeMap *pAtt = node->getAttributes();

			std::string imagename = xercesc::XMLString::transcode(pAtt->getNamedItem(xercesc::XMLString::transcode("Name"))->getTextContent());
			std::string imageset = xercesc::XMLString::transcode(pAtt->getNamedItem(xercesc::XMLString::transcode("Imageset"))->getTextContent());
			LOG_D("读取图片纹理：image Name:%s Imageset:%s\n", imagename.c_str(), imageset.c_str());
			//顺便检查一下是否有重名
			if (m_mapImageset.find(imagename) != m_mapImageset.end())
			{
				LOG_E_F("parseImageset","图片名有重复的。图片所在位置(name:imageset) %s:%s %s:%s\n", imagename.c_str(), imageset.c_str(), imagename.c_str(), m_mapImageset[imagename].c_str());
			}
			m_mapImageset[imagename] = imageset;
		}
		delete(parser);
	}
	catch (const xercesc_3_1::XMLException& toCatch) {
		char* message = xercesc_3_1::XMLString::transcode(toCatch.getMessage());
		LOG_E_F("parseImageset", "Error:%s\n", message);
		xercesc_3_1::XMLString::release(&message);
		return false;
	}
	return true;
}

TextureBuffer* TextureManager::loadTexture(const std::string&fileName)
{
	auto it = m_mapTexture.find(fileName);
	if (it == m_mapTexture.end())
	{
		TextureBuffer * buffer = TextureBuffer::create(fileName);
		if (buffer == nullptr)
		{
			return nullptr;
		}
		m_mapTexture[fileName] = buffer;
		buffer->retain();
		return buffer;
	}
	return it->second;
}

void TextureManager::releaseTexture(const std::string&fileName)
{
	auto it = m_mapTexture.find(fileName);
	if (it == m_mapTexture.end())
	{
		LOG_E("释放的纹理不存在：%s", fileName.c_str());
		assert(1);
		return;
	}
	it->second->release();
	m_mapTexture.erase(it);
}

void TextureManager::releaseAllTexture()
{
	for (auto it:m_mapTexture)
	{
		it.second->release();
	}
	m_mapTexture.clear();
}

Image* TextureManager::loadImage(const std::string&name)
{
	if (m_mapImageset.find(name) == m_mapImageset.end())
	{
		LOG_E_F("loadImage", "图片不存在:%s", name.c_str());
		return nullptr;
	}
	TextureBuffer* tb = loadTexture(m_mapImageset[name]);
	return tb->loadImage(name);
}
