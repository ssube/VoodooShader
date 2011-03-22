#include "Xml.hpp"

namespace VoodooShader
{
    namespace PugiXml
    {
        int API_ClassCount()
        {
            return 3;
        }

        const char * API_ClassInfo( _In_ int number )
        {
            switch ( number )
            {
            case 0:
                return "Parser";
            case 1:
                return "Document";
            case 2:
                return "Node";
            default:
                return NULL;
            }
        }

        IObject * API_ClassCreate( _In_ int number, _In_ Core * core )
        {
            if ( number == 0 )
            {
                return new PugiXml::XmlParser(core);
            } else {
                return NULL;
            }
        }

        XmlParser::XmlParser(Core * core)
        {
            
        }

        XmlParser::~XmlParser()
        {

        }

        void XmlParser::DestroyObject()
        {
            delete this;
        }

        int XmlParser::GetObjectID()
        {
            return 0;
        }

        const char * XmlParser::GetObjectName()
        {
            return "Parser";
        }

        IDocument * XmlParser::LoadDocument( _In_ String filename )
        {
            pugi::xml_document doc;

            if ( doc.load(filename.c_str()) )
            {
                return new XmlDocument(&doc);
            } else {
                return NULL;
            }
        }

        XmlDocument::XmlDocument(pugi::xml_document * doc)
        {
            mRootNode = new XmlNode(doc->root());
        }

        XmlDocument::~XmlDocument()
        {
            delete mRootNode;
        }

        void XmlDocument::DestroyObject()
        {
            delete this;
        }

        int XmlDocument::GetObjectID()
        {
            return 0;
        }

        const char * XmlDocument::GetObjectName()
        {
            return "Document";
        }

        INode * XmlDocument::GetRoot()
        {
            return mRootNode;
        }

        XmlNode::XmlNode(pugi::xml_node node)
        {
            using namespace pugi;

            if ( !node ) return;

            // Get all attributes
            mName = node.name();
            mValue = node.value();

            for ( xml_attribute attr = node.first_attribute(); attr; attr = attr.next_attribute() )
            {
                this->mAttributes[attr.name()] = attr.value();
            }

            for ( xml_node child = node.first_child(); child; child = child.next_sibling() )
            {
                XmlNode * childNode = new XmlNode(child);
                String name = child.name();
                this->mChildren.insert(std::pair<String,INode*>(name, childNode));
            }
        }

        XmlNode::~XmlNode()
        {
            for ( NodeMap::iterator iter = mChildren.begin(); iter != mChildren.end(); ++iter )
            {
                delete iter->second;
            }
        }

        void XmlNode::DestroyObject()
        {
            delete this;
        }

        int XmlNode::GetObjectID()
        {
            return 0;
        }

        const char * XmlNode::GetObjectName()
        {
            return "Node";
        }

        String XmlNode::GetName()
        {
            return mName;
        }

        String XmlNode::GetValue()
        {
            return mValue;
        }

        NodeMap::iterator XmlNode::GetFirstChild()
        {
            return mChildren.begin();
        }

        bool XmlNode::GetNextChild(NodeMap::iterator & iter)
        {
            if ( ++iter != mChildren.end() )
            {
                return true;
            } else {
                return false;
            }
        }

        NodeMap XmlNode::GetChildren(String regex)
        {
            using namespace std;

            tr1::regex query(regex);
            NodeMap nodes;

            for_each
            (
                mChildren.begin(),
                mChildren.end(),
                [&](pair<String, INode*> child)
                {
                    if ( regex_match(child.first, query) )
                    {
                        nodes.insert(child);
                    }
                }
            );

            return nodes;
        }

        INode * XmlNode::GetSingleChild(String regex)
        {
            using namespace std;

            tr1::regex query(regex);

            for ( NodeMap::iterator iter = mChildren.begin(); iter != mChildren.end(); ++iter )
            {
                if ( regex_match(iter->first, query) )
                {
                    return iter->second;
                }
            }

            return NULL;
        }

        AttributeMap::iterator XmlNode::GetFirstAttribute()
        {
            return mAttributes.begin();
        }

        bool XmlNode::GetNextAttribute(AttributeMap::iterator & iter)
        {
            if ( ++iter != mAttributes.end() )
            {
                return true;
            } else {
                return false;
            }
        }

        String XmlNode::GetAttribute(String name)
        {
            return mAttributes[name];
        }
    }
}